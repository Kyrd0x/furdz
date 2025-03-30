from scripts.hasher import hash
import subprocess
import ipaddress
import locale
import random
import socket
import math
import json
import yaml
import re
import os

# List of valid rotation values for hashing
WORKING_RO_VALUES = [17,19,21,23]

# Round a number up to the nearest power of 2
def round_pow2(n):
    return 2 ** math.ceil(math.log2(n))

# Generate a formatted string for a rotation operation
def ro_line(ro_value, direction):
    return f"ro{direction[0].lower()} ${ro_value}"

# Replace occurrences of a string in a file
def sed_file(filepath, old, new):
    # Open the file in read/write mode
    with open(filepath, 'r+') as file:
        content = file.read().replace(old, new)  # Replace old with new
        file.seek(0)
        file.write(content)
        file.truncate()

# Replace occurrences of a string in all files within a folder with specific extensions
def sed_files(folderpath, old, new, extension=[".nasm", ".c", ".h"]):
    for filename in os.listdir(folderpath):
        filepath = os.path.join(folderpath, filename)
        # Check if the file matches the specified extensions
        if os.path.isfile(filepath) and (filename.endswith(tuple(extension))):
            sed_file(filepath, old, new)

# Convert a NASM file into shellcode instructions
def nasm2instructions(filepath):
    print("\nSetting up shellcode...")
    obj_file = "build/shell.o"  # Intermediate object file
    shell_file = "build/shell"  # Final shell executable
    instructions_file = "build/instructions_trash"  # Temporary file for instructions

    # Commands to assemble, link, and extract instructions
    nasm_cmd = ["nasm", "-f", "elf64", filepath, "-o", obj_file]
    ld_cmd = ["ld", "-w", obj_file, "-o", shell_file]
    dd_cmd = ["dd", f"if={shell_file}", f"of={instructions_file}", "bs=1", "skip=4096", "count=1280"]
    xxd_cmd = ["xxd", "-p", instructions_file]
    try:
        # Run NASM assembler
        result = subprocess.run(nasm_cmd, check=True)

        # Run linker
        result = subprocess.run(ld_cmd, check=True)

        # Extract instructions using dd
        subprocess.run(dd_cmd, check=True)

        # Convert binary instructions to hexadecimal
        result = subprocess.run(f"xxd -p {instructions_file}", shell=True, check=True, capture_output=True, text=True)
        hexa_content = result.stdout.strip()

        # Extract and clean up the instructions
        instructions = hexa_content.split("00000000000000")[0].replace("\n", "")
        return instructions
    
    except subprocess.CalledProcessError as e:
        print(f"Erreur lors de l'exécution de la commande : {e}")
        return None

# Read instructions from a text file
def txt2instructions(filepath):
    with open(filepath, 'r') as file:
        content = file.read()
        return content

# Read instructions from a binary file
def bin2instructions(filepath):
    with open(filepath, 'rb') as file:
        content = file.read()
        return content.hex()

def dll2instructions(filepath):
    with open(filepath, 'rb') as file:
        content = file.read()
        return content.hex()

# Format instructions into a specific string format
def format_instructions(instructions):
    if len(instructions) == 0:
        return ""
    else:
        return "\\x" + "\\x".join(instructions[i:i+2] for i in range(0, len(instructions), 2))

# Extract tags from a file
def extract_tags_from_file(filepath):
    tags = []
    with open(filepath, 'r') as file:
        content = file.read()
        tags = re.findall(r'%[^%]+%', content)
    return tags

# Extract tags from all files within a folder
def extract_tags_from_folder(folderpath):
    result = []
    for filename in os.listdir(folderpath):
        filepath = os.path.join(folderpath, filename)
        if os.path.isfile(filepath) and (filename.endswith(".nasm") or filename.endswith(".c") or filename.endswith(".h")):
            result.append({"filename": filename, "tags": extract_tags_from_file(filepath)})
    return result

# XOR encryption/decryption with a single byte key
def xor_encrypt_decrypt(data, byte_key):

    data_bytes = bytes.fromhex(data)  

    # Perform XOR operation on each byte
    result = bytearray()
    for i in range(len(data_bytes)):
        result.append(data_bytes[i] ^ byte_key)  # XOR between data and key

    # Return the result in hexadecimal
    return result.hex()

# XOR encryption/decryption with a 16-bit word key
def xor2_encrypt_decrypt(data, word_key):
    # Convert the hexadecimal string to bytes
    data_bytes = bytes.fromhex(data)

    # Split the 16-bit key (WORD) into two bytes
    key_bytes = [word_key & 0xFF, (word_key >> 8) & 0xFF]

    # Apply XOR operation on each byte, alternating key bytes
    result = bytearray()
    for i in range(len(data_bytes)):
        result.append(data_bytes[i] ^ key_bytes[i % 2])

    # Return the result in hexadecimal
    return result.hex()

# Format RHOST and RPORT into a specific hexadecimal format
def format_rhost_rport(rhost, rport):
    if rport > 65535 or rport < 0:
        print("Port number must be between 0 and 65535")
    if rhost.count(".") != 3:
        print("Invalid IP address")

    ip = rhost.split(".")
    result = ""
    for part in ip[::-1]:
        hex_part = hex(int(part))[2:].zfill(2)  # Convert to hex and pad if necessary
        result += hex_part
    
    port_hex = hex(rport)[2:].zfill(4)
    port_hex = port_hex[2:] + port_hex[:2]

    result += port_hex
    print(f"RHOST={rhost}, RPORT={rport} -> {result}")
    return hex(int(result, 16))

# Validate an IP address
def is_valid_ip(ip):
    pattern = r'^((25[0-5]|2[0-4]\d|1\d{2}|[1-9]?\d)(\.|$)){4}$'
    return bool(re.match(pattern, ip))

# Resolve an IP address from a hostname
def resolve_ip(ip):
    try:
        ipaddress.ip_address(ip)
        return ip
    except ValueError:
        try:
            return socket.gethostbyname(ip)
        except socket.gaierror:
            return None
        
# Generate a hash object with random rotation values and direction
def hash_obj(module, function, verbose):
    direction = random.choice(["R", "L"])
    direction_word = "true" if direction == "R" else "false"
    value = random.choice(WORKING_RO_VALUES) # will test more
    return "{"+str(hex(hash(module, function, value, direction, verbose)))+", "+str(value)+", "+direction_word+"}"

# Generate a high entropy integer within a specified range
def generate_high_entropy_int(min_val=0x1111, max_val=0xFFFF):
    while True:
        num = random.randint(min_val, max_val)
        hex_digits = [int(d, 16) for d in f"{num:X}"]
        
        avg = sum(hex_digits) / len(hex_digits)
        total = sum(hex_digits)
        
        min_avg, max_avg = min_val / 0xFFFF * 12, max_val / 0xFFFF * 12
        min_total, max_total = min_val / 0xFFFF * 50, max_val / 0xFFFF * 50
        
        if min_avg <= avg <= max_avg and min_total <= total <= max_total:  # Good distribution
            return num
        
# Get LCID (Locale ID) for a given country code
def get_LCID(country_code):
    result = []
    with open("data/lcid.yaml", "r") as file:
        LANGUAGES = yaml.safe_load(file)

    country_code = country_code.upper()
    for key,element in LANGUAGES.items():
        if country_code in element:
            result.append(int(key))

    if len(result) == 0:
        raise ValueError(f"'{country_code}' is not valid.")
    
    return result

# Format LHOST and LPORT into a specific hexadecimal format
def format_lhost_lport(lhost, lport):
    if lport > 65535 or lport < 0:
        print("Port number must be between 0 and 65535")
    if lhost.count(".") != 3:
        print("Invalid IP address")

    ip = lhost.split(".")
    result = ""
    for part in ip[::-1]:
        result += hex(int(part))[2:]
    
    port_hex = hex(lport)[2:].zfill(4)
    port_hex = port_hex[2:] + port_hex[:2]  # Swap bytes

    result += port_hex
    return hex(int(result, 16))

def compile_dll(DLL_NAME):
    print("Compiling DLL...")
    subprocess.run(["bash", "dll/compile.sh", DLL_NAME], check=True)
    print("DLL compiled successfully.")
