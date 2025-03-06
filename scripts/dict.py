import random

def decode_instructions_and_generate_c(encoded_data):
    associations = {}
    with open("data/words.txt", "r") as file:
        WORDLIST = file.read().split

    for i in range(len(WORDLIST)):
        associations[i] = WORDLIST[i]

    print(associations)
    decoded_words = []
    for i in range(0, len(encoded_data), 2):
        byte = encoded_data[i:i+2]
        
        byte_int = int(byte, 16)
        
        # Récupérer le mot associé ou afficher l'octet brut
        word = associations.get(byte_int, f"[0x{byte_int:02X}]")
        
        decoded_words.append(word)


    c_message = ' '.join(decoded_words)

    association_c_array = 'const Association association_table[ASSOCATION_TABLE_SIZE] = {\n'
    for i in range(256):
        word = associations.get(i, f"mot{i}")
        association_c_array += f'    {{"{word}", 0x{i:02X}}},\n'
    association_c_array += '};'

    # 📋 Résultat final
    # print('\n✅ Copie directement dans ton code C :\n')
    return c_message, association_c_array, len(decoded_words)


def dictionary_encrypt(instructions):
    payload, association_table, size = decode_instructions_and_generate_c(instructions)
    
    # Nettoyage du payload pour enlever les séquences \x
    payload = payload.replace("\\x", "")

    words_per_line = 12
    words = payload.split()  # On sépare les mots par des espaces

    # Diviser les mots en groupes de 'words_per_line'
    payload_lines = [words[i:i + words_per_line] for i in range(0, len(words), words_per_line)]

    # Formater les instructions
    formatted_instructions = ""
    for i, line in enumerate(payload_lines):
        line_str = " ".join(line)  # Rejoindre les mots en une seule chaîne avec un espace
        if i == 0:
            formatted_instructions += f'"{line_str} "'  # Pour la première ligne, on ajoute directement les guillemets
        else:
            formatted_instructions += f'\n"{line_str} "'  # Pour les lignes suivantes, on ajoute un retour à la ligne et les guillemets

    return formatted_instructions, association_table, size