#!/bin/bash
# Manual test script for per-process signal mechanism

echo "======================================================================="
echo "  Per-Process Signal Mechanism - Manual Test Guide"
echo "======================================================================="
echo ""
echo "This will launch QEMU. Follow these steps to test:"
echo ""
echo "1. Wait for 'anl>' prompt"
echo "2. Type 'ps' to see process list"
echo "3. Type 'loop' to start infinite loop"
echo "4. Press Ctrl+D to send SIGINT to foreground process"
echo "5. Observe that ONLY the loop process receives the signal"
echo "6. Type 'fork 2 10' to create 2 child processes"
echo "7. Observe each child has its own signal_state"
echo "8. Press Ctrl+A then 'x' to exit QEMU"
echo ""
echo "Expected behavior:"
echo "  - Ctrl+D triggers UART ISR detection"
echo "  - ISR sends SIGINT to foreground process (PID shown)"
echo "  - Only targeted process receives and handles the signal"
echo "  - Other processes continue running unaffected"
echo ""
echo "======================================================================="
echo ""
read -p "Press Enter to start QEMU..."

west build -d build-anl-qemu -t run
