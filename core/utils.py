from .hasher import hash
import subprocess
import random
import math
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

# Format instructions into a specific string format
def format_instructions(instructions):
    if len(instructions) == 0:
        return ""
    else:
        return "\\x" + "\\x".join(instructions[i:i+2] for i in range(0, len(instructions), 2))

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

def hash_obj_null():
    return "{0, 0, false}"

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
    with open("data/lcid.csv", "r") as file:
        # pattern is as an example:
        # 4;en_US
        LANGUAGES = {}
        for line in file:
            parts = line.strip().split(";")
            if len(parts) == 2:
                LANGUAGES[parts[0]] = parts[1]

    country_code = country_code.upper()
    for key,element in LANGUAGES.items():
        if country_code in element:
            result.append(int(key))

    if len(result) == 0:
        raise ValueError(f"'{country_code}' is not valid.")
    
    return result
