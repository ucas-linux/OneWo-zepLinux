# LMZ Tests

本目录存放 LMZ 兼容层相关的测试、样例和 demo 资料。当前仓库中的实际目录结构如下：

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
    ├── board/
    ├── device/
    ├── perf/
    └── scheduler/
```

## 目录说明

- `tests/unit/`：接口级单元测试，当前覆盖线程、内存、调度
- `tests/integration/`：跨组件联动验证
- `tests/stress/`：资源压力和并发场景
- `tests/examples/`：最小可运行样例
- `tests/demo/`：更偏联调、观察现象和手工验证的 demo

## 当前已落地内容

### 1. 单元测试

- `tests/unit/test_thread.c`：`pthread` 基础行为
- `tests/unit/test_memory.c`：内存分配与重分配行为
- `tests/unit/test_sched.c`：LMZ 调度接口基本验证

### 2. 样例

- `tests/examples/hello_lmz/`：LMZ 初始化、多线程、互斥锁基础样例

### 3. Demo

- `tests/demo/scheduler/rr_vs_fifo/`
- `tests/demo/scheduler/priority_mapping/`
- `tests/demo/scheduler/rr_interval/`

## 构建建议

当前各测试和 demo 的构建方式并不完全统一，建议按具体目录进入后使用 `west build`。例如：

```bash
cd tests/demo/scheduler/rr_vs_fifo
west build -b qemu_cortex_m3 .
west build -t run
```

如果是单元测试，建议后续进一步整理 `tests/unit/` 的构建入口，使其更适合统一回归。

## 相关文档

- `docs/test-demos/README.md`
- `docs/test-demos/scheduler-tests.md`
- `docs/test-demos/device-demos.md`
- `docs/test-demos/board-validation.md`
- `docs/test-demos/performance-benchmarks.md`
