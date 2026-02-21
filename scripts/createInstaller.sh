#!/bin/bash
set -e

echo "Cleaning previous build artifacts..."
rm -rf build
rm -rf debian/tmp debian/.debhelper debian/accentpicker debian/files
rm -f ../*.deb ../*.buildinfo ../*.changes

echo "Building Debian package (Debian controls CMake)..."

# should change DCMAKE_PREFIX_PATH to the path of the Qt(6.7+) installation on the system where this 
# script is run, but for now it is hardcoded to my system, which is not ideal.
dpkg-buildpackage -us -uc --build-profiles=nocheck

mkdir build

mv ../*.deb ../*.ddeb ../*.changes ../*.buildinfo ../accentpicker_* build/

echo "Debian package built successfully!"

