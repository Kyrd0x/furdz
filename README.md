# FUD Malware

School project, deadline 1st of April.

All the payloads **working** on up-to-date Windows (10 & 11) and other AVs\
Scoring 2 on VirusTotal, 0 seems not possible without a legit signature.

## TODO
 - simple bash installer
 - Hostname check in sandbox detection
 - Make a better Python
 - Init function for opening bank file and load kernel32


## How to Use

### Installation

Clone this repo

```sh
git clone https://github.com/Kyrd0x/furdz.git
cd furdz
```

Run the installation script

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
