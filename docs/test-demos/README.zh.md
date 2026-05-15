# zepLinux 测试与 Demo 资料

本目录用于集中整理 zepLinux 的测试样例、联调资料和验证说明，并且明确对应到仓库里的真实测试目录，避免文档和代码各写各的。

## 1. 当前测试目录映射

目前仓库里已经存在的测试与样例如下：

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

其中：

- `tests/examples/hello_lmz/` 适合作为最小可运行样例
- `tests/unit/` 适合放接口级单元测试
- `tests/integration/` 适合放跨组件联动验证
- `tests/stress/` 适合放资源压力和并发场景
- `tests/demo/` 用于放更偏“演示/联调/可观察现象”的测试资料和后续样例代码

本目录下的文档将与这些路径一一对应。

## 2. 文档与目录对应关系

- `docs/test-demos/scheduler-tests.md` 对应 `tests/unit/` 与 `tests/demo/scheduler/`
- `docs/test-demos/device-demos.md` 对应 `tests/demo/device/`
- `docs/test-demos/board-validation.md` 对应 `tests/demo/board/`
- `docs/test-demos/performance-benchmarks.md` 对应 `tests/stress/` 与 `tests/demo/perf/`

## 3. 维护原则

后续补充 demo 时，建议同步做到两件事：

1. 在 `tests/` 下创建对应目录或代码文件
2. 在 `docs/test-demos/` 中更新构建方法、运行方法和观察点

这样文档不会停留在“建议目录”层面，而是始终和实际仓库结构保持一致。
