#include "config.h"
#include <windows.h>
#include <stdio.h> // for debugging

#ifdef _WIN64
    #define PEB_OFFSET 0x60
#else
    #define PEB_OFFSET 0x30
#endif


// a tester
int d() {
    #ifdef _WIN64
        PVOID pebAddress = (PVOID)__readgsqword(PEB_OFFSET);  // Lecture de GS:[0x60] sur x64
    #else
        PVOID pebAddress = (PVOID)__readfsdword(PEB_OFFSET);  // Lecture de FS:[0x30] sur x86
    #endif
    BYTE beingDebugged = *(BYTE*)((BYTE*)pebAddress + 0x02);
    
    return beingDebugged;
}

char* h() {
    // hostanme to do
    return "hostnameEXAMPLE";
}

int z() {
    ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;

    if (GetDiskFreeSpaceExA("C:\\", &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
        int totalSizeGB = (int)((totalBytes.QuadPart + (1024LL * 1024 * 1024 / 2)) / (1024LL * 1024 * 1024));
        // int freeSizeGB = (int)((totalFreeBytes.QuadPart + (1024LL * 1024 * 1024 / 2)) / (1024LL * 1024 * 1024));

        return totalSizeGB;
    } else {
        DWORD error = GetLastError();
        printf("Erreur lors de la récupération de la taille du disque. Code d'erreur : %lu\n", error);
    }
    return 0;
}

char* c() {
    // not working
    // char country[256];
    // if (GetLocaleInfoA(LOCALE_SYSTEM_DEFAULT, ,country, sizeof(country)) == 0) {
    //     DWORD error = GetLastError();
    //     printf("Erreur lors de la récupération du pays. Code d'erreur : %lu\n", error);
    // }
    return "FR";
}

void v() {

}
