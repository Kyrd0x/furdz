from scripts.hasher import hash
import ipaddress
import locale
import random
import socket
import math
import json
import yaml
import re
import os

WORKING_RO_VALUES = [13,17,19,21,23]

def round_pow2(n):
    return 2 ** math.ceil(math.log2(n))

def ro_line(ro_value, direction):
    return f"ro{direction[0].lower()} ${ro_value}"

def sed_file(filepath, old, new):
    with open(filepath, 'r+') as file:
        content = file.read().replace(old, new)
        file.seek(0)
        file.write(content)
        file.truncate()

def sed_files(folderpath, old, new):
    for filename in os.listdir(folderpath):
        filepath = os.path.join(folderpath, filename)
        if os.path.isfile(filepath):
            sed_file(filepath, old, new)


def txt2instructions(filepath):
    with open(filepath, 'r') as file:
        content = file.read()
        return content

def bin2instructions(filepath):
    with open(filepath, 'rb') as file:
        content = file.read()
        return content.hex()

def format_instructions(instructions):
    if len(instructions) == 0:
        return ""
    else:
        return "\\x" + "\\x".join(instructions[i:i+2] for i in range(0, len(instructions), 2))

def extract_tags_from_file(filepath):
    tags = []
    print(f"opening file {filepath}")
    with open(filepath, 'r') as file:
        content = file.read()
        tags = re.findall(r'%[^%]+%', content)
    return tags

def extract_tags_from_folder(folderpath):
    result = []
    for filename in os.listdir(folderpath):
        filepath = os.path.join(folderpath, filename)
        if os.path.isfile(filepath) and (filename.endswith(".nasm") or filename.endswith(".c") or filename.endswith(".h")):
            result.append({"filename": filename, "tags": extract_tags_from_file(filepath)})
    return result

def xor_encrypt_decrypt(data, byte_key):

    data_bytes = bytes.fromhex(data)  

    # Effectuer l'opération XOR sur chaque byte
    result = bytearray()
    for i in range(len(data_bytes)):
        result.append(data_bytes[i] ^ byte_key)  # XOR entre data et key

    # Retourner le résultat en hexadécimal
    return result.hex()

def xor2_encrypt_decrypt(data, word_key):
    # Convertir la chaîne hexadécimale en bytes
    data_bytes = bytes.fromhex(data)

    # Diviser la clé de 16 bits (WORD) en deux octets
    key_bytes = [word_key & 0xFF, (word_key >> 8) & 0xFF]

    # Appliquer l'opération XOR sur chaque byte, en alternant les octets de la clé
    result = bytearray()
    for i in range(len(data_bytes)):
        result.append(data_bytes[i] ^ key_bytes[i % 2])

    # Retourner le résultat sous forme hexadécimale
    return result.hex()


def format_lhost_lport(lhost, lport):
    if lport > 65535 or lport < 0:
        print("Port number must be between 0 and 65535")
    if lhost.count(".") != 3:
        print("Invalid IP address")

    ip = lhost.split(".")
    result = ""
    for part in ip[::-1]:
        hex_part = hex(int(part))[2:].zfill(2)  # Convertir en hex et remplir si nécessaire
        result += hex_part
    
    port_hex = hex(lport)[2:].zfill(4)
    port_hex = port_hex[2:] + port_hex[:2]

    result += port_hex
    print(f"LHOST={lhost}, LPORT={lport} -> {result}")
    return hex(int(result, 16))

def is_valid_ip(ip):
    pattern = r'^((25[0-5]|2[0-4]\d|1\d{2}|[1-9]?\d)(\.|$)){4}$'
    return bool(re.match(pattern, ip))

def resolve_ip(ip):
    try:
        ipaddress.ip_address(ip)
        return ip
    except ValueError:
        try:
            return socket.gethostbyname(ip)
        except socket.gaierror:
            return None
        
def hash_obj(module, function):
    direction = random.choice(["R", "L"])
    direction_word = "true" if direction == "R" else "false"
    value = random.choice(WORKING_RO_VALUES) # will test more
    return "{"+str(hex(hash(module, function, value, direction)))+", "+str(value)+", "+direction_word+"}"

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