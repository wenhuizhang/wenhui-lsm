/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SWITCH_TO_H
#define _ASM_X86_SWITCH_TO_H

#include <linux/sched/task_stack.h>

struct task_struct; 

struct task_struct *__switch_to_asm(struct task_struct *prev,
				    struct task_struct *next);

__visible struct task_struct *__switch_to(struct task_struct *prev,
					  struct task_struct *next);


static inline void prepare_switch_to(struct task_struct *next)
{
#ifdef CONFIG_VMAP_STACK
	
	READ_ONCE(*(unsigned char *)next->thread.sp);
#endif
}

asmlinkage void ret_from_fork(void);


struct inactive_task_frame {
#ifdef CONFIG_X86_64
	unsigned long r15;
	unsigned long r14;
	unsigned long r13;
	unsigned long r12;
#else
	unsigned long flags;
	unsigned long si;
	unsigned long di;
#endif
	unsigned long bx;

	
	unsigned long bp;
	unsigned long ret_addr;
};

struct fork_frame {
	struct inactive_task_frame frame;
	struct pt_regs regs;
};

#define switch_to(prev, next, last)					\
do {									\
	prepare_switch_to(next);					\
									\
	((last) = __switch_to_asm((prev), (next)));			\
} while (0)

#ifdef CONFIG_X86_32
static inline void refresh_sysenter_cs(struct thread_struct *thread)
{
	
	if (unlikely(this_cpu_read(cpu_tss_rw.x86_tss.ss1) == thread->sysenter_cs))
		return;

	this_cpu_write(cpu_tss_rw.x86_tss.ss1, thread->sysenter_cs);
	wrmsr(MSR_IA32_SYSENTER_CS, thread->sysenter_cs, 0);
}
#endif


static inline void update_task_stack(struct task_struct *task)
{
	
#ifdef CONFIG_X86_32
	if (static_cpu_has(X86_FEATURE_XENPV))
		load_sp0(task->thread.sp0);
	else
		this_cpu_write(cpu_tss_rw.x86_tss.sp1, task->thread.sp0);
#else
	
	if (static_cpu_has(X86_FEATURE_XENPV))
		load_sp0(task_top_of_stack(task));
#endif

}

#endif 
