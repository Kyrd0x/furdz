#!/bin/bash


#-----------CONFIGURATION-----------

payload_file="shellcode.nasm"
stub_file="temp/loader.c"

#-----------SHELLCODE---------------

nasm -f elf64 asm/$payload_file -o temp/shell.o
ld temp/shell.o -o temp/shell 2>/dev/null
dd if=temp/shell of=temp/instructions_trash bs=1 skip=$((0x1000)) count=$((0x500)) 2>/dev/null

hexa_content=$(xxd -p temp/instructions_trash)

instructions="${hexa_content%%00000000000000*}"

# on passe de fc4881 à \\xfc\\x48\\x81
tmp_instr=$(echo "$instructions" | tr -d '\n' | sed 's/\(..\)/\\x\1/g')
C_SHELLCODE=$(echo "$tmp_instr" | sed 's/\\/\\\\/g')
PATTERN_SHELLCODE="unsigned char msgbox_payload[] = \"$C_SHELLCODE\";"

echo -e "$PATTERN_SHELLCODE\n"

# Utiliser sed pour remplacer la chaîne "%SHELLCODE%" dans input.txt;
sed -i "s/%SHELLCODE%/$PATTERN_SHELLCODE/g" $stub_file

