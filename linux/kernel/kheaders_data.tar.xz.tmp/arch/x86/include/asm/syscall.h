/* SPDX-License-Identifier: GPL-2.0-only */


#ifndef _ASM_X86_SYSCALL_H
#define _ASM_X86_SYSCALL_H

#include <uapi/linux/audit.h>
#include <linux/sched.h>
#include <linux/err.h>
#include <asm/asm-offsets.h>	
#include <asm/thread_info.h>	
#include <asm/unistd.h>

#ifdef CONFIG_X86_64
typedef asmlinkage long (*sys_call_ptr_t)(const struct pt_regs *);
#else
typedef asmlinkage long (*sys_call_ptr_t)(unsigned long, unsigned long,
					  unsigned long, unsigned long,
					  unsigned long, unsigned long);
#endif 
extern const sys_call_ptr_t sys_call_table[];

#if defined(CONFIG_X86_32)
#define ia32_sys_call_table sys_call_table
#define __NR_syscall_compat_max __NR_syscall_max
#define IA32_NR_syscalls NR_syscalls
#endif

#if defined(CONFIG_IA32_EMULATION)
extern const sys_call_ptr_t ia32_sys_call_table[];
#endif


static inline int syscall_get_nr(struct task_struct *task, struct pt_regs *regs)
{
	return regs->orig_ax;
}

static inline void syscall_rollback(struct task_struct *task,
				    struct pt_regs *regs)
{
	regs->ax = regs->orig_ax;
}

static inline long syscall_get_error(struct task_struct *task,
				     struct pt_regs *regs)
{
	unsigned long error = regs->ax;
#ifdef CONFIG_IA32_EMULATION
	
	if (task->thread_info.status & (TS_COMPAT|TS_I386_REGS_POKED))
		
		error = (long) (int) error;
#endif
	return IS_ERR_VALUE(error) ? error : 0;
}

static inline long syscall_get_return_value(struct task_struct *task,
					    struct pt_regs *regs)
{
	return regs->ax;
}

static inline void syscall_set_return_value(struct task_struct *task,
					    struct pt_regs *regs,
					    int error, long val)
{
	regs->ax = (long) error ?: val;
}

#ifdef CONFIG_X86_32

static inline void syscall_get_arguments(struct task_struct *task,
					 struct pt_regs *regs,
					 unsigned long *args)
{
	memcpy(args, &regs->bx, 6 * sizeof(args[0]));
}

static inline void syscall_set_arguments(struct task_struct *task,
					 struct pt_regs *regs,
					 unsigned int i, unsigned int n,
					 const unsigned long *args)
{
	BUG_ON(i + n > 6);
	memcpy(&regs->bx + i, args, n * sizeof(args[0]));
}

static inline int syscall_get_arch(struct task_struct *task)
{
	return AUDIT_ARCH_I386;
}

#else	 

static inline void syscall_get_arguments(struct task_struct *task,
					 struct pt_regs *regs,
					 unsigned long *args)
{
# ifdef CONFIG_IA32_EMULATION
	if (task->thread_info.status & TS_COMPAT) {
		*args++ = regs->bx;
		*args++ = regs->cx;
		*args++ = regs->dx;
		*args++ = regs->si;
		*args++ = regs->di;
		*args   = regs->bp;
	} else
# endif
	{
		*args++ = regs->di;
		*args++ = regs->si;
		*args++ = regs->dx;
		*args++ = regs->r10;
		*args++ = regs->r8;
		*args   = regs->r9;
	}
}

static inline void syscall_set_arguments(struct task_struct *task,
					 struct pt_regs *regs,
					 const unsigned long *args)
{
# ifdef CONFIG_IA32_EMULATION
	if (task->thread_info.status & TS_COMPAT) {
		regs->bx = *args++;
		regs->cx = *args++;
		regs->dx = *args++;
		regs->si = *args++;
		regs->di = *args++;
		regs->bp = *args;
	} else
# endif
	{
		regs->di = *args++;
		regs->si = *args++;
		regs->dx = *args++;
		regs->r10 = *args++;
		regs->r8 = *args++;
		regs->r9 = *args;
	}
}

static inline int syscall_get_arch(struct task_struct *task)
{
	
	return (IS_ENABLED(CONFIG_IA32_EMULATION) &&
		task->thread_info.status & TS_COMPAT)
		? AUDIT_ARCH_I386 : AUDIT_ARCH_X86_64;
}
#endif	

#endif	
