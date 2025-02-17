from configparser import ConfigParser
from scripts.utils import *

config = ConfigParser()
config.read(".conf") 

# Access values
ROR_VALUE = config.getint("Payload", "ror_value")
PAYLOAD_FILE = config.get("Payload", "filepath")
STUB_FILE = config.get("Loader", "stub")

WORKING_FOLDER = "temp/"



def main():
    #Prepare the Payload
    sed_file(WORKING_FOLDER+PAYLOAD_FILE, "%ROR_VALUE%", f"byte {hex(ROR_VALUE)}")
    instructions = nasm2instructions(WORKING_FOLDER+PAYLOAD_FILE)
    nb_bytes = int(len(instructions)/2)
    print(f"Shellcode length: {nb_bytes} bytes")

    #Replace the shellcode in the loader
    sed_file(WORKING_FOLDER+STUB_FILE, "%SHELLCODE%", format_instructions(instructions))



if __name__ == "__main__":
    main()