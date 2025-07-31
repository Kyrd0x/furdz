#include <windows.h>
#include <tlhelp32.h>
#include <stdbool.h>
#include <wchar.h>

#include "entrypoint.h"


DWORD GetLsassPid() {
    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(PROCESSENTRY32W);
    DWORD pid = 0;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    if (Process32FirstW(snapshot, &entry)) {
        do {
            if (_wcsicmp(entry.szExeFile, L"lsass.exe") == 0) {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return pid;
}

void entrypoint() {

    DWORD pid = GetLsassPid();
    if (pid == 0) {
        MessageBoxA(NULL, "LSASS not found.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (hProcess == NULL) {
        MessageBoxA(NULL, "Failed to open LSASS process.", "Error", MB_OK | MB_ICONERROR);
        return;
    } else {
        MessageBoxA(NULL, "Successfully got a handle to LSASS (no memory read).", "Info", MB_OK | MB_ICONINFORMATION);
    }

    // Ne fait rien de plus — juste obtenir un handle suffit à déclencher certains EDR.
    CloseHandle(hProcess);
}