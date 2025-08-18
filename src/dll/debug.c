#include <windows.h>

#include "entrypoint.h"

void entrypoint() {
    // Ton code ici, ex: ReverseShell_TCP();
    MessageBoxA(NULL, "Debug DLL loaded & called successfully!", "Debug", MB_OK);
}