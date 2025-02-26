#ifndef CONFIG_H
#define CONFIG_H

#include <windows.h>

#define HOSTNAME_PREFIX "CYCORP-"
#define DEBUG_MODE 1

#define TRUE 1
#define FALSE 0

void XOR(unsigned char *data, size_t len, WORD key);

HMODULE CustomGetModuleHandle(unsigned int module_hash);
FARPROC CustomGetProcAdress(IN HMODULE hModule, unsigned int function_hash);

// HMODULE CustomGetModuleHandle(unsigned int module_hash);
// FARPROC CustomGetProcAdress(IN HMODULE hModule, unsigned int function_hash);
// void v();
// int d();
// int z();
// char* h();
// char* c();

typedef NTSTATUS (NTAPI* NtAllocVirtMem)(
    HANDLE ProcessHandle,
    PVOID* BaseAddress,
    ULONG_PTR ZeroBits,
    PSIZE_T RegionSize,
    ULONG AllocationType,
    ULONG Protect
);

typedef NTSTATUS(NTAPI* NtWriteVirtMem)(
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    PVOID Buffer,
    ULONG NumberOfBytesToWrite,
    PULONG NumberOfBytesWritten
);

typedef NTSTATUS(NTAPI* NtProtectVirtMem)(
    HANDLE ProcessHandle,
    PVOID* BaseAddress,
    PSIZE_T NumberOfBytesToProtect,
    ULONG NewAccessProtection,
    PULONG OldAccessProtection
);

typedef NTSTATUS(NTAPI* NtCreateThreadEx)(
    PHANDLE ThreadHandle,
    ACCESS_MASK DesiredAccess,
    PCRYPT_ATTRIBUTES ObjectAttributes,
    HANDLE ProcessHandle,
    PVOID StartRoutine,
    PVOID Argument,
    ULONG CreateFlags,
    ULONG_PTR ZeroBits,
    SIZE_T StackSize,
    SIZE_T MaximumStackSize,
    PVOID AttributeList
);

typedef NTSTATUS(NTAPI* NtWaitForSingleObj)(
    HANDLE Handle,
    BOOLEAN Alertable,
    PLARGE_INTEGER Timeout
);

#endif // CONFIG_H
