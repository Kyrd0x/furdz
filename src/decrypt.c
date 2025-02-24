#include "config.h"




// CQFD
void XOR(unsigned char *data, size_t len, BYTE key) {
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key;
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