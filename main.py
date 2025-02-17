from configparser import ConfigParser
from scripts.utils import *

config = ConfigParser()
config.read(".conf") 

# Access values
ROR_VALUE = config.getint("Payload", "ror_value")
PAYLOAD_FILE = config.get("Payload", "filepath")
STUB_FILE = config.get("Loader", "stub")

TEMP_FILE = "temp/shellcode_test.nasm"


def main():
    cp_file(PAYLOAD_FILE, TEMP_FILE)
    sed_file(TEMP_FILE, "%ROR_VALUE%", f"byte {hex(ROR_VALUE)}")
    nasm2instructions(TEMP_FILE)



if __name__ == "__main__":
    main()