/*
 * Copyright (c) 2024 OneWo-rtLinux Team
 * SPDX-License-Identifier: Apache-2.0
 */

#include "signal.h"
#include <zephyr/kernel.h>
#include <zephyr/kernel/process.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

/* Foreground process group ID */
static pid_t foreground_pgid = 0;
static struct k_mutex foreground_lock;

/* Suspended process tracking */
static pid_t suspended_fg_pid = 0;
static struct k_mutex suspended_lock;

/* Default signal actions */
static void default_signal_handler(int sig)
{
	struct z_process *proc = process_current();

	/* SIGTSTP - suspend process */
	if (sig == SIGTSTP) {
		printk("[SIGNAL] Process PID=%d received SIGTSTP, calling suspend...\n", proc ? proc->pid : 0);
		int ret = signal_suspend_process(proc ? proc->pid : 0);
		printk("[SIGNAL] suspend returned %d (THIS LINE SHOULD NOT APPEAR IF SUSPENDED)\n", ret);
		return;
	}

	/* SIGCONT - continue process */
	if (sig == SIGCONT) {
		printk("[SIGNAL] Process PID=%d continued\n", proc ? proc->pid : 0);
		/* Process is already running if it's handling this signal */
		return;
	}

	printk("[SIGNAL] Process PID=%d received signal %d, default action: terminate\n",
	       proc ? proc->pid : 0, sig);

	/* For most signals, default action is to terminate */
	/* In real implementation, SIGCHLD, SIGCONT etc would be ignored by default */
}

/**
 * @brief Initialize signal subsystem for a process
 */
int signal_process_init(struct z_process *proc)
{
	if (!proc || !proc->signal_state) {
		return -EINVAL;
	}

	struct process_signal *sig = proc->signal_state;

	sig->pending_signals = 0;
	sig->blocked_signals = 0;

	/* Initialize event for this process */
	k_event_init(&sig->signal_event);

	/* Initialize mutex */
	k_mutex_init(&sig->lock);

	/* Set default handlers */
	for (int i = 0; i < _NSIG; i++) {
		sig->handlers[i] = SIG_DFL;
	}

	/* SIGCHLD and SIGCONT are ignored by default in some contexts */
	sig->handlers[SIGCHLD] = SIG_IGN;
	/* SIGCONT default action is to continue - handled in default_signal_handler */

	return 0;
}

/**
 * @brief Cleanup signal subsystem for a process
 */
void signal_process_cleanup(struct z_process *proc)
{
	if (!proc || !proc->signal_state) {
		return;
	}

	/* Clear all pending signals */
	struct process_signal *sig = proc->signal_state;
	sig->pending_signals = 0;

	/* Wake up any waiters */
	k_event_post(&sig->signal_event, 0xFFFFFFFF);
}

/**
 * @brief Send a signal to a process
 */
int kill(pid_t pid, int sig)
{
	if (sig < 0 || sig >= _NSIG) {
		return -EINVAL;
	}

	if (pid <= 0) {
		return -EINVAL; /* Process groups not fully implemented yet */
	}

	struct z_process *proc = process_get(pid);
	if (!proc || !proc->signal_state) {
		printk("[SIGNAL] kill(%d, %d) FAILED: process not found or no signal_state\n", pid, sig);
		return -ESRCH;
	}

	struct process_signal *psig = proc->signal_state;

	k_mutex_lock(&psig->lock, K_FOREVER);

	/* Set the signal bit */
	psig->pending_signals |= (1U << sig);

	/* Wake up the process by posting to its event */
	k_event_post(&psig->signal_event, (1U << sig));

	k_mutex_unlock(&psig->lock);

	printk("[SIGNAL] kill(%d, %d) SUCCESS: pending_signals=0x%08x\n",
	       pid, sig, psig->pending_signals);

	return 0;
}

/**
 * @brief Send a signal to a process group
 */
int killpg(pid_t pgid, int sig)
{
	if (sig < 0 || sig >= _NSIG) {
		return -EINVAL;
	}

	/* Simplified: send to all processes (no real process groups yet) */
	int count = 0;
	for (int i = 0; i < CONFIG_MAX_PROCESS_COUNT; i++) {
		struct z_process *proc = process_get(i);
		if (proc && proc->pid > 0) {
			if (kill(proc->pid, sig) == 0) {
				count++;
			}
		}
	}

	return count > 0 ? 0 : -ESRCH;
}

/**
 * @brief Set signal handler for current process
 */
signal_handler_t signal(int sig, signal_handler_t handler)
{
	if (sig < 0 || sig >= _NSIG) {
		return SIG_ERR;
	}

	/* SIGKILL and SIGSTOP cannot be caught */
	if (sig == SIGKILL || sig == SIGSTOP) {
		return SIG_ERR;
	}

	struct z_process *proc = process_current();
	if (!proc || !proc->signal_state) {
		return SIG_ERR;
	}

	struct process_signal *psig = proc->signal_state;

	k_mutex_lock(&psig->lock, K_FOREVER);
	signal_handler_t old_handler = psig->handlers[sig];
	psig->handlers[sig] = handler;
	k_mutex_unlock(&psig->lock);

	return old_handler;
}

/**
 * @brief Block signals
 */
int sigblock(uint32_t mask)
{
	struct z_process *proc = process_current();
	if (!proc || !proc->signal_state) {
		return -ESRCH;
	}

	struct process_signal *psig = proc->signal_state;

	k_mutex_lock(&psig->lock, K_FOREVER);
	psig->blocked_signals |= mask;
	k_mutex_unlock(&psig->lock);

	return 0;
}

/**
 * @brief Unblock signals
 */
int sigunblock(uint32_t mask)
{
	struct z_process *proc = process_current();
	if (!proc || !proc->signal_state) {
		return -ESRCH;
	}

	struct process_signal *psig = proc->signal_state;

	k_mutex_lock(&psig->lock, K_FOREVER);
	psig->blocked_signals &= ~mask;
	k_mutex_unlock(&psig->lock);

	/* Check if we just unblocked any pending signals */
	signal_check_pending();

	return 0;
}

/**
 * @brief Wait for signal event with timeout
 */
int signal_wait(k_timeout_t timeout)
{
	struct z_process *proc = process_current();
	if (!proc || !proc->signal_state) {
		return -ESRCH;
	}

	struct process_signal *psig = proc->signal_state;

	/* Wait on this process's own event object */
	uint32_t events = k_event_wait(&psig->signal_event, 0xFFFFFFFF, false, timeout);

	if (events == 0) {
		return 0; /* Timeout */
	}

	/* Find which signal was received */
	for (int sig = 1; sig < _NSIG; sig++) {
		if (events & (1U << sig)) {
			return sig;
		}
	}

	return 0;
}

/**
 * @brief Check and handle pending signals
 */
int signal_check_pending(void)
{
	struct z_process *proc = process_current();
	if (!proc || !proc->signal_state) {
		return -ESRCH;
	}

	struct process_signal *psig = proc->signal_state;

	k_mutex_lock(&psig->lock, K_FOREVER);

	/* Get unblocked pending signals */
	uint32_t deliverable = psig->pending_signals & ~psig->blocked_signals;

	/* Debug: print if any signals are pending */
	if (psig->pending_signals != 0) {
		printk("[SIGNAL] PID=%d check_pending: pending=0x%08x, blocked=0x%08x, deliverable=0x%08x\n",
		       proc->pid, psig->pending_signals, psig->blocked_signals, deliverable);
	}

	if (deliverable == 0) {
		k_mutex_unlock(&psig->lock);
		return 0;
	}

	int handled = 0;

	/* Handle each pending signal */
	for (int sig = 1; sig < _NSIG; sig++) {
		if (!(deliverable & (1U << sig))) {
			continue;
		}

		/* Clear the pending bit */
		psig->pending_signals &= ~(1U << sig);

		signal_handler_t handler = psig->handlers[sig];

		k_mutex_unlock(&psig->lock);

		/* Call handler outside of lock */
		if (handler == SIG_IGN) {
			/* Ignore */
			printk("[SIGNAL] PID=%d ignored signal %d\n", proc->pid, sig);
		} else if (handler == SIG_DFL) {
			/* Default action */
			default_signal_handler(sig);
		} else {
			/* User-defined handler */
			printk("[SIGNAL] PID=%d calling handler for signal %d\n", proc->pid, sig);
			handler(sig);
		}

		handled++;

		k_mutex_lock(&psig->lock, K_FOREVER);
	}

	k_mutex_unlock(&psig->lock);

	return handled;
}

/**
 * @brief Set foreground process group
 */
void signal_set_foreground_pgid(pid_t pgid)
{
	k_mutex_lock(&foreground_lock, K_FOREVER);
	foreground_pgid = pgid;
	k_mutex_unlock(&foreground_lock);

	printk("[SIGNAL] Foreground process group set to PID=%d\n", pgid);
}

/**
 * @brief Get foreground process group
 */
pid_t signal_get_foreground_pgid(void)
{
	k_mutex_lock(&foreground_lock, K_FOREVER);
	pid_t pgid = foreground_pgid;
	k_mutex_unlock(&foreground_lock);

	return pgid;
}

/**
 * @brief Get process signal state
 */
struct process_signal *signal_get_state(struct z_process *proc)
{
	if (!proc) {
		return NULL;
	}
	return proc->signal_state;
}

/**
 * @brief Suspend (stop) a process
 */
int signal_suspend_process(pid_t pid)
{
	if (pid <= 0) {
		return -EINVAL;
	}

	struct z_process *proc = process_get(pid);
	if (!proc || !proc->main_thread) {
		return -ESRCH;
	}

	printk("[SIGNAL] Suspending process PID=%d\n", pid);

	/* Track suspended foreground process BEFORE suspending */
	k_mutex_lock(&suspended_lock, K_FOREVER);
	if (pid == foreground_pgid) {
		suspended_fg_pid = pid;
		/* Clear foreground since it's now suspended */
		k_mutex_lock(&foreground_lock, K_FOREVER);
		foreground_pgid = 0;
		k_mutex_unlock(&foreground_lock);
		printk("[SIGNAL] Foreground process suspended. Use 'fg' to resume.\n");
	}
	k_mutex_unlock(&suspended_lock);

	/* Suspend the main thread - THIS MUST BE LAST!
	 * After this call, if we're suspending ourselves, we stop executing here.
	 * When resumed, execution will continue after k_thread_suspend returns.
	 */
	k_thread_suspend(proc->main_thread);

	return 0;
}

/**
 * @brief Resume (continue) a suspended process
 */
int signal_resume_process(pid_t pid)
{
	if (pid <= 0) {
		return -EINVAL;
	}

	struct z_process *proc = process_get(pid);
	if (!proc || !proc->main_thread) {
		return -ESRCH;
	}

	printk("[SIGNAL] Resuming process PID=%d\n", pid);

	/* Resume the main thread */
	k_thread_resume(proc->main_thread);

	/* Restore as foreground process */
	k_mutex_lock(&suspended_lock, K_FOREVER);
	if (suspended_fg_pid == pid) {
		suspended_fg_pid = 0;
		signal_set_foreground_pgid(pid);
	}
	k_mutex_unlock(&suspended_lock);

	/* Send SIGCONT to the process */
	kill(pid, SIGCONT);

	return 0;
}

/**
 * @brief Check if a process is suspended
 */
int signal_is_suspended(pid_t pid)
{
	if (pid <= 0) {
		return -EINVAL;
	}

	struct z_process *proc = process_get(pid);
	if (!proc || !proc->main_thread) {
		return -ESRCH;
	}

	/* Check if thread is suspended */
	uint8_t state = proc->main_thread->base.thread_state;
	return (state & _THREAD_SUSPENDED) ? 1 : 0;
}

/**
 * @brief Get suspended foreground process ID
 */
pid_t signal_get_suspended_fg_pid(void)
{
	k_mutex_lock(&suspended_lock, K_FOREVER);
	pid_t pid = suspended_fg_pid;
	k_mutex_unlock(&suspended_lock);
	return pid;
}

/**
 * @brief Initialize signal subsystem
 */
static int signal_subsystem_init(void)
{
	k_mutex_init(&foreground_lock);
	k_mutex_init(&suspended_lock);
	foreground_pgid = 0;
	suspended_fg_pid = 0;
	return 0;
}

SYS_INIT(signal_subsystem_init, APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
