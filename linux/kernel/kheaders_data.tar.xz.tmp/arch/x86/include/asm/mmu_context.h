/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_MMU_CONTEXT_H
#define _ASM_X86_MMU_CONTEXT_H

#include <asm/desc.h>
#include <linux/atomic.h>
#include <linux/mm_types.h>
#include <linux/pkeys.h>

#include <trace/events/tlb.h>

#include <asm/pgalloc.h>
#include <asm/tlbflush.h>
#include <asm/paravirt.h>
#include <asm/mpx.h>
#include <asm/debugreg.h>

extern atomic64_t last_mm_ctx_id;

#ifndef CONFIG_PARAVIRT_XXL
static inline void paravirt_activate_mm(struct mm_struct *prev,
					struct mm_struct *next)
{
}
#endif	

#ifdef CONFIG_PERF_EVENTS

DECLARE_STATIC_KEY_FALSE(rdpmc_always_available_key);

static inline void load_mm_cr4(struct mm_struct *mm)
{
	if (static_branch_unlikely(&rdpmc_always_available_key) ||
	    atomic_read(&mm->context.perf_rdpmc_allowed))
		cr4_set_bits(X86_CR4_PCE);
	else
		cr4_clear_bits(X86_CR4_PCE);
}
#else
static inline void load_mm_cr4(struct mm_struct *mm) {}
#endif

#ifdef CONFIG_MODIFY_LDT_SYSCALL

struct ldt_struct {
	
	struct desc_struct	*entries;
	unsigned int		nr_entries;

	
	int			slot;
};


#define LDT_SLOT_STRIDE (LDT_ENTRIES * LDT_ENTRY_SIZE)

static inline void *ldt_slot_va(int slot)
{
	return (void *)(LDT_BASE_ADDR + LDT_SLOT_STRIDE * slot);
}


static inline void init_new_context_ldt(struct mm_struct *mm)
{
	mm->context.ldt = NULL;
	init_rwsem(&mm->context.ldt_usr_sem);
}
int ldt_dup_context(struct mm_struct *oldmm, struct mm_struct *mm);
void destroy_context_ldt(struct mm_struct *mm);
void ldt_arch_exit_mmap(struct mm_struct *mm);
#else	
static inline void init_new_context_ldt(struct mm_struct *mm) { }
static inline int ldt_dup_context(struct mm_struct *oldmm,
				  struct mm_struct *mm)
{
	return 0;
}
static inline void destroy_context_ldt(struct mm_struct *mm) { }
static inline void ldt_arch_exit_mmap(struct mm_struct *mm) { }
#endif

static inline void load_mm_ldt(struct mm_struct *mm)
{
#ifdef CONFIG_MODIFY_LDT_SYSCALL
	struct ldt_struct *ldt;

	
	ldt = READ_ONCE(mm->context.ldt);

	

	if (unlikely(ldt)) {
		if (static_cpu_has(X86_FEATURE_PTI)) {
			if (WARN_ON_ONCE((unsigned long)ldt->slot > 1)) {
				
				clear_LDT();
				return;
			}

			
			set_ldt(ldt_slot_va(ldt->slot), ldt->nr_entries);
		} else {
			set_ldt(ldt->entries, ldt->nr_entries);
		}
	} else {
		clear_LDT();
	}
#else
	clear_LDT();
#endif
}

static inline void switch_ldt(struct mm_struct *prev, struct mm_struct *next)
{
#ifdef CONFIG_MODIFY_LDT_SYSCALL
	
	if (unlikely((unsigned long)prev->context.ldt |
		     (unsigned long)next->context.ldt))
		load_mm_ldt(next);
#endif

	DEBUG_LOCKS_WARN_ON(preemptible());
}

void enter_lazy_tlb(struct mm_struct *mm, struct task_struct *tsk);


static inline int init_new_context(struct task_struct *tsk,
				   struct mm_struct *mm)
{
	mutex_init(&mm->context.lock);

	mm->context.ctx_id = atomic64_inc_return(&last_mm_ctx_id);
	atomic64_set(&mm->context.tlb_gen, 0);

#ifdef CONFIG_X86_INTEL_MEMORY_PROTECTION_KEYS
	if (cpu_feature_enabled(X86_FEATURE_OSPKE)) {
		
		mm->context.pkey_allocation_map = 0x1;
		
		mm->context.execute_only_pkey = -1;
	}
#endif
	init_new_context_ldt(mm);
	return 0;
}
static inline void destroy_context(struct mm_struct *mm)
{
	destroy_context_ldt(mm);
}

extern void switch_mm(struct mm_struct *prev, struct mm_struct *next,
		      struct task_struct *tsk);

extern void switch_mm_irqs_off(struct mm_struct *prev, struct mm_struct *next,
			       struct task_struct *tsk);
#define switch_mm_irqs_off switch_mm_irqs_off

#define activate_mm(prev, next)			\
do {						\
	paravirt_activate_mm((prev), (next));	\
	switch_mm((prev), (next), NULL);	\
} while (0);

#ifdef CONFIG_X86_32
#define deactivate_mm(tsk, mm)			\
do {						\
	lazy_load_gs(0);			\
} while (0)
#else
#define deactivate_mm(tsk, mm)			\
do {						\
	load_gs_index(0);			\
	loadsegment(fs, 0);			\
} while (0)
#endif

static inline void arch_dup_pkeys(struct mm_struct *oldmm,
				  struct mm_struct *mm)
{
#ifdef CONFIG_X86_INTEL_MEMORY_PROTECTION_KEYS
	if (!cpu_feature_enabled(X86_FEATURE_OSPKE))
		return;

	
	mm->context.pkey_allocation_map = oldmm->context.pkey_allocation_map;
	mm->context.execute_only_pkey   = oldmm->context.execute_only_pkey;
#endif
}

static inline int arch_dup_mmap(struct mm_struct *oldmm, struct mm_struct *mm)
{
	arch_dup_pkeys(oldmm, mm);
	paravirt_arch_dup_mmap(oldmm, mm);
	return ldt_dup_context(oldmm, mm);
}

static inline void arch_exit_mmap(struct mm_struct *mm)
{
	paravirt_arch_exit_mmap(mm);
	ldt_arch_exit_mmap(mm);
}

#ifdef CONFIG_X86_64
static inline bool is_64bit_mm(struct mm_struct *mm)
{
	return	!IS_ENABLED(CONFIG_IA32_EMULATION) ||
		!(mm->context.ia32_compat == TIF_IA32);
}
#else
static inline bool is_64bit_mm(struct mm_struct *mm)
{
	return false;
}
#endif

static inline void arch_bprm_mm_init(struct mm_struct *mm,
		struct vm_area_struct *vma)
{
	mpx_mm_init(mm);
}

static inline void arch_unmap(struct mm_struct *mm, unsigned long start,
			      unsigned long end)
{
	
	if (unlikely(cpu_feature_enabled(X86_FEATURE_MPX)))
		mpx_notify_unmap(mm, start, end);
}


static inline bool vma_is_foreign(struct vm_area_struct *vma)
{
	if (!current->mm)
		return true;
	
	if (current->mm != vma->vm_mm)
		return true;

	return false;
}

static inline bool arch_vma_access_permitted(struct vm_area_struct *vma,
		bool write, bool execute, bool foreign)
{
	
	if (execute)
		return true;
	
	if (foreign || vma_is_foreign(vma))
		return true;
	return __pkru_allows_pkey(vma_pkey(vma), write);
}


static inline unsigned long __get_current_cr3_fast(void)
{
	unsigned long cr3 = build_cr3(this_cpu_read(cpu_tlbstate.loaded_mm)->pgd,
		this_cpu_read(cpu_tlbstate.loaded_mm_asid));

	
	VM_WARN_ON(in_nmi() || preemptible());

	VM_BUG_ON(cr3 != __read_cr3());
	return cr3;
}

typedef struct {
	struct mm_struct *mm;
} temp_mm_state_t;


static inline temp_mm_state_t use_temporary_mm(struct mm_struct *mm)
{
	temp_mm_state_t temp_state;

	lockdep_assert_irqs_disabled();
	temp_state.mm = this_cpu_read(cpu_tlbstate.loaded_mm);
	switch_mm_irqs_off(NULL, mm, current);

	
	if (hw_breakpoint_active())
		hw_breakpoint_disable();

	return temp_state;
}

static inline void unuse_temporary_mm(temp_mm_state_t prev_state)
{
	lockdep_assert_irqs_disabled();
	switch_mm_irqs_off(NULL, prev_state.mm, current);

	
	if (hw_breakpoint_active())
		hw_breakpoint_restore();
}

#endif 
