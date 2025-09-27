#!/bin/bash

# Exit early on any failure
set -euo pipefail

# default values
OUTPUT_FILE="executable.exe" # -o | --output <output_file>
PRIORIZE_SIZE="false" # -p | --prioritize-size <true|false>

# retrieve arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -o|--output)
            OUTPUT_FILE="$2"
            shift 2
            ;;
        -p|--prioritize-size)
            PRIORIZE_SIZE="true"
            shift
            ;;
        *)
            echo "Error: Unknown argument: $1"
            usage
            ;;
    esac
done
    


# Define source and header files
shopt -s nullglob
SRC=(
    build/src/common/*.c
    build/src/exe/*.c
)
HEADERS=(
    build/include/common/*.h
    build/include/exe/*.h
)
shopt -u nullglob

# Configure options based on PRIORIZE_SIZE
if [[ "$PRIORIZE_SIZE" == "true" ]]; then
    STDLIB="-nostdlib -DLIGHTWEIGHT=1"
    ICON=""
    ENTRYPOINT="WinMainCRTStartup"
else
    STDLIB=""
    ICON="build/icon.o"
    SRC+=("build/icon.o")
    ENTRYPOINT="WinMain"
fi

# Compile the icon if necessary
if [[ -n "$ICON" ]]; then
    x86_64-w64-mingw32-windres icon/icon.rc -o "$ICON"
fi

CC="x86_64-w64-mingw32-gcc"
CFLAGS=($STDLIB -Ibuild/include/common -Ibuild/include/exe -Wall -Wextra -Os -mwindows -ffunction-sections -fdata-sections -fno-stack-protector -fno-stack-check -fno-strict-aliasing -ffreestanding)
WARNINGS=(-Wtype-limits -Wno-cast-function-type -Wno-unused-parameter -Wno-unused-variable -Wattributes)
LIBS=(-lkernel32 -luser32 -ladvapi32 -lshell32 -lm -lgcc)
OBFUSCATION=(-s -fvisibility=hidden -fno-inline -fno-builtin -fno-ident)
LDFLAGS=(-Wl,--gc-sections,--entry=$ENTRYPOINT,--disable-auto-import,--no-insert-timestamp)


# Compile the main program
$CC "${CFLAGS[@]}" "${WARNINGS[@]}" "${SRC[@]}" -static -static-libgcc -o "bin/$OUTPUT_FILE" "${LDFLAGS[@]}" "${LIBS[@]}" "${OBFUSCATION[@]}"

echo "✅ Build completed. Output file: bin/$OUTPUT_FILE"
echo "🔍 Size: $(du -h bin/$OUTPUT_FILE | cut -f1)"
echo "📦 DLL size: $(du -h build/bin/injected-dll.dll | cut -f1 || echo 'N/A')"