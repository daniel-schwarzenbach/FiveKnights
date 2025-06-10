#!/usr/bin/env bash

# Exit on error
set -euo pipefail

# Variables (tweak as needed)
PROJECT_ROOT="."
BUILD_DIR="${PROJECT_ROOT}/build-release"
DIST_DIR="${PROJECT_ROOT}/dist"
GAME_EXECUTABLE="FiveKnights"
PACKAGE_NAME="Five-Knights"
VERSION="1.0.0"

# Clean up previous artifacts
rm -rf "$BUILD_DIR" "$DIST_DIR"
mkdir -p "$BUILD_DIR" "$DIST_DIR"

# 1) Configure Release build with CMake
echo "Configuring CMake (Release) in $BUILD_DIR..."
cmake -S "$PROJECT_ROOT" -B "$BUILD_DIR" \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=/usr/local

# 2) Build
echo "Building (Release)..."
cmake --build "$BUILD_DIR" --parallel

# 3) Run tests (uncomment if you have tests configured)
# echo "Running tests..."
# cmake --build "$BUILD_DIR" --target test

# 4) Install into a staging tree
STAGING_DIR="${BUILD_DIR}/package"
mkdir -p "$STAGING_DIR"
echo "Installing into staging directory..."
cmake --install "$BUILD_DIR" --prefix "$STAGING_DIR/usr/local"

# 5) Copy assets into staging
echo "Copying assets..."
mkdir -p "$STAGING_DIR/usr/share/${PACKAGE_NAME}/assets"
cp -a "${PROJECT_ROOT}/Game/assets/." \
      "$STAGING_DIR/usr/share/${PACKAGE_NAME}/assets/"

# 6) Package as tar.gz
ARCHIVE_NAME="${PACKAGE_NAME}-${VERSION}-linux.tar.gz"
echo "Creating release archive $ARCHIVE_NAME..."
tar -czf "${DIST_DIR}/${ARCHIVE_NAME}" -C "$STAGING_DIR" .

echo "Release build complete!"
echo "  - Binary + libs in: $STAGING_DIR/usr/local"
echo "  - Assets in:       $STAGING_DIR/usr/share/${PACKAGE_NAME}/assets"
echo "  - Archive in:      $DIST_DIR/${ARCHIVE_NAME}"