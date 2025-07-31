#!/bin/bash

echo "🚀 Building Network Ranger for all platforms..."
echo "================================================"

mkdir -p builds

echo "📦 Creating clean builds directory..."
rm -rf builds/*
mkdir -p builds/{windows,macos-intel,macos-silicon,linux}

echo "🔧 Building for Linux (x86_64)..."
if command -v g++ &> /dev/null; then
    g++ -std=c++11 -Wall -Wextra -O2 -static \
        -I/opt/homebrew/Cellar/jsoncpp/1.9.6/include \
        ranger.c++ network-utils.cpp \
        -o builds/linux/network-ranger \
        -L/opt/homebrew/Cellar/jsoncpp/1.9.6/lib \
        -lcurl -ljsoncpp -lpthread -lssl -lcrypto
    echo "✅ Linux build completed"
else
    echo "❌ g++ not found for Linux build"
fi

echo "🍎 Building for macOS Intel..."
if command -v clang++ &> /dev/null; then
    clang++ -std=c++11 -Wall -Wextra -O2 \
        -I/opt/homebrew/Cellar/jsoncpp/1.9.6/include \
        ranger.c++ network-utils.cpp \
        -o builds/macos-intel/network-ranger \
        -L/opt/homebrew/Cellar/jsoncpp/1.9.6/lib \
        -lcurl -ljsoncpp -lpthread
    echo "✅ macOS Intel build completed"
else
    echo "❌ clang++ not found for macOS Intel build"
fi

echo "🍎 Building for macOS Silicon (ARM64)..."
if command -v clang++ &> /dev/null; then
    clang++ -std=c++11 -Wall -Wextra -O2 -target arm64-apple-macos11 \
        -I/opt/homebrew/Cellar/jsoncpp/1.9.6/include \
        ranger.c++ network-utils.cpp \
        -o builds/macos-silicon/network-ranger \
        -L/opt/homebrew/Cellar/jsoncpp/1.9.6/lib \
        -lcurl -ljsoncpp -lpthread
    echo "✅ macOS Silicon build completed"
else
    echo "❌ clang++ not found for macOS Silicon build"
fi

echo "🪟 Building for Windows (cross-compilation)..."
if command -v x86_64-w64-mingw32-g++ &> /dev/null; then
    x86_64-w64-mingw32-g++ -std=c++11 -Wall -Wextra -O2 \
        ranger-windows.c++ \
        -o builds/windows/network-ranger.exe \
        -lcurl -ljsoncpp -lpthread -lws2_32
    echo "✅ Windows build completed"
else
    echo "⚠️  Windows cross-compiler not found. Install MinGW-w64 for Windows builds."
    echo "   On macOS: brew install mingw-w64"
    echo "   On Ubuntu: sudo apt-get install mingw-w64"
fi

echo ""
echo "📋 Build Summary:"
echo "=================="

if [ -f "builds/linux/network-ranger" ]; then
    echo "✅ Linux: builds/linux/network-ranger"
    file builds/linux/network-ranger
fi

if [ -f "builds/macos-intel/network-ranger" ]; then
    echo "✅ macOS Intel: builds/macos-intel/network-ranger"
    file builds/macos-intel/network-ranger
fi

if [ -f "builds/macos-silicon/network-ranger" ]; then
    echo "✅ macOS Silicon: builds/macos-silicon/network-ranger"
    file builds/macos-silicon/network-ranger
fi

if [ -f "builds/windows/network-ranger.exe" ]; then
    echo "✅ Windows: builds/windows/network-ranger.exe"
    file builds/windows/network-ranger.exe
fi

echo ""
echo "📦 Creating distribution packages..."

cd builds

echo "📦 Creating Linux package..."
if [ -f "linux/network-ranger" ]; then
    tar -czf network-ranger-linux-x86-64.tar.gz linux/
    echo "✅ Created: network-ranger-linux-x86-64.tar.gz"
fi

echo "📦 Creating macOS Intel package..."
if [ -f "macos-intel/network-ranger" ]; then
    tar -czf network-ranger-macos-intel.tar.gz macos-intel/
    echo "✅ Created: network-ranger-macos-intel.tar.gz"
fi

echo "📦 Creating macOS Silicon package..."
if [ -f "macos-silicon/network-ranger" ]; then
    tar -czf network-ranger-macos-silicon.tar.gz macos-silicon/
    echo "✅ Created: network-ranger-macos-silicon.tar.gz"
fi

echo "📦 Creating Windows package..."
if [ -f "windows/network-ranger.exe" ]; then
    tar -czf network-ranger-windows-x86-64.tar.gz windows/
    echo "✅ Created: network-ranger-windows-x86-64.tar.gz"
fi

echo ""
echo "🎉 Build process completed!"
echo "📁 Executables are in the builds/ directory"
echo "📦 Distribution packages are in the builds/ directory" 
