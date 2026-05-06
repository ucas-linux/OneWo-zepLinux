# 线程管理接口规范

## 概述

本模块定义7个POSIX线程管理接口在Zephyr RTOS上的实现规范。

## 接口列表

1. `pthread_create` - 创建线程
2. `pthread_join` - 等待线程结束
3. `pthread_cancel` - 取消线程
4. `pthread_mutex_lock` - 互斥锁加锁
5. `pthread_mutex_unlock` - 互斥锁解锁
6. `pthread_cond_wait` - 条件变量等待
7. `pthread_cond_signal` - 条件变量信号

## 核心挑战

### 1. 动态内存分配
**问题**：Zephyr传统使用`K_THREAD_STACK_DEFINE`静态分配，Linux应用依赖动态创建。

**解决方案**：
- 使用`k_malloc`动态分配线程栈
- 在线程退出时自动释放栈内存
- 提供编译选项在静态/动态间切换

### 2. 调度策略映射
**问题**：Linux SCHED_FIFO/RR (优先级1-99) vs Zephyr优先级模型。

**解决方案**：
- 建立优先级映射引擎
- SCHED_FIFO → Zephyr协作式（负优先级）
- SCHED_RR → Zephyr抢占式（正优先级）
- 线性映射Linux优先级到Zephyr范围

### 3. 线程退出状态保持
**问题**：Zephyr线程退出即销毁，pthread_join需要获取返回值。

**解决方案**：
- 为每个线程附加同步信号量和状态缓冲区
- 线程退出时保存返回值，触发信号量
- pthread_join等待信号量，读取返回值后释放资源

### 4. 异步取消安全
**问题**：pthread_cancel强制终止可能导致死锁。

**解决方案**：
- 引入"取消点"机制
- 在阻塞调用中检测取消标志
- 线程在安全点释放资源后自毁

### 5. 互斥锁属性
**问题**：Zephyr k_mutex不支持递归锁和错误检查。

**解决方案**：
- 封装k_mutex，添加属性检测外壳
- 记录锁持有者线程ID
- 模拟递归锁和错误检查行为

### 6. 条件变量原子性
**问题**：pthread_cond_wait需要原子地释放锁并等待。

**解决方案**：
- 使用Zephyr k_condvar
- 确保与封装后的pthread_mutex协同
- 消除竞争条件

## 文件组织

- `pthread_create.md` - 线程创建详细规范
- `pthread_join.md` - 线程等待详细规范
- `pthread_cancel.md` - 线程取消详细规范
- `pthread_mutex.md` - 互斥锁详细规范
- `pthread_cond.md` - 条件变量详细规范
- `types.md` - 数据类型定义
- `implementation.md` - 实现指导

## 优先级

1. **高优先级**：pthread_create, pthread_mutex_lock/unlock（基础功能）
2. **中优先级**：pthread_join, pthread_cond_wait/signal（同步功能）
3. **低优先级**：pthread_cancel（高级功能）

## 测试要求

- 单线程创建和退出
- 多线程并发
- 互斥锁竞争
- 条件变量生产者-消费者模型
- 线程取消安全性
- 资源泄漏检测
