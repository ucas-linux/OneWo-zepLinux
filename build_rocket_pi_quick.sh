#!/bin/bash
# Quick build and test script for RocketPi shell with Ctrl+C support

echo "=========================================="
echo "  RocketPi Shell - Quick Build & Info"
echo "=========================================="
echo ""

# Build
echo "[1/3] Building for RocketPi..."
west build -p always -b rocket_pi -d build-rocket-pi-anl zephyr/samples/ansilic/rocket_pi_shell_process

# Show size
echo ""
echo "[2/3] Firmware size:"
~/zephyr-sdk-0.17.4/arm-zephyr-eabi/bin/arm-zephyr-eabi-size build-rocket-pi-anl/zephyr/zephyr.elf

# Show build output
echo ""
echo "[3/3] Build artifacts:"
ls -lh build-rocket-pi-anl/zephyr/zephyr.{bin,elf,hex}

echo ""
echo "=========================================="
echo "  Build complete!"
echo "=========================================="
echo ""
echo "Next steps:"
echo "  1. Flash: west flash -d build-rocket-pi-anl --runner openocd"
echo "  2. Connect: picocom -b 115200 /dev/ttyUSB0"
echo "  3. Test: shell> loop 10"
echo "  4. Interrupt: Press Ctrl+C"
echo ""
