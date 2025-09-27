#!/bin/bash

set -euo pipefail

green()  { printf '\033[0;32m%s\033[0m\n' "$*"; }
yellow() { printf '\033[0;33m%s\033[0m\n' "$*"; }
red()    { printf '\033[0;31m%s\033[0m\n' "$*"; }

dependencies_install() {
    echo "Installing required dependencies..."

    # Check if running on macOS or Linux
    if [[ "$(uname)" == "Darwin" ]]; then
        # macOS
        # check if brew installed, if so warn and exit
        if ! command -v brew &> /dev/null; then
            red "Homebrew not found. Please install Homebrew first."
            exit 1
        fi
        echo "Installing dependencies with Homebrew..."
        brew install mingw-w64 dos2unix python3
        if [[ $? -ne 0 ]]; then
            red "Failed to install dependencies with Homebrew."
            exit 1
        fi
    else
        # Linux, apt require sudo privileges
        if [[ $EUID -ne 0 ]]; then
            red "This script must be run as root"
            exit 1
        fi

        echo "Installing dependencies with apt..."
        apt update && apt install -y build-essential mingw-w64 dos2unix python3-venv
        if [[ $? -ne 0 ]]; then
            red "Failed to install dependencies with apt."
            exit 1
        fi
    fi
    green "Dependencies installed successfully."
}

virtualenv_setup() {
    if [[ -f .env/bin/activate ]]; then
        echo "Python virtual environment already exists. Skipping creation."
    else
        echo "Creating Python virtual environment..."
        python3 -m venv .env
        source .env/bin/activate
        if [[ -f requirements.txt ]]; then
            echo "Installing Python dependencies..."
            pip install -r requirements.txt
        else
            yellow "No requirements.txt found. Skipping Python dependencies installation."
        fi
    fi
}

project_init() {
    # to change to config.ini later
    if [[ -f .conf ]]; then
        echo ".conf file already exists. Skipping creation."
    else
        echo "Creating .conf file from template..."
        cp .conf.template .conf
    fi

    echo "Fixing bash scripts and making them executable..."
    find . -name "*.sh" -type f -exec dos2unix {} \; -exec chmod +x {} \;
}

fullinstall() {
    git clone https://github.com/Kyrd0x/furdz.git
    cd furdz

    dependencies_install
    virtualenv_setup
    project_init

    green "Install done. Activate your virtual environment with 'source .env/bin/activate'."
    green "Don't forget to edit the .conf file to configure the build options."
    green "Then run './build.py -h' to see build options."
}

fullinstall

