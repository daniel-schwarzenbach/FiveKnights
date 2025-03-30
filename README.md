# Five Knights against King Fredrick & the Senpai Game Engine

A Five Nights at Fredies Inspired Chess Game and the Engine its running on

# Requirements:

- C++23 Compile
- CMake min 3.20
- Git

## C++23 Compiler

### Windows

install MinGW-w64 (GCC)

[Donwload MinGW-w64](https://code.visualstudio.com/docs/cpp/config-mingw)

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

- Official installer: [Download Cmake](https://cmake.org/download/#latest)

   

### macOS
   - Homebrew: 
   ```
   brew install cmake@3.20
   ```
   - Official .dmg: [Download Cmake](https://cmake.org/files/v3.20/cmake-3.20.0-macos-universal.dmg)


### Linux (Ubuntu/Debian)

```bash
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | sudo apt-key add -
sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ focal main'
sudo apt update
sudo apt install cmake=3.20.*
```

## Git

neccecary for fetching SDL3

## SDL3, SDL3_image, SDL3_Mixer, SDL3_ttf

SDL3 will be fetched when executing Senpai/fetch-lib*

build-* calls Senpai/fetch-lib* automatically

# Classes