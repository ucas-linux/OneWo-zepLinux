# Device Interface Demos

This file is used to plan device control path demos and map documentation to actual directories in the repository.

## 1. Corresponding Directories

Currently recommended device demo directory:

```text
tests/demo/device/
├── README.md
├── gpio-device-demo.md
├── irq-notify-demo.md
└── nonblock-io-demo.md
```

The current repository doesn't yet have a ready-made `test_device.c`, so this part is first solidified in demo documentation form, with code to be added gradually later.

## 2. Why Separate `tests/demo/device/`

Device interface verification is quite different from ordinary unit tests, as it often requires:

- Specifying device objects or board-level peripherals
- Explaining the source of `fd`
- Explaining `ioctl` commands and parameter structures
- Explaining phenomenal differences between blocking/non-blocking modes

Therefore, having a separate directory is more conducive to accumulating examples later, rather than stuffing all content into a single `test_device.c`.

## 3. Recommended Demo Divisions

### 3.1 GPIO Control Demo

Recommended file: `tests/demo/device/gpio-device-demo.md`

Recommended content:

- How the compatibility layer establishes mapping between logical devices and `fd`
- How applications control GPIO through `open()`, `write()`, or `ioctl()`
- How to observe GPIO state changes

Recommended future code landing point:

- `tests/demo/device/gpio_device_demo.c`

### 3.2 IRQ Notification Demo

Recommended file: `tests/demo/device/irq-notify-demo.md`

Recommended content:

- How to trigger device interrupts or simulate interrupts
- How application layer reads events
- What phenomena occur with blocking vs non-blocking reads

Recommended future code landing point:

- `tests/demo/device/irq_notify_demo.c`

### 3.3 Non-Blocking I/O Demo

Recommended file: `tests/demo/device/nonblock-io-demo.md`

Recommended content:

- How to set device to non-blocking mode
- Return values when device has no data
- Behavior after retry reads

Recommended future code landing point:

- `tests/demo/device/nonblock_io_demo.c`

## 4. Current Closest Existing Content to Device Path

Although the repository doesn't yet have independent device tests, the following locations can serve as future implementation entry points:

- Header file: `code/include/lmz_device.h`
- Implementation: `code/src/lmz_device.c`
- Demo materials: `tests/demo/device/`

In other words, the device path is most suitable for adopting a "code interface + demo documentation + board-level validation" approach with all three advancing synchronously.
