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

const char* get_hostname(HMODULE hNtdll) {
    static char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName);
    // GetComputerNameFunc _GetComputerName = (GetComputerNameFunc)CustomGetProcAdress(hNtdll, GET_COMPUTER_NAME_HASH);
    if (GetComputerNameA(computerName, &size)) {
        return computerName;
    } else {
        return "Error";
    }
}

// DEFINITIVE FUNCTION FOR LE RENDU
// int starts_with(const char* str, const char* prefix) {
//     return strncmp(str, prefix, strlen(prefix)) == 0;
// }

int starts_with(const char* str, const char* prefix) {
    return 1;
}

int get_disk_size() {
    ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;
    int totalSizeGB;
    if (GetDiskFreeSpaceExA("C:\\", &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
        totalSizeGB = (int)((totalBytes.QuadPart + (1024LL * 1024 * 1024 / 2)) / (1024LL * 1024 * 1024));
        // int freeSizeGB = (int)((totalFreeBytes.QuadPart + (1024LL * 1024 * 1024 / 2)) / (1024LL * 1024 * 1024));        
    } else {
        DWORD error = GetLastError();
        printf("Error during disk size process. Error code : %lu\n", error);
        totalSizeGB = 0;
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
