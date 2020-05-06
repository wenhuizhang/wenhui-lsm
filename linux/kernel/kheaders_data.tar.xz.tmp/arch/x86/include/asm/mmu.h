/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_MMU_H
#define _ASM_X86_MMU_H

#include <linux/spinlock.h>
#include <linux/rwsem.h>
#include <linux/mutex.h>
#include <linux/atomic.h>


typedef struct {
	
	u64 ctx_id;

	
	atomic64_t tlb_gen;

#ifdef CONFIG_MODIFY_LDT_SYSCALL
	struct rw_semaphore	ldt_usr_sem;
	struct ldt_struct	*ldt;
#endif

#ifdef CONFIG_X86_64
	
	unsigned short ia32_compat;
#endif

	struct mutex lock;
	void __user *vdso;			
	const struct vdso_image *vdso_image;	

	atomic_t perf_rdpmc_allowed;	
#ifdef CONFIG_X86_INTEL_MEMORY_PROTECTION_KEYS
	
	u16 pkey_allocation_map;
	s16 execute_only_pkey;
#endif
#ifdef CONFIG_X86_INTEL_MPX
	
	void __user *bd_addr;
#endif
} mm_context_t;

#define INIT_MM_CONTEXT(mm)						\
	.context = {							\
		.ctx_id = 1,						\
		.lock = __MUTEX_INITIALIZER(mm.context.lock),		\
	}

void leave_mm(int cpu);

#endif 
