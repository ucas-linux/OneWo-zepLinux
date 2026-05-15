# zepLinux Test and Demo Materials

This directory is used to centrally organize zepLinux test examples, integration materials, and validation documentation, with clear correspondence to actual test directories in the repository to avoid documentation and code being written separately.

## 1. Current Test Directory Mapping

Currently existing tests and examples in the repository:

```text
tests/
├── examples/
│   └── hello_lmz/
├── integration/
│   └── test_process.c
├── stress/
│   └── test_stress.c
├── unit/
│   ├── test_memory.c
│   ├── test_sched.c
│   └── test_thread.c
└── demo/
    ├── scheduler/
    ├── device/
    ├── board/
    └── perf/
```

Where:

- `tests/examples/hello_lmz/` - Suitable as minimal runnable example
- `tests/unit/` - Suitable for interface-level unit tests
- `tests/integration/` - Suitable for cross-component integration validation
- `tests/stress/` - Suitable for resource pressure and concurrency scenarios
- `tests/demo/` - Used for more "demonstration/integration/observable phenomena" test materials and subsequent example code

Documentation in this directory will correspond one-to-one with these paths.

## 2. Documentation to Directory Correspondence

- `docs/test-demos/scheduler-tests.md` corresponds to `tests/unit/` and `tests/demo/scheduler/`
- `docs/test-demos/device-demos.md` corresponds to `tests/demo/device/`
- `docs/test-demos/board-validation.md` corresponds to `tests/demo/board/`
- `docs/test-demos/performance-benchmarks.md` corresponds to `tests/stress/` and `tests/demo/perf/`

## 3. Maintenance Principles

When adding demos in the future, it's recommended to do two things synchronously:

1. Create corresponding directories or code files under `tests/`
2. Update build methods, execution methods, and observation points in `docs/test-demos/`

This way documentation won't remain at the "suggested directory" level but will always stay consistent with actual repository structure.
