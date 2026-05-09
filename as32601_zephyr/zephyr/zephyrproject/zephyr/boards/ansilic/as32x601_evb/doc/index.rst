AS32X601 EVB
############

Current minimal bring-up status
*******************************

This board currently supports a minimal Zephyr bring-up image that:

- boots from internal flash
- initializes the AS32X601 clock tree through vendor library code
- uses vendor UART0 print path
- uses vendor LED BSP path
- builds Zephyr ``.elf``, ``.bin`` and ``.hex`` images successfully

The current integration is intended for minimal compile-and-flash validation.
GPIO interrupts, full pinctrl/pinmux and broader peripheral enablement are not
complete yet.

Flashing
********

The board is wired for the ``openocd`` runner.

Build and flash the minimal PG15 LED sample with:

.. code-block:: bash

   export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
   export ZEPHYR_SDK_INSTALL_DIR=/home/mengqingyu/zypher/zephyr-sdk-0.17.4
   source ./setup_zephyr_env.sh
   west build -p always -b as32x601_evb/as32x601 apps/as32x601_pg15_min -d build-as32x601-pg15-min
   west flash -d build-as32x601-pg15-min

The OpenOCD target file is ``boards/ansilic/as32x601_evb/support/openocd.cfg``.
If your probe or JTAG chain differs, update the adapter/transport settings in
that file first.
