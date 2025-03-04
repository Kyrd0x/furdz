#!/bin/bash

# Install necessary packages for compilation
if [[ "$(uname -s)" == "Linux" ]]; then
    echo "Installing dependencies for compilation..."
    sudo apt update && sudo apt install -y mingw-w64 gcc make
else
    echo "This script is only for Linux"
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
