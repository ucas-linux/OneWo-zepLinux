/*
 * Bytecode Virtual Machine Implementation
 */

#include "bytecode_vm.h"
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <string.h>

#define MAX_PROGRAMS 8

static struct bytecode_program program_pool[MAX_PROGRAMS];

/* Initialize VM state */
int vm_init(struct vm_state *vm, const uint8_t *code, size_t code_size)
{
    if (!vm || !code || code_size == 0 || code_size > VM_MAX_PROGRAM_SIZE) {
        return -EINVAL;
    }

    memset(vm, 0, sizeof(*vm));
    vm->code = code;
    vm->code_size = code_size;
    vm->pc = 0;
    vm->sp = -1;  // Empty stack
    vm->running = true;

    return 0;
}

/* Stack operations */
static inline int vm_push(struct vm_state *vm, int32_t value)
{
    if (vm->sp >= VM_STACK_SIZE - 1) {
        printk("VM ERROR: Stack overflow\n");
        return -ENOMEM;
    }
    vm->stack[++vm->sp] = value;
    return 0;
}

static inline int vm_pop(struct vm_state *vm, int32_t *value)
{
    if (vm->sp < 0) {
        printk("VM ERROR: Stack underflow\n");
        return -EINVAL;
    }
    *value = vm->stack[vm->sp--];
    return 0;
}

static inline int32_t vm_peek(struct vm_state *vm)
{
    if (vm->sp < 0) {
        return 0;
    }
    return vm->stack[vm->sp];
}

/* Read bytecode helpers */
static inline uint8_t read_byte(struct vm_state *vm)
{
    if (vm->pc >= vm->code_size) {
        return OP_HALT;
    }
    return vm->code[vm->pc++];
}

static inline int32_t read_int32(struct vm_state *vm)
{
    int32_t value = 0;
    if (vm->pc + 4 <= vm->code_size) {
        value = (vm->code[vm->pc] << 24) |
                (vm->code[vm->pc + 1] << 16) |
                (vm->code[vm->pc + 2] << 8) |
                vm->code[vm->pc + 3];
        vm->pc += 4;
    }
    return value;
}

/* Execute one instruction */
static int vm_step(struct vm_state *vm)
{
    uint8_t opcode = read_byte(vm);
    int32_t a, b, result;
    int ret;

    switch (opcode) {
    case OP_PUSH:
        result = read_int32(vm);
        return vm_push(vm, result);

    case OP_POP:
        return vm_pop(vm, &a);

    case OP_DUP:
        a = vm_peek(vm);
        return vm_push(vm, a);

    case OP_ADD:
        ret = vm_pop(vm, &b);
        if (ret < 0) return ret;
        ret = vm_pop(vm, &a);
        if (ret < 0) return ret;
        return vm_push(vm, a + b);

    case OP_SUB:
        ret = vm_pop(vm, &b);
        if (ret < 0) return ret;
        ret = vm_pop(vm, &a);
        if (ret < 0) return ret;
        return vm_push(vm, a - b);

    case OP_MUL:
        ret = vm_pop(vm, &b);
        if (ret < 0) return ret;
        ret = vm_pop(vm, &a);
        if (ret < 0) return ret;
        return vm_push(vm, a * b);

    case OP_DIV:
        ret = vm_pop(vm, &b);
        if (ret < 0) return ret;
        if (b == 0) {
            printk("VM ERROR: Division by zero\n");
            return -EINVAL;
        }
        ret = vm_pop(vm, &a);
        if (ret < 0) return ret;
        return vm_push(vm, a / b);

    case OP_EQ:
        ret = vm_pop(vm, &b);
        if (ret < 0) return ret;
        ret = vm_pop(vm, &a);
        if (ret < 0) return ret;
        return vm_push(vm, a == b ? 1 : 0);

    case OP_NE:
        ret = vm_pop(vm, &b);
        if (ret < 0) return ret;
        ret = vm_pop(vm, &a);
        if (ret < 0) return ret;
        return vm_push(vm, a != b ? 1 : 0);

    case OP_LT:
        ret = vm_pop(vm, &b);
        if (ret < 0) return ret;
        ret = vm_pop(vm, &a);
        if (ret < 0) return ret;
        return vm_push(vm, a < b ? 1 : 0);

    case OP_GT:
        ret = vm_pop(vm, &b);
        if (ret < 0) return ret;
        ret = vm_pop(vm, &a);
        if (ret < 0) return ret;
        return vm_push(vm, a > b ? 1 : 0);

    case OP_JMP:
        result = read_int32(vm);
        vm->pc = (size_t)result;
        if (vm->pc >= vm->code_size) {
            printk("VM ERROR: Jump out of bounds\n");
            return -EINVAL;
        }
        return 0;

    case OP_JZ:
        result = read_int32(vm);
        ret = vm_pop(vm, &a);
        if (ret < 0) return ret;
        if (a == 0) {
            vm->pc = (size_t)result;
            if (vm->pc >= vm->code_size) {
                printk("VM ERROR: Jump out of bounds\n");
                return -EINVAL;
            }
        }
        return 0;

    case OP_JNZ:
        result = read_int32(vm);
        ret = vm_pop(vm, &a);
        if (ret < 0) return ret;
        if (a != 0) {
            vm->pc = (size_t)result;
            if (vm->pc >= vm->code_size) {
                printk("VM ERROR: Jump out of bounds\n");
                return -EINVAL;
            }
        }
        return 0;

    case OP_PRINT:
        ret = vm_pop(vm, &a);
        if (ret < 0) return ret;
        printk("%d\n", a);
        return 0;

    case OP_PRINT_STR: {
        uint8_t len = read_byte(vm);
        if (vm->pc + len > vm->code_size) {
            printk("VM ERROR: String out of bounds\n");
            return -EINVAL;
        }
        for (uint8_t i = 0; i < len; i++) {
            printk("%c", vm->code[vm->pc++]);
        }
        return 0;
    }

    case OP_SLEEP:
        ret = vm_pop(vm, &a);
        if (ret < 0) return ret;
        if (a > 0) {
            k_msleep(a);
        }
        return 0;

    case OP_GETPID:
        /* In Zephyr, we can use k_current_get() */
        return vm_push(vm, (int32_t)(uintptr_t)k_current_get());

    case OP_HALT:
        vm->running = false;
        return 0;

    default:
        printk("VM ERROR: Unknown opcode 0x%02x at PC=%zu\n", opcode, vm->pc - 1);
        return -EINVAL;
    }
}

/* Run the VM until halt or error */
int vm_run(struct vm_state *vm)
{
    int ret;
    int instruction_count = 0;
    const int MAX_INSTRUCTIONS = 100000;  // Prevent infinite loops

    if (!vm || !vm->running) {
        return -EINVAL;
    }

    while (vm->running && vm->pc < vm->code_size) {
        ret = vm_step(vm);
        if (ret < 0) {
            printk("VM execution failed at PC=%zu\n", vm->pc);
            return ret;
        }

        instruction_count++;
        if (instruction_count > MAX_INSTRUCTIONS) {
            printk("VM ERROR: Instruction limit exceeded (infinite loop?)\n");
            return -ETIMEDOUT;
        }

        /* Yield to other threads periodically */
        if (instruction_count % 100 == 0) {
            k_yield();
        }
    }

    return 0;
}

/* Cleanup VM state */
void vm_cleanup(struct vm_state *vm)
{
    if (vm) {
        vm->running = false;
        vm->sp = -1;
        vm->pc = 0;
    }
}

/* Program Management Functions */

int vm_load_program(const char *name, const uint8_t *code, size_t size)
{
    if (!name || !code || size == 0 || size > VM_MAX_PROGRAM_SIZE) {
        return -EINVAL;
    }

    /* Check if program already exists */
    for (int i = 0; i < MAX_PROGRAMS; i++) {
        if (program_pool[i].in_use &&
            strcmp(program_pool[i].name, name) == 0) {
            printk("Program '%s' already exists\n", name);
            return -EEXIST;
        }
    }

    /* Find free slot */
    for (int i = 0; i < MAX_PROGRAMS; i++) {
        if (!program_pool[i].in_use) {
            /* Allocate memory for bytecode */
            program_pool[i].code = k_malloc(size);
            if (!program_pool[i].code) {
                printk("Failed to allocate memory for program\n");
                return -ENOMEM;
            }

            /* Copy bytecode */
            memcpy(program_pool[i].code, code, size);
            strncpy(program_pool[i].name, name, sizeof(program_pool[i].name) - 1);
            program_pool[i].name[sizeof(program_pool[i].name) - 1] = '\0';
            program_pool[i].code_size = size;
            program_pool[i].in_use = true;

            printk("Program '%s' loaded (%zu bytes)\n", name, size);
            return 0;
        }
    }

    printk("No free program slots\n");
    return -ENOMEM;
}

int vm_delete_program(const char *name)
{
    if (!name) {
        return -EINVAL;
    }

    for (int i = 0; i < MAX_PROGRAMS; i++) {
        if (program_pool[i].in_use &&
            strcmp(program_pool[i].name, name) == 0) {
            k_free(program_pool[i].code);
            memset(&program_pool[i], 0, sizeof(program_pool[i]));
            printk("Program '%s' deleted\n", name);
            return 0;
        }
    }

    printk("Program '%s' not found\n", name);
    return -ENOENT;
}

struct bytecode_program *vm_find_program(const char *name)
{
    if (!name) {
        return NULL;
    }

    for (int i = 0; i < MAX_PROGRAMS; i++) {
        if (program_pool[i].in_use &&
            strcmp(program_pool[i].name, name) == 0) {
            return &program_pool[i];
        }
    }

    return NULL;
}

void vm_list_programs(void)
{
    int count = 0;

    printk("Loaded programs:\n");
    for (int i = 0; i < MAX_PROGRAMS; i++) {
        if (program_pool[i].in_use) {
            printk("  %-16s  %5zu bytes\n",
                   program_pool[i].name,
                   program_pool[i].code_size);
            count++;
        }
    }

    if (count == 0) {
        printk("  (none)\n");
    }
}

/* Execute program in current context (for testing) */
int vm_execute_program(const char *name, int argc, char **argv)
{
    struct bytecode_program *prog = vm_find_program(name);
    if (!prog) {
        printk("Program '%s' not found\n", name);
        return -ENOENT;
    }

    struct vm_state vm;
    int ret;

    ret = vm_init(&vm, prog->code, prog->code_size);
    if (ret < 0) {
        printk("Failed to initialize VM\n");
        return ret;
    }

    printk("Executing '%s'...\n", name);
    ret = vm_run(&vm);

    if (ret == 0) {
        printk("Program '%s' completed successfully\n", name);
    } else {
        printk("Program '%s' failed with error %d\n", name, ret);
    }

    vm_cleanup(&vm);
    return ret;
}
