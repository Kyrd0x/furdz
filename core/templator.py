import os
import re

# from src/templates/*
# template pattern is:
# //__<TAG>__//
# <content>
# //__END_<TAG>__//
def get_template(tag):
    for file in os.listdir("src/templates"):
        if file.endswith(".c"):
            with open(os.path.join("src/templates", file), "r") as f:
                content = f.read()
                pattern = re.compile(rf"//__{tag}__//(.*?)//__END_{tag}__//", re.DOTALL)
                match = pattern.search(content)
                if match:
                    return match.group(1).strip()


class Templator:
    """Class to handle templating operations.
    This can be:
        - Replacing config values (LHOST, LPORT, etc)
        - Adding anti-sandbox checks (RAM, CPU, etc)
        - Adding evasions (ETW, AMSI, etc)
        - Replacing hashes (function, module, etc)
    """
    pass