#include "definitions.h"

void XOR(unsigned char *data, size_t len, uint16_t key) {
    __asm__ (
        "mov %[len], %%rsi\n\t"
        "mov %[data], %%rdi\n\t"
        "movzx %[key], %%rdx\n\t"
        "mov %%rdx, %%rcx\n\t"
        "shr $8, %%rcx\n\t"
        "xor %%rax, %%rax\n\t"
        "test %%rsi, %%rsi\n\t"
        "jz done\n\t"
    "loop:\n\t"
        "mov %%rax, %%r8\n\t"
        "and $1, %%r8\n\t"                // Test si i est pair ou impair avec i % 2
        "jnz odd_\n\t"                      // Si impair, saute
        "movb %%dl, %%bl\n\t"             // Si pair, utilise key_bytes[0]
        "jmp even_\n\t"
    "odd_:\n\t"
        "movb %%cl, %%bl\n\t"             // Si impair, utilise key_bytes[1]
    "even_:\n\t"
        "xorb %%bl, (%%rdi, %%rax)\n\t"   // XOR data[i] avec l'octet de clé approprié
        "inc %%rax\n\t"
        "cmp %%rsi, %%rax\n\t"
        "jb loop\n\t"
    "done:\n\t"
        :
        : [data] "r" (data), [len] "r" (len), [key] "r" (key)
        : "rax", "rbx", "rcx", "rdx", "rdi", "rsi", "r8", "memory"
    );
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