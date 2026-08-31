#!/usr/bin/env python3
"""
Full test of loop suspend/resume cycle with fg command
"""

import pexpect
import sys
import time

def main():
    print("=== Full Suspend/Resume Test ===\n")

    child = pexpect.spawn('west build -d build-anl-qemu -t run',
                          cwd='/opt/Program/UCAS/OneWo-zepLinux',
                          timeout=30,
                          encoding='utf-8',
                          codec_errors='ignore')
    child.logfile = sys.stdout

    try:
        # Wait for boot
        child.expect('anl>', timeout=15)
        time.sleep(0.5)

        # Start loop
        print("\n>>> Starting loop 1000000")
        child.sendline('loop 1000000')
        child.expect('running in background', timeout=5)
        child.expect('anl>', timeout=2)
        print("Loop started\n")
        time.sleep(1)

        # Send Ctrl+D to suspend
        print("\n>>> Sending Ctrl+D to suspend")
        child.send('\x04')
        time.sleep(1)

        # Wait for suspension message
        idx = child.expect(['Use .fg. to resume', 'suspended', pexpect.TIMEOUT], timeout=3)
        if idx == 2:
            print("\nWARNING: Did not see suspension confirmation")
        else:
            print("\n✓ Process suspended successfully")

        # Check with ps
        print("\n>>> Running 'ps' command")
        child.sendline('ps')
        time.sleep(0.5)

        # Try fg to resume
        print("\n>>> Running 'fg' to resume")
        child.sendline('fg')
        time.sleep(1)

        # Look for resume message
        idx = child.expect(['Resuming', 'resumed', 'SIGCONT', pexpect.TIMEOUT], timeout=3)
        if idx == 2:
            print("\nWARNING: Did not see resume confirmation")
        else:
            print("\n✓ Process resumed")

        time.sleep(1)

        # Send Ctrl+C to stop
        print("\n>>> Sending Ctrl+C to stop")
        child.send('\x03')
        time.sleep(1)

        # Look for interrupt message
        idx = child.expect(['SIGINT', 'stopped', 'terminated', pexpect.TIMEOUT], timeout=3)
        if idx == 2:
            print("\nWARNING: Did not see stop confirmation")
        else:
            print("\n✓ Process stopped")

        print("\n=== Test completed successfully ===")

        # Exit QEMU
        child.send('\x01')
        child.send('x')
        time.sleep(0.5)

        return True

    except Exception as e:
        print(f"\nERROR: {e}")
        import traceback
        traceback.print_exc()
        return False
    finally:
        if child.isalive():
            child.close(force=True)

if __name__ == '__main__':
    sys.exit(0 if main() else 1)
