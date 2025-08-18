#ifndef REFLECTIVE_LOADER_H
#define REFLECTIVE_LOADER_H

#include <windows.h>

#include "libc.h"

// No need to expose
// DWORD64 rva_to_offset(DWORD64 rva, DWORD64 base_address);
DWORD64 get_reflective_loader_offset(DWORD64 base_address, LPCSTR function_name);

#endif // REFLECTIVE_LOADER_H