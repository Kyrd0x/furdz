import argparse
import os

PAYLOAD_DIR = "src/dll/payloads"

# Custom formatter: show defaults normally, but DO NOT append "(default: ...)"
# for arguments whose dest starts with "payload_" or which are evasions.
class _Fmt(argparse.ArgumentDefaultsHelpFormatter, argparse.RawDescriptionHelpFormatter):
    # Add here any dest names for which we don't want the "(default: ...)" suffix
    _no_default_dests = {"etw", "ntdll"}

    def _get_help_string(self, action):
        dest = getattr(action, "dest", "")
        # Hide defaults for dynamic payload flags (dest starts with payload_)
        # and for explicit evasions listed in _no_default_dests.
        if dest.startswith("payload_") or dest in self._no_default_dests:
            return action.help
        return argparse.ArgumentDefaultsHelpFormatter._get_help_string(self, action)


EPILOG = """Examples:
  build.py -o evil.exe --ransom --etw -v
  build.py --small -o demo.exe --debug
  build.py --help
"""

DESCRIPTION = """Usage:
  build.py [options] --<payload>
"""

def _discover_payloads(payload_dir: str):
    names = []
    if os.path.isdir(payload_dir):
        for f in sorted(os.listdir(payload_dir)):
            if f.endswith(".c"):
                names.append(f[:-2])
    return names

def get_selected_payload(args):
    payload = None
    for name in _discover_payloads(PAYLOAD_DIR):
        if getattr(args, f"payload_{name}", False):
            payload = name
            break
    assert payload is not None  # devrait être garanti par parse_args()
    return payload

def parse_args(argv=None):
    # We disable the automatic -h so we can add it inside our "Options" group.
    parser = argparse.ArgumentParser(
        description=DESCRIPTION,
        epilog=EPILOG,
        formatter_class=_Fmt,
        add_help=False,
    )

    # ----- Options (group) -----
    opt = parser.add_argument_group("Options")
    # Manually add help so it appears under the "Options" header
    opt.add_argument("-h", "--help", action="help", help="show this help message and exit")
    opt.add_argument("-o", "--output", type=str, default="executable.exe",
                     help="Output file name")
    opt.add_argument("-s", "--small", action="store_true",
                     help="Prioritize size over other factors")
    opt.add_argument("-v", "--verbose", action="store_true",
                     help="Enable verbose mode")

    # ----- Payloads (group + mutually exclusive) -----
    payload_group = parser.add_argument_group(
        "Payload",
        "Exactly one payload must be specified. "
        f"Payload flags map to {PAYLOAD_DIR}/<name>.c"
    )
    mex = payload_group.add_mutually_exclusive_group(required=False)

    payload_names = _discover_payloads(PAYLOAD_DIR)
    for name in payload_names:
        # dest names start with "payload_" so our custom formatter can detect them
        mex.add_argument(f"--{name}", dest=f"payload_{name}",
                         action="store_true", help=f"Use payload '{name}'")


    # ----- Evasions (separate group) -----
    evasions = parser.add_argument_group("Evasions")
    evasions.add_argument("--etw", action="store_true", help="Disable ETW")
    # evasions.add_argument("--ntdll", action="store_true", help="Overwrite ntdll from disk")


    # You previously used a "command" idea; keep it if you need commands later.
    # For now we keep no subcommands, so just parse.
    args = parser.parse_args(argv)

    # ----- Validation: require exactly one payload -----
    selected = [k for k, v in vars(args).items() if k.startswith("payload_") and v]
    if len(selected) != 1:
        if payload_names:
            choices = ", ".join(f"--{n}" for n in payload_names)
            parser.error(f"Exactly one payload must be specified. Available: {choices}")
        else:
            parser.error(f"No payloads found in {PAYLOAD_DIR}")

    args.payload_name = selected[0][8:]  # drop "payload_" prefix

    return args