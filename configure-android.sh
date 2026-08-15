#!/bin/bash
# Build script for OpenXRay on Android Bionic (Termux)

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build-android"
INSTALL_PREFIX="/data/data/com.termux/files/usr/local"

echo "=== OpenXRay Android Bionic Build Script ==="
echo "Target: aarch64-unknown-linux-android24"
echo "Build directory: ${BUILD_DIR}"
echo ""

# Clean previous build
if [ -d "${BUILD_DIR}" ]; then
    echo "Cleaning previous build..."
    rm -rf "${BUILD_DIR}"
fi

mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

echo "Configuring CMake..."
cmake .. \
    -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE="${SCRIPT_DIR}/cmake/AndroidBionicToolchain.cmake" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" \
    -DBUILD_SHARED_LIBS=ON \
    -DXRAY_USE_LUAJIT=ON \
    -DMEMORY_ALLOCATOR=standard \
    -DLuaJIT_INCLUDE_DIR="/data/data/com.termux/files/usr/include/luajit-2.1" \
    -DLuaJIT_LIBRARY="/data/data/com.termux/files/usr/lib/libluajit-5.1.so" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_VERBOSE_MAKEFILE=OFF

echo ""
echo "Configuration complete!"
echo "To build, run: ninja -C ${BUILD_DIR}"
echo "To install, run: ninja -C ${BUILD_DIR} install"
