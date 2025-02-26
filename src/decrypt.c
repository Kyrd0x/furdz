#include "config.h"

// CQFD
    // __asm__ (
    //     "mov %[len], %%rsi\n\t"
    //     "mov %[data], %%rdi\n\t"
    //     "movzx %[key], %%rdx\n\t"
    //     "test %%rsi, %%rsi\n\t"
    //     "jz done\n\t"
    // "loop:\n\t"
    //     "mov (%%rdi), %%al\n\t"
    //     "mov %%al, %%bl\n\t"
    //     "and %%dil, %%bl\n\t"
    //     "or %%dil, %%al\n\t"
    //     "not %%bl\n\t"
    //     "and %%bl, %%al\n\t"
    //     "mov %%al, (%%rdi)\n\t"
    //     "inc %%rdi\n\t"
    //     "dec %%rsi\n\t"
    //     "jnz loop\n\t"
    // "done:\n\t"
    //     :
    //     : [data] "r" (data), [len] "r" (len), [key] "r" (key)
    //     : "rax", "rbx", "rdx", "rdi", "rsi", "memory"
    // );

void XOR(unsigned char *data, size_t len, WORD key) {
    BYTE key_bytes[2];
    key_bytes[0] = key & 0xFF;
    key_bytes[1] = (key >> 8) & 0xFF;

    for (size_t i = 0; i < len; i++) {
        data[i] ^= key_bytes[i % 2];
    }
}


// // Word decryption
// unsigned char* DICT_decrypt(const char *message) {
//     int assoc_size = sizeof(tableau) / sizeof(tableau[0]); // Calculer la taille du tableau d'associations
//     static unsigned char payload[TAILLE];  // Tableau pour stocker les octets associés
//     int result_index = 0;

//     // Dupliquer la chaîne pour éviter de modifier l'originale
//     char buffer[TAILLE * 8]; 
//     strcpy(buffer, message);  // Copie le message dans le buffer

//     // Découper la chaîne en mots
//     char *token = strtok(buffer, " ");
//     while (token != NULL) {
//         for (int i = 0; i < assoc_size; i++) {
//             // Si le mot correspond à un mot dans le tableau, ajouter l'octet correspondant à la payload
//             if (strcmp(token, tableau[i].mot) == 0) {
//                 payload[result_index++] = tableau[i].octet;
//                 break;
//             }
//         }
//         token = strtok(NULL, " "); // Passer au mot suivant
//     }

//     // Retourner l'adresse de la payload
//     return payload;
// }

//todo
// void AES_decrypt(unsigned char *data, size_t len, BYTE key) {
//     for (size_t i = 0; i < len; i++) {
//         data[i] = data[i] ^ key;
//     }
// }