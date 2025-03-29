#!/bin/bash

# Ensure the script is run as root
if [ "$(id -u)" -ne 0 ]; then
    echo "This script must be run as root."
    exit 1
fi


if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <metasploit|sliver>"
    exit 1
fi

TOOL=$1

if grep -qE '^ID=(ubuntu|debian|kali)' /etc/os-release; then
    echo "Installing dependencies for Debian-based systems..."
    sudo apt-get update && sudo apt-get install -y mingw-w64 gcc make rar
elif grep -qE '^ID=arch' /etc/os-release; then
    echo "Installing dependencies for Arch Linux..."
    sudo pacman -Sy --noconfirm mingw-w64-gcc gcc make rar
else
    echo "This script is only for Debian-based or Arch Linux."
    exit 1
fi

if [ "$TOOL" == "metasploit" ]; then
    # Check if Metasploit is already installed
    if command -v msfconsole &> /dev/null; then
        echo "Metasploit Framework is already installed"
    else
        # Download and install Metasploit in /tmp
        echo "Installing Metasploit Framework..."
        curl -s https://raw.githubusercontent.com/rapid7/metasploit-omnibus/master/config/templates/metasploit-framework-wrappers/msfupdate.erb > /tmp/msf_install
        chmod 755 /tmp/msf_install
        /tmp/msf_install

        echo "Metasploit installation completed"
    fi
elif [ "$TOOL" == "sliver" ]; then
    # Check if Sliver is already installed
    if command -v sliver &> /dev/null; then
        echo "Sliver is already installed"
    else
        # Download and install Sliver in /tmp
        echo "Installing Sliver..."
        curl -s https://sliver.sh/install > /tmp/sliver_install
        chmod 755 /tmp/sliver_install
        /tmp/sliver_install

        echo "Sliver installation completed"
    fi
else
    echo "Invalid argument. Use 'metasploit' or 'sliver'."
    exit 1
fi

echo "Setup completed"