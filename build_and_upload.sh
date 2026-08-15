#!/bin/bash
set -e

# Configuration
TOOLCHAIN=~/zephyr-sdk-0.17.4/riscv64-zephyr-elf/bin
SOURCE_FILE=$1
PROGRAM_NAME=$(basename "$SOURCE_FILE" .c)
PORT=${2:-/dev/ttyUSB0}
BAUD=${3:-115200}

if [ -z "$SOURCE_FILE" ]; then
    echo "Usage: $0 <source.c> [port] [baud]"
    echo "Example: $0 tools/examples/fork_demo.c /dev/ttyUSB0 115200"
    exit 1
fi

# Add toolchain to PATH
export PATH=$TOOLCHAIN:$PATH

echo "==> Compiling $SOURCE_FILE to object file..."
riscv64-zephyr-elf-gcc -march=rv32imc -mabi=ilp32 \
    -c -O2 -nostdlib -fno-builtin -ffreestanding \
    "$SOURCE_FILE" -o "/tmp/${PROGRAM_NAME}.o"

echo "==> Linking to ANL format..."
python3 tools/anl_link.py "/tmp/${PROGRAM_NAME}.o" "/tmp/${PROGRAM_NAME}.anl" --entry main

echo "==> Uploading to board at $PORT..."
sudo python3 tools/upload/upload_hex.py "$PORT" "$BAUD" "$PROGRAM_NAME" "/tmp/${PROGRAM_NAME}.anl"

echo ""
echo "==> Done! Connect to serial and run:"
echo "    shell> run $PROGRAM_NAME"
