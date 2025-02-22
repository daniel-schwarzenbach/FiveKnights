#!/bin/bash


# first of all SDL3 must be build
SDL3lib="./library/SDL3/lib"
if [! -d  "$SDL3lib"]
    mkdir "$SDL3lib"
    echo "Build SDL3"
    cd "$SDL3lib"
    cmake ..
    cmake --build .
    echo "SDL3 builded"
fi

# then execute the locals cmake that builds Game
    


# Create build directory if it doesn't exist
BUILD_DIR="./build"
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
    echo "Created build directory"
fi

# Navigate to build directory
cd "$BUILD_DIR"

# Configure CMake project
echo "Configuring CMake project..."
cmake .. || { echo "CMake configuration failed"; exit 1; }

# Build the project
echo "Building project..."
cmake --build . || { echo "Build failed"; exit 1; }

# Return to original directory
cd ..