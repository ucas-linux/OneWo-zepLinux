# pipe/mqueue - 进程间通信

## POSIX标准定义

```c
#include <unistd.h>
#include <mqueue.h>

int pipe(int pipefd[2]);
mqd_t mq_open(const char *name, int oflag, ...);
int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio);
ssize_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned *msg_prio);
```

## Zephyr适配

```c
int pipe(int pipefd[2])
{
    struct k_pipe *kpipe = k_malloc(sizeof(*kpipe));
    if (!kpipe) {
        errno = ENOMEM;
        return -1;
    }

    k_pipe_init(kpipe, k_malloc(4096), 4096);

    // 分配两个fd
    pipefd[0] = alloc_fd();  // 读端
    pipefd[1] = alloc_fd();  // 写端

    g_fd_table[pipefd[0]].type = FD_TYPE_PIPE;
    g_fd_table[pipefd[0]].handle.pipe = kpipe;
    g_fd_table[pipefd[0]].flags = O_RDONLY;

    g_fd_table[pipefd[1]].type = FD_TYPE_PIPE;
    g_fd_table[pipefd[1]].handle.pipe = kpipe;
    g_fd_table[pipefd[1]].flags = O_WRONLY;

    return 0;
}

// 消息队列使用Zephyr的k_msgq
mqd_t mq_open(const char *name, int oflag, ...)
{
    struct k_msgq *msgq = k_malloc(sizeof(*msgq));
    k_msgq_init(msgq, k_malloc(1024), 64, 16);
    return (mqd_t)msgq;
}
```

## 测试

```c
void test_pipe(void)
{
    int pipefd[2];
    pipe(pipefd);

    const char *msg = "Hello";
    write(pipefd[1], msg, strlen(msg));

    char buf[10];
    read(pipefd[0], buf, sizeof(buf));
    assert(strcmp(buf, msg) == 0);

    close(pipefd[0]);
    close(pipefd[1]);
}
```
