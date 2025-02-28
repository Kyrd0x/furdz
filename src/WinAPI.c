#include "definitions.h"

HMODULE CustomGetModuleHandle(ObjHash module_hash) {
    void* module_base = NULL;
    #ifdef _WIN64
        __asm__ (
            // Initialize r10d with hash and set up PEB access
            "xor %%rax, %%rax\n\t"
            "movl %[hash], %%r10d\n\t"
            "xor %%r9d, %%r9d\n\t"
            "mov $0xff, %%rax\n\t"
            "xor %%rdx, %%rdx\n\t"
            "movzx %[is_ror], %%r12d\n\t"
            "xor $0x9f, %%rax\n\t"
            "mov %%gs:(%%rax), %%rax\n\t"
            "xor $947851, %%rax\n\t"
            "mov $1000, %%r9d\n\t"
            "mov %%rax, %%rdx\n\t"
            "xor $947851, %%rdx\n\t"          
            "mov 0x18(%%rdx), %%rdx\n\t"
            "xor %%rax, %%rdx\n\t"
            "shl $0x4, %%r9d\n\t"
            "xor %%rax, %%rdx\n\t"
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
            "dec %%rcx\n\t"
            "push %%rcx\n\t"
            "movzx %[rotation_value], %%ecx\n\t"   
            "test %%r12d, %%r12d\n\t"
            "jz m_rotate_left_%=\n\t"
            "ror %%cl, %%r9d\n\t"              
            "jmp m_rotate_end_%=\n\t"            
        "m_rotate_left_%=:\n\t"
            "rol %%cl, %%r9d\n\t"              
        "m_rotate_end_%=:\n\t"
            "pop %%rcx\n\t"
            "add %%eax, %%r9d\n\t"            // Add AL to hash
            "test %%rcx, %%rcx\n\t"
            "jnz module_hash_loop\n\t"
        "then:\n\t"
            "cmp %%r10d, %%r9d\n\t"           // Compare hash
            "je found\n\t"                     // If equal, jump to found
        "load_next_module:\n\t"
            "mov (%%rdx), %%rdx\n\t"          // Next module
            "jmp load_module_name\n\t"
        "found:\n\t"
            "mov %%rdx, %[result]\n\t"            // Store result
            : [result] "=r" (module_base)              // Output
            : [hash] "r" (module_hash.value), [rotation_value] "r" (module_hash.rotation_value), [is_ror] "m" (module_hash.is_rotation_right)
            : "rax", "rdx", "rsi", "rcx", "r9", "r10", "r11", "r12", "memory"  // Clobbers
        );
    #else
        // 32-bit code TODO
    #endif
    return module_base;
}

FARPROC CustomGetProcAdress(IN HMODULE hModule, ObjHash function_hash) {
    if (hModule == NULL) {
        return NULL;
    }
    void* function_base = NULL;
    #ifdef _WIN64
        __asm__ (
            // Initialize r10d with hash and set up PEB access
            "xor %%r9d, %%r9d\n\t"
            "mov %[module], %%rdx\n\t"
            "movzx %[is_ror], %%r12d\n\t"
            "xor %%rax, %%rax\n\t"
            "mov $0x40, %%rax\n\t"
            "push %%rax\n\t"
            "mov $47651200, %%r9d\n\t"
            "movl %[hash], %%r10d\n\t"  
            "shr $1, %%rax\n\t"
            "add %%rdx, %%rax\n\t"
            "mov (%%rax), %%rdx\n\t" // rdx+0x20
            "mov %%rdx, %%r8\n\t"
            "mov $0x7e, %%rbx\n\t"
            "push %%rbx\n\t"
            "sub $0x42, %%rdx\n\t"
            "xor $5454, %%r9d\n\t"
            "add %%rbx, %%rdx\n\t"
            "mov (%%rdx), %%eax\n\t" // rdx+0x3C
            "mov %%r8, %%rdx\n\t"
            "add %%rdx, %%rax\n\t"
            "pop %%rbx\n\t"
            "add $0xa, %%rbx\n\t"
            "add %%rax, %%rbx\n\t"
            "mov (%%rbx), %%eax\n\t" // rax+0x88
            "add %%rdx, %%rax\n\t"
            "push %%rax\n\t"
            "mov $0x18, %%r9\n\t"
            "add %%r9, %%rax\n\t"
            "mov (%%rax), %%ecx\n\t" // rax+0x18
            "pop %%rax\n\t"
            "pop %%rbx\n\t"
            "shr $1, %%rbx\n\t"
            "add %%rax, %%rbx\n\t"
            "mov (%%rbx), %%r8d\n\t" // rax+0x20
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
            "lodsb\n\t"
            "push %%rcx\n\t"
            "test %%r12d, %%r12d\n\t"
            "movzx %[rotation_value], %%ecx\n\t"   
            "jz f_rotate_left_%=\n\t"
            "ror %%cl, %%r9d\n\t"              
            "jmp f_rotate_end_%=\n\t"            
        "f_rotate_left_%=:\n\t"
            "rol %%cl, %%r9d\n\t"              
        "f_rotate_end_%=:\n\t"
            "pop %%rcx\n\t"
            "add %%eax, %%r9d\n\t"            // Add AL to hash
            "cmp %%ah, %%al\n\t"
            "jnz function_hash_loop\n\t"
        "next:\n\t"
            "cmp %%r10d, %%r9d\n\t"           // Compare hash
            "jnz find_function\n\t"           // If equal, jump to found
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
            : [hash] "r" (function_hash.value), [module] "r" ((uintptr_t)hModule), [rotation_value] "r" (function_hash.rotation_value), [is_ror] "m" (function_hash.is_rotation_right)                // Input
            : "rax", "rbx", "rcx", "rdx", "rsi", "r8", "r9", "r10", "r11", "r12", "memory"  // Clobbers
        );

    #else
        // 32-bit code TODO
    #endif
    return function_base;
}