#!/bin/bash

PAYLOAD=$1
VERBOSE=false

if [[ -z "$PAYLOAD" ]]; then
  echo "Error: No DLL payload provided."
  echo "Usage: $0 <payload>"
  exit 1
fi

if [[ ! -f "build/src/dll/$PAYLOAD.c" ]]; then
  echo "Error: Source file build/src/dll/$PAYLOAD.c does not exist."
  exit 1
fi

if [[ "$PAYLOAD" == "main" ]]; then
  echo "Error: payload.type cannot be 'main'."
  exit 1
fi

SRC=(
  build/src/common/*.c
  build/src/dll/main.c
  build/src/dll/$PAYLOAD.c
)
HEADERS=(
    build/include/common/*.h
    build/include/dll/entrypoint.h
    build/include/dll/$PAYLOAD.h
)
OUTPUT_FILE="build/bin/injected-dll.dll"

# Log the compile command
# echo "Using SRC: ${SRC[@]}"
# echo "Using HEADERS: ${HEADERS[@]}"

x86_64-w64-mingw32-gcc -s -shared -ffreestanding \
  -Ibuild/include/common -Ibuild/include/dll \
  -o $OUTPUT_FILE \
  ${SRC[@]} \
  -e DllMain \
  -lws2_32 -nostdlib -lkernel32 -luser32 -lmsvcrt

if [[ $? -ne 0 ]]; then
    echo "Failed to compile DLL."
    exit 1
fi

strip --strip-all $OUTPUT_FILE # cause why not