/* SPDX-License-Identifier: Apache-2.0 */

#include <zephyr/toolchain.h>
#include <zephyr/linker/sections.h>
#include <zephyr/arch/riscv/irq.h>
WTEXT(__soc_handle_irq)
GTEXT(as32x601_handle_irq_c)

SECTION_FUNC(exception.other, __soc_handle_irq)
	li t1, RISCV_IRQ_MEXT
	beq a0, t1, 1f

	li t1, 1
	sll t0, t1, a0
	csrrc t1, mip, t0
	ret

1:
	addi sp, sp, -16
	sw t0, 0(sp)
	sw t1, 4(sp)
	sw ra, 8(sp)
	call as32x601_handle_irq_c
	lw ra, 8(sp)
	lw t1, 4(sp)
	lw t0, 0(sp)
	addi sp, sp, 16
	ret
