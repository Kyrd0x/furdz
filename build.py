#!/usr/bin/env python3
import argparse
import os


"""
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
"""

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", "--output", type=str, default="executable.exe", help="Specify the output file name (default: executable.exe)")
    parser.add_argument("-s", "--small", action="store_true", help="Prioritize size over detection (no icon & no stdlib)")
    parser.add_argument("-v", "--verbose", action="store_true", help="Enable verbose mode")
    parser.add_argument("--etw", action="store_true", help="Disable ETW")
    parser.add_argument("--ntdll", action="store_true", help="Overwrite ntdll from disk")
    parser.add_argument("command", nargs='?', choices=["clean"], help="Clean the build directory")
    
    # Dynamically add payload arguments based on files in src/dll/payloads
    payload_dir = "src/dll/payloads"
    if os.path.isdir(payload_dir):
        for f in os.listdir(payload_dir):
            if f.endswith(".c"):
                payload_name = f[:-2]  # Remove .c extension
                parser.add_argument(f"--{payload_name}", action="store_true", help=f"Use the {payload_name} payload")
    
    args = parser.parse_args()
    
    # Ensure exactly one payload is specified if command is not 'clean'
    if args.command != "clean":
        payload_args = [arg for arg in vars(args) if getattr(args, arg) and arg not in ["output", "small", "verbose", "etw", "ntdll", "command"]]
        if len(payload_args) != 1:
            parser.error("Exactly one payload must be specified.")
    
    return args

def main():
    args = parse_args()

if __name__ == "__main__":
    main()