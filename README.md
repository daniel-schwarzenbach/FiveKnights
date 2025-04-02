# Five Knights against King Fredrick & the Senpai Game Engine

A Five Nights at Fredies Inspired Chess Game and the Engine its running on

Video Link: https://cloud.uni-konstanz.de/index.php/s/6kfoGoXGxdaXbqY

Game Play Fotage: https://cloud.uni-konstanz.de/index.php/s/gJi4qawndimeS3E

# Documentation

Senpai Engine: [Senpai Documentation](Docs/Senpai.md)

Five Knights against King Fredrick Game: [Game Documentation](Docs/Game.md)

Used Programing Concepts: [Documentation](Docs/UsedProgamingConcepts.md)

# How To Run:

Linux/MacOS: ./build-unix.sh

Windows: ./build-windows.bat

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

### Windows

https://desktop.github.com/download/

### macOS

````
brew install git
````

### Linux (Ubuntu/Debian)

````
sudo apt update
sudo apt install git
````



## SDL3, SDL3_image, SDL3_Mixer, SDL3_ttf

SDL3 will be fetched when executing Senpai/fetch-lib*

build-* calls Senpai/fetch-lib* automatically
