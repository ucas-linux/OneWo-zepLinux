#!/usr/bin/env python3
"""Upload .anl binary to ANL loader via serial (load <name> <hexdata> protocol)."""
import sys, time, serial, argparse

def read_until(ser, prompt=b'anl> ', timeout=5.0):
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

def upload(port, baud, name, filepath):
    data = open(filepath, 'rb').read()
    hex_str = data.hex().upper()
    cmd = f'load {name} {hex_str}\r'

    with serial.Serial(port, baud, timeout=5) as ser:
        ser.write(b'\r')
        banner = read_until(ser, b'anl> ', timeout=5.0)
        print(banner.decode(errors='replace'), end='')

        # Send in chunks to avoid overrun
        chunk = 64
        for i in range(0, len(cmd), chunk):
            ser.write(cmd[i:i+chunk].encode())
            time.sleep(0.05)
        ser.flush()

        resp = read_until(ser, b'anl> ', timeout=15.0)
        print(resp.decode(errors='replace'))

if __name__ == '__main__':
    ap = argparse.ArgumentParser()
    ap.add_argument('port')
    ap.add_argument('baud', type=int)
    ap.add_argument('name')
    ap.add_argument('file')
    args = ap.parse_args()
    upload(args.port, args.baud, args.name, args.file)
