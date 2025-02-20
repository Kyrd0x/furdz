CC = x86_64-w64-mingw32-gcc
CFLAGS = -Itemp -Wall -Wextra -O2 -ffreestanding -mconsole -mno-stack-arg-probe
LIBS = -luser32 -lkernel32 -lshell32 -ladvapi32
LINKER_FLAGS = -Xlinker --stack=0x100000,0x100000
OBFUSCATION=

SHELLCODE=asm/shellcode.nasm
SRC = temp/loader.c #temp/anti-s.c
EXE = bin/calculatrice.exe


.PHONY: all clean pre-build

all: pre-build $(EXE)

pre-build:
	@mkdir -p bin build temp
	@cp src/* temp/
	@cp asm/* temp/
	@python3 main.py

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $(LIBS) $(LINKER_FLAGS) $^ -o $@

clean:
	rm -rf bin build temp