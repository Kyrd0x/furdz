#include "config.h"
#include <windows.h>

void version() {

}

// a tester
bool anti_debug() {
    #ifdef _WIN64
        PVOID pebAddress = (PVOID)__readgsqword(0x60);  // Lecture de GS:[0x60] sur x64
    #else
        PVOID pebAddress = (PVOID)__readfsdword(0x30);  // Lecture de FS:[0x30] sur x86
    #endif
    BYTE beingDebugged = *(BYTE*)((BYTE*)pebAddress + 0x02);
    
    return (beingDebugged != 0);
}

bool detect_vm() {
    // hostname and ?
}

int disk_size() {
    return 0;
}

char[2] country() {

}
