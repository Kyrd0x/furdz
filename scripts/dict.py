import random

# Decode encoded data into words and generate a C array for associations
def decode_instructions_and_generate_c(encoded_data):
    associations = {}
    with open("data/words.txt", "r") as file:
        WORDLIST = file.read().split("\n")  # Load the word list

    # Map each byte value to a word
    for i in range(len(WORDLIST)):
        associations[i] = WORDLIST[i]

    print(associations)
    decoded_words = []
    for i in range(0, len(encoded_data), 2):
        byte = encoded_data[i:i+2]  # Extract two hex characters
        byte_int = int(byte, 16)  # Convert to integer
        
        # Retrieve the associated word or fallback to raw byte
        word = associations.get(byte_int, f"[0x{byte_int:02X}]")
        decoded_words.append(word)

    c_message = ' '.join(decoded_words)  # Join decoded words into a single string

    # Generate a C array for the association table
    association_c_array = 'const Association association_table[ASSOCATION_TABLE_SIZE] = {\n'
    for i in range(256):
        word = associations.get(i, f"mot{i}")
        association_c_array += f'    {{"{word}", 0x{i:02X}}},\n'
    association_c_array += '};'

    return c_message, association_c_array, len(decoded_words)

# Encrypt instructions using a dictionary-based approach
def dictionary_encrypt(instructions):
    payload, association_table, size = decode_instructions_and_generate_c(instructions)
    
    # Remove \x sequences from the payload
    payload = payload.replace("\\x", "")

    words_per_line = 12  # Number of words per line in the formatted output
    words = payload.split()  # Split the payload into individual words

    # Group words into lines of 'words_per_line'
    payload_lines = [words[i:i + words_per_line] for i in range(0, len(words), words_per_line)]

    # Format the instructions into a multi-line string
    formatted_instructions = ""
    for i, line in enumerate(payload_lines):
        line_str = " ".join(line)  # Join words in the line with spaces
        if i == 0:
            formatted_instructions += f'"{line_str} "'  # Add quotes for the first line
        else:
            formatted_instructions += f'\n"{line_str} "'  # Add newline and quotes for subsequent lines

    return formatted_instructions, association_table, size