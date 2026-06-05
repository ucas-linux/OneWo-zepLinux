# SPDX-License-Identifier: Apache-2.0

zephyr_sources(
  soc.c
  ${WEST_TOPDIR}/modules/hal/ansilic/as32x601/Peripherals/src/as32x601_plic.c
  ${WEST_TOPDIR}/modules/hal/ansilic/as32x601/Peripherals/src/as32x601_core.c
  ${WEST_TOPDIR}/modules/hal/ansilic/as32x601/Peripherals/src/as32x601_smu.c
  ${WEST_TOPDIR}/modules/hal/ansilic/as32x601/Peripherals/src/as32x601_adc.c
  ${WEST_TOPDIR}/modules/hal/ansilic/as32x601/Peripherals/src/as32x601_flash.c
  ${WEST_TOPDIR}/modules/hal/ansilic/as32x601/Peripherals/src/as32x601_gpio.c
  ${WEST_TOPDIR}/modules/hal/ansilic/as32x601/Peripherals/src/as32x601_usart.c
  ${WEST_TOPDIR}/modules/hal/ansilic/as32x601/Peripherals/src/as32x601_i2c.c
  ${WEST_TOPDIR}/modules/hal/ansilic/as32x601/Peripherals/src/as32x601_spi.c
  ${WEST_TOPDIR}/modules/hal/ansilic/as32x601/Peripherals/src/as32x601_qspi.c
  ${WEST_TOPDIR}/modules/hal/ansilic/as32x601/Peripherals/src/as32x601_canfd.c
)

zephyr_sources(
  ${ZEPHYR_BASE}/soc/ansilic/as32x601/soc_irq.S
  ${ZEPHYR_BASE}/soc/common/riscv-privileged/vector.S
)

set(SOC_LINKER_SCRIPT ${ZEPHYR_BASE}/include/zephyr/arch/riscv/common/linker.ld CACHE INTERNAL "")
zephyr_linker_sources(ROM_START SORT_KEY 0x0 linker.ld)

zephyr_include_directories(.)
zephyr_include_directories(${WEST_TOPDIR}/modules/hal/ansilic/as32x601/Core)
zephyr_include_directories(${WEST_TOPDIR}/modules/hal/ansilic/as32x601/Peripherals/inc)
zephyr_include_directories(${WEST_TOPDIR}/modules/hal/ansilic/as32x601/Peripherals/reg)
