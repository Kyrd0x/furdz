#ifndef WINAPI_H
#define WINAPI_H

#include <windows.h>
#include <winnls.h>
#include <tlhelp32.h>
#include <evntprov.h> 

#include "objhash.h"

// Structure to represent a process environment block (PEB)
typedef struct __PEB {
    UCHAR InheritedAddressSpace;
    UCHAR ReadImageFileExecOptions;
    UCHAR BeingDebugged;
    UCHAR BitField;
    UCHAR Padding0[4];
    PVOID Mutant;        // 0x08
    PVOID ImageBaseAddr; // 0x10
    PVOID LdrData;       // 0x18
    PVOID ProcessParams; // 0x20
} _PEB, *_PPEB;


// Function pointer typedefs for various Windows API functions
typedef LPVOID (WINAPI* VirtualAllocEx_t)(HANDLE, LPVOID, SIZE_T, DWORD, DWORD);
typedef BOOL (WINAPI* WriteProcessMemory_t)(HANDLE, LPVOID, LPCVOID, SIZE_T, SIZE_T*);
typedef BOOL (WINAPI* VirtualProtectEx_t)(HANDLE, LPVOID, SIZE_T, DWORD, PDWORD);
typedef HMODULE (WINAPI* LoadLibraryA_t)(LPCSTR);
typedef FARPROC (WINAPI* GetProcAddress_t)(HMODULE, LPCSTR);
typedef BOOL (WINAPI* GetComputerNameFunc)(LPSTR, LPDWORD);
typedef BOOL (WINAPI* GetDiskFreeSpaceExFuncA)(LPCSTR, PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER);
typedef void (WINAPI* GetSystemInfoFunc)(LPSYSTEM_INFO);
typedef HKL (WINAPI* GetKeyboardLayoutFunc)(DWORD);
typedef BOOL (WINAPI* GlobalMemoryStatusExFunc)(LPMEMORYSTATUSEX);
typedef LANGID (WINAPI* GetSystemDefaultLangIDFunc)(void);
typedef DWORD (WINAPI* GetSystemDefaultLCIDFunc)(void);
// ETW
typedef ULONG (WINAPI* EtwEventWrite_t)(REGHANDLE, PCEVENT_DESCRIPTOR, ULONG, PEVENT_DATA_DESCRIPTOR);
typedef BOOL (WINAPI* FlushInstructionCache_t)(HANDLE, LPCVOID, SIZE_T);


// ULONG EVNTAPI EventWrite(
//   [in]           REGHANDLE              RegHandle,
//   [in]           PCEVENT_DESCRIPTOR     EventDescriptor,
//   [in]           ULONG                  UserDataCount,
//   [in, optional] PEVENT_DATA_DESCRIPTOR UserData
// );

// Custom function to retrieve the base address of a module by its hash
HMODULE CustomGetModuleHandle(ObjHash module_hash);
FARPROC CustomGetProcAddress(IN HMODULE hModule, ObjHash function_hash);
PIMAGE_NT_HEADERS64 PE_getNtheaders(unsigned char* dll_data);

#endif // WINAPI_H