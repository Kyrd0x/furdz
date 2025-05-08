#!/bin/bash

SRC=(
  build/src/common/*.c
  build/src/dll/*.c
)
HEADERS=(
    build/include/common/*.h
    build/include/dll/*.h
)
OUTPUT_FILE="build/bin/injected-dll.dll"

x86_64-w64-mingw32-gcc -s -shared -ffreestanding \
  -Ibuild/include/common -Ibuild/include/dll \
  -o $OUTPUT_FILE \
  ${SRC[@]} \
  -e DllMain \
  -lws2_32 -nostdlib -lkernel32 -lmsvcrt

if [[ $? -ne 0 ]]; then
    echo "Failed to compile DLL."
    exit 1
fi

strip --strip-all $OUTPUT_FILE # cause why not