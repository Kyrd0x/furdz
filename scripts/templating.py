import yaml

# Retrieve a template by name from a YAML file
def get_template(name):
    with open(f"data/templates.yaml", "r") as f:
        templates = yaml.safe_load(f)  # Load all templates from the YAML file
        return templates[str(name)]  # Return the template matching the given name