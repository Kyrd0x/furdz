import yaml

def get_template(name):
    with open(f"data/templates.yaml", "r") as f:
        templates = yaml.safe_load(f)
        return templates[str(name)]