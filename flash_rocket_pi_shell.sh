#!/bin/bash
# Flash script for RocketPi shell process with Ctrl+C signal support

set -e

BUILD_DIR="build-rocket-pi-anl"

echo "=========================================="
echo "  RocketPi Shell Process Flash Script"
echo "  with Ctrl+C Signal Support"
echo "=========================================="
echo ""

# Check if build exists
if [ ! -f "$BUILD_DIR/zephyr/zephyr.elf" ]; then
    echo "Error: Build not found. Please run:"
    echo "  west build -p always -b rocket_pi -d $BUILD_DIR zephyr/samples/ansilic/rocket_pi_shell_process"
    exit 1
fi

# Show firmware info
echo "Firmware Information:"
echo "--------------------"
arm-none-eabi-size $BUILD_DIR/zephyr/zephyr.elf
echo ""

# Flash using west
echo "Flashing to RocketPi board..."
echo ""
west flash -d $BUILD_DIR --runner openocd

echo ""
echo "Flash complete!"
echo ""
echo "Connect to serial console:"
echo "  picocom -b 115200 /dev/ttyUSB0"
echo ""
echo "To test Ctrl+C signal support:"
echo "  1. shell> loop 10"
echo "  2. Press Ctrl+C to interrupt"
echo "  3. shell> test_signal  (self-test)"
echo ""
