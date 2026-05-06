#!/bin/bash
# 生成接口桩代码的脚本

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
OUTPUT_DIR="${PROJECT_ROOT}/generated"

# 颜色输出
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}POSIX接口桩代码生成器${NC}"
echo "================================"

# 创建输出目录
mkdir -p "${OUTPUT_DIR}"/{include,src}

# 生成pthread接口桩
generate_pthread_stubs() {
    echo -e "${YELLOW}生成pthread接口桩...${NC}"

    cat > "${OUTPUT_DIR}/include/pthread.h" << 'EOF'
#ifndef POSIX_PTHREAD_H
#define POSIX_PTHREAD_H

#include <zephyr/kernel.h>
#include <errno.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 线程类型 */
typedef uint32_t pthread_t;

/* 线程属性 */
typedef struct {
    int detachstate;
    size_t stacksize;
    int schedpolicy;
    struct sched_param {
        int sched_priority;
    } schedparam;
} pthread_attr_t;

/* 互斥锁 */
typedef struct {
    struct k_mutex kmutex;
    int type;
} pthread_mutex_t;

typedef struct {
    int type;
} pthread_mutexattr_t;

/* 条件变量 */
typedef struct {
    struct k_condvar kcondvar;
} pthread_cond_t;

typedef struct {
    int dummy;
} pthread_condattr_t;

/* 线程管理 */
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine)(void *), void *arg);
int pthread_join(pthread_t thread, void **retval);
int pthread_detach(pthread_t thread);
int pthread_cancel(pthread_t thread);
void pthread_exit(void *retval) __attribute__((noreturn));
pthread_t pthread_self(void);

/* 线程属性 */
int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t *attr);
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);

/* 互斥锁 */
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);

/* 条件变量 */
int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
int pthread_cond_destroy(pthread_cond_t *cond);
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);

#ifdef __cplusplus
}
#endif

#endif /* POSIX_PTHREAD_H */
EOF

    cat > "${OUTPUT_DIR}/src/pthread_stub.c" << 'EOF'
#include "pthread.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(pthread, LOG_LEVEL_DBG);

/* TODO: 实现pthread接口 */

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine)(void *), void *arg)
{
    LOG_ERR("pthread_create: Not implemented yet");
    return -ENOSYS;
}

int pthread_join(pthread_t thread, void **retval)
{
    LOG_ERR("pthread_join: Not implemented yet");
    return -ENOSYS;
}

/* 其他接口桩... */
EOF

    echo -e "${GREEN}✓ pthread接口桩已生成${NC}"
}

# 生成进程管理接口桩
generate_process_stubs() {
    echo -e "${YELLOW}生成进程管理接口桩...${NC}"

    cat > "${OUTPUT_DIR}/include/unistd_process.h" << 'EOF'
#ifndef POSIX_UNISTD_PROCESS_H
#define POSIX_UNISTD_PROCESS_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 进程管理 */
pid_t fork(void);
int execv(const char *path, char *const argv[]);
int execve(const char *path, char *const argv[], char *const envp[]);
void exit(int status) __attribute__((noreturn));

/* 进程ID */
pid_t getpid(void);
pid_t getppid(void);

/* 等待子进程 */
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);

#ifdef __cplusplus
}
#endif

#endif /* POSIX_UNISTD_PROCESS_H */
EOF

    echo -e "${GREEN}✓ 进程管理接口桩已生成${NC}"
}

# 生成设备管理接口桩
generate_device_stubs() {
    echo -e "${YELLOW}生成设备管理接口桩...${NC}"

    cat > "${OUTPUT_DIR}/include/fcntl_device.h" << 'EOF'
#ifndef POSIX_FCNTL_DEVICE_H
#define POSIX_FCNTL_DEVICE_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 文件打开标志 */
#define O_RDONLY    0x0000
#define O_WRONLY    0x0001
#define O_RDWR      0x0002
#define O_CREAT     0x0100
#define O_TRUNC     0x0200
#define O_APPEND    0x0400
#define O_NONBLOCK  0x0800

/* 设备管理 */
int open(const char *pathname, int flags, ...);
int close(int fd);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
int ioctl(int fd, unsigned long request, ...);

#ifdef __cplusplus
}
#endif

#endif /* POSIX_FCNTL_DEVICE_H */
EOF

    echo -e "${GREEN}✓ 设备管理接口桩已生成${NC}"
}

# 生成CMakeLists.txt
generate_cmake() {
    echo -e "${YELLOW}生成CMakeLists.txt...${NC}"

    cat > "${OUTPUT_DIR}/CMakeLists.txt" << 'EOF'
# POSIX适配层库

zephyr_library_named(posix_adapter)

zephyr_library_sources(
    src/pthread_stub.c
    # 添加更多源文件...
)

zephyr_library_include_directories(
    include
)

zephyr_library_compile_definitions(
    -DCONFIG_POSIX_ADAPTER=1
)
EOF

    echo -e "${GREEN}✓ CMakeLists.txt已生成${NC}"
}

# 主函数
main() {
    generate_pthread_stubs
    generate_process_stubs
    generate_device_stubs
    generate_cmake

    echo ""
    echo -e "${GREEN}================================${NC}"
    echo -e "${GREEN}接口桩代码生成完成！${NC}"
    echo -e "输出目录: ${OUTPUT_DIR}"
    echo ""
    echo "下一步："
    echo "1. 查看生成的头文件和源文件"
    echo "2. 根据规范文档实现具体功能"
    echo "3. 编写测试用例验证实现"
}

main "$@"
