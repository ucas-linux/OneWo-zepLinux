#include <zephyr/toolchain.h>
#include <zephyr/arch/riscv/irq.h>

WTEXT(__soc_handle_irq)
SECTION_FUNC(exception.other, __soc_handle_irq)
	/* Clear the pending bit for this IRQ in CSR mip */
	li t1, 1
	sll t0, t1, a0
	csrrc t1, mip, t0
	ret
