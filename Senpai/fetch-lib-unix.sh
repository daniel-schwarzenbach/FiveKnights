#!/bin/bash

# stop script if error occurs
set -e

# Directory where libraries will be stored
LIB_DIR="lib"

# Create libs directory if it doesn't exist
if [ ! -d "$LIB_DIR" ]; then
    echo "Creating directory: $LIB_DIR"
    mkdir -p "$LIB_DIR"
fi

cd "$LIB_DIR"

# Clone SDL repository if not already present
if [ ! -d "SDL" ]; then
    echo "Cloning SDL..."
    git clone --depth 1 https://github.com/libsdl-org/SDL.git
fi

# Clone SDL_ttf repository if not already present
if [ ! -d "SDL_ttf" ]; then
    echo "Cloning SDL_ttf..."
    git clone --depth 1 https://github.com/libsdl-org/SDL_ttf.git
fi

# Clone SDL_image repository if not already present
if [ ! -d "SDL_image" ]; then
    echo "Cloning SDL_image..."
    git clone --depth 1 https://github.com/libsdl-org/SDL_image.git
fi

# Clone SDL_mixer repository if not already present
if [ ! -d "SDL_mixer" ]; then
    echo "Cloning SDL_mixer..."
    git clone --depth 1 https://github.com/libsdl-org/SDL_mixer.git
fi

echo "All necessary SDL libraries have been fetched."