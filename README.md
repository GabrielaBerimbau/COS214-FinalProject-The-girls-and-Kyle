# COS214-FinalProject-The-girls-and-Kyle

## Initial Setup

### Prerequisites
- **Linux/WSL**: Ubuntu 20.04 or later
- **macOS**: macOS 10.15 or later with Homebrew installed
- **Git**: Required for cloning and managing submodules

### Setup Instructions

1. **Clone the repository**
```bash
   git clone <your-repo-url>
   cd <your-repo-name>
```

2. **Make setup script executable**
```bash
   chmod +x setup.sh
```

3. **Run setup script**
```bash
   ./setup.sh
```
   
   This will:
   - Install all required dependencies for your platform
   - Initialize and update git submodules (raylib, googletest)
   - Prepare the project for building

4. **Build and run**
```bash
   make run-demo
```

### Platform-Specific Notes

**Linux/WSL Ubuntu:**
The setup script will install the following packages:
- build-essential (gcc, g++, make)
- git, pkg-config
- X11 development libraries (libx11-dev, libxrandr-dev, libxinerama-dev, libxcursor-dev, libxi-dev)
- OpenGL libraries (libgl1-mesa-dev)
- Audio libraries (libasound2-dev)

**macOS:**
Requirements:
- Xcode Command Line Tools (script will prompt to install if missing)
- Homebrew (install from https://brew.sh if not present)
- cmake (script will install via Homebrew if missing)

### Troubleshooting

**Permission denied when running setup.sh:**
```bash
chmod +x setup.sh
./setup.sh
```

**Homebrew not found (macOS):**
Install Homebrew first:
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

**Build fails after setup:**
Try cleaning and rebuilding:
```bash
make clean
make run-demo
```

### Available Commands

After setup, you can use:
```bash
make              # Build TestingMain and Demo
make run          # Run TestingMain
make run-demo     # Run Demo with raylib
make test         # Run all tests
make clean        # Clean build artifacts
make help         # Show all available commands
```