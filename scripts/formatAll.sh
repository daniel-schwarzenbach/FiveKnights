#!/bin/bash
cd ..

echo "Formatting all source files..."
find . -type f -name "*.c++" ! -path "./Senpai/lib/*" -exec clang-format -i {} \;

echo "Formatting all header files..."
find . -type f -name "*.h++" ! -path "./Senpai/lib/*" -exec clang-format -i {} \;