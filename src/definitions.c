#include "definitions.h"

// hash, rotation_value, is_rotation_right

FctHash NTDLL_HASH = %MODHASH__ntdll.dll%;
FctHash KERNEL32_HASH = %MODHASH__kernel32.dll%;

FctHash QUERY_INFORMATION_PROCESS_HASH = %FCTHASH__NtQueryInformationProcess%;
FctHash GET_COMPUTER_NAME_HASH = %FCTHASH__GetComputerNameA%;
FctHash LOAD_LIBRARY_HASH = %FCTHASH__LoadLibraryA%;

FctHash VIRTUAL_ALLOC_HASH = %FCTHASH__NtAllocateVirtualMemory%;
FctHash WRITE_MEMORY_HASH = %FCTHASH__NtWriteVirtualMemory%;
FctHash VIRTUAL_PROTECT_HASH = %FCTHASH__NtProtectVirtualMemory%;
FctHash CREATE_THREAD_HASH = %FCTHASH__NtCreateThreadEx%;
FctHash WAIT_FOR_SINGLE_OBJECT_HASH = %FCTHASH__NtWaitForSingleObject%;