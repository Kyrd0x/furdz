#!/bin/bash

if [[ $# -lt 1 ]]; then
    echo "Usage: $0 <DLL>"
    exit 1
fi

DLL=$1

x86_64-w64-mingw32-gcc -s -shared -ffreestanding -o build/injected-dll.dll build/$DLL -e DllMain -lws2_32 -nostdlib -lkernel32 -lmsvcrt

if [[ $? -ne 0 ]]; then
    echo "Failed to compile DLL."
    exit 1
fi
strip --strip-all build/injected-dll.dll # cause why not