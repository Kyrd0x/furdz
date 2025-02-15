CC = x86_64-w64-mingw32-gcc
CFLAGS = -Itemp -Wall -Wextra -O2 -mwindows -nostdlib -ffreestanding -mconsole -mno-stack-arg-probe
LIBS = -luser32 -lkernel32 -lshell32
LINKER_FLAGS = -Xlinker --stack=0x100000,0x100000

SRC = temp/loader.c
EXE = bin/calculatrice.exe


.PHONY: all clean pre-build

all: pre-build $(EXE)

pre-build:
	@mkdir -p bin build temp
	@cp src/* temp/
	@sh comp.sh

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $(LIBS) $(LINKER_FLAGS) $^ -o $@

clean:
	rm -rf bin build temp