#!/bin/bash

# Ensure the script is run as root
if [ "$(id -u)" -ne 0 ]; then
    echo "This script must be run as root."
    exit 1
fi

if grep -qE '^ID=(ubuntu|debian|kali)' /etc/os-release; then
    echo "Installing dependencies for Debian-based systems..."
    apt-get update && apt-get install -y mingw-w64 gcc nasm binutils vim-common coreutils
elif grep -qE '^ID=arch' /etc/os-release; then
    echo "Installing dependencies for Arch Linux..."
    pacman -Sy --noconfirm mingw-w64-gcc gcc binutils vim
else
    echo "This script is only for Debian-based or Arch Linux."
    exit 1
fi

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

echo "Setup completed"