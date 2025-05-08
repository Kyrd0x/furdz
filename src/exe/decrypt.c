#include "definitions.h"

// XOR encryption/decryption function
// This function uses inline assembly to XOR each byte of the data with a key.
// The key alternates between two bytes depending on whether the index is even or odd.
void XOR(unsigned char *data, size_t len, uint16_t key) {
    __asm__ (
        // Initialize registers with input parameters
        "mov %[len], %%rsi\n\t"          // Length of data
        "mov %[data], %%rdi\n\t"         // Pointer to data
        "movzx %[key], %%rdx\n\t"        // Load key into rdx
        "mov %%rdx, %%rcx\n\t"           // Copy key to rcx
        "shr $8, %%rcx\n\t"              // Extract high byte of key
        "xor %%rax, %%rax\n\t"           // Initialize index (rax = 0)
        "test %%rsi, %%rsi\n\t"          // Check if length is zero
        "jz done\n\t"                    // If zero, jump to done
    "loop:\n\t"
        // Determine whether the index is even or odd
        "mov %%rax, %%r8\n\t"
        "and $1, %%r8\n\t"               // Check if index % 2 == 0
        "jnz odd_\n\t"                   // If odd, jump to odd_
        "movb %%dl, %%bl\n\t"            // Use low byte of key if even
        "jmp even_\n\t"
    "odd_:\n\t"
        "movb %%cl, %%bl\n\t"            // Use high byte of key if odd
    "even_:\n\t"
        // XOR the current byte with the selected key byte
        "xorb %%bl, (%%rdi, %%rax)\n\t"
        "inc %%rax\n\t"                  // Increment index
        "cmp %%rsi, %%rax\n\t"           // Compare index with length
        "jb loop\n\t"                    // If index < length, repeat loop
    "done:\n\t"
        :
        : [data] "r" (data), [len] "r" (len), [key] "r" (key)
        : "rax", "rbx", "rcx", "rdx", "rdi", "rsi", "r8", "memory"
    );
}

// Function to decrypt a dictionary-based payload
// Each word in the payload is mapped to a byte using an association table.
void DICT_decrypt(const char* dict_payload) {
    int i = 0;
    int j = 0;
    unsigned char decoded_byte;
    
    // Iterate through each word in the dictionary payload
    while (*dict_payload != '\0') {
        // Find the start and end of the current word
        const char* start = dict_payload;
        while (*dict_payload != ' ' && *dict_payload != '\0') {
            dict_payload++;
        }
        
        // Extract the word and null-terminate it
        int length = dict_payload - start;  // Calculate word length
        char word[length + 1];  // Allocate space for the word
        strncpy(word, start, length);
        word[length] = '\0';  // Null-terminate the word

        // Look up the word in the association table
        decoded_byte = 0xFF;  // Default value if the word is not found
        for (int k = 0; k < ASSOCATION_TABLE_SIZE; k++) {
            if (strcmp(word, association_table[k].word) == 0) {
                decoded_byte = association_table[k].byte;  // Get the associated byte
                break;
            }
        }
        
        // Add the decoded byte to the payload
        payload[j++] = decoded_byte;
        
        // Skip the space or end of the string
        if (*dict_payload != '\0') {
            dict_payload++;  // Move past the space
        }
    }

    // Null-terminate the payload if necessary
    payload[j] = '\0'; 
}

// Placeholder for AES decryption function
// void AES_decrypt(unsigned char *data, size_t len, BYTE key) {
//     for (size_t i = 0; i < len; i++) {
//         data[i] = data[i] ^ key;
//     }
// }