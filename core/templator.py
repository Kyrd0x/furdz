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
    %__<TAG>__%

    When reading bloc code from a template file, the pattern is:
    //__<TAG>__//
    <content>
    //__END__<TAG>__//
    """
    def __init__(self, working_folder="build/src/", template_folder="build/src/templates/", verbose=False):
        self.working_folder = working_folder if working_folder.endswith("/") else working_folder + "/"
        self.templates_folder = template_folder if template_folder.endswith("/") else template_folder + "/"
        self.verbose = verbose
        self.pattern = re.compile(r"%__(\w+)__%")

    def is_valid_tag(self, tag):
        return self.pattern.match(tag)

    # Replace occurrences of a string in a file
    def sed_file(self, filepath, pattern, new):
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
                pattern = re.compile(rf"%__{tag}__%(.*?)%__END__{tag}__%", re.DOTALL)
                match = pattern.search(content)
                if match:
                    return match.group(1).strip()

    # Extract tags from a file
    def extract_tags_from_file(self, filepath):
        tags = []
        with open(filepath, 'r') as file:
            content = file.read()
            # Use regex to find all tags in the format %tag%
            matches = re.findall(r'%[^%]+%', content)
            for match in matches:
                if self.is_valid_tag(match):
                    tags.append(match)
        return tags

    # Extract tags from all files within a folder recursively
    def extract_tags_from_folder(self, folderpath):
        result = []
        for root, _dirs, files in os.walk(folderpath):
            if root.endswith("templates"):
                continue  # Skip templates folder
            for filename in files:
                if filename.endswith((".nasm", ".c", ".h")):
                    filepath = os.path.join(root, filename)
                    relative_path = os.path.relpath(filepath, folderpath)
                    result.append({
                        "filename": relative_path.replace("\\", "/"),
                        "tags": self.extract_tags_from_file(filepath)
                    })
        return result
                
    def get_all_tags_from_folder(self, exception=[]):
        tags_by_file = self.extract_tags_from_folder(self.working_folder)
        if exception:
            for file in tags_by_file:
                file["tags"] = [tag for tag in file["tags"] if tag not in exception]
        return tags_by_file


    def replace_tags(self, tags_by_file):
        for file in tags_by_file:
            for tag in file["tags"]:
                template = self.get_template(tag)
                if template:
                    self.sed_files(tag, template)