/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_KVM_PARA_H
#define _ASM_X86_KVM_PARA_H

#include <asm/processor.h>
#include <asm/alternative.h>
#include <uapi/asm/kvm_para.h>

extern void kvmclock_init(void);

#ifdef CONFIG_KVM_GUEST
bool kvm_check_and_clear_guest_paused(void);
#else
static inline bool kvm_check_and_clear_guest_paused(void)
{
	return false;
}
#endif 

#define KVM_HYPERCALL \
        ALTERNATIVE(".byte 0x0f,0x01,0xc1", ".byte 0x0f,0x01,0xd9", X86_FEATURE_VMMCALL)



static inline long kvm_hypercall0(unsigned int nr)
{
	long ret;
	asm volatile(KVM_HYPERCALL
		     : "=a"(ret)
		     : "a"(nr)
		     : "memory");
	return ret;
}

static inline long kvm_hypercall1(unsigned int nr, unsigned long p1)
{
	long ret;
	asm volatile(KVM_HYPERCALL
		     : "=a"(ret)
		     : "a"(nr), "b"(p1)
		     : "memory");
	return ret;
}

static inline long kvm_hypercall2(unsigned int nr, unsigned long p1,
				  unsigned long p2)
{
	long ret;
	asm volatile(KVM_HYPERCALL
		     : "=a"(ret)
		     : "a"(nr), "b"(p1), "c"(p2)
		     : "memory");
	return ret;
}

static inline long kvm_hypercall3(unsigned int nr, unsigned long p1,
				  unsigned long p2, unsigned long p3)
{
	long ret;
	asm volatile(KVM_HYPERCALL
		     : "=a"(ret)
		     : "a"(nr), "b"(p1), "c"(p2), "d"(p3)
		     : "memory");
	return ret;
}

static inline long kvm_hypercall4(unsigned int nr, unsigned long p1,
				  unsigned long p2, unsigned long p3,
				  unsigned long p4)
{
	long ret;
	asm volatile(KVM_HYPERCALL
		     : "=a"(ret)
		     : "a"(nr), "b"(p1), "c"(p2), "d"(p3), "S"(p4)
		     : "memory");
	return ret;
}

#ifdef CONFIG_KVM_GUEST
bool kvm_para_available(void);
unsigned int kvm_arch_para_features(void);
unsigned int kvm_arch_para_hints(void);
void kvm_async_pf_task_wait(u32 token, int interrupt_kernel);
void kvm_async_pf_task_wake(u32 token);
u32 kvm_read_and_reset_pf_reason(void);
extern void kvm_disable_steal_time(void);
void do_async_page_fault(struct pt_regs *regs, unsigned long error_code, unsigned long address);

#ifdef CONFIG_PARAVIRT_SPINLOCKS
void __init kvm_spinlock_init(void);
#else 
static inline void kvm_spinlock_init(void)
{
}
#endif 

#else 
#define kvm_async_pf_task_wait(T, I) do {} while(0)
#define kvm_async_pf_task_wake(T) do {} while(0)

static inline bool kvm_para_available(void)
{
	return false;
}

static inline unsigned int kvm_arch_para_features(void)
{
	return 0;
}

static inline unsigned int kvm_arch_para_hints(void)
{
	return 0;
}

static inline u32 kvm_read_and_reset_pf_reason(void)
{
	return 0;
}

static inline void kvm_disable_steal_time(void)
{
	return;
}
#endif

#endif 
