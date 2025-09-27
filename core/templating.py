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
