#!/usr/bin/env python3
"""
Diagnostic test to see if SIGTSTP is being delivered and processed
"""

import pexpect
import sys
import time

def main():
    print("=== Signal Diagnostic Test ===\n")

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

        # Start loop with 1 million iterations
        print("\n>>> Starting loop 1000000")
        child.sendline('loop 1000000')
        child.expect('running in background', timeout=5)
        child.expect('anl>', timeout=2)
        print("Loop started in background\n")
        time.sleep(1)

        # Send Ctrl+D
        print("\n>>> Sending Ctrl+D")
        child.send('\x04')
        time.sleep(1)

        # Collect output for 3 seconds to see what happens
        print("\n>>> Collecting output for 3 seconds...")
        start = time.time()
        lines = []
        while time.time() - start < 3:
            try:
                line = child.readline()
                if line:
                    lines.append(line.strip())
            except:
                break

        print("\n=== Output after Ctrl+D ===")
        for line in lines[-50:]:  # Last 50 lines
            print(line)

        # Check for key messages
        print("\n=== Analysis ===")
        signal_sent = any('Sending SIGTSTP' in l for l in lines)
        signal_kill_success = any('kill(' in l and 'SUCCESS' in l for l in lines)
        signal_received = any('received SIGTSTP' in l for l in lines)
        signal_pending = any('pending=' in l for l in lines)
        suspending = any('Suspending process' in l for l in lines)
        suspended = any('suspended' in l.lower() for l in lines)

        print(f"Shell sent SIGTSTP: {signal_sent}")
        print(f"kill() succeeded: {signal_kill_success}")
        print(f"Process received SIGTSTP: {signal_received}")
        print(f"Pending signals detected: {signal_pending}")
        print(f"Suspending action called: {suspending}")
        print(f"Process marked suspended: {suspended}")

        # Exit
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
    main()
