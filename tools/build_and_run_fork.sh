#!/bin/bash
# Build fork_demo.c -> fork_demo.anl and run it in QEMU
set -e

TOOLS=$(dirname "$0")
EXAMPLE="$TOOLS/examples/fork_demo.c"
OBJ="/tmp/fork_demo.o"
ANL="/tmp/fork_demo.anl"

CC=arm-zephyr-eabi-gcc
SDK_CC="$HOME/zephyr-sdk-0.17.4/arm-zephyr-eabi/bin/$CC"
[ -x "$SDK_CC" ] && CC="$SDK_CC"

echo "=== Compiling $EXAMPLE ==="
$CC -c -O1 -mthumb -mcpu=cortex-m3 \
    -fno-builtin -ffreestanding -mlong-calls \
    -o "$OBJ" "$EXAMPLE"

echo "=== Linking to ANL ==="
python3 "$TOOLS/anl_link.py" "$OBJ" "$ANL" --entry main

echo "=== ANL binary: $ANL ==="
HEX=$(xxd -p "$ANL" | tr -d '\n')
echo "Hex length: ${#HEX} chars ($(( ${#HEX}/2 )) bytes)"

echo ""
echo "=== Running in QEMU ==="
cd "$(dirname "$TOOLS")"
printf "load fork_demo %s\r\n" "$HEX" | timeout 20 west build -t run 2>&1 | cat
