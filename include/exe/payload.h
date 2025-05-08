#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <stdint.h>

#define ASSOCATION_TABLE_SIZE 256

// Structure to map words to bytes
typedef struct {
    char *word;   // Word to be mapped
    uint8_t byte; // Corresponding byte value
} Association;

// Declaration of an external association table
extern const Association association_table[ASSOCATION_TABLE_SIZE];

// Payload-related declarations
extern unsigned char payload[%PAYLOAD_SIZE%];
extern const char* dict_payload;

#endif // PAYLOAD_H