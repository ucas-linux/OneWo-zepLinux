#!/usr/bin/env python3
"""
Bytecode Assembler for MCU VM
Converts assembly-like syntax to bytecode
"""

import struct
import sys

# Opcode definitions (must match bytecode_vm.h)
OPCODES = {
    # Stack Operations
    'PUSH': 0x01,
    'POP': 0x02,
    'DUP': 0x03,

    # Arithmetic
    'ADD': 0x10,
    'SUB': 0x11,
    'MUL': 0x12,
    'DIV': 0x13,

    # Comparison
    'EQ': 0x20,
    'NE': 0x21,
    'LT': 0x22,
    'GT': 0x23,

    # Control Flow
    'JMP': 0x30,
    'JZ': 0x31,
    'JNZ': 0x32,

    # System Calls
    'PRINT': 0x40,
    'PRINT_STR': 0x41,
    'SLEEP': 0x42,
    'GETPID': 0x43,

    # Control
    'HALT': 0xFF,
}

def assemble(lines):
    """Assemble text to bytecode"""
    bytecode = bytearray()
    labels = {}

    # First pass: collect labels
    pc = 0
    for line in lines:
        line = line.strip()
        if not line or line.startswith('#'):
            continue
        if line.endswith(':'):
            labels[line[:-1]] = pc
            continue

        parts = line.split()
        opcode_name = parts[0].upper()

        if opcode_name not in OPCODES:
            print(f"Unknown opcode: {opcode_name}")
            sys.exit(1)

        pc += 1  # opcode

        if opcode_name == 'PUSH' or opcode_name in ['JMP', 'JZ', 'JNZ']:
            pc += 4  # int32 operand
        elif opcode_name == 'PRINT_STR':
            string = ' '.join(parts[1:]).strip('"')
            pc += 1 + len(string)  # length + string

    # Second pass: generate bytecode
    for line in lines:
        line = line.strip()
        if not line or line.startswith('#') or line.endswith(':'):
            continue

        parts = line.split(None, 1)
        opcode_name = parts[0].upper()
        opcode = OPCODES[opcode_name]

        bytecode.append(opcode)

        if opcode_name == 'PUSH':
            value = int(parts[1])
            bytecode.extend(struct.pack('>i', value))
        elif opcode_name in ['JMP', 'JZ', 'JNZ']:
            label = parts[1]
            if label in labels:
                target = labels[label]
                bytecode.extend(struct.pack('>i', target))
            else:
                print(f"Undefined label: {label}")
                sys.exit(1)
        elif opcode_name == 'PRINT_STR':
            string = parts[1].strip('"')
            bytecode.append(len(string))
            bytecode.extend(string.encode('ascii'))

    return bytes(bytecode)

def generate_c_array(bytecode, name):
    """Generate C array initialization"""
    print(f"static const uint8_t prog_{name}[] = {{")
    for i in range(0, len(bytecode), 16):
        chunk = bytecode[i:i+16]
        hex_str = ', '.join(f'0x{b:02x}' for b in chunk)
        print(f"    {hex_str},")
    print("};")
    print(f"// Size: {len(bytecode)} bytes")

def main():
    if len(sys.argv) < 2:
        print("Usage: bytecode_asm.py <input.asm> [output_name]")
        print("\nExample assembly program:")
        print("# Hello World")
        print("PRINT_STR \"Hello, World!\\n\"")
        print("PUSH 42")
        print("PRINT")
        print("HALT")
        sys.exit(1)

    input_file = sys.argv[1]
    output_name = sys.argv[2] if len(sys.argv) > 2 else "program"

    with open(input_file, 'r') as f:
        lines = f.readlines()

    bytecode = assemble(lines)
    generate_c_array(bytecode, output_name)

    # Also save as binary
    bin_file = input_file.rsplit('.', 1)[0] + '.bin'
    with open(bin_file, 'wb') as f:
        f.write(bytecode)
    print(f"\nBinary saved to: {bin_file}")

if __name__ == '__main__':
    main()
