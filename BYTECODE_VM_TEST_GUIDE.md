# Bytecode VM Test Guide

## Overview
This guide shows how to test the bytecode virtual machine implementation on QEMU Cortex-M3.

## Starting QEMU
```bash
cd /opt/Program/UCAS/OneWo-zepLinux
west build -t run
```

## Available Commands

### 1. ls - List programs
Lists both built-in commands and loaded bytecode programs.
```
qemu> ls
```

### 2. upload - Upload a bytecode program
Syntax: `upload <name> <program_id>`

Available program_id:
- `hello` - Prints "Hello World!" and the number 42
- `counter` - Counts from 1 to 10 with delays
- `calc` - Demonstrates arithmetic: (5+3)*2 = 16

Example:
```
qemu> upload myhello hello
qemu> upload mycounter counter
qemu> upload mycalc calc
```

### 3. run - Execute a bytecode program
Syntax: `run <program_name>`

Example:
```
qemu> run myhello
qemu> run mycounter
qemu> run mycalc
```

### 4. rm - Delete a bytecode program
Syntax: `rm <program_name>`

Example:
```
qemu> rm myhello
```

## Test Scenarios

### Scenario 1: Hello World
```
qemu> ls
qemu> upload myhello hello
qemu> ls
qemu> run myhello
```
Expected output:
```
Hello World!
42
```

### Scenario 2: Counter
```
qemu> upload mycounter counter
qemu> run mycounter
```
Expected output: Numbers 1-10 printed with delays

### Scenario 3: Calculator
```
qemu> upload mycalc calc
qemu> run mycalc
```
Expected output:
```
Calc: (5+3)*2 =
16
```

### Scenario 4: Multiple Programs
```
qemu> upload prog1 hello
qemu> upload prog2 counter
qemu> upload prog3 calc
qemu> ls
qemu> run prog1
qemu> run prog2
qemu> run prog3
qemu> rm prog1
qemu> ls
```

## Bytecode Instruction Set

The VM implements a simple stack-based instruction set:

### Stack Operations
- `OP_PUSH` - Push immediate value
- `OP_POP` - Pop value from stack
- `OP_DUP` - Duplicate top of stack

### Arithmetic
- `OP_ADD`, `OP_SUB`, `OP_MUL`, `OP_DIV`

### Comparison
- `OP_EQ`, `OP_NE`, `OP_LT`, `OP_GT`

### Control Flow
- `OP_JMP` - Unconditional jump
- `OP_JZ` - Jump if zero
- `OP_JNZ` - Jump if not zero

### System Calls
- `OP_PRINT` - Print integer
- `OP_PRINT_STR` - Print string
- `OP_SLEEP` - Sleep for milliseconds
- `OP_GETPID` - Get process ID

### Control
- `OP_HALT` - Stop execution

## Network Upload (Future)

In the future, programs can be uploaded via network:
1. Use TCP/UDP to send bytecode
2. MCU receives and stores in memory
3. Programs become available via `ls`
4. Execute with `run <name>`

This is ideal for:
- Dynamic firmware updates
- Field-deployable scripts
- Remote debugging tools
- Downloadable utilities
