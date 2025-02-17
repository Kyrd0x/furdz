from configparser import ConfigParser
from scripts.ROR import hash
from scripts.utils import *

config = ConfigParser()
config.read(".conf") 

# Access values
ROR_VALUE = config.getint("Payload", "ror_value")
PAYLOAD_FILE = config.get("Payload", "filepath")
STUB_FILE = config.get("Loader", "stub")

WORKING_FOLDER = "temp/"



def main():
    print("=====CONFIG=====")
    print(f"ROR value: {ROR_VALUE}")
    print("=================")
    print()
    print("===========PAYLOAD==============")
    sed_file(WORKING_FOLDER+PAYLOAD_FILE, "%ROR_VALUE%", f"byte {hex(ROR_VALUE)}")
    remaining_tags = extract_tags_from_file(WORKING_FOLDER+PAYLOAD_FILE)

    for tag in remaining_tags:
        parts = tag.replace("%", "").split("_")
        if parts[0] == "HASH":
            sed_file(WORKING_FOLDER+PAYLOAD_FILE, tag, str(hex(hash(parts[1], parts[2], ROR_VALUE))))


    instructions = nasm2instructions(WORKING_FOLDER+PAYLOAD_FILE)
    nb_bytes = int(len(instructions)/2)
    print()
    print(f"Shellcode length: {nb_bytes} bytes")
    print("================================")

    #Replace the shellcode in the loader
    sed_file(WORKING_FOLDER+STUB_FILE, "%SHELLCODE%", format_instructions(instructions))



if __name__ == "__main__":
    main()