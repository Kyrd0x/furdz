import requests
import random

class Encryptor:
    def __init__(self, method, verbose=False):
        self.method = method
        self.verbose = verbose
        if method not in ["xor", "dict"]:
            raise ValueError("Method must be 'xor' or 'dict'")
        self.key = None
        self.dict = None # correspondance table for dict encryption
        self.wordlist = None  # list of words for dictionary encryption
        self.size = 0  # size of the final encrypted payload

    def set_key(self, key):
        self.key = key

    # ---------- DICTIONARY-BASED ENCRYPTION ----------

    def get_association_table(self):
        # return C formatted association table
        if not self.dict:
            raise ValueError("Dictionary not set for dict encryption")
        c_array = 'const Association association_table[ASSOCIATION_TABLE_SIZE] = {\n'
        for i, word in enumerate(self.wordlist):
            c_array += f'    {{"{word}", 0x{i:02X}}},\n'
        c_array += '};'
        return c_array

    def get_nb_words(self):
        return self.nb_words

    def set_wordlist(self, source_file, size=256):
        if "http" in source_file:
            words = requests.get(source_file).text.splitlines()
        else:
            with open(source_file, "r") as file:
                words = file.read().split("\n")  # Load the word list

        words = [word.strip() for word in words]
        self.wordlist = random.sample(words, size)

    def format_instructions(self, instructions):
        # Remove \x sequences from the payload
        instructions = instructions.replace("\\x", "")

        words_per_line = 12  # Number of words per line in the formatted output
        words = instructions.split()  # Split the payload into individual words

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

        return formatted_instructions

    # Encrypt instructions using a dictionary-based approach
    def dict_encrypt(self, instructions):
        if not self.wordlist:
            raise ValueError("Wordlist not set for dictionary encryption")
        if len(self.wordlist) < 256:
            raise ValueError("Wordlist must contain at least 256 words for dictionary encryption")
        if self.method != "dict":
            raise ValueError("Encryptor method is not set to 'dict'")
        if self.dict:
            raise ValueError("Dictionary already set, cannot re-encrypt")

        # Main encryption logic
        associations = {}

        # Map each byte value to a word
        for i in range(len(self.wordlist)):
            associations[i] = self.wordlist[i]

        print(associations) if self.verbose else None
        decoded_words = []
        print(instructions) if self.verbose else None
        for i in range(0, len(instructions), 2):
            byte = instructions[i:i+2]  # Extract two hex characters
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

        c_message = self.format_instructions(c_message)
        self.dict = associations
        self.nb_words = len(decoded_words)

        print(f"[i] dictionary size: {self.nb_words} words") if self.verbose else None

        return c_message