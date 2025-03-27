# FUD Malware

Fully Undetectable Malware written in C, embedding a Metasploit Payload\
**Working** without any detection on up-to-date Windows 10/11 and bypassing other AVs\
Also scoring minimal on VirusTotal (dict encryption with metasploit staged reverse https)

School project, deadline 1st of April

## TODO
 - AutoLoad infected dll
 - Make a better Python

## Options

### Sandbox detection

Hardware specs limits can se set, such as :
 - CPU cores
 - RAM size
 - Disk size

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

Run the installation script:

```sh
chmod +x install.sh
./install.sh
```

This script will:
- Download and install Metasploit if needed
- Install necessary dependencies for compilation

### Usage

After installation, you need to READ and edit the .conf file

```sh
nano .conf
make
```

## Disclaimer & Legal Warning

This project is intended for educational and research purposes only. The use of this code in any unauthorized, unethical, or illegal manner is strictly prohibited. By using this software, you agree that you are responsible for your actions and will comply with all applicable laws and regulations.

The creator(s) and contributor(s) of this repository are not responsible for any misuse, damages, or legal consequences resulting from the use of this software. If you plan to test this tool, ensure that you have explicit permission to do so on your own devices or environments.

Use at your own risk.
