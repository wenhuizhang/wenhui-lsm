/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_SIGNAL_H
#define _LINUX_SCHED_SIGNAL_H

#include <linux/rculist.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/sched/jobctl.h>
#include <linux/sched/task.h>
#include <linux/cred.h>
#include <linux/refcount.h>



struct sighand_struct {
	spinlock_t		siglock;
	refcount_t		count;
	wait_queue_head_t	signalfd_wqh;
	struct k_sigaction	action[_NSIG];
};


struct pacct_struct {
	int			ac_flag;
	long			ac_exitcode;
	unsigned long		ac_mem;
	u64			ac_utime, ac_stime;
	unsigned long		ac_minflt, ac_majflt;
};

struct cpu_itimer {
	u64 expires;
	u64 incr;
};


struct task_cputime_atomic {
	atomic64_t utime;
	atomic64_t stime;
	atomic64_t sum_exec_runtime;
};

#define INIT_CPUTIME_ATOMIC \
	(struct task_cputime_atomic) {				\
		.utime = ATOMIC64_INIT(0),			\
		.stime = ATOMIC64_INIT(0),			\
		.sum_exec_runtime = ATOMIC64_INIT(0),		\
	}

struct thread_group_cputimer {
	struct task_cputime_atomic cputime_atomic;
	bool running;
	bool checking_timer;
};

struct multiprocess_signals {
	sigset_t signal;
	struct hlist_node node;
};


struct signal_struct {
	refcount_t		sigcnt;
	atomic_t		live;
	int			nr_threads;
	struct list_head	thread_head;

	wait_queue_head_t	wait_chldexit;	

	
	struct task_struct	*curr_target;

	
	struct sigpending	shared_pending;

	
	struct hlist_head	multiprocess;

	
	int			group_exit_code;
	
	int			notify_count;
	struct task_struct	*group_exit_task;

	
	int			group_stop_count;
	unsigned int		flags; 

	
	unsigned int		is_child_subreaper:1;
	unsigned int		has_child_subreaper:1;

#ifdef CONFIG_POSIX_TIMERS

	
	int			posix_timer_id;
	struct list_head	posix_timers;

	
	struct hrtimer real_timer;
	ktime_t it_real_incr;

	
	struct cpu_itimer it[2];

	
	struct thread_group_cputimer cputimer;

	
	struct task_cputime cputime_expires;

	struct list_head cpu_timers[3];

#endif

	
	struct pid *pids[PIDTYPE_MAX];

#ifdef CONFIG_NO_HZ_FULL
	atomic_t tick_dep_mask;
#endif

	struct pid *tty_old_pgrp;

	
	int leader;

	struct tty_struct *tty; 

#ifdef CONFIG_SCHED_AUTOGROUP
	struct autogroup *autogroup;
#endif
	
	seqlock_t stats_lock;
	u64 utime, stime, cutime, cstime;
	u64 gtime;
	u64 cgtime;
	struct prev_cputime prev_cputime;
	unsigned long nvcsw, nivcsw, cnvcsw, cnivcsw;
	unsigned long min_flt, maj_flt, cmin_flt, cmaj_flt;
	unsigned long inblock, oublock, cinblock, coublock;
	unsigned long maxrss, cmaxrss;
	struct task_io_accounting ioac;

	
	unsigned long long sum_sched_runtime;

	
	struct rlimit rlim[RLIM_NLIMITS];

#ifdef CONFIG_BSD_PROCESS_ACCT
	struct pacct_struct pacct;	
#endif
#ifdef CONFIG_TASKSTATS
	struct taskstats *stats;
#endif
#ifdef CONFIG_AUDIT
	unsigned audit_tty;
	struct tty_audit_buf *tty_audit_buf;
#endif

	
	bool oom_flag_origin;
	short oom_score_adj;		
	short oom_score_adj_min;	
	struct mm_struct *oom_mm;	

	struct mutex cred_guard_mutex;	
} __randomize_layout;


#define SIGNAL_STOP_STOPPED	0x00000001 
#define SIGNAL_STOP_CONTINUED	0x00000002 
#define SIGNAL_GROUP_EXIT	0x00000004 
#define SIGNAL_GROUP_COREDUMP	0x00000008 

#define SIGNAL_CLD_STOPPED	0x00000010
#define SIGNAL_CLD_CONTINUED	0x00000020
#define SIGNAL_CLD_MASK		(SIGNAL_CLD_STOPPED|SIGNAL_CLD_CONTINUED)

#define SIGNAL_UNKILLABLE	0x00000040 

#define SIGNAL_STOP_MASK (SIGNAL_CLD_MASK | SIGNAL_STOP_STOPPED | \
			  SIGNAL_STOP_CONTINUED)

static inline void signal_set_stop_flags(struct signal_struct *sig,
					 unsigned int flags)
{
	WARN_ON(sig->flags & (SIGNAL_GROUP_EXIT|SIGNAL_GROUP_COREDUMP));
	sig->flags = (sig->flags & ~SIGNAL_STOP_MASK) | flags;
}


static inline int signal_group_exit(const struct signal_struct *sig)
{
	return	(sig->flags & SIGNAL_GROUP_EXIT) ||
		(sig->group_exit_task != NULL);
}

extern void flush_signals(struct task_struct *);
extern void ignore_signals(struct task_struct *);
extern void flush_signal_handlers(struct task_struct *, int force_default);
extern int dequeue_signal(struct task_struct *task,
			  sigset_t *mask, kernel_siginfo_t *info);

static inline int kernel_dequeue_signal(void)
{
	struct task_struct *task = current;
	kernel_siginfo_t __info;
	int ret;

	spin_lock_irq(&task->sighand->siglock);
	ret = dequeue_signal(task, &task->blocked, &__info);
	spin_unlock_irq(&task->sighand->siglock);

	return ret;
}

static inline void kernel_signal_stop(void)
{
	spin_lock_irq(&current->sighand->siglock);
	if (current->jobctl & JOBCTL_STOP_DEQUEUED)
		set_special_state(TASK_STOPPED);
	spin_unlock_irq(&current->sighand->siglock);

	schedule();
}
#ifdef __ARCH_SI_TRAPNO
# define ___ARCH_SI_TRAPNO(_a1) , _a1
#else
# define ___ARCH_SI_TRAPNO(_a1)
#endif
#ifdef __ia64__
# define ___ARCH_SI_IA64(_a1, _a2, _a3) , _a1, _a2, _a3
#else
# define ___ARCH_SI_IA64(_a1, _a2, _a3)
#endif

int force_sig_fault_to_task(int sig, int code, void __user *addr
	___ARCH_SI_TRAPNO(int trapno)
	___ARCH_SI_IA64(int imm, unsigned int flags, unsigned long isr)
	, struct task_struct *t);
int force_sig_fault(int sig, int code, void __user *addr
	___ARCH_SI_TRAPNO(int trapno)
	___ARCH_SI_IA64(int imm, unsigned int flags, unsigned long isr));
int send_sig_fault(int sig, int code, void __user *addr
	___ARCH_SI_TRAPNO(int trapno)
	___ARCH_SI_IA64(int imm, unsigned int flags, unsigned long isr)
	, struct task_struct *t);

int force_sig_mceerr(int code, void __user *, short);
int send_sig_mceerr(int code, void __user *, short, struct task_struct *);

int force_sig_bnderr(void __user *addr, void __user *lower, void __user *upper);
int force_sig_pkuerr(void __user *addr, u32 pkey);

int force_sig_ptrace_errno_trap(int errno, void __user *addr);

extern int send_sig_info(int, struct kernel_siginfo *, struct task_struct *);
extern void force_sigsegv(int sig);
extern int force_sig_info(struct kernel_siginfo *);
extern int __kill_pgrp_info(int sig, struct kernel_siginfo *info, struct pid *pgrp);
extern int kill_pid_info(int sig, struct kernel_siginfo *info, struct pid *pid);
extern int kill_pid_usb_asyncio(int sig, int errno, sigval_t addr, struct pid *,
				const struct cred *);
extern int kill_pgrp(struct pid *pid, int sig, int priv);
extern int kill_pid(struct pid *pid, int sig, int priv);
extern __must_check bool do_notify_parent(struct task_struct *, int);
extern void __wake_up_parent(struct task_struct *p, struct task_struct *parent);
extern void force_sig(int);
extern int send_sig(int, struct task_struct *, int);
extern int zap_other_threads(struct task_struct *p);
extern struct sigqueue *sigqueue_alloc(void);
extern void sigqueue_free(struct sigqueue *);
extern int send_sigqueue(struct sigqueue *, struct pid *, enum pid_type);
extern int do_sigaction(int, struct k_sigaction *, struct k_sigaction *);

static inline int restart_syscall(void)
{
	set_tsk_thread_flag(current, TIF_SIGPENDING);
	return -ERESTARTNOINTR;
}

static inline int signal_pending(struct task_struct *p)
{
	return unlikely(test_tsk_thread_flag(p,TIF_SIGPENDING));
}

static inline int __fatal_signal_pending(struct task_struct *p)
{
	return unlikely(sigismember(&p->pending.signal, SIGKILL));
}

static inline int fatal_signal_pending(struct task_struct *p)
{
	return signal_pending(p) && __fatal_signal_pending(p);
}

static inline int signal_pending_state(long state, struct task_struct *p)
{
	if (!(state & (TASK_INTERRUPTIBLE | TASK_WAKEKILL)))
		return 0;
	if (!signal_pending(p))
		return 0;

	return (state & TASK_INTERRUPTIBLE) || __fatal_signal_pending(p);
}


extern void recalc_sigpending_and_wake(struct task_struct *t);
extern void recalc_sigpending(void);
extern void calculate_sigpending(void);

extern void signal_wake_up_state(struct task_struct *t, unsigned int state);

static inline void signal_wake_up(struct task_struct *t, bool resume)
{
	signal_wake_up_state(t, resume ? TASK_WAKEKILL : 0);
}
static inline void ptrace_signal_wake_up(struct task_struct *t, bool resume)
{
	signal_wake_up_state(t, resume ? __TASK_TRACED : 0);
}

void task_join_group_stop(struct task_struct *task);

#ifdef TIF_RESTORE_SIGMASK



static inline void set_restore_sigmask(void)
{
	set_thread_flag(TIF_RESTORE_SIGMASK);
}

static inline void clear_tsk_restore_sigmask(struct task_struct *task)
{
	clear_tsk_thread_flag(task, TIF_RESTORE_SIGMASK);
}

static inline void clear_restore_sigmask(void)
{
	clear_thread_flag(TIF_RESTORE_SIGMASK);
}
static inline bool test_tsk_restore_sigmask(struct task_struct *task)
{
	return test_tsk_thread_flag(task, TIF_RESTORE_SIGMASK);
}
static inline bool test_restore_sigmask(void)
{
	return test_thread_flag(TIF_RESTORE_SIGMASK);
}
static inline bool test_and_clear_restore_sigmask(void)
{
	return test_and_clear_thread_flag(TIF_RESTORE_SIGMASK);
}

#else	


static inline void set_restore_sigmask(void)
{
	current->restore_sigmask = true;
}
static inline void clear_tsk_restore_sigmask(struct task_struct *task)
{
	task->restore_sigmask = false;
}
static inline void clear_restore_sigmask(void)
{
	current->restore_sigmask = false;
}
static inline bool test_restore_sigmask(void)
{
	return current->restore_sigmask;
}
static inline bool test_tsk_restore_sigmask(struct task_struct *task)
{
	return task->restore_sigmask;
}
static inline bool test_and_clear_restore_sigmask(void)
{
	if (!current->restore_sigmask)
		return false;
	current->restore_sigmask = false;
	return true;
}
#endif

static inline void restore_saved_sigmask(void)
{
	if (test_and_clear_restore_sigmask())
		__set_current_blocked(&current->saved_sigmask);
}

extern int set_user_sigmask(const sigset_t __user *umask, size_t sigsetsize);

static inline void restore_saved_sigmask_unless(bool interrupted)
{
	if (interrupted)
		WARN_ON(!test_thread_flag(TIF_SIGPENDING));
	else
		restore_saved_sigmask();
}

static inline sigset_t *sigmask_to_save(void)
{
	sigset_t *res = &current->blocked;
	if (unlikely(test_restore_sigmask()))
		res = &current->saved_sigmask;
	return res;
}

static inline int kill_cad_pid(int sig, int priv)
{
	return kill_pid(cad_pid, sig, priv);
}


#define SEND_SIG_NOINFO ((struct kernel_siginfo *) 0)
#define SEND_SIG_PRIV	((struct kernel_siginfo *) 1)


static inline int on_sig_stack(unsigned long sp)
{
	
	if (current->sas_ss_flags & SS_AUTODISARM)
		return 0;

#ifdef CONFIG_STACK_GROWSUP
	return sp >= current->sas_ss_sp &&
		sp - current->sas_ss_sp < current->sas_ss_size;
#else
	return sp > current->sas_ss_sp &&
		sp - current->sas_ss_sp <= current->sas_ss_size;
#endif
}

static inline int sas_ss_flags(unsigned long sp)
{
	if (!current->sas_ss_size)
		return SS_DISABLE;

	return on_sig_stack(sp) ? SS_ONSTACK : 0;
}

static inline void sas_ss_reset(struct task_struct *p)
{
	p->sas_ss_sp = 0;
	p->sas_ss_size = 0;
	p->sas_ss_flags = SS_DISABLE;
}

static inline unsigned long sigsp(unsigned long sp, struct ksignal *ksig)
{
	if (unlikely((ksig->ka.sa.sa_flags & SA_ONSTACK)) && ! sas_ss_flags(sp))
#ifdef CONFIG_STACK_GROWSUP
		return current->sas_ss_sp;
#else
		return current->sas_ss_sp + current->sas_ss_size;
#endif
	return sp;
}

extern void __cleanup_sighand(struct sighand_struct *);
extern void flush_itimer_signals(void);

#define tasklist_empty() \
	list_empty(&init_task.tasks)

#define next_task(p) \
	list_entry_rcu((p)->tasks.next, struct task_struct, tasks)

#define for_each_process(p) \
	for (p = &init_task ; (p = next_task(p)) != &init_task ; )

extern bool current_is_single_threaded(void);


#define do_each_thread(g, t) \
	for (g = t = &init_task ; (g = t = next_task(g)) != &init_task ; ) do

#define while_each_thread(g, t) \
	while ((t = next_thread(t)) != g)

#define __for_each_thread(signal, t)	\
	list_for_each_entry_rcu(t, &(signal)->thread_head, thread_node)

#define for_each_thread(p, t)		\
	__for_each_thread((p)->signal, t)


#define for_each_process_thread(p, t)	\
	for_each_process(p) for_each_thread(p, t)

typedef int (*proc_visitor)(struct task_struct *p, void *data);
void walk_process_tree(struct task_struct *top, proc_visitor, void *);

static inline
struct pid *task_pid_type(struct task_struct *task, enum pid_type type)
{
	struct pid *pid;
	if (type == PIDTYPE_PID)
		pid = task_pid(task);
	else
		pid = task->signal->pids[type];
	return pid;
}

static inline struct pid *task_tgid(struct task_struct *task)
{
	return task->signal->pids[PIDTYPE_TGID];
}


static inline struct pid *task_pgrp(struct task_struct *task)
{
	return task->signal->pids[PIDTYPE_PGID];
}

static inline struct pid *task_session(struct task_struct *task)
{
	return task->signal->pids[PIDTYPE_SID];
}

static inline int get_nr_threads(struct task_struct *task)
{
	return task->signal->nr_threads;
}

static inline bool thread_group_leader(struct task_struct *p)
{
	return p->exit_signal >= 0;
}


static inline bool has_group_leader_pid(struct task_struct *p)
{
	return task_pid(p) == task_tgid(p);
}

static inline
bool same_thread_group(struct task_struct *p1, struct task_struct *p2)
{
	return p1->signal == p2->signal;
}

static inline struct task_struct *next_thread(const struct task_struct *p)
{
	return list_entry_rcu(p->thread_group.next,
			      struct task_struct, thread_group);
}

static inline int thread_group_empty(struct task_struct *p)
{
	return list_empty(&p->thread_group);
}

#define delay_group_leader(p) \
		(thread_group_leader(p) && !thread_group_empty(p))

extern struct sighand_struct *__lock_task_sighand(struct task_struct *task,
							unsigned long *flags);

static inline struct sighand_struct *lock_task_sighand(struct task_struct *task,
						       unsigned long *flags)
{
	struct sighand_struct *ret;

	ret = __lock_task_sighand(task, flags);
	(void)__cond_lock(&task->sighand->siglock, ret);
	return ret;
}

static inline void unlock_task_sighand(struct task_struct *task,
						unsigned long *flags)
{
	spin_unlock_irqrestore(&task->sighand->siglock, *flags);
}

static inline unsigned long task_rlimit(const struct task_struct *task,
		unsigned int limit)
{
	return READ_ONCE(task->signal->rlim[limit].rlim_cur);
}

static inline unsigned long task_rlimit_max(const struct task_struct *task,
		unsigned int limit)
{
	return READ_ONCE(task->signal->rlim[limit].rlim_max);
}

static inline unsigned long rlimit(unsigned int limit)
{
	return task_rlimit(current, limit);
}

static inline unsigned long rlimit_max(unsigned int limit)
{
	return task_rlimit_max(current, limit);
}

#endif 
