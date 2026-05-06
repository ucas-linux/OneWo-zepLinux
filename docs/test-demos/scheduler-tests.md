# 调度接口测试

本文件说明调度相关 demo 和测试如何落到仓库中的真实目录，并给出建议的样例组织方式。

## 1. 对应目录

当前建议的调度相关路径如下：

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

说明：

- `tests/unit/test_thread.c` 已存在，适合承载 `pthread` 基础行为验证
- `tests/unit/test_sched.c` 已补入，负责 LMZ 调度接口的基本单元验证
- `tests/demo/scheduler/` 先用于承载 demo 说明，后续可以逐步补充真正的样例程序

## 2. 已有基础样例

### `tests/examples/hello_lmz/`

这个例子虽然不是专门的调度 demo，但已经可以作为调度验证的起点，因为它具备：

- `lmz_init()` 初始化路径
- `pthread_create()` 创建线程
- `pthread_join()` 等待线程结束
- `pthread_mutex_*()` 的基本同步行为

在没有独立调度 demo 代码之前，可以先用它验证：

- 线程能否正常创建与回收
- 多线程输出是否符合基本预期
- 互斥锁路径是否正常工作

## 3. 建议新增的调度 demo

### 3.1 RR 与 FIFO 区分 demo

建议目录：`tests/demo/scheduler/`

建议文件：

- 当前目录：`tests/demo/scheduler/rr_vs_fifo/`
- 当前入口：`tests/demo/scheduler/rr_vs_fifo/main.c`

建议内容：

- 创建两个相同优先级线程
- 一个设置为 `SCHED_FIFO`
- 一个设置为 `SCHED_RR`
- 通过循环打印、计数器或时间戳观察轮转行为

希望验证的问题：

- `SCHED_RR` 是否真的受时间片驱动
- `SCHED_FIFO` 是否不会在同优先级下被误轮转

### 3.2 优先级映射 demo

建议目录：`tests/demo/scheduler/`

建议文件：

- 当前目录：`tests/demo/scheduler/priority_mapping/`
- 当前入口：`tests/demo/scheduler/priority_mapping/main.c`

建议内容：

- 调用 `sched_setparam()` 或 `pthread_setschedparam()` 设置优先级
- 用 `sched_getparam()` 或 `pthread_getschedparam()` 回读
- 再通过线程运行顺序观察设置是否真正生效

### 3.3 RR 时间片导出 demo

建议目录：`tests/demo/scheduler/`

建议文件：

- 当前目录：`tests/demo/scheduler/rr_interval/`
- 当前入口：`tests/demo/scheduler/rr_interval/main.c`

建议内容：

- 调用 `sched_rr_get_interval()`
- 读取当前时间片设置
- 结合线程切换节奏验证返回值是否可信

## 4. 当前可以直接运行的相关测试

### 4.1 线程单元测试

文件：`tests/unit/test_thread.c`

适合验证：

- 线程创建
- 线程退出与回收
- 基本互斥行为

### 4.2 Hello LMZ 样例

目录：`tests/examples/hello_lmz/`

适合验证：

- LMZ 初始化路径
- 多线程运行
- 基本同步原语

## 5. 后续建议

如果下一步要把调度 demo 做成真正可跑的测试，最合适的落点是：

- 接口级检查已经落到 `tests/unit/test_sched.c`
- 行为可视化 demo 放到 `tests/demo/scheduler/`

这样既方便自动化回归，也方便人工观察调度现象。

## 6. 已落地的首批 demo

当前仓库中已经加入第一批可编译的调度 demo：

- `tests/demo/scheduler/rr_vs_fifo/`
- `tests/demo/scheduler/priority_mapping/`
- `tests/demo/scheduler/rr_interval/`

它们的定位是：

- 先打通构建路径
- 先确认 LMZ 调度接口在当前代码状态下是否可调用
- 在 `CONFIG_SCHED_LINUX` 打开后，作为进一步行为验证的起点

### 构建示例

```bash
cd tests/demo/scheduler/rr_vs_fifo
west build -b qemu_cortex_m3 .
west build -t run
```

其他两个 demo 的构建方式相同，只需要把目录切换到对应 demo 根目录。
