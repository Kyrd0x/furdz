#ifndef SANDBOX_H
#define SANDBOX_H

#include <windows.h>
#include <stdbool.h>

#include "objhash.h"

bool is_being_debugged();
const char* get_hostname(HMODULE hKernel32dll);
unsigned int compute_hash(const char* str, uint8_t rotation_value, bool is_rotation_right);
bool is_objhash(ObjHash obj_hash);
bool is_string_matching_prefixHash(const char* str, ObjHash prefix_hash);

bool is_sandboxed(); // Run all checks
bool is_valid_hostname(const char* hostname);
bool is_valid_language(HMODULE hKernel32, HMODULE hUser32);
bool is_valid_computer(HMODULE hKernel32);

#endif // SANDBOX_H