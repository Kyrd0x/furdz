# FUD Malware

Fully Undetectable Malware (DLL Injection) written in C.

**Working** without any detection on up-to-date Windows 10/11 and bypassing other AVs.\
Also scoring minimal on VirusTotal.

## TODO

 - **Autoload the DLL**
 - Increase ```winapi.c``` obfuscation
 - Ability to choose between multiple techniques
 - Add anti sandboxs checks, based on virtualization, cpu name, bios name, connected devices

## Options

### Sandbox detection

Hardware specs limits can se set, such as :
 - CPU cores / RAM size / Disk size

Target a specific hostname prefix / Avoid specific hostname prefixes
 - ComputerName

Avoid countries
 - KeyboardLayout/SystemDefaultLang/SystemDefaultLCID

### Payloads

Can be used:
 - C written payloads

## How to Use

### Installation

**Linux (Debian, Ubuntu, WSL, etc.)**

```bash
curl -fsSL https://furdz.kyrd0x.net/install | sudo bash
```

**macOS**

```bash
curl -fsSL https://furdz.kyrd0x.net/install | bash
```

**Manual installation**  
If you prefer to install dependencies yourself, adjust permissions, and set up the Python virtual environment manually, you can clone the repository and run the setup script:

```bash
git clone https://github.com/Kyrd0x/furdz.git
cd furdz
chmod +x scripts/setup.sh
sudo ./scripts/setup.sh
```

The ```setup.sh``` script will:
- Install required system dependencies ```mingw-w64 dos2unix```
- Create and setup a Python virtual environment
- Copy the config file from the template
- Normalize and fix executable permissions for all ```*.sh``` scripts

### Usage

After installation, you need to READ and edit the .conf file.

Then running ```./build.py -h``` is a nice idea.

## Disclaimer & Legal Warning

This project is intended for educational and research purposes only. The use of this code in any unauthorized, unethical, or illegal manner is strictly prohibited. By using this software, you agree that you are responsible for your actions and will comply with all applicable laws and regulations.

The creator(s) and contributor(s) of this repository are not responsible for any misuse, damages, or legal consequences resulting from the use of this software. If you plan to test this tool, ensure that you have explicit permission to do so on your own devices or environments.

Use at your own risk.
