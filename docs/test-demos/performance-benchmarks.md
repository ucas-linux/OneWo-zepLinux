# Performance Benchmarks

This file explains how performance testing materials map to repository directories and provides recommended organization methods for future work.

## 1. Corresponding Directories

It's recommended to split performance-related content into two parts:

```text
tests/stress/
├── test_stress.c
└── prj.conf

tests/demo/perf/
├── README.md
├── sched-latency-benchmark.md
├── rr-jitter-benchmark.md
└── device-latency-benchmark.md
```

Where:

- `tests/stress/` - Suitable for carrying automatically runnable stress tests
- `tests/demo/perf/` - Suitable for recording performance test methods, observation points, and result organization methods

## 2. Existing Foundation

The current repository already has:

- `tests/stress/test_stress.c`

It already covers some valuable basic stress scenarios:

- Maximum thread creation
- Memory allocation pressure
- Mutex lock contention
- Simple fragmentation patterns

This means performance and stress testing is not starting from zero; we can continue expanding based on this file.

## 3. Recommended Additional Performance Topics

### 3.1 Scheduling Latency Benchmark

Recommended file: `tests/demo/perf/sched-latency-benchmark.md`

Recommended content:

- Thread switching observation methods
- Response times under different policies
- Latency changes under high load scenarios

### 3.2 RR Timeslice Jitter Benchmark

Recommended file: `tests/demo/perf/rr-jitter-benchmark.md`

Recommended content:

- Configured timeslice values
- Actually observed rotation rhythm
- Whether jitter is within acceptable range

### 3.3 Device Control Path Latency

Recommended file: `tests/demo/perf/device-latency-benchmark.md`

Recommended content:

- `ioctl` or device access call overhead
- Latency from interrupt to user-space visible event
- Average wait time in non-blocking retry mode

## 4. Recommended Positioning for Existing Stress Tests

File: `tests/stress/test_stress.c`

It's more suitable as a "resource and concurrency pressure" validation entry point rather than a complete performance analysis report. Therefore, it's recommended:

- Automated stress regression continues in `tests/stress/`
- Manual analysis and result accumulation goes in `tests/demo/perf/`

This way "testing" and "recording" are separated, making the structure clearer.
