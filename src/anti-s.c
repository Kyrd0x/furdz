#include "definitions.h"

#define ProcessBasicInformation 0

// to test, should be ok
// BYTE is_being_debugged() {
//     BYTE beingDebugged = 0xa;
//     WORD a = 0x1234;
//     #ifdef _WIN64
//         PVOID pebAddress = (PVOID)__readgsqword(0x60);  // GS:[0x60] on x64
//         beingDebugged = *(BYTE*)((BYTE*)pebAddress + 0x02);
//     #else
//         beingDebugged = 20; // 0x32 = 
//     #endif
//     return beingDebugged;
// }

BYTE is_being_debugged() {
    PVOID pebAddress;
    BYTE beingDebugged = 0;
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

void GetDiskSize(const char* driveLetter) {
    // Structure pour récupérer les informations de l'espace disque
    ULARGE_INTEGER freeBytes;
    ULARGE_INTEGER totalBytes;
    ULARGE_INTEGER totalFreeBytes;

    // Appel de GetDiskFreeSpaceEx pour obtenir les informations du disque
    if (GetDiskFreeSpaceEx(driveLetter, &freeBytes, &totalBytes, &totalFreeBytes)) {
        printf("Taille totale du disque %s: %llu octets\n", driveLetter, totalBytes.QuadPart);
        printf("Espace libre: %llu octets\n", totalFreeBytes.QuadPart);
        printf("Espace libre pour l'utilisateur: %llu octets\n", freeBytes.QuadPart);
    } else {
        // En cas d'erreur, afficher un message
        printf("Erreur lors de la récupération des informations sur le disque %s\n", driveLetter);
    }
}


//     return isDebugged;
// }
// char* h() {
//     // hostanme to do
//     return "hostnameEXAMPLE";
// }

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
