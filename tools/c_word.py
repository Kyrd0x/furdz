def decode_instructions_and_generate_c(instruction_file, association_file):
    # Charger les associations (octet -> mot)
    associations = {}
    try:
        with open(association_file, 'r', encoding='utf-8') as assoc_file:
            for i, line in enumerate(assoc_file):
                word = line.strip()
                associations[i] = word
    except FileNotFoundError:
        print(f"Erreur : Le fichier '{association_file}' est introuvable.")
        return
    except Exception as e:
        print(f"Erreur lors de la lecture des associations : {e}")
        return

    # Lire les instructions encodées
    try:
        with open(instruction_file, 'rb') as instr_file:
            encoded_data = instr_file.read()
    except FileNotFoundError:
        print(f"Erreur : Le fichier '{instruction_file}' est introuvable.")
        return
    except Exception as e:
        print(f"Erreur lors de la lecture des instructions : {e}")
        return

    # Décoder les octets
    decoded_words = []
    for byte in encoded_data:
        word = associations.get(byte, f"[0x{byte:02X}]")  # Si pas trouvé, afficher l'octet brut
        decoded_words.append(word)

    # ✅ Générer la chaîne C
    c_message = ' '.join(decoded_words)

    # ✅ Générer le tableau d'associations
    association_c_array = 'Association tableau[TAILLE] = {\n'
    for i in range(256):
        word = associations.get(i, f"mot{i}")
        association_c_array += f'    {{"{word}", 0x{i:02X}}},\n'
    association_c_array += '};'

    # 📋 Résultat final
    print('\n✅ Copie directement dans ton code C :\n')
    print(f'const char* message = "{c_message}";\n')
    print(association_c_array)

# 📁 Fichiers d'entrée
instruction_file = 'instructions_trash'  # Fichier binaire avec les octets encodés
association_file = 'words.txt'         # Fichier texte avec 256 mots (un par ligne)

# 🔄 Exécuter la fonction
decode_instructions_and_generate_c(instruction_file, association_file)
