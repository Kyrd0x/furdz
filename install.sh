#!/bin/bash

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

# Check if Metasploit is already installed
if command -v msfconsole &> /dev/null; then
    echo "Metasploit Framework is already installed"
    exit 0
else
    # Download and install Metasploit in /tmp
    echo "Installing Metasploit Framework..."
    curl -s https://raw.githubusercontent.com/rapid7/metasploit-omnibus/master/config/templates/metasploit-framework-wrappers/msfupdate.erb > /tmp/msfinstall
    chmod 755 /tmp/msfinstall
    /tmp/msfinstall

    echo "Metasploit installation completed"
fi

echo "Setup completed"
