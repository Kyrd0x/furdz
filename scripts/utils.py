import subprocess
import re

"""
Input : raw shellcode with tags

Output : shellcode with tags replaced by hashes and hash function
"""

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
    ld_cmd = ["ld", "-w", obj_file, "-o", shell_file]
    dd_cmd = ["dd", f"if={shell_file}", "of=temp/instructions_trash", "bs=1", "skip=4096", "count=1280"]
    try:
        subprocess.run(nasm_cmd, check=True)
        subprocess.run(ld_cmd, check=True, stderr=subprocess.DEVNULL)
        subprocess.run(dd_cmd, check=True, stderr=subprocess.DEVNULL)

        result = subprocess.run("xxd -p temp/instructions_trash", shell=True, check=True, capture_output=True, text=True)
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
