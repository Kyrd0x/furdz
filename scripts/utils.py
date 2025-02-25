import subprocess
import re


def sed_file(filepath, old, new):
    with open(filepath, 'r+') as file:
        content = file.read().replace(old, new)
        file.seek(0)
        file.write(content)
        file.truncate()

def nasm2instructions(filepath):
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
    return tags

def xor_encrypt_decrypt(data, byte_key):

    data_bytes = bytes.fromhex(data)  

    # Effectuer l'opération XOR sur chaque byte
    result = bytearray()
    for i in range(len(data_bytes)):
        result.append(data_bytes[i] ^ byte_key)  # XOR entre data et key

    # Retourner le résultat en hexadécimal
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