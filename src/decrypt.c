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

// int custom_strcmp(const char *s1, const char *s2) {
//     while (*s1 && (*s1 == *s2)) { 
//         s1++;
//         s2++;
//     }
//     return *(unsigned char *)s1 - *(unsigned char *)s2;
// }

// char *custom_strncpy(char *dest, const char *src, size_t n) {
//     size_t i;
//     for (i = 0; i < n && src[i] != '\0'; i++) {
//         dest[i] = src[i];
//     }
//     while (i < n) {  // Remplit le reste avec des '\0' si src est plus courte
//         dest[i++] = '\0';
//     }
//     return dest;
// }


// Word decryption
void DICT_decrypt(const char* dict_payload) {
    int i = 0;
    int j = 0;
    unsigned char decoded_byte;
    
    // Parcours de chaque mot dans dict_payload
    while (*dict_payload != '\0') {
        // Chercher chaque mot dans dict_payload
        const char* start = dict_payload;
        while (*dict_payload != ' ' && *dict_payload != '\0') {
            dict_payload++;
        }
        
        // Si on a trouvé un mot (on a atteint un espace ou la fin de la chaîne)
        int length = dict_payload - start;  // Calcul de la longueur du mot
        char word[length + 1];  // Créer un tableau pour le mot
        strncpy(word, start, length);
        word[length] = '\0';  // Ajouter le caractère de fin de chaîne

        // Chercher l'association du mot dans le association_table
        decoded_byte = 0xFF;  // Valeur par défaut (si le mot n'est pas trouvé)
        for (int k = 0; k < ASSOCATION_TABLE_SIZE; k++) {
            if (strcmp(word, association_table[k].word) == 0) {
                decoded_byte = association_table[k].byte;
                break;
            }
        }
        
        // Ajouter l'octet décrypté au payload si trouvé
        payload[j++] = decoded_byte;
        
        // Passer à l'espace suivant ou à la fin de la chaîne
        if (*dict_payload != '\0') {
            dict_payload++;  // Saute l'espace
        }
    }

    // Terminer le tableau payload avec un zéro si nécessaire
    payload[j] = '\0'; 
}

//todo
// void AES_decrypt(unsigned char *data, size_t len, BYTE key) {
//     for (size_t i = 0; i < len; i++) {
//         data[i] = data[i] ^ key;
//     }
// }