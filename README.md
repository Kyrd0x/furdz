# FUD Malware

Fully Undetectable Malware (DLL Injection) written in C.

**Working** without any detection on up-to-date Windows 10/11 and bypassing other AVs.\
Also scoring minimal on VirusTotal (dict encryption with reverse shell tcp).

VirusTotal 0 on 27/03/2025

## TODO
 - a lot

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
 - Any Metasploit payload
 - ~~Custom ASM one (from ```asm/```)~~


## How to Use

### Installation

Clone this repository and navigate to the project directory:

```sh
git clone https://github.com/Kyrd0x/furdz.git
cd furdz
```

Install the required dependencies

```sh
sudo apt update & sudo apt install -y mingw-w64
```

### Usage

After installation, you need to READ and edit the .conf file

```sh
nano .conf
chmod +x build.sh
./build.sh --help
```

## Disclaimer & Legal Warning

This project is intended for educational and research purposes only. The use of this code in any unauthorized, unethical, or illegal manner is strictly prohibited. By using this software, you agree that you are responsible for your actions and will comply with all applicable laws and regulations.

The creator(s) and contributor(s) of this repository are not responsible for any misuse, damages, or legal consequences resulting from the use of this software. If you plan to test this tool, ensure that you have explicit permission to do so on your own devices or environments.

Use at your own risk.
