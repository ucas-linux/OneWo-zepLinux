#!/usr/bin/env python3
"""Upload bytecode file to AS32x601 via serial upload_hex command."""

import sys
import time
import serial

def read_until(ser, prompt=b'shell>', timeout=5.0):
    buf = b''
    end = time.time() + timeout
    while time.time() < end:
        if ser.in_waiting:
            buf += ser.read(ser.in_waiting)
            if prompt in buf:
                return buf
        else:
            time.sleep(0.05)
    return buf

def send_cmd(ser, cmd):
    for c in (cmd + '\r').encode():
        ser.write(bytes([c]))
        time.sleep(0.02)
    ser.flush()

def upload(port, baud, name, filepath):
    with open(filepath, 'rb') as f:
        data = f.read()

    hex_str = ''.join(f'{b:02X}' for b in data)
    cmd = f'upload_hex {name} {hex_str}'

    with serial.Serial(port, baud, timeout=5) as ser:
        # Wake shell
        ser.write(b'\r')
        banner = read_until(ser, b'shell>')
        print(banner.decode(errors='replace'))

        send_cmd(ser, cmd)
        response = read_until(ser, b'shell>', timeout=10.0)
        print(response.decode(errors='replace'))

if __name__ == '__main__':
    if len(sys.argv) != 5:
        print(f'Usage: {sys.argv[0]} <port> <baud> <name> <file>')
        sys.exit(1)
    upload(sys.argv[1], int(sys.argv[2]), sys.argv[3], sys.argv[4])
