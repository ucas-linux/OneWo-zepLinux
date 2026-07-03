/*
 * Copyright (c) 2024 OneWo-rtLinux Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Debug utilities for stack trace
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/**
 * @brief Print current thread stack trace
 */
void print_stack_trace(void)
{
	struct k_thread *current = k_current_get();

	printk("\n=== Stack Trace ===\n");
	printk("Current thread: %p\n", current);

	if (current) {
		printk("Thread name: %s\n", k_thread_name_get(current));
		printk("Thread ID: %p\n", current);
		printk("Thread stack: %p\n", current->stack_info.start);
		printk("Thread stack size: %zu\n", current->stack_info.size);
	}

	/* Get frame pointer and print backtrace */
	register unsigned long fp __asm__("s0");
	register unsigned long sp __asm__("sp");
	register unsigned long ra __asm__("ra");

	printk("\nRegisters:\n");
	printk("  fp (s0) = 0x%08lx\n", fp);
	printk("  sp      = 0x%08lx\n", sp);
	printk("  ra      = 0x%08lx\n", ra);

	printk("\nStack frames:\n");
	unsigned long *frame = (unsigned long *)fp;
	int depth = 0;

	while (frame && depth < 10) {
		unsigned long return_addr = *(frame - 1);
		unsigned long prev_fp = *(frame - 2);

		printk("  [%d] fp=0x%08lx, ra=0x%08lx\n",
		       depth, (unsigned long)frame, return_addr);

		/* Validate frame pointer */
		if (prev_fp == 0 || prev_fp < 0x20000000 || prev_fp > 0x20080000) {
			printk("  [%d] Invalid frame pointer, stopping\n", depth);
			break;
		}

		frame = (unsigned long *)prev_fp;
		depth++;
	}

	printk("===================\n\n");
}
