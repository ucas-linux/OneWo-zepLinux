.. _as32x601_shell_process:

Shell with Process-based Command Execution
##########################################

Overview
********

This sample demonstrates Embox-style shell command execution on the AS32X601
platform, where each shell command executes in a new process created via
``new_task()``. This provides process isolation similar to Unix shells.

Features
********

- Each command runs in a separate process with its own context
- Process management using the Zephyr process subsystem
- Support for foreground command execution with wait
- Command registration framework
- Process hierarchy and resource inheritance

Building and Running
********************

Build the sample for as32x601_evb board:

.. zephyr-app-commands::
   :zephyr-app: samples/ansilic/as32x601_shell_process
   :board: as32x601_evb/as32x601
   :goals: build flash
   :compact:

Sample Commands
***************

The following commands are available:

- ``hello`` - Print hello message
- ``ps`` - List all processes
- ``echo <args>`` - Echo arguments
- ``help`` - Show available commands

Sample Output
*************

.. code-block:: console

   shell> hello
   Hello from process PID 2!

   shell> ps
   PID    PPID   Command
   1      0      init
   2      1      hello

   shell> echo test 123
   test 123
