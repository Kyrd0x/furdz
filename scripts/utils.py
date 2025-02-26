import subprocess
import ipaddress
import socket
import re
import os


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

def nasm2instructions(filepath):
    # with open("asm/test.txt",'r') as f:
    #     content = f.read()
    #     print(content)
    #     return content
    print("\nSetting up shellcode...")
    obj_file = "temp/shell.o"
    shell_file = "temp/shell"
    instructions_file = "temp/instructions_trash"

    nasm_cmd = ["nasm", "-f", "elf64", filepath, "-o", obj_file]
    ld_cmd = ["ld", "-w", obj_file, "-o", shell_file]
    dd_cmd = ["dd", f"if={shell_file}", f"of={instructions_file}", "bs=1", "skip=4096", "count=1280"]
    xxd_cmd = ["xxd", "-p", instructions_file]
    try:
        result = subprocess.run(nasm_cmd, check=True)

        result = subprocess.run(ld_cmd, check=True)

        subprocess.run(dd_cmd, check=True)

        result = subprocess.run(f"xxd -p {instructions_file}", shell=True, check=True, capture_output=True, text=True)
        hexa_content = result.stdout.strip()

        instructions = hexa_content.split("00000000000000")[0].replace("\n", "")
        return instructions
    
    except subprocess.CalledProcessError as e:
        print(f"Erreur lors de l'exécution de la commande : {e}")
        return None
       
def format_instructions(instructions):
    return "\\x" + "\\x".join(instructions[i:i+2] for i in range(0, len(instructions), 2))

def extract_tags_from_file(filepath):
    tags = []
    with open(filepath, 'r') as file:
        content = file.read()
        tags = re.findall(r'%[^%]+%', content)
    print(f"Tags found in {filepath}: {tags}")
    return tags

def extract_tags_from_folder(folderpath):
    result = []
    for filename in os.listdir(folderpath):
        filepath = os.path.join(folderpath, filename)
        if os.path.isfile(filepath):
            result.append({"filename": filename, "tags": extract_tags_from_file(filepath)})

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
        print(f"{part} -> {hex_part}")
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