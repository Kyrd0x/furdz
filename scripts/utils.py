from scripts.hasher import hash
import subprocess
import random
import math
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

def xor1_encrypt_decrypt(data, key):
    # Convert the hexadecimal string to bytes
    data_bytes = bytes.fromhex(data)

    # Apply XOR operation on each byte with the key
    result = bytearray()
    for byte in data_bytes:
        result.append(byte ^ key)

    # Return the result in hexadecimal
    return result.hex()

# XOR encryption/decryption with a WORD key
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

# Run the dll/compile.sh script to compile the DLL to inject
def compile_dll(DLL_NAME):
    subprocess.run(["bash", "scripts/compile_dll.sh", DLL_NAME], check=True)