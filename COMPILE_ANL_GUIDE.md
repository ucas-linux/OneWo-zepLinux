# ANL Module Compilation and Upload Guide (AS32x601)

## Prerequisites

1. **Zephyr SDK installed** at `~/zephyr-sdk-0.17.4/`
2. **AS32x601 board** connected via USB (appears as `/dev/ttyUSB0`)
3. **Shell firmware running** on the board (`as32x601_shell_process`)

## Step-by-Step Process

### 1. Compile Your C Program to Object File

For **AS32x601 (RISC-V 32-bit)**:

```bash
# Set toolchain path
export PATH=~/zephyr-sdk-0.17.4/riscv64-zephyr-elf/bin:$PATH

# Compile C source to .o file
riscv64-zephyr-elf-gcc -march=rv32imc -mabi=ilp32 \
    -c -O2 -nostdlib -fno-builtin -ffreestanding \
    tools/examples/fork_demo.c -o /tmp/fork_demo.o
```

**Important compiler flags:**
- `-march=rv32imc`: RISC-V 32-bit with compressed instructions
- `-mabi=ilp32`: 32-bit integer ABI
- `-nostdlib`: Don't link standard library
- `-fno-builtin`: Don't use built-in functions
- `-ffreestanding`: Freestanding environment (no hosted environment)

### 2. Link Object File to ANL Format

```bash
python3 tools/anl_link.py /tmp/fork_demo.o /tmp/fork_demo.anl --entry main
```

This converts the ELF `.o` file to ANL binary format with relocations.

### 3. Upload to Board via Serial

```bash
sudo python3 tools/upload/upload_hex.py /dev/ttyUSB0 115200 fork_demo /tmp/fork_demo.anl
```

**Parameters:**
- `/dev/ttyUSB0`: Serial port
- `115200`: Baud rate
- `fork_demo`: Program name (used to reference it on the board)
- `/tmp/fork_demo.anl`: Path to ANL binary

### 4. Run the Program on the Board

Connect to the serial console:
```bash
sudo minicom -D /dev/ttyUSB0 -b 115200
```

Or use screen:
```bash
sudo screen /dev/ttyUSB0 115200
```

Then in the shell prompt:
```
shell> ls
shell> run fork_demo
```

## Complete Example Script

Save this as `build_and_upload.sh`:

```bash
#!/bin/bash
set -e

# Configuration
TOOLCHAIN=~/zephyr-sdk-0.17.4/riscv64-zephyr-elf/bin
SOURCE_FILE=$1
PROGRAM_NAME=$(basename "$SOURCE_FILE" .c)
PORT=${2:-/dev/ttyUSB0}
BAUD=${3:-115200}

if [ -z "$SOURCE_FILE" ]; then
    echo "Usage: $0 <source.c> [port] [baud]"
    echo "Example: $0 tools/examples/fork_demo.c /dev/ttyUSB0 115200"
    exit 1
fi

# Add toolchain to PATH
export PATH=$TOOLCHAIN:$PATH

echo "==> Compiling $SOURCE_FILE to object file..."
riscv64-zephyr-elf-gcc -march=rv32imc -mabi=ilp32 \
    -c -O2 -nostdlib -fno-builtin -ffreestanding \
    "$SOURCE_FILE" -o "/tmp/${PROGRAM_NAME}.o"

echo "==> Linking to ANL format..."
python3 tools/anl_link.py "/tmp/${PROGRAM_NAME}.o" "/tmp/${PROGRAM_NAME}.anl" --entry main

echo "==> Uploading to board at $PORT..."
sudo python3 tools/upload/upload_hex.py "$PORT" "$BAUD" "$PROGRAM_NAME" "/tmp/${PROGRAM_NAME}.anl"

echo ""
echo "==> Done! Connect to serial and run:"
echo "    shell> run $PROGRAM_NAME"
```

Make it executable:
```bash
chmod +x build_and_upload.sh
```

Use it:
```bash
./build_and_upload.sh tools/examples/fork_demo.c
```

## Example Programs

### Simple Hello World (`hello_simple.c`)

```c
#include <stdint.h>

extern void printk(const char *fmt, ...);

void main(void)
{
    printk("Hello from ANL module!\n");
}
```

### Fork Demo (`fork_demo.c`)

Already available in `tools/examples/fork_demo.c` - demonstrates creating child tasks.

## Troubleshooting

### Permission Denied on /dev/ttyUSB0

```bash
sudo usermod -a -G dialout $USER
# Log out and log back in
```

Or use sudo:
```bash
sudo python3 tools/upload/upload_hex.py ...
```

### Toolchain Not Found

Check SDK installation:
```bash
ls ~/zephyr-sdk-0.17.4/riscv64-zephyr-elf/bin/riscv64-zephyr-elf-gcc
```

### Upload Timeout

- Ensure the shell firmware is running on the board
- Check serial port: `ls -la /dev/ttyUSB*`
- Verify baud rate matches (115200)
- Press Enter in the shell to wake it up

### Program Crashes on Run

- Check that you're using the correct architecture flags (`-march=rv32imc`)
- Ensure all external functions are properly declared
- Verify entry point is set to `main`

## Available Functions in ANL Modules

These functions are available from the kernel:

```c
extern void printk(const char *fmt, ...);
extern void k_msleep(int ms);
extern int new_task(const char *name, void *(*fn)(void *), void *arg);
extern int getpid(void);
extern void exit(int status);
```

Check the kernel symbols for more available functions.
