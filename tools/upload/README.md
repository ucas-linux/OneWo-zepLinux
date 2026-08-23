# tools/upload

Serial uploader and example bytecode programs for the AS32x601 bytecode VM.

## Usage

```bash
sudo python3 upload_hex.py /dev/ttyUSB0 115200 <name> <file.bin>
```

Then in the shell:
```
shell> run <name>
shell> ls
shell> rm <name>
```

## Instruction Set

| Opcode | Mnemonic   | Operand         | Description                  |
|--------|------------|-----------------|------------------------------|
| `01`   | PUSH       | 4-byte big-endian int | Push value onto stack   |
| `02`   | POP        | —               | Discard top of stack         |
| `03`   | DUP        | —               | Duplicate top of stack       |
| `10`   | ADD        | —               | Pop two, push sum            |
| `11`   | SUB        | —               | Pop two, push difference     |
| `12`   | MUL        | —               | Pop two, push product        |
| `13`   | DIV        | —               | Pop two, push quotient       |
| `40`   | PRINT      | —               | Print top of stack as int    |
| `41`   | PRINT_STR  | len + bytes     | Print string                 |
| `42`   | SLEEP      | —               | Sleep top-of-stack ms        |
| `FF`   | HALT       | —               | Stop execution               |

## Example Programs

### hello.bin — Print 42
```
01 00 00 00 2A   PUSH 42
40               PRINT       → outputs: 42
FF               HALT
```

### add.bin — Add two numbers
```
01 00 00 00 0A   PUSH 10
01 00 00 00 14   PUSH 20
10               ADD         → 10 + 20 = 30
40               PRINT       → outputs: 30
FF               HALT
```

### div.bin — Integer division
```
01 00 00 00 64   PUSH 100
01 00 00 00 07   PUSH 7
13               DIV         → 100 / 7 = 14
40               PRINT       → outputs: 14
FF               HALT
```

### sleep.bin — Sleep then print
```
01 00 00 01 F4   PUSH 500
42               SLEEP       → sleep 500ms
01 00 00 00 63   PUSH 99
40               PRINT       → outputs: 99
FF               HALT
```

## Writing Your Own

Create a `.bin` file with raw opcode bytes. Example in Python:

```python
import struct

program = bytes([
    0x01, *struct.pack('>I', 42),  # PUSH 42
    0x40,                           # PRINT
    0xFF,                           # HALT
])

with open('my_program.bin', 'wb') as f:
    f.write(program)
```
