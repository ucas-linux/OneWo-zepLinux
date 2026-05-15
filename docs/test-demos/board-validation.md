# Board-Level Validation

This file explains how board-level integration demos should map to repository structure and where future actual validation materials should be placed.

## 1. Corresponding Directories

Recommended board-level validation directory:

```text
tests/demo/board/
├── README.md
├── qemu-cortex-m3-validation.md
├── rocket-pi-validation.md
└── concurrent-device-validation.md
```

This still primarily uses documentation, because board-level validation often depends on actual hardware, logs, and wiring information, and may not be suitable for pure automated testing from the start.

## 2. Existing Foundation

From the current README, two points can be confirmed:

- One of the current main validation platforms is `qemu_cortex_m3`
- The repository goal is to implement Linux-style interfaces on Zephyr/MCU scenarios

Therefore, board-level validation can at least be initially divided into two categories:

- QEMU validation
- Real board validation

## 3. Recommended Validation Material Organization

### 3.1 QEMU Validation

Recommended file: `tests/demo/board/qemu-cortex-m3-validation.md`

Recommended content:

- Test directories to run
- Build commands
- Key points of QEMU output
- Which interfaces have already run successfully in QEMU
- Which issues cannot be observed with QEMU alone

### 3.2 Rocket Pi Validation

Recommended file: `tests/demo/board/rocket-pi-validation.md`

Recommended content:

- Board model
- Flashing method
- Serial port baud rate
- Peripherals used
- Execution steps
- Expected phenomena and abnormal phenomena

### 3.3 Concurrent Device Access Validation

Recommended file: `tests/demo/board/concurrent-device-validation.md`

Recommended content:

- Multi-thread scheduling policy combinations
- Device access combination scenarios
- Whether priority inversion, starvation, or event loss occurs

## 4. Relationship with Other Test Directories

- `tests/unit/` - More focused on interface-level correctness
- `tests/integration/` - More focused on cross-module coordination
- `tests/demo/board/` - More focused on actual integration process and observable phenomena

The three are not mutually exclusive but represent different levels of validation.
