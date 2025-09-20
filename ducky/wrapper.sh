#!/bin/bash

# Default values
DEFAULT_INPUT="payload.duck"
DEFAULT_OUTPUT="payload/payload.ino"
INPUT_FILE=""
OUTPUT_FILE="$DEFAULT_OUTPUT"

TEMP_BIN_FILE="payload.bin"

# Help function
show_help() {
    echo "DuckyScript to Arduino Payload Converter"
    echo ""
    echo "Usage: $0 -i INPUT_FILE [-o OUTPUT_FILE] [-h]"
    echo ""
    echo "Options:"
    echo "  -i INPUT_FILE    Input file (.duck or .txt) (required)"
    echo "  -o OUTPUT_FILE   Output file (.ino) (default: $DEFAULT_OUTPUT)"
    echo "  -h               Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 -i script.duck"
    echo "  $0 -i script.txt -o custom/custom.ino"
}

# Parse command line arguments
while getopts "i:o:h" opt; do
    case $opt in
        i)
            INPUT_FILE="$OPTARG"
            ;;
        o)
            OUTPUT_FILE="$OPTARG"
            ;;
        h)
            show_help
            exit 0
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            show_help
            exit 1
            ;;
    esac
done

# Check if input file is provided
if [ -z "$INPUT_FILE" ]; then
    echo "Error: Input file is required"
    show_help
    exit 1
fi

# Check if input file exists
if [ ! -f "$INPUT_FILE" ]; then
    echo "Error: Input file '$INPUT_FILE' not found"
    exit 1
fi

# Check file extension
if [[ ! "$INPUT_FILE" =~ \.(duck|txt)$ ]]; then
    echo "Error: Input file must be .duck or .txt"
    exit 1
fi

# Create output directory if it doesn't exist
OUTPUT_DIR=$(dirname "$OUTPUT_FILE")
rm -rf "$OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR"


echo "Converting $INPUT_FILE to Arduino payload..."

# Convert .duck/.txt to .bin
python3 duck2ino.py "$INPUT_FILE" -o $TEMP_BIN_FILE

# Check if conversion was successful
if [ $? -ne 0 ]; then
    echo "Error: Failed to convert input file to binary"
    exit 1
fi

# Convert .bin to .ino
python3 duck2spark.py -i $TEMP_BIN_FILE -o "$OUTPUT_FILE"

# Check if conversion was successful
if [ $? -ne 0 ]; then
    echo "Error: Failed to convert binary to Arduino payload"
    exit 1
fi

# Clean up temporary file
rm -f $TEMP_BIN_FILE

echo "Payload generated successfully: $OUTPUT_FILE"