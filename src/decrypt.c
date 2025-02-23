#include "config.h"



void XOR(unsigned char *data, size_t len, BYTE key) {
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key;
    }
}

void DICT_decrypt(unsigned char *data, size_t len, BYTE key) {
    for (size_t i = 0; i < len; i++) {
        data[i] = data[i] ^ key;
    }
}

//todo
void AES_decrypt(unsigned char *data, size_t len, BYTE key) {
    for (size_t i = 0; i < len; i++) {
        data[i] = data[i] ^ key;
    }
}