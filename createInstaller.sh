#!/bin/bash
set -e

echo "Cleaning previous build artifacts..."
rm -rf build
rm -rf debian/tmp debian/.debhelper debian/accentpicker debian/files
rm -f ../*.deb ../*.buildinfo ../*.changes

echo "Building Debian package (Debian controls CMake)..."

dpkg-buildpackage -us -uc

mkdir build

mv ../*.deb ../*.ddeb ../*.changes ../*.buildinfo ../accentpicker_* build/

echo "Debian package built successfully!"

