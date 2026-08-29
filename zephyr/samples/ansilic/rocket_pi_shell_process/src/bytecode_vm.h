/*
 * Bytecode Virtual Machine for MCU
 * Simple stack-based VM for executing uploaded programs
 */

#ifndef BYTECODE_VM_H
#define BYTECODE_VM_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Bytecode Instruction Set */
enum vm_opcode {
    /* Stack Operations */
    OP_PUSH = 0x01,      // PUSH <value>      - Push immediate value to stack
    OP_POP = 0x02,       // POP               - Pop value from stack
    OP_DUP = 0x03,       // DUP               - Duplicate top of stack

    /* Arithmetic Operations */
    OP_ADD = 0x10,       // ADD               - Pop two values, push sum
    OP_SUB = 0x11,       // SUB               - Pop two values, push difference
    OP_MUL = 0x12,       // MUL               - Pop two values, push product
    OP_DIV = 0x13,       // DIV               - Pop two values, push quotient

    /* Comparison Operations */
    OP_EQ = 0x20,        // EQ                - Push 1 if equal, 0 otherwise
    OP_NE = 0x21,        // NE                - Push 1 if not equal
    OP_LT = 0x22,        // LT                - Push 1 if less than
    OP_GT = 0x23,        // GT                - Push 1 if greater than

    /* Control Flow */
    OP_JMP = 0x30,       // JMP <offset>      - Unconditional jump
    OP_JZ = 0x31,        // JZ <offset>       - Jump if top of stack is zero
    OP_JNZ = 0x32,       // JNZ <offset>      - Jump if top of stack is not zero

    /* System Calls */
    OP_PRINT = 0x40,     // PRINT             - Print top of stack as integer
    OP_PRINT_STR = 0x41, // PRINT_STR <len>   - Print string from bytecode
    OP_SLEEP = 0x42,     // SLEEP             - Sleep for top of stack ms
    OP_GETPID = 0x43,    // GETPID            - Push current process ID

    /* Program Control */
    OP_HALT = 0xFF,      // HALT              - Stop execution
};

/* VM Configuration */
#define VM_STACK_SIZE 64
#define VM_MAX_PROGRAM_SIZE 2048

/* VM State */
struct vm_state {
    int32_t stack[VM_STACK_SIZE];
    int sp;                      // Stack pointer (-1 = empty)
    const uint8_t *code;         // Program bytecode
    size_t code_size;            // Program size
    size_t pc;                   // Program counter
    bool running;                // Execution status
};

/* Program Metadata */
struct bytecode_program {
    char name[32];               // Program name
    uint8_t *code;               // Bytecode
    size_t code_size;            // Code size
    bool in_use;                 // Slot occupied
};

/* VM API */
int vm_init(struct vm_state *vm, const uint8_t *code, size_t code_size);
int vm_run(struct vm_state *vm);
void vm_cleanup(struct vm_state *vm);

/* Program Management */
int vm_load_program(const char *name, const uint8_t *code, size_t size);
int vm_delete_program(const char *name);
struct bytecode_program *vm_find_program(const char *name);
void vm_list_programs(void);
int vm_execute_program(const char *name, int argc, char **argv);

#endif /* BYTECODE_VM_H */
