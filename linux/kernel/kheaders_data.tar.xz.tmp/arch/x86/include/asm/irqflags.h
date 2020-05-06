/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _X86_IRQFLAGS_H_
#define _X86_IRQFLAGS_H_

#include <asm/processor-flags.h>

#ifndef __ASSEMBLY__

#include <asm/nospec-branch.h>


#define __cpuidle __attribute__((__section__(".cpuidle.text")))




extern inline unsigned long native_save_fl(void);
extern inline unsigned long native_save_fl(void)
{
	unsigned long flags;

	
	asm volatile("# __raw_save_flags\n\t"
		     "pushf ; pop %0"
		     : "=rm" (flags)
		     : 
		     : "memory");

	return flags;
}

extern inline void native_restore_fl(unsigned long flags);
extern inline void native_restore_fl(unsigned long flags)
{
	asm volatile("push %0 ; popf"
		     : 
		     :"g" (flags)
		     :"memory", "cc");
}

static inline void native_irq_disable(void)
{
	asm volatile("cli": : :"memory");
}

static inline void native_irq_enable(void)
{
	asm volatile("sti": : :"memory");
}

static inline __cpuidle void native_safe_halt(void)
{
	mds_idle_clear_cpu_buffers();
	asm volatile("sti; hlt": : :"memory");
}

static inline __cpuidle void native_halt(void)
{
	mds_idle_clear_cpu_buffers();
	asm volatile("hlt": : :"memory");
}

#endif

#ifdef CONFIG_PARAVIRT_XXL
#include <asm/paravirt.h>
#else
#ifndef __ASSEMBLY__
#include <linux/types.h>

static inline notrace unsigned long arch_local_save_flags(void)
{
	return native_save_fl();
}

static inline notrace void arch_local_irq_restore(unsigned long flags)
{
	native_restore_fl(flags);
}

static inline notrace void arch_local_irq_disable(void)
{
	native_irq_disable();
}

static inline notrace void arch_local_irq_enable(void)
{
	native_irq_enable();
}


static inline __cpuidle void arch_safe_halt(void)
{
	native_safe_halt();
}


static inline __cpuidle void halt(void)
{
	native_halt();
}


static inline notrace unsigned long arch_local_irq_save(void)
{
	unsigned long flags = arch_local_save_flags();
	arch_local_irq_disable();
	return flags;
}
#else

#define ENABLE_INTERRUPTS(x)	sti
#define DISABLE_INTERRUPTS(x)	cli

#ifdef CONFIG_X86_64
#ifdef CONFIG_DEBUG_ENTRY
#define SAVE_FLAGS(x)		pushfq; popq %rax
#endif

#define SWAPGS	swapgs

#define SWAPGS_UNSAFE_STACK	swapgs

#define INTERRUPT_RETURN	jmp native_iret
#define USERGS_SYSRET64				\
	swapgs;					\
	sysretq;
#define USERGS_SYSRET32				\
	swapgs;					\
	sysretl

#else
#define INTERRUPT_RETURN		iret
#endif

#endif 
#endif 

#ifndef __ASSEMBLY__
static inline int arch_irqs_disabled_flags(unsigned long flags)
{
	return !(flags & X86_EFLAGS_IF);
}

static inline int arch_irqs_disabled(void)
{
	unsigned long flags = arch_local_save_flags();

	return arch_irqs_disabled_flags(flags);
}
#endif 

#ifdef __ASSEMBLY__
#ifdef CONFIG_TRACE_IRQFLAGS
#  define TRACE_IRQS_ON		call trace_hardirqs_on_thunk;
#  define TRACE_IRQS_OFF	call trace_hardirqs_off_thunk;
#else
#  define TRACE_IRQS_ON
#  define TRACE_IRQS_OFF
#endif
#ifdef CONFIG_DEBUG_LOCK_ALLOC
#  ifdef CONFIG_X86_64
#    define LOCKDEP_SYS_EXIT		call lockdep_sys_exit_thunk
#    define LOCKDEP_SYS_EXIT_IRQ \
	TRACE_IRQS_ON; \
	sti; \
	call lockdep_sys_exit_thunk; \
	cli; \
	TRACE_IRQS_OFF;
#  else
#    define LOCKDEP_SYS_EXIT \
	pushl %eax;				\
	pushl %ecx;				\
	pushl %edx;				\
	call lockdep_sys_exit;			\
	popl %edx;				\
	popl %ecx;				\
	popl %eax;
#    define LOCKDEP_SYS_EXIT_IRQ
#  endif
#else
#  define LOCKDEP_SYS_EXIT
#  define LOCKDEP_SYS_EXIT_IRQ
#endif
#endif 

#endif
