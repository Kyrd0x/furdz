#!/bin/bash

set -euo pipefail

dependencies_install() {
    echo "Installing required dependencies..."

    # Check if running on macOS or Linux
    if [[ "$(uname)" == "Darwin" ]]; then
        # macOS
        # check if brew installed, if so warn and exit
        if ! command -v brew &> /dev/null; then
            echo "Homebrew not found. Please install Homebrew first."
            exit 1
        fi
        echo "Installing dependencies with Homebrew..."
        brew install mingw-w64 dos2unix python3
        if [[ $? -ne 0 ]]; then
            echo "Failed to install dependencies with Homebrew."
            exit 1
        fi
    else
        # Linux, apt require sudo privileges
        if [[ $EUID -ne 0 ]]; then
            echo "This script must be run as root"
            exit 1
        fi

        echo "Installing dependencies with apt..."
        apt update && apt install -y build-essential mingw-w64 dos2unix python3-venv
        if [[ $? -ne 0 ]]; then
            echo "Failed to install dependencies with apt."
            exit 1
        fi
    fi
    echo "Dependencies installed successfully."
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
            echo "No requirements.txt found. Skipping Python dependencies installation."
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

    echo "Please edit the .conf file to configure the build options."

    echo "Fixing bash scripts and making them executable..."
    find . -name "*.sh" -type f -exec dos2unix {} \; -exec chmod +x {} \;
}

fullinstall() {
    git clone https://github.com/Kyrd0x/furdz.git
    cd furdz

    dependencies_install
    virtualenv_setup
    project_init

    echo "Then run './build.sh -h' to see build options."
    echo "Setup done. Please edit the .conf file to configure the build options."
}

fullinstall

