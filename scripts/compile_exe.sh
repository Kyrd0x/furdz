#!/bin/bash

# Exit early on any failure
set -euo pipefail

# usage : ./compile_exe.sh <output_file> <prioritize_size:true|false> <verbose:true|false>
OUTPUT_FILE="$1"
PRIORIZE_SIZE="$2"
VERBOSE="$3"

if [[ "$VERBOSE" == "true" ]]; then
    echo "⚙️  Compiling with options:"
    echo "   - Output file: ${OUTPUT_FILE:-furdz.exe}"
    echo "   - Prioritize size: ${PRIORIZE_SIZE:-false}"
    echo "   - Verbose: ${VERBOSE:-false}"
fi

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

# Log the compile command if verbose
if [[ "$VERBOSE" == "true" ]]; then
    echo "Using SRC: ${SRC[@]}"
    echo "Using HEADERS: ${HEADERS[@]}"
    echo "Using CFLAGS: ${CFLAGS[@]}"
    echo "Using WARNINGS: ${WARNINGS[@]}"
    echo "Using LIBS: ${LIBS[@]}"
    echo "Using OBFUSCATION: ${OBFUSCATION[@]}"
    echo "Using LDFLAGS: ${LDFLAGS[@]}"
fi

# Compile the main program
$CC "${CFLAGS[@]}" "${WARNINGS[@]}" "${SRC[@]}" -static -static-libgcc -o "bin/$OUTPUT_FILE" "${LDFLAGS[@]}" "${LIBS[@]}" "${OBFUSCATION[@]}"

echo "✅ Build completed. Output file: bin/$OUTPUT_FILE"
echo "🔍 Size: $(du -h bin/$OUTPUT_FILE | cut -f1)"
echo "📦 DLL size: $(du -h build/bin/injected-dll.dll | cut -f1 || echo 'N/A')"