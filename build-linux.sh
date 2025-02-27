#!/bin/bash

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

ctest || { echo "Tests failed"; exit 1; }

# Return to original directory
cd ..