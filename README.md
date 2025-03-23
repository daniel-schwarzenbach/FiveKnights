# FiveKnights Against King Fredrick

# Requirements:

## C++23 Compiler

### Windows

install MinGW-w64 (GCC)
```
pacman -S mingw-w64-ucrt-x86_64-gcc
```

### macOS

Let macOS install the XCode Command Line Extensions by either
- running 'gcc' or 'clang' and confirming the prompt
- running 'xcode-select --install'

### Linux (Ubuntu/Debian)
```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt install g++-13
```

## Cmake 3.20

### Windows

### macOS

```
brew install cmake
```



Here's how to install CMake 3.20 alongside C++23 compilers for each platform:

**Windows**
1. **CMake 3.20 Installation**
   - Official installer: [Download Cmake](https://cmake.org/download/)
   

### macOS
   - Homebrew: 
   ```
   brew install cmake@3.20 && brew link --force cmake@3.20
   ```
   - Official .dmg: [Download Cmake](https://cmake.org/files/v3.20/cmake-3.20.0-macos-universal.dmg)


### Linux (Ubuntu/Debian)

```bash
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | sudo apt-key add -
sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ focal main'
sudo apt update
sudo apt install cmake=3.20.*
```

### Git

for fetching SDL3
