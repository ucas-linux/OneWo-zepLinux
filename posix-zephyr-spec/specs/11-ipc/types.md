# 数据类型 - IPC

```c
typedef void *mqd_t;

struct mq_attr {
    long mq_flags;
    long mq_maxmsg;
    long mq_msgsize;
    long mq_curmsgs;
};

// 管道使用k_pipe
// 消息队列使用k_msgq
```
