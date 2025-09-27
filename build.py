#!/usr/bin/env python3
import argparse
import os
from core.orchestrator import *   # à remplacer par imports ciblés
from core.utils import *          # idem: éviter import *

PAYLOAD_DIR = "src/dll/payloads"

class _Fmt(argparse.ArgumentDefaultsHelpFormatter, argparse.RawDescriptionHelpFormatter):
    pass

""" usage() { echo "Usage: $0 [options] --<payload> | clean" echo "" echo "Options:" echo " -o, --output <output_file> Specify the output file name (default: executable.exe)" echo " -s, --small Prioritize size over detection (no icon & no stdlib)" echo " -v, --verbose Enable verbose mode" echo " -h, --help Show this help message" echo "" echo "Payload:" echo " --<payload> Exactly one payload must be specified." echo " Maps to src/dll/<payload>.c (e.g., --foo -> src/dll/foo.c)" echo "" echo "Evasion:" echo " --etw Disable ETW" echo " --ntdll Overwrite ntdll from disk" echo "" echo "Available payloads:" if compgen -G "src/dll/payloads/*.c" >/dev/null; then for f in src/dll/payloads/*.c; do printf ' --%s\n' "$(basename "$f" .c)" done else echo " (none found in src/dll/payloads)" fi echo "" echo "Commands:" echo " clean Clean the build directory" echo " setup Setup the build environment" echo "" echo "Examples:" echo " $0 --small -o out.exe --foo" echo " $0 --help" echo " $0 clean" exit 1 } """

EPILOG = """Examples:
  build.py --small -o out.exe --foo
  build.py --help
  build.py clean
"""

DESCRIPTION = """Usage:
  build.py [options] --<payload>
  build.py clean|setup

Exactly one payload must be specified unless you run a command.
"""

def _discover_payloads(payload_dir: str):
    names = []
    if os.path.isdir(payload_dir):
        for f in sorted(os.listdir(payload_dir)):
            if f.endswith(".c"):
                names.append(f[:-2])  # nom sans .c
    return names

def parse_args(argv=None):
    parser = argparse.ArgumentParser(
        description=DESCRIPTION,
        epilog=EPILOG,
        formatter_class=_Fmt,
    )

    # ----- Options (groupe) -----
    opt = parser.add_argument_group("Options")
    opt.add_argument("-o", "--output", type=str, default="executable.exe",
                     help="Output file name")
    opt.add_argument("-s", "--small", action="store_true",
                     help="Prioritize size over other factors")
    opt.add_argument("-v", "--verbose", action="store_true",
                     help="Enable verbose mode")

    # ----- Payloads (groupe + mutually exclusive) -----
    payload_group = parser.add_argument_group(
        "Payload",
        "Exactly one payload must be specified (unless you use a command). "
        f"Payload flags map to {PAYLOAD_DIR}/<name>.c"
    )
    mex = payload_group.add_mutually_exclusive_group(required=False)

    payload_names = _discover_payloads(PAYLOAD_DIR)
    for name in payload_names:
        mex.add_argument(f"--{name}", dest=f"payload_{name}",
                         action="store_true", help=f"Use payload '{name}'")

    # ----- Commands (subparsers) -----
    subparsers = parser.add_subparsers(dest="command", metavar="command")

    sp_clean = subparsers.add_parser("clean", help="Clean the build directory")
    sp_setup = subparsers.add_parser("setup", help="Setup the build environment")

    args = parser.parse_args(argv)

    # ----- Validation: si pas de commande, exiger exactement 1 payload -----
    if args.command is None:
        selected = [k for k, v in vars(args).items() if k.startswith("payload_") and v]
        if len(selected) != 1:
            # Construit un message d’erreur clair avec les payloads dispo
            if payload_names:
                choices = ", ".join(f"--{n}" for n in payload_names)
                parser.error(f"Exactly one payload must be specified. Available: {choices}")
            else:
                parser.error(f"No payloads found in {PAYLOAD_DIR}")

    return args

def main():
    args = parse_args()

    if args.command == "clean":
        os.system("bash scripts/clean.sh")
        print("Build directory cleaned.")
        return

    if args.command == "setup":
        os.system("bash scripts/setup.sh")
        return

    # Ici on est en mode "build" avec exactement 1 payload sélectionné
    # Récupère le nom du payload choisi
    payload = None
    for name in _discover_payloads(PAYLOAD_DIR):
        if getattr(args, f"payload_{name}", False):
            payload = name
            break

    # Exemple d’utilisation :
    if args.verbose:
        print(f"[i] building with payload='{payload}', output='{args.output}', small={args.small}")

    # TODO: appelle ton orchestrateur avec payload / options
    # run_build(payload=payload, output=args.output, small=args.small, verbose=args.verbose)
    print(f"Build completed: {args.output}")

if __name__ == "__main__":
    main()
