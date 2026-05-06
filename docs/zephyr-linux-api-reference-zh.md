# zepLinux 接口定义文档（正式版）


- 接口功能描述
- 输入参数定义
- 输出参数定义与返回值语义



## 1. 文档目的与适用范围

本项目并不是将 Linux 内核整体移植到 MCU 上，而是在 Zephyr RTOS 上实现一层 Linux/POSIX 风格兼容接口层，使得部分 Linux 应用代码可以以较小改动迁移到 ARM Cortex-M3 等资源受限平台运行。

本文档中的“接口定义”具有如下特点：

1. **接口形式尽量兼容 POSIX/Linux**：函数名、参数形式、返回值风格尽可能保持一致。
2. **接口语义尽量贴近 Linux**：对于线程、调度、设备、信号等常见能力，尽量保留应用开发者熟悉的行为模型。
3. **实现范围结合 MCU 场景裁剪**：对于需要 MMU、完整用户态隔离、复杂 VFS 或完整进程模型的语义，只保留项目验证所需的可落地子集。
4. **底层承载基于 Zephyr 原生机制**：线程基于 `k_thread`，同步基于 Zephyr 内核对象，设备访问基于 Zephyr 驱动模型与文件描述符兼容层。

## 2. 接口分类总览

本文档将接口划分为以下十二类：

1. 线程管理接口
2. 进程管理接口
3. 调度与优先级接口
4. 信号与定时器接口
5. 内存管理接口
6. 设备管理接口
7. 文件系统与目录接口
8. 文件描述符操作接口
9. 网络套接字接口
10. IPC 与环境接口
11. 时间与进程控制接口
12. I/O 多路复用接口

---

## 3. 线程管理接口

### 3.1 `pthread_create`

**函数原型**
```c
#include <pthread.h>

int pthread_create(pthread_t *thread,
                   const pthread_attr_t *attr,
                   void *(*start_routine)(void *),
                   void *arg);
```

**功能描述**
创建一个新的线程，并使其从 `start_routine` 指定的入口函数开始执行。该接口是应用创建并发执行实体的主要入口，对上呈现 POSIX 线程语义，对下映射到 Zephyr 的线程对象与调度实体。

**输入参数定义**
- `thread`：调用者提供的线程句柄输出位置，必须指向一段可写内存。
- `attr`：线程属性对象指针；若为 `NULL`，表示采用默认线程属性。
- `start_routine`：新线程入口函数指针，函数签名为 `void *(*)(void *)`。
- `arg`：传递给线程入口函数的单个参数，类型为无类型指针。

**输出参数定义**
- `thread`：创建成功后写入新线程的线程标识符。

**返回值定义**
- `0`：线程创建成功。
- `EAGAIN`：系统资源不足，无法创建新线程。
- `EINVAL`：线程属性非法或不受支持。
- `EPERM`：调用者无权设置请求的调度属性。

**实现说明**
- 新线程的栈空间通常由堆动态分配。
- 线程优先级需要从 POSIX 语义空间映射到 Zephyr 内核优先级空间。
- 线程创建后应纳入统一资源跟踪表，便于回收与调试。

---

### 3.2 `pthread_join`

**函数原型**
```c
#include <pthread.h>

int pthread_join(pthread_t thread, void **retval);
```

**功能描述**
等待目标线程结束执行，并在必要时获取其退出返回值。该接口用于线程间生命周期同步，确保一个线程可以有序等待另一个线程完成工作。

**输入参数定义**
- `thread`：需要等待的目标线程 ID。
- `retval`：用于接收目标线程返回值的指针；若不关心返回值，可传入 `NULL`。

**输出参数定义**
- `retval`：当该参数非空且等待成功时，写入目标线程退出时的返回值。

**返回值定义**
- `0`：等待成功，目标线程已退出。
- `ESRCH`：目标线程不存在。
- `EINVAL`：目标线程不可 join，例如已 detached。
- `EDEADLK`：等待关系将导致死锁，例如线程等待自身。

**实现说明**
- 成功 join 后，目标线程相关资源应被释放。
- 目标线程必须处于 joinable 状态。

---

### 3.3 `pthread_cancel`

**函数原型**
```c
#include <pthread.h>

int pthread_cancel(pthread_t thread);
```

**功能描述**
向目标线程发送取消请求。线程是否立即退出，取决于其取消状态与取消点设置。该接口提供的是“协作式终止”，而不是强制摧毁线程。

**输入参数定义**
- `thread`：接收取消请求的目标线程 ID。

**输出参数定义**
- 无。

**返回值定义**
- `0`：取消请求发送成功。
- `ESRCH`：目标线程不存在。

**实现说明**
- 取消请求通常在下一个取消点生效。
- 运行时应保证清理处理逻辑与锁释放规则正确执行。

---

### 3.4 `pthread_mutex_lock`

**函数原型**
```c
#include <pthread.h>

int pthread_mutex_lock(pthread_mutex_t *mutex);
```

**功能描述**
对互斥锁加锁。如果该锁已被其他线程持有，则当前线程阻塞等待。该接口用于保护共享资源，避免竞态访问。

**输入参数定义**
- `mutex`：指向已初始化互斥锁对象的指针。

**输出参数定义**
- 无。

**返回值定义**
- `0`：加锁成功。
- `EINVAL`：互斥锁对象非法、未初始化或已销毁。
- `EDEADLK`：检测到死锁风险。

**实现说明**
- 对递归锁应支持同一线程重复加锁。
- 实时线程场景可结合优先级继承机制。

---

### 3.5 `pthread_mutex_unlock`

**函数原型**
```c
#include <pthread.h>

int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

**功能描述**
释放当前线程已持有的互斥锁，使其他等待线程有机会获得该锁。

**输入参数定义**
- `mutex`：待解锁的互斥锁对象指针。

**输出参数定义**
- 无。

**返回值定义**
- `0`：解锁成功。
- `EINVAL`：互斥锁对象非法。
- `EPERM`：当前线程不是该锁的持有者。

**实现说明**
- 若存在等待线程，应按调度策略唤醒合适的等待者。
- 递归锁需先减少递归计数，再决定是否真正释放。

---

### 3.6 `pthread_cond_wait`

**函数原型**
```c
#include <pthread.h>

int pthread_cond_wait(pthread_cond_t *cond,
                      pthread_mutex_t *mutex);
```

**功能描述**
调用时原子地释放互斥锁，并将当前线程挂到条件变量等待队列中；当被唤醒后，再重新获得互斥锁并返回。该接口用于事件条件同步。

**输入参数定义**
- `cond`：条件变量对象指针。
- `mutex`：与条件变量配合使用的互斥锁，调用时必须已由当前线程持有。

**输出参数定义**
- 无。

**返回值定义**
- `0`：等待成功并重新获得互斥锁。
- `EINVAL`：参数非法或对象状态不正确。
- `EPERM`：调用时未持有 `mutex`。

**实现说明**
- 必须保证“释放锁并进入等待”是原子动作，避免丢失唤醒。

---

### 3.7 `pthread_cond_signal`

**函数原型**
```c
#include <pthread.h>

int pthread_cond_signal(pthread_cond_t *cond);
```

**功能描述**
唤醒至少一个正在等待该条件变量的线程，用于通知共享条件已发生变化。

**输入参数定义**
- `cond`：待发信号的条件变量对象。

**输出参数定义**
- 无。

**返回值定义**
- `0`：操作成功。
- `EINVAL`：条件变量非法。

**实现说明**
- 若没有等待线程，调用成功但不产生可见唤醒效果。

---

## 4. 进程管理接口

### 4.1 `fork`

**函数原型**
```c
#include <unistd.h>

pid_t fork(void);
```

**功能描述**
复制当前进程，创建一个新的子进程。父进程与子进程从同一位置继续执行，但看到不同的返回值。在本项目中，该接口通常是轻量化的“类进程复制语义”。

**输入参数定义**
- 无。

**输出参数定义**
- 无。

**返回值定义**
- 大于 `0`：在父进程中返回子进程 PID。
- `0`：在子进程中返回。
- `-1`：创建失败，并设置 `errno`。

**错误码定义**
- `ENOMEM`：内存不足。
- `EAGAIN`：进程数量或系统资源达到上限。

**实现说明**
- 在无 MMU 场景下，通常采用简化复制模型，而非完整 copy-on-write。

---

### 4.2 `execve`

**函数原型**
```c
#include <unistd.h>

int execve(const char *filename,
           char *const argv[],
           char *const envp[]);
```

**功能描述**
使用指定可执行文件替换当前进程映像。成功后原有程序不再继续执行，而是转到新程序入口开始运行。

**输入参数定义**
- `filename`：目标可执行文件路径。
- `argv`：新程序的参数向量，以 `NULL` 结尾。
- `envp`：新程序的环境变量向量，以 `NULL` 结尾。

**输出参数定义**
- 无。

**返回值定义**
- 成功时不返回。
- `-1`：执行失败，并设置 `errno`。

**错误码定义**
- `ENOENT`：目标文件不存在。
- `EACCES`：无执行权限。
- `ENOMEM`：装载时内存不足。
- `EINVAL`：可执行格式非法。

**实现说明**
- 本项目中可结合 ELF 装载与 Zephyr LLEXT 等机制实现。

---

### 4.3 `exit`

**函数原型**
```c
#include <stdlib.h>

void exit(int status);
```

**功能描述**
终止当前进程，并执行进程级资源清理，将退出状态提供给父进程或等待接口。

**输入参数定义**
- `status`：进程退出码。

**输出参数定义**
- 无。

**返回值定义**
- 该函数不返回。

**实现说明**
- 应关闭全部已打开文件描述符。
- 应释放动态资源，并通知父进程可进行 `wait`/`waitpid` 回收。

---

## 5. 调度与优先级接口

### 5.1 `sched_yield`

**函数原型**
```c
#include <sched.h>

int sched_yield(void);
```

**功能描述**
当前线程主动让出处理器，使其他就绪线程有机会运行。该接口体现的是“自愿放弃本次 CPU 使用权”的语义。

**输入参数定义**
- 无。

**输出参数定义**
- 无。

**返回值定义**
- `0`：让出 CPU 成功。
- `-1`：失败，并设置 `errno`。

**实现说明**
- 应真正影响调度队列，而不是仅返回成功。

---

### 5.2 `sched_setparam`

**函数原型**
```c
#include <sched.h>

int sched_setparam(pid_t pid, const struct sched_param *param);
```

**功能描述**
设置指定进程或线程上下文的调度参数，主要是调度优先级。

**输入参数定义**
- `pid`：目标进程或线程标识，`0` 表示当前执行上下文。
- `param`：调度参数结构体指针，通常包含 `sched_priority`。

**输出参数定义**
- 无。

**返回值定义**
- `0`：设置成功。
- `-1`：设置失败，并设置 `errno`。

**错误码定义**
- `ESRCH`：目标不存在。
- `EINVAL`：参数非法。
- `EPERM`：无权限修改。

**实现说明**
- 需要将 Linux/POSIX 优先级映射到 Zephyr 优先级空间。

---

### 5.3 `sched_getparam`

**函数原型**
```c
#include <sched.h>

int sched_getparam(pid_t pid, struct sched_param *param);
```

**功能描述**
获取指定进程或线程的当前调度参数。

**输入参数定义**
- `pid`：目标进程或线程标识，`0` 表示当前执行上下文。
- `param`：调用者提供的输出结构体指针。

**输出参数定义**
- `param`：成功时写入当前有效的调度参数。

**返回值定义**
- `0`：获取成功。
- `-1`：失败，并设置 `errno`。

**错误码定义**
- `ESRCH`：目标不存在。
- `EINVAL`：参数非法。

---

## 6. 信号与定时器接口

### 6.1 `signal`

**函数原型**
```c
#include <signal.h>

void (*signal(int sig, void (*func)(int)))(int);
```

**功能描述**
为指定信号安装新的处理函数，或设置为默认处理、忽略处理。

**输入参数定义**
- `sig`：信号编号。
- `func`：信号处理函数，或 `SIG_DFL`、`SIG_IGN`。

**输出参数定义**
- 无。

**返回值定义**
- 成功时返回旧的信号处理方式。
- 失败时返回 `SIG_ERR`。

---

### 6.2 `kill`

**函数原型**
```c
#include <signal.h>

int kill(pid_t pid, int sig);
```

**功能描述**
向指定进程发送一个信号。

**输入参数定义**
- `pid`：目标进程 ID。
- `sig`：要发送的信号编号。

**输出参数定义**
- 无。

**返回值定义**
- `0`：发送成功。
- `-1`：发送失败。

**错误码定义**
- `ESRCH`：目标进程不存在。
- `EINVAL`：信号编号非法。
- `EPERM`：权限不足。

---

### 6.3 `alarm`

**函数原型**
```c
#include <unistd.h>

unsigned int alarm(unsigned int seconds);
```

**功能描述**
设置一个一次性定时器，在指定秒数后向当前进程发送 `SIGALRM`。

**输入参数定义**
- `seconds`：距离触发的秒数；若为 `0`，表示取消已有 alarm。

**输出参数定义**
- 无。

**返回值定义**
- 返回上一个 alarm 剩余的秒数；若原先没有 alarm，则返回 `0`。

---

### 6.4 `timer_create`

**函数原型**
```c
#include <time.h>

int timer_create(clockid_t clockid,
                 struct sigevent *sevp,
                 timer_t *timerid);
```

**功能描述**
创建一个 POSIX 定时器对象，供后续 `timer_settime` 配置。

**输入参数定义**
- `clockid`：定时器所依赖的时钟源。
- `sevp`：超时通知配置；可为 `NULL`。
- `timerid`：调用者提供的定时器 ID 输出位置。

**输出参数定义**
- `timerid`：成功时写入新创建的定时器标识。

**返回值定义**
- `0`：创建成功。
- `-1`：创建失败。

**错误码定义**
- `EAGAIN`：资源已满。
- `EINVAL`：时钟源非法。
- `ENOMEM`：内存不足。

---

### 6.5 `sleep`

**函数原型**
```c
#include <unistd.h>

unsigned int sleep(unsigned int seconds);
```

**功能描述**
让当前线程休眠指定秒数，或在被信号打断时提前返回。

**输入参数定义**
- `seconds`：请求休眠的秒数。

**输出参数定义**
- 无。

**返回值定义**
- `0`：睡眠完整结束。
- 大于 `0`：被中断后剩余未睡眠的秒数。

---

### 6.6 `usleep`

**函数原型**
```c
#include <unistd.h>

int usleep(useconds_t usec);
```

**功能描述**
使当前线程休眠指定微秒数。

**输入参数定义**
- `usec`：休眠时长，单位为微秒。

**输出参数定义**
- 无。

**返回值定义**
- `0`：成功。
- `-1`：失败。

---

### 6.7 `nanosleep`

**函数原型**
```c
#include <time.h>

int nanosleep(const struct timespec *req,
              struct timespec *rem);
```

**功能描述**
按纳秒精度请求线程休眠，是高精度相对定时睡眠接口。

**输入参数定义**
- `req`：请求睡眠时长。
- `rem`：若被中断，用于返回剩余睡眠时长；可为 `NULL`。

**输出参数定义**
- `rem`：被中断时写入剩余时间。

**返回值定义**
- `0`：睡眠完成。
- `-1`：失败或被打断。

**错误码定义**
- `EINVAL`：时间参数非法。
- `EINTR`：睡眠被信号中断。

---

### 6.8 `timer_settime`

**函数原型**
```c
#include <time.h>

int timer_settime(timer_t timerid,
                  int flags,
                  const struct itimerspec *new_value,
                  struct itimerspec *old_value);
```

**功能描述**
配置、启动、修改或停止指定的 POSIX 定时器。

**输入参数定义**
- `timerid`：待配置的定时器 ID。
- `flags`：配置标志，例如 `TIMER_ABSTIME`。
- `new_value`：新的定时器参数。
- `old_value`：用于保存旧参数；可为 `NULL`。

**输出参数定义**
- `old_value`：若非空，则返回定时器旧配置。

**返回值定义**
- `0`：设置成功。
- `-1`：设置失败。

---

## 7. 内存管理接口

### 7.1 `malloc`

**函数原型**
```c
#include <stdlib.h>

void *malloc(size_t size);
```

**功能描述**
从堆中申请指定大小的未初始化内存块。

**输入参数定义**
- `size`：请求的字节数。

**输出参数定义**
- 无。

**返回值定义**
- 非 `NULL`：申请成功，返回内存首地址。
- `NULL`：申请失败。

---

### 7.2 `realloc`

**函数原型**
```c
#include <stdlib.h>

void *realloc(void *ptr, size_t size);
```

**功能描述**
调整一块已分配内存的大小，并尽量保留原有内容。

**输入参数定义**
- `ptr`：原始内存块指针；若为 `NULL`，等价于 `malloc(size)`。
- `size`：新的目标大小。

**输出参数定义**
- 无。

**返回值定义**
- 非 `NULL`：调整成功，返回新地址或原地址。
- `NULL`：调整失败，原内存块一般保持有效。

---

### 7.3 `free`

**函数原型**
```c
#include <stdlib.h>

void free(void *ptr);
```

**功能描述**
释放先前动态申请的内存块。

**输入参数定义**
- `ptr`：待释放内存块指针；允许为 `NULL`。

**输出参数定义**
- 无。

**返回值定义**
- 无返回值。

---

## 8. 设备管理接口

### 8.1 `open`

**函数原型**
```c
#include <fcntl.h>

int open(const char *pathname, int flags, mode_t mode);
```

**功能描述**
打开设备或文件风格对象，并返回一个文件描述符，供后续 `read`、`write`、`ioctl` 等接口使用。

**输入参数定义**
- `pathname`：目标路径，如 `/dev/uart0`。
- `flags`：打开方式和行为标志。
- `mode`：创建类场景下的权限位；对于纯设备路径通常可忽略。

**输出参数定义**
- 无。

**返回值定义**
- 非负整数：打开成功，返回文件描述符。
- `-1`：打开失败。

**错误码定义**
- `ENOENT`：路径不存在。
- `EACCES`：权限不足。
- `EMFILE`：文件描述符不足。

---

### 8.2 `close`

**函数原型**
```c
#include <unistd.h>

int close(int fd);
```

**功能描述**
关闭一个文件描述符，释放相关资源。

**输入参数定义**
- `fd`：待关闭文件描述符。

**输出参数定义**
- 无。

**返回值定义**
- `0`：关闭成功。
- `-1`：关闭失败。

**错误码定义**
- `EBADF`：文件描述符非法。
- `EIO`：设备关闭过程中发生 I/O 错误。

---

### 8.3 `read`

**函数原型**
```c
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t count);
```

**功能描述**
从文件描述符对应对象中读取数据到用户提供的缓冲区。

**输入参数定义**
- `fd`：源文件描述符。
- `buf`：目标缓冲区。
- `count`：最多读取的字节数。

**输出参数定义**
- `buf`：成功时写入读取到的数据。

**返回值定义**
- 正数：实际读取的字节数。
- `0`：到达 EOF。
- `-1`：读取失败。

**错误码定义**
- `EBADF`：描述符非法或不可读。
- `EAGAIN`：非阻塞模式下当前无数据。
- `EIO`：I/O 错误。

---

### 8.4 `write`

**函数原型**
```c
#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t count);
```

**功能描述**
向文件描述符对应对象写入数据。

**输入参数定义**
- `fd`：目标文件描述符。
- `buf`：源数据缓冲区。
- `count`：最多写入的字节数。

**输出参数定义**
- 无。

**返回值定义**
- 非负数：实际写入字节数。
- `-1`：写入失败。

**错误码定义**
- `EBADF`：描述符非法或不可写。
- `EAGAIN`：非阻塞模式下当前不可写。
- `EIO`：I/O 错误。

---

### 8.5 `ioctl`

**函数原型**
```c
#include <sys/ioctl.h>

int ioctl(int fd, unsigned long request, ...);
```

**功能描述**
执行设备特定的控制操作，用于完成简单字节流读写无法表达的设备配置或状态查询。

**输入参数定义**
- `fd`：目标文件描述符。
- `request`：控制命令编码。
- `...`：随命令变化的参数，一般为结构体指针或标量值。

**输出参数定义**
- 可变参数在部分命令下兼作输出缓冲区。

**返回值定义**
- `0`：操作成功。
- `-1`：操作失败。

**错误码定义**
- `EBADF`：描述符非法。
- `ENOTTY`：设备不支持该控制命令。
- `EINVAL`：命令参数非法。

---

## 9. 文件系统与目录接口

### 9.1 `stat`

**函数原型**
```c
#include <sys/stat.h>

int stat(const char *pathname, struct stat *statbuf);
```

**功能描述**
获取指定路径对象的状态信息，例如文件大小、类型等。

**输入参数定义**
- `pathname`：目标文件或目录路径。
- `statbuf`：状态信息输出缓冲区。

**输出参数定义**
- `statbuf`：成功时写入目标对象状态信息。

**返回值定义**
- `0`：成功。
- `-1`：失败。

---

### 9.2 `fstat`

**函数原型**
```c
#include <sys/stat.h>

int fstat(int fd, struct stat *statbuf);
```

**功能描述**
获取指定文件描述符关联对象的状态信息。

**输入参数定义**
- `fd`：目标文件描述符。
- `statbuf`：状态信息输出缓冲区。

**输出参数定义**
- `statbuf`：成功时写入状态信息。

**返回值定义**
- `0`：成功。
- `-1`：失败。

---

### 9.3 `opendir`

**函数原型**
```c
#include <dirent.h>

DIR *opendir(const char *name);
```

**功能描述**
打开目录流，以便后续遍历目录项。

**输入参数定义**
- `name`：目录路径。

**输出参数定义**
- 无。

**返回值定义**
- 非 `NULL`：成功，返回目录流对象。
- `NULL`：失败。

---

### 9.4 `readdir`

**函数原型**
```c
#include <dirent.h>

struct dirent *readdir(DIR *dirp);
```

**功能描述**
读取目录流中的下一个目录项。

**输入参数定义**
- `dirp`：目录流对象。

**输出参数定义**
- 返回的 `struct dirent *`：指向当前目录项信息。

**返回值定义**
- 非 `NULL`：成功读取到目录项。
- `NULL`：到达目录末尾或读取失败。

---

### 9.5 `closedir`

**函数原型**
```c
#include <dirent.h>

int closedir(DIR *dirp);
```

**功能描述**
关闭目录流并释放相关资源。

**输入参数定义**
- `dirp`：待关闭目录流。

**输出参数定义**
- 无。

**返回值定义**
- `0`：成功。
- `-1`：失败。

---

### 9.6 `mkdir`

**函数原型**
```c
#include <sys/stat.h>

int mkdir(const char *pathname, mode_t mode);
```

**功能描述**
创建目录。

**输入参数定义**
- `pathname`：新目录路径。
- `mode`：目录权限位。

**输出参数定义**
- 无。

**返回值定义**
- `0`：成功。
- `-1`：失败。

---

## 10. 文件描述符操作接口

### 10.1 `lseek`

**函数原型**
```c
#include <unistd.h>

off_t lseek(int fd, off_t offset, int whence);
```

**功能描述**
修改文件描述符的当前偏移位置，支持随机访问。

**输入参数定义**
- `fd`：目标文件描述符。
- `offset`：偏移量。
- `whence`：基准位置，如 `SEEK_SET`、`SEEK_CUR`、`SEEK_END`。

**输出参数定义**
- 无。

**返回值定义**
- 非负数：新的偏移位置。
- `-1`：失败。

---

### 10.2 `dup`

**函数原型**
```c
#include <unistd.h>

int dup(int oldfd);
```

**功能描述**
复制一个已有文件描述符，返回新的描述符。

**输入参数定义**
- `oldfd`：原文件描述符。

**输出参数定义**
- 无。

**返回值定义**
- 非负数：新文件描述符。
- `-1`：失败。

---

### 10.3 `dup2`

**函数原型**
```c
#include <unistd.h>

int dup2(int oldfd, int newfd);
```

**功能描述**
将已有文件描述符复制到指定编号的新描述符上。

**输入参数定义**
- `oldfd`：原文件描述符。
- `newfd`：目标文件描述符编号。

**输出参数定义**
- 无。

**返回值定义**
- 成功时返回 `newfd`。
- `-1`：失败。

---

### 10.4 `fcntl`

**函数原型**
```c
#include <fcntl.h>

int fcntl(int fd, int cmd, ...);
```

**功能描述**
执行文件描述符控制操作，例如读写标志设置、描述符复制等。

**输入参数定义**
- `fd`：目标文件描述符。
- `cmd`：控制命令。
- `...`：命令相关参数。

**输出参数定义**
- 命令相关。

**返回值定义**
- 成功时返回与命令相关的非错误值。
- `-1`：失败。

---

## 11. 网络套接字接口

### 11.1 `socket`

**函数原型**
```c
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```

**功能描述**
创建一个套接字通信端点。

**输入参数定义**
- `domain`：协议族。
- `type`：套接字类型。
- `protocol`：协议号。

**输出参数定义**
- 无。

**返回值定义**
- 非负数：套接字描述符。
- `-1`：失败。

---

### 11.2 `bind`

**函数原型**
```c
#include <sys/socket.h>

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

**功能描述**
为套接字绑定本地地址。

**输入参数定义**
- `sockfd`：套接字描述符。
- `addr`：本地地址结构体指针。
- `addrlen`：地址结构体长度。

**输出参数定义**
- 无。

**返回值定义**
- `0`：成功。
- `-1`：失败。

---

### 11.3 `listen`

**函数原型**
```c
#include <sys/socket.h>

int listen(int sockfd, int backlog);
```

**功能描述**
将流式套接字设置为监听状态。

**输入参数定义**
- `sockfd`：套接字描述符。
- `backlog`：等待连接队列长度。

**输出参数定义**
- 无。

**返回值定义**
- `0`：成功。
- `-1`：失败。

---

### 11.4 `accept`

**函数原型**
```c
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

**功能描述**
接受一个已到达的连接请求，并返回一个新的已连接套接字。

**输入参数定义**
- `sockfd`：监听套接字。
- `addr`：用于接收对端地址的缓冲区；可为 `NULL`。
- `addrlen`：输入时表示缓冲区大小，输出时表示实际地址长度。

**输出参数定义**
- `addr`：成功时写入对端地址。
- `addrlen`：成功时写入地址实际长度。

**返回值定义**
- 非负数：新的已连接套接字描述符。
- `-1`：失败。

---

### 11.5 `connect`

**函数原型**
```c
#include <sys/socket.h>

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

**功能描述**
发起到远端地址的连接。

**输入参数定义**
- `sockfd`：套接字描述符。
- `addr`：远端地址。
- `addrlen`：地址长度。

**输出参数定义**
- 无。

**返回值定义**
- `0`：连接成功或请求发起成功。
- `-1`：失败。

---

### 11.6 `send`

**函数原型**
```c
#include <sys/socket.h>

ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```

**功能描述**
通过套接字发送数据。

**输入参数定义**
- `sockfd`：套接字描述符。
- `buf`：待发送数据缓冲区。
- `len`：待发送字节数。
- `flags`：发送行为标志。

**输出参数定义**
- 无。

**返回值定义**
- 非负数：实际发送字节数。
- `-1`：失败。

---

### 11.7 `recv`

**函数原型**
```c
#include <sys/socket.h>

ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

**功能描述**
从套接字接收数据。

**输入参数定义**
- `sockfd`：套接字描述符。
- `buf`：接收缓冲区。
- `len`：最多接收字节数。
- `flags`：接收行为标志。

**输出参数定义**
- `buf`：成功时写入接收到的数据。

**返回值定义**
- 正数：实际接收字节数。
- `0`：对端正常关闭连接。
- `-1`：失败。

---

## 12. IPC 与环境接口

### 12.1 `pipe`

**函数原型**
```c
#include <unistd.h>

int pipe(int pipefd[2]);
```

**功能描述**
创建一个单向管道，用于两个执行上下文之间的数据传输。

**输入参数定义**
- `pipefd`：长度为 2 的整型数组。

**输出参数定义**
- `pipefd[0]`：读端文件描述符。
- `pipefd[1]`：写端文件描述符。

**返回值定义**
- `0`：成功。
- `-1`：失败。

---

### 12.2 `getenv`

**函数原型**
```c
#include <stdlib.h>

char *getenv(const char *name);
```

**功能描述**
按名称获取环境变量的值。

**输入参数定义**
- `name`：环境变量名。

**输出参数定义**
- 无。

**返回值定义**
- 非 `NULL`：返回环境变量值字符串。
- `NULL`：变量不存在。

---

### 12.3 `getuid`

**函数原型**
```c
#include <unistd.h>

uid_t getuid(void);
```

**功能描述**
获取当前进程的真实用户 ID。在 MCU 场景下通常为兼容占位接口。

**输入参数定义**
- 无。

**输出参数定义**
- 无。

**返回值定义**
- 返回当前用户 ID。

---

## 13. 时间与进程控制接口

### 13.1 `clock_gettime`

**函数原型**
```c
#include <time.h>

int clock_gettime(clockid_t clockid, struct timespec *tp);
```

**功能描述**
读取指定时钟源的当前时间值。

**输入参数定义**
- `clockid`：时钟源标识。
- `tp`：时间输出结构体。

**输出参数定义**
- `tp`：成功时写入当前时间。

**返回值定义**
- `0`：成功。
- `-1`：失败。

---

### 13.2 `wait`

**函数原型**
```c
#include <sys/wait.h>

pid_t wait(int *status);
```

**功能描述**
等待任意一个子进程进入可回收状态，通常是退出状态。

**输入参数定义**
- `status`：用于接收子进程退出状态；可为 `NULL`。

**输出参数定义**
- `status`：若非空，则写入子进程状态编码。

**返回值定义**
- 正数：被回收子进程的 PID。
- `-1`：失败。

---

### 13.3 `waitpid`

**函数原型**
```c
#include <sys/wait.h>

pid_t waitpid(pid_t pid, int *status, int options);
```

**功能描述**
等待指定子进程或一组子进程状态变化，是更通用的进程等待接口。

**输入参数定义**
- `pid`：等待目标选择规则。
- `status`：状态输出缓冲区。
- `options`：等待选项，例如 `WNOHANG`。

**输出参数定义**
- `status`：成功回收时写入子进程状态。

**返回值定义**
- 正数：目标子进程 PID。
- `0`：非阻塞模式下暂无可回收子进程。
- `-1`：失败。

---

### 13.4 `posix_spawn`

**函数原型**
```c
#include <spawn.h>

int posix_spawn(pid_t *pid, const char *path,
                const posix_spawn_file_actions_t *file_actions,
                const posix_spawnattr_t *attrp,
                char *const argv[], char *const envp[]);
```

**功能描述**
在单次操作中创建新进程并装载目标程序，适合无 MMU 系统替代 `fork + exec` 的组合方式。

**输入参数定义**
- `pid`：新进程 PID 输出位置。
- `path`：待执行程序路径。
- `file_actions`：子进程启动前执行的文件动作列表；可为 `NULL`。
- `attrp`：spawn 属性；可为 `NULL`。
- `argv`：参数向量。
- `envp`：环境变量向量。

**输出参数定义**
- `pid`：成功时写入子进程 PID。

**返回值定义**
- `0`：成功。
- 非零错误码：失败。

---

## 14. I/O 多路复用接口

### 14.1 `select`

**函数原型**
```c
#include <sys/select.h>

int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout);
```

**功能描述**
同时监视多个文件描述符的读、写与异常状态变化。

**输入参数定义**
- `nfds`：需要检查的最大文件描述符值加一。
- `readfds`：待监视可读状态的集合；可为 `NULL`。
- `writefds`：待监视可写状态的集合；可为 `NULL`。
- `exceptfds`：待监视异常状态的集合；可为 `NULL`。
- `timeout`：等待超时时间；可为 `NULL`。

**输出参数定义**
- `readfds`：返回时仅保留可读描述符。
- `writefds`：返回时仅保留可写描述符。
- `exceptfds`：返回时仅保留异常描述符。

**返回值定义**
- 正数：就绪描述符数量。
- `0`：超时。
- `-1`：失败。

---

### 14.2 `poll`

**函数原型**
```c
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

**功能描述**
监视多个文件描述符的事件状态，相比 `select` 更适合稀疏描述符集。

**输入参数定义**
- `fds`：待监视描述符数组。
- `nfds`：数组项数量。
- `timeout`：超时时间，单位毫秒。

**输出参数定义**
- `fds[i].revents`：返回时写入实际发生事件。

**返回值定义**
- 正数：发生事件的描述符数量。
- `0`：超时。
- `-1`：失败。

---

### 14.3 `epoll`

**函数原型**
```c
#include <sys/epoll.h>

int epoll_create1(int flags);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event *events,
               int maxevents, int timeout);
```

**功能描述**
提供事件驱动型 I/O 多路复用机制，将“监视对象注册”与“等待事件发生”分离，适合频繁重复监听一组动态文件描述符。

**输入参数定义**
- `flags`：`epoll_create1` 创建标志。
- `epfd`：epoll 实例描述符。
- `op`：控制操作类型，例如添加、修改、删除。
- `fd`：被管理的目标描述符。
- `event`：事件配置结构体。
- `events`：事件结果输出数组。
- `maxevents`：输出数组容量。
- `timeout`：等待超时时间。

**输出参数定义**
- `events`：`epoll_wait` 返回时写入就绪事件列表。

**返回值定义**
- `epoll_create1`：成功返回 epoll 描述符，失败返回 `-1`。
- `epoll_ctl`：成功返回 `0`，失败返回 `-1`。
- `epoll_wait`：返回就绪事件数，超时返回 `0`，失败返回 `-1`。

---

## 15. 常见错误码说明

| 错误码 | 含义 |
|--------|------|
| `EAGAIN` | 资源暂不可用 |
| `EBADF` | 非法文件描述符 |
| `EDEADLK` | 检测到死锁 |
| `EINTR` | 系统调用被中断 |
| `EINVAL` | 参数非法 |
| `EIO` | I/O 错误 |
| `ENOMEM` | 内存不足 |
| `ENOENT` | 对象不存在 |
| `EPERM` | 权限不足 |
| `ESRCH` | 目标进程或线程不存在 |

## 16. 文档说明

1. 本文档重点强调接口层定义，因此对每个接口优先说明“怎么调用、传什么、返回什么”。
2. 对于更形式化的前置条件、后置条件、不变量与并发约束，可进一步参考：`ai/interface-specifications.md`。
3. 对于底层实现映射与调度器设计细节，可进一步参考：`docs/zephyr-linux-interface-definition.md`。
4. 对于测试与验证资料，可进一步参考：`docs/zepLinux-interface-and-validation.md` 与 `tests/demo/`。

