#!/bin/bash
rm -r build
cmake -S . -B build \
      -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=../lib \
      -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=../lib
cmake --build build