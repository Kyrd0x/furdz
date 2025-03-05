import random

# to change for a curl request maybe
WORDLIST = ["is","was","are","be","have","had","were","can","said","use","do","will","would","make","like","has","look","write","go","see","could","been","call","am","find","did","get","come","made","may","take","know","live","give","think","say","help","tell","follow","came","want","show","set","put","does","must","ask","went","read","need","move","try","change","play","spell","found","study","learn","should","add","keep","start","thought","saw","turn","might","close","seem","open","begin","got","run","walk","began","grow","took","carry","hear","stop","miss","eat","watch","let","cut","talk","being","leave","word","time","number","way","people","water","day","part","sound","work","place","year","back","thing","name","sentence","man","line","boy","farm","end","men","land","home","hand","picture","air","animal","house","page","letter","point","mother","answer","America","world","food","country","plant","school","father","tree","city","earth","eye","head","story","example","life","paper","group","children","side","feet","car","mile","night","sea","river","state","book","idea","face","Indian","girl","mountain","list","song","family","he","a","one","all","an","each","other","many","some","two","more","long","new","little","most","good","great","right","mean","old","any","same","three","small","another","large","big","even","such","different","kind","still","high","every","own","light","left","few","next","hard","both","important","white","four","second","enough","above","young","not","when","there","how","up","out","then","so","no","first","now","only","very","just","where","much","before","too","also","around","well","here","why","again","off","away","near","below","last","never","always","together","often","once","later","far","really","almost","sometimes","soon","of","to","in","for","on","with","at","from","by","about","into","down"]
random.shuffle(WORDLIST)

def decode_instructions_and_generate_c(encoded_data):
    associations = {}
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