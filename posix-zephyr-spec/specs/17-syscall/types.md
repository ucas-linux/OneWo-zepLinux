# 系统调用类型定义

## 基本类型

```c
#include <stdint.h>
#include <stdbool.h>

// 系统调用号类型
typedef uint32_t syscall_nr_t;

// 系统调用处理函数类型
typedef int32_t (*syscall_handler_t)(uint32_t *args);
```

## 系统调用表条目

```c
typedef struct {
    syscall_nr_t nr;            // 系统调用号
    syscall_handler_t handler;  // 处理函数
    const char *name;           // 名称（调试用）
    uint8_t num_args;           // 参数数量
    uint8_t flags;              // 标志
} syscall_entry_t;

// 标志位
#define SYSCALL_FLAG_NONE       0x00
#define SYSCALL_FLAG_RESTARTABLE 0x01  // 可重启（被信号中断后）
#define SYSCALL_FLAG_FAST       0x02  // 快速路径
#define SYSCALL_FLAG_DEPRECATED 0x04  // 已废弃
```

## 上下文结构

```c
// 完整寄存器上下文
typedef struct {
    // 手动保存的寄存器
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;   // 系统调用号
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    // 异常自动保存的寄存器
    uint32_t r0;   // 参数1/返回值
    uint32_t r1;   // 参数2
    uint32_t r2;   // 参数3
    uint32_t r3;   // 参数4
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t xpsr;
} syscall_context_t;

// 简化的异常栈帧
typedef struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t xpsr;
} exception_frame_t;
```

## 系统调用统计

```c
#ifdef CONFIG_SYSCALL_STATS

typedef struct {
    uint64_t count;         // 调用次数
    uint64_t total_time_us; // 总耗时（微秒）
    uint64_t max_time_us;   // 最大耗时
    uint64_t min_time_us;   // 最小耗时
    uint64_t errors;        // 错误次数
} syscall_stats_t;

#endif
```

## 系统调用跟踪

```c
#ifdef CONFIG_SYSCALL_TRACE

typedef struct {
    uint32_t nr;            // 系统调用号
    uint32_t args[7];       // 参数
    int32_t result;         // 返回值
    uint64_t timestamp_us;  // 时间戳
    pid_t pid;              // 进程ID
    pid_t tid;              // 线程ID
} syscall_trace_entry_t;

#define SYSCALL_TRACE_BUFFER_SIZE 256

typedef struct {
    syscall_trace_entry_t entries[SYSCALL_TRACE_BUFFER_SIZE];
    uint32_t head;
    uint32_t tail;
    struct k_mutex lock;
} syscall_trace_buffer_t;

#endif
```

## Linux系统调用号定义

```c
// 进程管理
#define __NR_exit           1
#define __NR_fork           2
#define __NR_read           3
#define __NR_write          4
#define __NR_open           5
#define __NR_close          6
#define __NR_waitpid        7
#define __NR_creat          8
#define __NR_link           9
#define __NR_unlink         10
#define __NR_execve         11
#define __NR_chdir          12
#define __NR_time           13
#define __NR_mknod          14
#define __NR_chmod          15
#define __NR_lchown         16
#define __NR_lseek          19
#define __NR_getpid         20
#define __NR_mount          21
#define __NR_umount         22
#define __NR_setuid         23
#define __NR_getuid         24
#define __NR_stime          25
#define __NR_ptrace         26
#define __NR_alarm          27
#define __NR_pause          29
#define __NR_utime          30
#define __NR_access         33
#define __NR_nice           34
#define __NR_sync           36
#define __NR_kill           37
#define __NR_rename         38
#define __NR_mkdir          39
#define __NR_rmdir          40
#define __NR_dup            41
#define __NR_pipe           42
#define __NR_times          43
#define __NR_brk            45
#define __NR_setgid         46
#define __NR_getgid         47
#define __NR_signal         48
#define __NR_geteuid        49
#define __NR_getegid        50
#define __NR_acct           51
#define __NR_umount2        52
#define __NR_ioctl          54
#define __NR_fcntl          55
#define __NR_setpgid        57
#define __NR_umask          60
#define __NR_chroot         61
#define __NR_ustat          62
#define __NR_dup2           63
#define __NR_getppid        64
#define __NR_getpgrp        65
#define __NR_setsid         66
#define __NR_sigaction      67
#define __NR_setreuid       70
#define __NR_setregid       71
#define __NR_sigsuspend     72
#define __NR_sigpending     73
#define __NR_sethostname    74
#define __NR_setrlimit      75
#define __NR_getrlimit      76
#define __NR_getrusage      77
#define __NR_gettimeofday   78
#define __NR_settimeofday   79
#define __NR_getgroups      80
#define __NR_setgroups      81
#define __NR_symlink        83
#define __NR_readlink       85
#define __NR_uselib         86
#define __NR_swapon         87
#define __NR_reboot         88
#define __NR_readdir        89
#define __NR_mmap           90
#define __NR_munmap         91
#define __NR_truncate       92
#define __NR_ftruncate      93
#define __NR_fchmod         94
#define __NR_fchown         95
#define __NR_getpriority    96
#define __NR_setpriority    97
#define __NR_statfs         99
#define __NR_fstatfs        100
#define __NR_socketcall     102
#define __NR_syslog         103
#define __NR_setitimer      104
#define __NR_getitimer      105
#define __NR_stat           106
#define __NR_lstat          107
#define __NR_fstat          108
#define __NR_vhangup        111
#define __NR_wait4          114
#define __NR_swapoff        115
#define __NR_sysinfo        116
#define __NR_ipc            117
#define __NR_fsync          118
#define __NR_sigreturn      119
#define __NR_clone          120
#define __NR_setdomainname  121
#define __NR_uname          122
#define __NR_adjtimex       124
#define __NR_mprotect       125
#define __NR_sigprocmask    126
#define __NR_init_module    128
#define __NR_delete_module  129
#define __NR_quotactl       131
#define __NR_getpgid        132
#define __NR_fchdir         133
#define __NR_bdflush        134
#define __NR_sysfs          135
#define __NR_personality    136
#define __NR_setfsuid       138
#define __NR_setfsgid       139
#define __NR__llseek        140
#define __NR_getdents       141
#define __NR__newselect     142
#define __NR_flock          143
#define __NR_msync          144
#define __NR_readv          145
#define __NR_writev         146
#define __NR_getsid         147
#define __NR_fdatasync      148
#define __NR__sysctl        149
#define __NR_mlock          150
#define __NR_munlock        151
#define __NR_mlockall       152
#define __NR_munlockall     153
#define __NR_sched_setparam         154
#define __NR_sched_getparam         155
#define __NR_sched_setscheduler     156
#define __NR_sched_getscheduler     157
#define __NR_sched_yield            158
#define __NR_sched_get_priority_max 159
#define __NR_sched_get_priority_min 160
#define __NR_sched_rr_get_interval  161
#define __NR_nanosleep      162
#define __NR_mremap         163
#define __NR_setresuid      164
#define __NR_getresuid      165
#define __NR_poll           168
#define __NR_nfsservctl     169
#define __NR_setresgid      170
#define __NR_getresgid      171
#define __NR_prctl          172
#define __NR_rt_sigreturn   173
#define __NR_rt_sigaction   174
#define __NR_rt_sigprocmask 175
#define __NR_rt_sigpending  176
#define __NR_rt_sigtimedwait 177
#define __NR_rt_sigqueueinfo 178
#define __NR_rt_sigsuspend  179
#define __NR_pread64        180
#define __NR_pwrite64       181
#define __NR_chown          182
#define __NR_getcwd         183
#define __NR_capget         184
#define __NR_capset         185
#define __NR_sigaltstack    186
#define __NR_sendfile       187
#define __NR_vfork          190
#define __NR_ugetrlimit     191
#define __NR_mmap2          192
#define __NR_truncate64     193
#define __NR_ftruncate64    194
#define __NR_stat64         195
#define __NR_lstat64        196
#define __NR_fstat64        197
#define __NR_lchown32       198
#define __NR_getuid32       199
#define __NR_getgid32       200
#define __NR_geteuid32      201
#define __NR_getegid32      202
#define __NR_setreuid32     203
#define __NR_setregid32     204
#define __NR_getgroups32    205
#define __NR_setgroups32    206
#define __NR_fchown32       207
#define __NR_setresuid32    208
#define __NR_getresuid32    209
#define __NR_setresgid32    210
#define __NR_getresgid32    211
#define __NR_chown32        212
#define __NR_setuid32       213
#define __NR_setgid32       214
#define __NR_setfsuid32     215
#define __NR_setfsgid32     216
#define __NR_getdents64     217
#define __NR_pivot_root     218
#define __NR_mincore        219
#define __NR_madvise        220
#define __NR_fcntl64        221
#define __NR_gettid         224
#define __NR_readahead      225
#define __NR_setxattr       226
#define __NR_lsetxattr      227
#define __NR_fsetxattr      228
#define __NR_getxattr       229
#define __NR_lgetxattr      230
#define __NR_fgetxattr      231
#define __NR_listxattr      232
#define __NR_llistxattr     233
#define __NR_flistxattr     234
#define __NR_removexattr    235
#define __NR_lremovexattr   236
#define __NR_fremovexattr   237
#define __NR_tkill          238
#define __NR_sendfile64     239
#define __NR_futex          240
#define __NR_sched_setaffinity 241
#define __NR_sched_getaffinity 242
#define __NR_io_setup       243
#define __NR_io_destroy     244
#define __NR_io_getevents   245
#define __NR_io_submit      246
#define __NR_io_cancel      247
#define __NR_exit_group     248
#define __NR_lookup_dcookie 249
#define __NR_epoll_create   250
#define __NR_epoll_ctl      251
#define __NR_epoll_wait     252
#define __NR_remap_file_pages 253
#define __NR_set_tid_address 256
#define __NR_timer_create   257
#define __NR_timer_settime  258
#define __NR_timer_gettime  259
#define __NR_timer_getoverrun 260
#define __NR_timer_delete   261
#define __NR_clock_settime  262
#define __NR_clock_gettime  263
#define __NR_clock_getres   264
#define __NR_clock_nanosleep 265
#define __NR_statfs64       266
#define __NR_fstatfs64      267
#define __NR_tgkill         268
#define __NR_utimes         269
#define __NR_arm_fadvise64_64 270
#define __NR_pciconfig_iobase 271
#define __NR_pciconfig_read 272
#define __NR_pciconfig_write 273
#define __NR_mq_open        274
#define __NR_mq_unlink      275
#define __NR_mq_timedsend   276
#define __NR_mq_timedreceive 277
#define __NR_mq_notify      278
#define __NR_mq_getsetattr  279
#define __NR_waitid         280
#define __NR_socket         281
#define __NR_bind           282
#define __NR_connect        283
#define __NR_listen         284
#define __NR_accept         285
#define __NR_getsockname    286
#define __NR_getpeername    287
#define __NR_socketpair     288
#define __NR_send           289
#define __NR_sendto         290
#define __NR_recv           291
#define __NR_recvfrom       292
#define __NR_shutdown       293
#define __NR_setsockopt     294
#define __NR_getsockopt     295
#define __NR_sendmsg        296
#define __NR_recvmsg        297
#define __NR_semop          298
#define __NR_semget         299
#define __NR_semctl         300
#define __NR_msgsnd         301
#define __NR_msgrcv         302
#define __NR_msgget         303
#define __NR_msgctl         304
#define __NR_shmat          305
#define __NR_shmdt          306
#define __NR_shmget         307
#define __NR_shmctl         308

// 最大系统调用号
#define __NR_syscalls       400
```

## 配置常量

```c
#ifndef CONFIG_SYSCALL_MAX_ARGS
#define CONFIG_SYSCALL_MAX_ARGS 7
#endif

#ifndef SYSCALL_TABLE_SIZE
#define SYSCALL_TABLE_SIZE __NR_syscalls
#endif
```
