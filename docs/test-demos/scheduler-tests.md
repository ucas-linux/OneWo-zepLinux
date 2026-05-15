# Scheduler Interface Tests

This file explains how scheduling-related demos and tests map to actual directories in the repository and provides recommended example organization.

## 1. Corresponding Directories

Currently recommended scheduling-related paths:

```text
tests/unit/
├── test_sched.c
└── test_thread.c

tests/demo/scheduler/
├── README.md
├── rr_vs_fifo/
│   ├── CMakeLists.txt
│   ├── main.c
│   └── prj.conf
├── priority_mapping/
│   ├── CMakeLists.txt
│   ├── main.c
│   └── prj.conf
└── rr_interval/
    ├── CMakeLists.txt
    ├── main.c
    └── prj.conf
```

Explanation:

- `tests/unit/test_thread.c` already exists, suitable for carrying `pthread` basic behavior verification
- `tests/unit/test_sched.c` has been added, responsible for basic unit verification of LMZ scheduling interfaces
- `tests/demo/scheduler/` initially used to carry demo descriptions, can gradually add actual example programs later

## 2. Existing Base Examples

### `tests/examples/hello_lmz/`

Although this example is not specifically a scheduling demo, it can serve as a starting point for scheduling verification because it includes:

- `lmz_init()` initialization path
- `pthread_create()` thread creation
- `pthread_join()` waiting for thread completion
- Basic synchronization behavior of `pthread_mutex_*()`

Before having independent scheduling demo code, it can be used to verify:

- Whether threads can be created and reclaimed normally
- Whether multi-thread output meets basic expectations
- Whether mutex lock paths work normally

## 3. Recommended New Scheduling Demos

### 3.1 RR vs FIFO Distinction Demo

Recommended directory: `tests/demo/scheduler/`

Recommended files:

- Current directory: `tests/demo/scheduler/rr_vs_fifo/`
- Current entry: `tests/demo/scheduler/rr_vs_fifo/main.c`

Recommended content:

- Create two threads with the same priority
- Set one to `SCHED_FIFO`
- Set one to `SCHED_RR`
- Observe rotation behavior through loop printing, counters, or timestamps

Questions to verify:

- Does `SCHED_RR` truly driven by timeslice?
- Does `SCHED_FIFO` avoid being mistakenly rotated at the same priority?

### 3.2 Priority Mapping Demo

Recommended directory: `tests/demo/scheduler/`

Recommended files:

- Current directory: `tests/demo/scheduler/priority_mapping/`
- Current entry: `tests/demo/scheduler/priority_mapping/main.c`

Recommended content:

- Call `sched_setparam()` or `pthread_setschedparam()` to set priority
- Read back with `sched_getparam()` or `pthread_getschedparam()`
- Observe through thread execution order whether settings actually take effect

### 3.3 RR Timeslice Export Demo

Recommended directory: `tests/demo/scheduler/`

Recommended files:

- Current directory: `tests/demo/scheduler/rr_interval/`
- Current entry: `tests/demo/scheduler/rr_interval/main.c`

Recommended content:

- Call `sched_rr_get_interval()`
- Read current timeslice setting
- Verify whether return value is reliable combined with thread switching rhythm

## 4. Currently Directly Runnable Related Tests

### 4.1 Thread Unit Tests

File: `tests/unit/test_thread.c`

Suitable for verifying:

- Thread creation
- Thread exit and reclamation
- Basic mutex behavior

### 4.2 Hello LMZ Example

Directory: `tests/examples/hello_lmz/`

Suitable for verifying:

- LMZ initialization path
- Multi-thread execution
- Basic synchronization primitives

## 5. Future Recommendations

If the next step is to make scheduling demos into truly runnable tests, the most suitable landing points are:

- Interface-level checks already landed in `tests/unit/test_sched.c`
- Behavior visualization demos placed in `tests/demo/scheduler/`

This way it's convenient for both automated regression and manual observation of scheduling phenomena.

## 6. First Batch of Landed Demos

The first batch of compilable scheduling demos has been added to the current repository:

- `tests/demo/scheduler/rr_vs_fifo/`
- `tests/demo/scheduler/priority_mapping/`
- `tests/demo/scheduler/rr_interval/`

Their positioning is:

- First establish build paths
- First confirm whether LMZ scheduling interfaces are callable in the current code state
- After `CONFIG_SCHED_LINUX` is enabled, serve as starting points for further behavior verification

### Build Example

```bash
cd tests/demo/scheduler/rr_vs_fifo
west build -b qemu_cortex_m3 .
west build -t run
```

The other two demos are built the same way, just switch the directory to the corresponding demo root directory.
