#!/bin/bash

ZIP_SRC="/mnt/d/Noah/Documents/i686-elf-tools-linux.zip"
DEST_DIR="$HOME/i686-elf-tools-linux"

echo "🔍 Checking for ZIP file..."
if [ ! -f "$ZIP_SRC" ]; then
    echo "❌ ZIP not found at: $ZIP_SRC"
    exit 1
fi

echo "📦 Copying ZIP into WSL home directory..."
pv "$ZIP_SRC" > "$HOME/i686-elf-tools-linux.zip"

echo "📂 Extracting..."
mkdir -p "$DEST_DIR"
unzip -o "$HOME/i686-elf-tools-linux.zip" -d "$DEST_DIR"

echo "🧹 Cleaning up ZIP..."
rm "$HOME/i686-elf-tools-linux.zip"

echo "🔧 Adding toolchain to PATH..."
if ! grep -q "i686-elf-tools-linux/bin" ~/.bashrc; then
    echo 'export PATH="$HOME/i686-elf-tools-linux/bin:$PATH"' >> ~/.bashrc
fi

echo "✨ Done! Reloading shell..."
source ~/.bashrc

echo "🧪 Testing compiler:"
which i386-elf-gcc || echo "⚠️ Compiler not found in PATH"
