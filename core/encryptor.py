import requests
import random
import re

class Encryptor:
    def __init__(self, method, verbose=False):
        self.method = method
        self.verbose = verbose
        if method not in ["xor", "dict"]:
            raise ValueError("Method must be 'xor' or 'dict'")
        self.key = None # XOR key (WORD)
        self.dict = None # correspondance table for dict encryption
        self.wordlist = None  # list of words for dictionary encrypt
        self.nb_words = 0 # number of words in the encrypted payload (for dict)
        self._assoc_table_c = ""

    # ---------- COMMON METHODS ----------
    def set_key(self, key):
        self.key = key

    def _normalize_to_hexstr(self, data):
        """
        Accepts: hex string with or without spaces, \\x.., 0x.. or bytes.
        Returns a contiguous hex string (ex: '4F2A...') in uppercase.
        """
        if isinstance(data, (bytes, bytearray)):
            return data.hex().upper()

        if not isinstance(data, str):
            raise TypeError("payload needs to be str or bytes")

        s = data.strip()
        s = s.replace("\\x", "")
        s = s.replace("0x", "").replace("0X", "")
        s = re.sub(r"[^0-9a-fA-F]", "", s)

        if len(s) % 2 != 0:
            raise ValueError("Hex string must have an even length")

        return s.upper()

    # ---------- DICTIONARY-BASED ENCRYPTION ----------

    def get_association_table(self):
        # return C formatted association table
        if not self.dict:
            raise ValueError("Dictionary not set for dict encryption")
        # tu peux régénérer ou renvoyer le cache
        if self._assoc_table_c:
            return self._assoc_table_c

        c_array = 'const Association association_table[ASSOCIATION_TABLE_SIZE] = {\n'
        for i in range(256):
            word = self.dict.get(i, f"mot{i}")
            c_array += f'    {{"{word}", 0x{i:02X}}},\n'
        c_array += '};'
        self._assoc_table_c = c_array
        return c_array

    def get_nb_words(self):
        return self.nb_words

    def set_wordlist(self, source_file, size=256):
        if "http" in source_file:
            try:
                words = requests.get(source_file).text.splitlines()
            except requests.RequestException as e:
                raise ValueError(f"Error fetching wordlist from {source_file}: {e}")
        else:
            with open(source_file, "r") as file:
                words = file.read().split("\n")  # Load the wordlist from a file
        words = [word.strip() for word in words if word.strip()]
        if len(words) < size:
            raise ValueError(f"Wordlist must contain at least {size} words")
        self.wordlist = random.sample(words, size)

    def format_instructions(self, instructions):
        # instructions ici est une chaîne de mots séparés par espace
        words_per_line = 12
        words = instructions.split()
        lines = [words[i:i+words_per_line] for i in range(0, len(words), words_per_line)]
        out = []
        for i, line in enumerate(lines):
            line_str = " ".join(line)
            out.append(f'"{line_str} "')
        return "\n".join(out)

    # Encrypt instructions using a dictionary-based approach
    def dict_encrypt(self, instructions):
        if self.method != "dict":
            raise ValueError("Encryptor method is not set to 'dict'")
        if not self.wordlist or len(self.wordlist) < 256:
            raise ValueError("Wordlist must contain at least 256 words for dictionary encryption")
        if self.dict:
            raise ValueError("Dictionary already set, cannot re-encrypt")

        # Normaliser l'entrée
        hexstr = self._normalize_to_hexstr(instructions)
        if self.verbose:
            print(f"[i] hex length: {len(hexstr)} (bytes: {len(hexstr)//2})")

        # Construire l'association byte->mot
        associations = {i: self.wordlist[i] for i in range(256)}
        if self.verbose:
            print(associations)

        # Décodage en mots
        decoded_words = []
        for i in range(0, len(hexstr), 2):
            b = hexstr[i:i+2]
            byte_int = int(b, 16)
            decoded_words.append(associations.get(byte_int, f"[0x{byte_int:02X}]"))

        # Message + formatage
        c_message = " ".join(decoded_words)
        formatted = self.format_instructions(c_message)

        # Table C (cache)
        c_array = 'const Association association_table[ASSOCIATION_TABLE_SIZE] = {\n'
        for i in range(256):
            word = associations.get(i, f"mot{i}")
            c_array += f'    {{"{word}", 0x{i:02X}}},\n'
        c_array += '};'
        self._assoc_table_c = c_array

        # État interne
        self.dict = associations
        self.nb_words = len(decoded_words)
        if self.verbose:
            print(f"[i] dictionary size: {self.nb_words} words")

        return formatted
    
    # ---------- XOR-BASED ENCRYPTION ----------

    # XOR encryption/decryption with a WORD key
    def xor_encrypt(self, data):
        # Convert the hexadecimal string to bytes
        data_bytes = bytes.fromhex(data)

        # Split the 16-bit key (WORD) into two bytes
        key_bytes = [self.key & 0xFF, (self.key >> 8) & 0xFF]

        # Apply XOR operation on each byte, alternating key bytes
        result = bytearray()
        for i in range(len(data_bytes)):
            result.append(data_bytes[i] ^ key_bytes[i % 2])

        # Return the result in hexadecimal
        return result.hex()