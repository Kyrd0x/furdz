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

// DEFINITIVE FUNCTION FOR LE RENDU
// int starts_with(const char* str, const char* prefix) {
//     return strncmp(str, prefix, strlen(prefix)) == 0;
// }

int starts_with(const char* str, const char* prefix) {
    return 1;
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
