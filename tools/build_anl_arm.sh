#!/bin/bash
# Build ANL module for ARM Cortex-M4 (Rocket Pi) and optionally upload
#
# Usage:
#   ./tools/build_anl_arm.sh <source.c> [--upload /dev/ttyACM0]
#
# Examples:
#   ./tools/build_anl_arm.sh tools/examples/fork_demo.c
#   ./tools/build_anl_arm.sh tools/examples/fork_demo.c --upload /dev/ttyACM0

set -e

cd "$(dirname "$0")/.."

# Setup Zephyr SDK ARM toolchain
export ZEPHYR_SDK_INSTALL_DIR=${ZEPHYR_SDK_INSTALL_DIR:-$HOME/zephyr-sdk-0.17.4}
ARM_GCC=$ZEPHYR_SDK_INSTALL_DIR/arm-zephyr-eabi/bin/arm-zephyr-eabi-gcc

if [ ! -x "$ARM_GCC" ]; then
    echo "ERROR: ARM toolchain not found at $ARM_GCC"
    exit 1
fi

if [ -z "$1" ]; then
    echo "Usage: $0 <source.c> [--upload <port>]"
    exit 1
fi

SOURCE="$1"
BASENAME=$(basename "$SOURCE" .c)
OBJ="/tmp/${BASENAME}.o"
ANL="/tmp/${BASENAME}.anl"

echo "=== Building ARM ANL Module: $BASENAME ==="

# 1. Compile to object file (Thumb2, Cortex-M4)
echo "1. Compiling $SOURCE ..."
$ARM_GCC -mthumb -mcpu=cortex-m4 -c -O2 \
    -nostdlib -fno-builtin -ffreestanding -mlong-calls \
    "$SOURCE" -o "$OBJ"

# 2. Link to ANL format
echo "2. Linking to ANL format..."
python3 tools/anl_link.py "$OBJ" "$ANL" --entry main

# 3. Show result
echo "3. ANL file created:"
ls -lh "$ANL"

# 4. Upload if requested
if [ "$2" = "--upload" ]; then
    PORT="${3:-/dev/ttyACM0}"
    BAUD="${4:-115200}"
    echo ""
    echo "4. Uploading to $PORT @ $BAUD ..."
    sudo python3 tools/upload/upload_hex.py "$PORT" "$BAUD" "$BASENAME" "$ANL"
else
    echo ""
    echo "=== Upload Instructions ==="
    echo ""
    echo "To upload to Rocket Pi:"
    echo "  sudo python3 tools/upload/upload_hex.py /dev/ttyACM0 115200 $BASENAME $ANL"
    echo ""
fi
