#!/bin/bash
# Build and upload ANL test program to AS32x601 board

set -e

cd "$(dirname "$0")/.."

# Setup Zephyr SDK path
export ZEPHYR_SDK_INSTALL_DIR=${ZEPHYR_SDK_INSTALL_DIR:-$HOME/zephyr-sdk-0.17.4}
export PATH=$ZEPHYR_SDK_INSTALL_DIR/riscv64-zephyr-elf/bin:$PATH

echo "=== Building ANL Test Program ==="

# Compile to object file
echo "1. Compiling hello_anl.c to object file..."
riscv64-zephyr-elf-gcc -march=rv32imc -mabi=ilp32 -c -O2 \
    -nostdlib -fno-builtin -ffreestanding \
    tools/examples/hello_anl.c -o /tmp/hello_anl.o

echo "2. Converting to ANL format..."
python3 tools/anl_link.py /tmp/hello_anl.o /tmp/hello_anl.anl --entry main

echo "3. ANL file created: /tmp/hello_anl.anl"
ls -lh /tmp/hello_anl.anl

echo ""
echo "=== Upload Instructions ==="
echo ""
echo "To upload to AS32x601 board:"
echo ""
echo "1. Connect to board serial port:"
echo "   picocom /dev/ttyUSB0 -b 115200"
echo "   (or minicom -D /dev/ttyUSB0 -b 115200)"
echo ""
echo "2. Run upload script in another terminal:"
echo "   python3 tools/upload/upload_hex.py /dev/ttyUSB0 115200 hello /tmp/hello_anl.anl"
echo ""
echo "3. The program will be uploaded and executed automatically!"
echo ""
echo "Expected output:"
echo "   Hello from ANL loader!"
echo "   This is running on RISC-V RV32I"
echo "   ANL test completed successfully!"
echo ""
