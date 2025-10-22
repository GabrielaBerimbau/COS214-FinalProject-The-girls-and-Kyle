#!/bin/bash
set -e

echo "Setting up Plant Shop Project..."
echo ""

# Detect OS
if [[ "$OSTYPE" == "linux-gnu"* ]] || [[ -f /proc/version ]] && grep -qi microsoft /proc/version; then
    echo "Detected: Linux/WSL Ubuntu"
    echo "Installing dependencies..."
    echo ""
    
    sudo apt-get update
    sudo apt-get install -y \
        build-essential \
        git \
        pkg-config \
        libx11-dev \
        libxrandr-dev \
        libxinerama-dev \
        libxcursor-dev \
        libxi-dev \
        libgl1-mesa-dev \
        libasound2-dev
    
    echo ""
    echo "Dependencies installed successfully."
    
elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo "Detected: macOS"
    echo ""
    
    # Check for Homebrew
    if ! command -v brew &> /dev/null; then
        echo "Error: Homebrew not found."
        echo "Install Homebrew first: https://brew.sh"
        echo ""
        echo "Run this command:"
        echo '/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"'
        exit 1
    fi
    
    # Check for Xcode Command Line Tools
    if ! xcode-select -p &> /dev/null; then
        echo "Installing Xcode Command Line Tools..."
        xcode-select --install
        echo ""
        echo "Please wait for the installation to complete in the popup window,"
        echo "then run this script again."
        exit 1
    fi
    
    echo "Checking dependencies..."
    # Raylib dependencies for macOS are minimal since frameworks are built-in
    # But we ensure we have basic tools
    brew list cmake &> /dev/null || brew install cmake
    
    echo "Dependencies verified."
    
else
    echo "Error: Unsupported operating system."
    echo "This script supports Linux, WSL, and macOS only."
    exit 1
fi

echo ""
echo "Initializing git submodules..."
git submodule update --init --recursive

echo ""
echo "Setup complete."
echo ""
echo "Next steps:"
echo "  make run-demo    Run the demo"
echo "  make test        Run tests"
echo "  make help        See all commands"