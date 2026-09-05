/*
 * Copyright (c) 2024 OneWo-rtLinux Team
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SIGNAL_H_
#define SIGNAL_H_

#include <zephyr/kernel.h>
#include <zephyr/kernel/process.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Signal numbers (POSIX-like) */
#define SIGHUP    1   /* Hangup */
#define SIGINT    2   /* Interrupt (Ctrl+C) */
#define SIGQUIT   3   /* Quit (Ctrl+\) */
#define SIGILL    4   /* Illegal instruction */
#define SIGTRAP   5   /* Trace/breakpoint trap */
#define SIGABRT   6   /* Aborted */
#define SIGBUS    7   /* Bus error */
#define SIGFPE    8   /* Floating point exception */
#define SIGKILL   9   /* Kill (cannot be caught) */
#define SIGUSR1   10  /* User-defined signal 1 */
#define SIGSEGV   11  /* Segmentation fault */
#define SIGUSR2   12  /* User-defined signal 2 */
#define SIGPIPE   13  /* Broken pipe */
#define SIGALRM   14  /* Alarm clock */
#define SIGTERM   15  /* Termination */
#define SIGCHLD   17  /* Child stopped or terminated */
#define SIGCONT   18  /* Continue if stopped */
#define SIGSTOP   19  /* Stop process */
#define SIGTSTP   20  /* Stop typed at terminal (Ctrl+Z) */
#define SIGTTIN   21  /* Terminal input for background */
#define SIGTTOU   22  /* Terminal output for background */

#define _NSIG     32  /* Maximum number of signals */

/* Signal handler types */
#define SIG_DFL   ((signal_handler_t)0)  /* Default handler */
#define SIG_IGN   ((signal_handler_t)1)  /* Ignore signal */
#define SIG_ERR   ((signal_handler_t)-1) /* Error return */

/* Signal handler function type */
typedef void (*signal_handler_t)(int sig);

/* Per-process signal state */
struct process_signal {
	uint32_t pending_signals;        /* Pending signals bitmap */
	uint32_t blocked_signals;        /* Blocked signals bitmap */
	struct k_event signal_event;     /* Process-specific event for signal delivery */
	signal_handler_t handlers[_NSIG]; /* Signal handler functions */
	struct k_mutex lock;             /* Protect signal state */
};

/**
 * @brief Initialize signal subsystem for a process
 *
 * @param proc Process to initialize
 * @return 0 on success, negative errno on failure
 */
int signal_process_init(struct z_process *proc);

/**
 * @brief Cleanup signal subsystem for a process
 *
 * @param proc Process to cleanup
 */
void signal_process_cleanup(struct z_process *proc);

/**
 * @brief Send a signal to a process
 *
 * @param pid Target process ID
 * @param sig Signal number
 * @return 0 on success, negative errno on failure
 */
int kill(pid_t pid, int sig);

/**
 * @brief Send a signal to a process group
 *
 * @param pgid Process group ID
 * @param sig Signal number
 * @return 0 on success, negative errno on failure
 */
int killpg(pid_t pgid, int sig);

/**
 * @brief Set signal handler
 *
 * @param sig Signal number
 * @param handler Handler function (SIG_DFL, SIG_IGN, or function pointer)
 * @return Previous handler on success, SIG_ERR on failure
 */
signal_handler_t signal(int sig, signal_handler_t handler);

/**
 * @brief Block signals
 *
 * @param mask Signals to block (bitmap)
 * @return 0 on success, negative errno on failure
 */
int sigblock(uint32_t mask);

/**
 * @brief Unblock signals
 *
 * @param mask Signals to unblock (bitmap)
 * @return 0 on success, negative errno on failure
 */
int sigunblock(uint32_t mask);

/**
 * @brief Wait for signal event with timeout
 *
 * Check if any signals are pending for the current process.
 * This should be called periodically from process main loops.
 *
 * @param timeout_ms Timeout in milliseconds (K_MSEC value)
 * @return Signal number if pending, 0 if timeout, negative errno on error
 */
int signal_wait(k_timeout_t timeout);

/**
 * @brief Check and handle pending signals for current process
 *
 * Call this from interruptible wait loops.
 * Returns immediately if no signals pending.
 *
 * @return Number of signals handled, or negative errno on error
 */
int signal_check_pending(void);

/**
 * @brief Set foreground process group
 *
 * Only the foreground process group receives terminal signals (SIGINT, SIGTSTP, etc.)
 *
 * @param pgid Process group ID (0 = no foreground)
 */
void signal_set_foreground_pgid(pid_t pgid);

/**
 * @brief Get foreground process group
 *
 * @return Current foreground process group ID
 */
pid_t signal_get_foreground_pgid(void);

/**
 * @brief Get process signal state
 *
 * @param proc Process
 * @return Pointer to signal state, or NULL if not initialized
 */
struct process_signal *signal_get_state(struct z_process *proc);

/**
 * @brief Suspend (stop) a process
 *
 * @param pid Process ID to suspend
 * @return 0 on success, negative errno on failure
 */
int signal_suspend_process(pid_t pid);

/**
 * @brief Resume (continue) a suspended process
 *
 * @param pid Process ID to resume
 * @return 0 on success, negative errno on failure
 */
int signal_resume_process(pid_t pid);

/**
 * @brief Check if a process is suspended
 *
 * @param pid Process ID to check
 * @return 1 if suspended, 0 if not, negative errno on error
 */
int signal_is_suspended(pid_t pid);

/**
 * @brief Get suspended foreground process ID
 *
 * @return PID of suspended foreground process, or 0 if none
 */
pid_t signal_get_suspended_fg_pid(void);

#ifdef __cplusplus
}
#endif

#endif /* SIGNAL_H_ */
