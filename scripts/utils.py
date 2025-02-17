# from ROR import hash
import subprocess
import time
import sys

"""
Input : raw shellcode with tags

Output : shellcode with tags replaced by hashes and hash function
"""

def cp_file(source, destination):
    nasm_cmd = ["cp", source, destination]
    try:
        subprocess.run(nasm_cmd, check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error: cp {source} {destination} failed!\n{e}")

def sed_file(filepath, old, new):
    with open(filepath, 'r+') as file:
        content = file.read().replace(old, new)
        file.seek(0)
        file.write(content)
        file.truncate()

def nasm2instructions(filepath):
    obj_file = "temp/shell.o"
    shell_file = "temp/shell"

    nasm_cmd = ["nasm", "-f", "elf64", filepath, "-o", obj_file]
    ld_cmd = ["ld", obj_file, "-o", shell_file]
    dd_cmd = ["dd", "if", shell_file, "of", "temp/instructions_trash", "bs", "1", "skip", "$((0x1000))", "count", "$((0x500))"]
    try:
        print("here")
        subprocess.run(nasm_cmd, check=True)
        # Lier le fichier objet
        subprocess.run(ld_cmd, shell=True, check=True, stderr=subprocess.DEVNULL)
        # Extraire une partie du binaire
        subprocess.run(dd_cmd, shell=True, check=True, stderr=subprocess.DEVNULL)

        # Convertir en hexadécimal
        result = subprocess.run("xxd -p temp/instructions_trash", shell=True, check=True, capture_output=True, text=True)
        hexa_content = result.stdout.strip()
        print(hexa_content)
        

        # Extraire les instructions avant `00000000000000`
        instructions = hexa_content.split("00000000000000")[0]

        return instructions
    except subprocess.CalledProcessError as e:
        print(f"Erreur lors de l'exécution de la commande : {e}")
        return None
       
