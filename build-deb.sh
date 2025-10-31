#!/bin/bash
set -e

PACKAGE_NAME="void-flasher"
PACKAGE_VERSION="1.0.0"
DEB_NAME="${PACKAGE_NAME}_${PACKAGE_VERSION}_all.deb"

# 1. Create the file structure
echo "[*] Creating package structure..."
rm -rf debian-build
mkdir -p debian-build/DEBIAN
mkdir -p debian-build/usr/bin
mkdir -p debian-build/usr/share/applications
mkdir -p debian-build/usr/share/icons/hicolor/scalable/apps

# 2. Copy files into the structure
echo "[*] Copying files..."
cp control debian-build/DEBIAN/
cp postinst debian-build/DEBIAN/

cp void-flasher debian-build/usr/bin/
cp void-flasher.desktop debian-build/usr/share/applications/
cp void-flasher.svg debian-build/usr/share/icons/hicolor/scalable/apps/void-flasher.svg

# 3. Set correct permissions
echo "[*] Setting permissions..."
chmod +x debian-build/DEBIAN/postinst
chmod +x debian-build/usr/bin/void-flasher
chmod 644 debian-build/DEBIAN/control
chmod 644 debian-build/usr/share/applications/void-flasher.desktop
chmod 644 debian-build/usr/share/icons/hicolor/scalable/apps/void-flasher.svg

# 4. Build the .deb package
echo "[*] Building .deb package..."
dpkg-deb --build debian-build "$DEB_NAME"

# 5. Clean up
echo "[*] Cleaning up..."
rm -rf debian-build

echo ""
echo "  BUILD COMPLETE ✓"
echo "  Package: $DEB_NAME"
echo ""
echo "You can now install it with: sudo apt install ./$DEB_NAME"
