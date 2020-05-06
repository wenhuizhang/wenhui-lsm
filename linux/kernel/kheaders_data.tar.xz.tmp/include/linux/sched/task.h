/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_TASK_H
#define _LINUX_SCHED_TASK_H



#include <linux/sched.h>
#include <linux/uaccess.h>

struct task_struct;
struct rusage;
union thread_union;


#define CLONE_LEGACY_FLAGS 0xffffffffULL

struct kernel_clone_args {
	u64 flags;
	int __user *pidfd;
	int __user *child_tid;
	int __user *parent_tid;
	int exit_signal;
	unsigned long stack;
	unsigned long stack_size;
	unsigned long tls;
};


extern rwlock_t tasklist_lock;
extern spinlock_t mmlist_lock;

extern union thread_union init_thread_union;
extern struct task_struct init_task;

#ifdef CONFIG_PROVE_RCU
extern int lockdep_tasklist_lock_is_held(void);
#endif 

extern asmlinkage void schedule_tail(struct task_struct *prev);
extern void init_idle(struct task_struct *idle, int cpu);

extern int sched_fork(unsigned long clone_flags, struct task_struct *p);
extern void sched_dead(struct task_struct *p);

void __noreturn do_task_dead(void);

extern void proc_caches_init(void);

extern void fork_init(void);

extern void release_task(struct task_struct * p);

#ifdef CONFIG_HAVE_COPY_THREAD_TLS
extern int copy_thread_tls(unsigned long, unsigned long, unsigned long,
			struct task_struct *, unsigned long);
#else
extern int copy_thread(unsigned long, unsigned long, unsigned long,
			struct task_struct *);


static inline int copy_thread_tls(
		unsigned long clone_flags, unsigned long sp, unsigned long arg,
		struct task_struct *p, unsigned long tls)
{
	return copy_thread(clone_flags, sp, arg, p);
}
#endif
extern void flush_thread(void);

#ifdef CONFIG_HAVE_EXIT_THREAD
extern void exit_thread(struct task_struct *tsk);
#else
static inline void exit_thread(struct task_struct *tsk)
{
}
#endif
extern void do_group_exit(int);

extern void exit_files(struct task_struct *);
extern void exit_itimers(struct signal_struct *);

extern long _do_fork(struct kernel_clone_args *kargs);
extern bool legacy_clone_args_valid(const struct kernel_clone_args *kargs);
extern long do_fork(unsigned long, unsigned long, unsigned long, int __user *, int __user *);
struct task_struct *fork_idle(int);
struct mm_struct *copy_init_mm(void);
extern pid_t kernel_thread(int (*fn)(void *), void *arg, unsigned long flags);
extern long kernel_wait4(pid_t, int __user *, int, struct rusage *);

extern void free_task(struct task_struct *tsk);


#ifdef CONFIG_SMP
extern void sched_exec(void);
#else
#define sched_exec()   {}
#endif

#define get_task_struct(tsk) do { refcount_inc(&(tsk)->usage); } while(0)

extern void __put_task_struct(struct task_struct *t);

static inline void put_task_struct(struct task_struct *t)
{
	if (refcount_dec_and_test(&t->usage))
		__put_task_struct(t);
}

struct task_struct *task_rcu_dereference(struct task_struct **ptask);

#ifdef CONFIG_ARCH_WANTS_DYNAMIC_TASK_STRUCT
extern int arch_task_struct_size __read_mostly;
#else
# define arch_task_struct_size (sizeof(struct task_struct))
#endif

#ifndef CONFIG_HAVE_ARCH_THREAD_STRUCT_WHITELIST

static inline void arch_thread_struct_whitelist(unsigned long *offset,
						unsigned long *size)
{
	*offset = 0;
	
	*size = arch_task_struct_size - offsetof(struct task_struct, thread);
}
#endif

#ifdef CONFIG_VMAP_STACK
static inline struct vm_struct *task_stack_vm_area(const struct task_struct *t)
{
	return t->stack_vm_area;
}
#else
static inline struct vm_struct *task_stack_vm_area(const struct task_struct *t)
{
	return NULL;
}
#endif


static inline void task_lock(struct task_struct *p)
{
	spin_lock(&p->alloc_lock);
}

static inline void task_unlock(struct task_struct *p)
{
	spin_unlock(&p->alloc_lock);
}

#endif 
