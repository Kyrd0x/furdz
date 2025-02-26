#include "definitions.h"

// to test, should be ok
BYTE is_being_debugged() {
    BYTE beingDebugged = 0xa;
    WORD a = 0x1234;
    #ifdef _WIN64
        PVOID pebAddress = (PVOID)__readgsqword(0x60);  // GS:[0x60] on x64
        beingDebugged = *(BYTE*)((BYTE*)pebAddress + 0x02);
    #else
        beingDebugged = 20; // 0x32 = 
    #endif
    return beingDebugged;
}

// int d() {
//     PVOID pebAddress;

//     #ifdef _WIN64
//         __asm__(
//             "movq %%gs:0x60, %0\n\t"  // Charge l'adresse du PEB dans RAX
//             : "=r" (pebAddress)       // Stocke le résultat dans pebAddress
//         );
//     #else
//         return 1;
//     #endif

//     BYTE beingDebugged = *(BYTE*)((BYTE*)pebAddress + 0x02);
//     return beingDebugged;
// }


// char* h() {
//     // hostanme to do
//     return "hostnameEXAMPLE";
// }

// int z() {
//     ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;

//     if (GetDiskFreeSpaceExA("C:\\", &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
//         int totalSizeGB = (int)((totalBytes.QuadPart + (1024LL * 1024 * 1024 / 2)) / (1024LL * 1024 * 1024));
//         // int freeSizeGB = (int)((totalFreeBytes.QuadPart + (1024LL * 1024 * 1024 / 2)) / (1024LL * 1024 * 1024));

//         return totalSizeGB;
//     } else {
//         DWORD error = GetLastError();
//         printf("Error during disk size process. Error code : %lu\n", error);
//     }
//     return 0;
// }

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
