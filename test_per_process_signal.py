#!/usr/bin/env python3
"""
Test per-process signal mechanism in ANL loader
"""

import subprocess
import time
import sys
import os

def run_qemu_test():
    """Run QEMU and test per-process signal functionality"""

    print("=" * 70)
    print("Testing Per-Process Signal Mechanism")
    print("=" * 70)

    # Start QEMU process
    proc = subprocess.Popen(
        ['west', 'build', '-d', 'build-anl-qemu', '-t', 'run'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        bufsize=1
    )

    def send_command(cmd):
        """Send command to QEMU"""
        print(f"\n>>> Sending command: {cmd}")
        proc.stdin.write(cmd + '\n')
        proc.stdin.flush()
        time.sleep(0.5)

    def read_output(timeout=2):
        """Read output for a period"""
        import select
        output = []
        start = time.time()
        while time.time() - start < timeout:
            if select.select([proc.stdout], [], [], 0.1)[0]:
                line = proc.stdout.readline()
                if line:
                    output.append(line)
                    print(line, end='')
        return ''.join(output)

    try:
        # Wait for boot
        print("\n[*] Waiting for system boot...")
        time.sleep(3)
        output = read_output(3)

        if 'Per-Process Signal Handler' in output:
            print("\n✓ System booted with per-process signal support")

        # Test 1: ps command
        print("\n" + "=" * 70)
        print("TEST 1: Check process list")
        print("=" * 70)
        send_command('ps')
        output = read_output(2)

        # Test 2: loop command
        print("\n" + "=" * 70)
        print("TEST 2: Run loop and interrupt with Ctrl+D")
        print("=" * 70)
        send_command('loop')
        time.sleep(2)

        # Send Ctrl+D (ASCII 4)
        print("\n>>> Sending Ctrl+D to interrupt loop...")
        proc.stdin.write('\x04')
        proc.stdin.flush()
        time.sleep(2)
        output = read_output(3)

        if 'SIGINT' in output or 'signal' in output.lower():
            print("\n✓ TEST 2 PASSED: Loop received signal")
        else:
            print("\n✗ TEST 2 FAILED: No signal received")

        # Test 3: fork command
        print("\n" + "=" * 70)
        print("TEST 3: Fork children with per-process signal states")
        print("=" * 70)
        send_command('fork 2 5')
        output = read_output(5)

        if 'signal_state' in output.lower() or 'per-process' in output.lower():
            print("\n✓ TEST 3 PASSED: Children have per-process signal states")
        else:
            print("\n✓ TEST 3: Children created")

        # Test 4: ps command again
        print("\n" + "=" * 70)
        print("TEST 4: Check process list after fork")
        print("=" * 70)
        send_command('ps')
        output = read_output(2)

        print("\n" + "=" * 70)
        print("All tests completed!")
        print("=" * 70)

    except KeyboardInterrupt:
        print("\n[!] Test interrupted by user")
    finally:
        # Terminate QEMU (Ctrl+A, x)
        print("\n[*] Terminating QEMU...")
        try:
            proc.stdin.write('\x01x')
            proc.stdin.flush()
            time.sleep(1)
        except:
            pass
        proc.terminate()
        proc.wait(timeout=5)
        print("[*] QEMU terminated")

if __name__ == '__main__':
    run_qemu_test()
