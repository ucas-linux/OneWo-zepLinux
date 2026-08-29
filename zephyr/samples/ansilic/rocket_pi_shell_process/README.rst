.. _rocket_pi_shell_process:

RocketPi Shell with Process Support
####################################

Overview
********

This sample demonstrates a shell environment with full process support on
the RocketPi (STM32F401) board. It implements an Embox-compatible process
model with signal support, including Ctrl+C interrupt handling.

Features
********

- Process-based command execution
- Signal support (SIGINT, SIGTERM, etc.)
- Ctrl+C keyboard interrupt (sends SIGINT to foreground process)
- Foreground/background process management
- Process listing and control
- Memory management commands
- Bytecode VM integration
- ANL (Ansilic Native Library) loader support

Building and Running
********************

Build the application for RocketPi:

.. code-block:: console

   west build -p always -b rocket_pi -d build-rocket-pi-anl zephyr/samples/ansilic/rocket_pi_shell_process

Flash to the board using OpenOCD:

.. code-block:: console

   west flash -d build-rocket-pi-anl --runner openocd

Connect via serial terminal:

.. code-block:: console

   picocom -b 115200 /dev/ttyUSB0

Sample Output
*************

.. code-block:: console

   ========================================
     RocketPi Shell with Process Support
     Based on Embox process model
   ========================================

   Init process PID: 1

   Available commands:
     echo      - Echo arguments
     ps        - List processes
     loop      - Loop for signal testing
     sigint    - Send SIGINT to foreground process
     test_signal - Self-test signal delivery
     ...

   Each command runs in a separate process.
   Run 'test_signal' to verify signal delivery works.
   Press Ctrl+C to send SIGINT to foreground process.

   shell>

Testing Ctrl+C Signal Support
******************************

1. Start a long-running command:

   .. code-block:: console

      shell> loop 10

2. Press Ctrl+C to interrupt it:

   .. code-block:: console

      Loop started (PID 2). Running for 10 seconds...
      ....^CKilled
      shell>

3. Run signal self-test:

   .. code-block:: console

      shell> test_signal

Architecture
************

The implementation consists of:

- **main.c**: Shell command registration and bridging
- **shell_process.c**: Process creation and management
- **signal.c**: Signal delivery and handling (POSIX-like)
- **commands.c**: Individual command implementations
- **shell_uart.c**: UART backend with Ctrl+C interception (Zephyr subsystem)

Key difference from AS32x601 version: RocketPi uses ARM Cortex-M4 instead
of RISC-V architecture.

References
**********

- Commit 1534e5fb: Ctrl+C signal support for AS32x601
- Embox process model documentation
