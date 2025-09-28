import os
import re

class Templator:
    """Class to handle templating operations.
    This can be:
        - Replacing config values (LHOST, LPORT, etc)
        - Adding anti-sandbox checks (RAM, CPU, etc)
        - Adding evasions (ETW, AMSI, etc)
        - Replacing hashes (function, module, etc)

    All the files to sed are in src/*

    The pattern to replace is:
    //__<TAG>__//

    When reading bloc code from a template file, the pattern is:
    //__<TAG>__//
    <content>
    //__END_<TAG>__//
    """
    def __init__(self, working_folder="src/", verbose=False):
        self.working_folder = working_folder
        self.templates_folder = working_folder + "templates/"
        self.verbose = verbose
        self.pattern = re.compile(r"//__(\w+)__//")

    def check_pattern(self, text):
        return self.pattern.findall(text)

    # Replace occurrences of a string in a file
    def sed_file(self, filepath, pattern, new):
        self.check_pattern(pattern)
        # Open the file in read/write mode
        # print(f"Replacing '{old}' with '{new}' in {filepath}")  # Debugging line
        with open(filepath, 'r+') as file:
            content = file.read().replace(pattern, new)  # Replace old with new
            file.seek(0)
            file.write(content)
            file.truncate()

    # Replace occurrences of a string in all files within a folder with specific extensions
    def sed_files(self, pattern, new, extension=[".nasm", ".c", ".h"]):
        for root, _dirs, files in os.walk(self.working_folder):
            for filename in files:
                if filename.endswith(tuple(extension)):
                    filepath = os.path.join(root, filename)
                    self.sed_file(filepath, pattern, new)

    # Get template content by tag from templates folder
    def get_template(self, tag):
        for file in os.listdir(self.templates_folder):
            with open(os.path.join(self.templates_folder, file), "r") as f:
                content = f.read()
                pattern = re.compile(rf"//__{tag}__//(.*?)//__END_{tag}__//", re.DOTALL)
                match = pattern.search(content)
                if match:
                    return match.group(1).strip()