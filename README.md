# FUD Malware

School project, deadline 1st of April.

Harmfull payload **working** on up-to-date Windows 10 & 11\
Scoring 2 on VirusTotal for now, aiming for 0

## TODO
 - simple bash installer
 - Hostname check in sandbox detection
 - Make a better Python
 - Init function for opening bank file and load kernel32
 - Improve loader (how to shellcode properly) :
        - grep and use a legit PPID maybe
        - xxx


## How to Use

### Installation

Clone this repository and navigate to the project directory:

```sh
git clone https://github.com/yourusername/yourrepository.git
cd yourrepository
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
