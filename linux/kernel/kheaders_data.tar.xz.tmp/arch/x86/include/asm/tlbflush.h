/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_TLBFLUSH_H
#define _ASM_X86_TLBFLUSH_H

#include <linux/mm.h>
#include <linux/sched.h>

#include <asm/processor.h>
#include <asm/cpufeature.h>
#include <asm/special_insns.h>
#include <asm/smp.h>
#include <asm/invpcid.h>
#include <asm/pti.h>
#include <asm/processor-flags.h>




#define CR3_HW_ASID_BITS		12


#ifdef CONFIG_PAGE_TABLE_ISOLATION
# define PTI_CONSUMED_PCID_BITS	1
#else
# define PTI_CONSUMED_PCID_BITS	0
#endif

#define CR3_AVAIL_PCID_BITS (X86_CR3_PCID_BITS - PTI_CONSUMED_PCID_BITS)


#define MAX_ASID_AVAILABLE ((1 << CR3_AVAIL_PCID_BITS) - 2)


#define TLB_NR_DYN_ASIDS	6


static inline u16 kern_pcid(u16 asid)
{
	VM_WARN_ON_ONCE(asid > MAX_ASID_AVAILABLE);

#ifdef CONFIG_PAGE_TABLE_ISOLATION
	
	BUILD_BUG_ON(TLB_NR_DYN_ASIDS >= (1 << X86_CR3_PTI_PCID_USER_BIT));

	
	VM_WARN_ON_ONCE(asid & (1 << X86_CR3_PTI_PCID_USER_BIT));
#endif
	
	return asid + 1;
}


static inline u16 user_pcid(u16 asid)
{
	u16 ret = kern_pcid(asid);
#ifdef CONFIG_PAGE_TABLE_ISOLATION
	ret |= 1 << X86_CR3_PTI_PCID_USER_BIT;
#endif
	return ret;
}

struct pgd_t;
static inline unsigned long build_cr3(pgd_t *pgd, u16 asid)
{
	if (static_cpu_has(X86_FEATURE_PCID)) {
		return __sme_pa(pgd) | kern_pcid(asid);
	} else {
		VM_WARN_ON_ONCE(asid != 0);
		return __sme_pa(pgd);
	}
}

static inline unsigned long build_cr3_noflush(pgd_t *pgd, u16 asid)
{
	VM_WARN_ON_ONCE(asid > MAX_ASID_AVAILABLE);
	
	VM_WARN_ON_ONCE(!boot_cpu_has(X86_FEATURE_PCID));
	return __sme_pa(pgd) | kern_pcid(asid) | CR3_NOFLUSH;
}

#ifdef CONFIG_PARAVIRT
#include <asm/paravirt.h>
#else
#define __flush_tlb() __native_flush_tlb()
#define __flush_tlb_global() __native_flush_tlb_global()
#define __flush_tlb_one_user(addr) __native_flush_tlb_one_user(addr)
#endif

struct tlb_context {
	u64 ctx_id;
	u64 tlb_gen;
};

struct tlb_state {
	
	struct mm_struct *loaded_mm;

#define LOADED_MM_SWITCHING ((struct mm_struct *)1UL)

	
	union {
		struct mm_struct	*last_user_mm;
		unsigned long		last_user_mm_ibpb;
	};

	u16 loaded_mm_asid;
	u16 next_asid;

	
	bool is_lazy;

	
	bool invalidate_other;

	
	unsigned short user_pcid_flush_mask;

	
	unsigned long cr4;

	
	struct tlb_context ctxs[TLB_NR_DYN_ASIDS];
};
DECLARE_PER_CPU_SHARED_ALIGNED(struct tlb_state, cpu_tlbstate);


static inline bool nmi_uaccess_okay(void)
{
	struct mm_struct *loaded_mm = this_cpu_read(cpu_tlbstate.loaded_mm);
	struct mm_struct *current_mm = current->mm;

	VM_WARN_ON_ONCE(!loaded_mm);

	
	if (loaded_mm != current_mm)
		return false;

	VM_WARN_ON_ONCE(current_mm->pgd != __va(read_cr3_pa()));

	return true;
}

#define nmi_uaccess_okay nmi_uaccess_okay


static inline void cr4_init_shadow(void)
{
	this_cpu_write(cpu_tlbstate.cr4, __read_cr4());
}

static inline void __cr4_set(unsigned long cr4)
{
	lockdep_assert_irqs_disabled();
	this_cpu_write(cpu_tlbstate.cr4, cr4);
	__write_cr4(cr4);
}


static inline void cr4_set_bits(unsigned long mask)
{
	unsigned long cr4, flags;

	local_irq_save(flags);
	cr4 = this_cpu_read(cpu_tlbstate.cr4);
	if ((cr4 | mask) != cr4)
		__cr4_set(cr4 | mask);
	local_irq_restore(flags);
}


static inline void cr4_clear_bits(unsigned long mask)
{
	unsigned long cr4, flags;

	local_irq_save(flags);
	cr4 = this_cpu_read(cpu_tlbstate.cr4);
	if ((cr4 & ~mask) != cr4)
		__cr4_set(cr4 & ~mask);
	local_irq_restore(flags);
}

static inline void cr4_toggle_bits_irqsoff(unsigned long mask)
{
	unsigned long cr4;

	cr4 = this_cpu_read(cpu_tlbstate.cr4);
	__cr4_set(cr4 ^ mask);
}


static inline unsigned long cr4_read_shadow(void)
{
	return this_cpu_read(cpu_tlbstate.cr4);
}


static inline void invalidate_other_asid(void)
{
	this_cpu_write(cpu_tlbstate.invalidate_other, true);
}


extern unsigned long mmu_cr4_features;
extern u32 *trampoline_cr4_features;

static inline void cr4_set_bits_and_update_boot(unsigned long mask)
{
	mmu_cr4_features |= mask;
	if (trampoline_cr4_features)
		*trampoline_cr4_features = mmu_cr4_features;
	cr4_set_bits(mask);
}

extern void initialize_tlbstate_and_flush(void);


static inline void invalidate_user_asid(u16 asid)
{
	
	if (!IS_ENABLED(CONFIG_PAGE_TABLE_ISOLATION))
		return;

	
	if (!cpu_feature_enabled(X86_FEATURE_PCID))
		return;

	if (!static_cpu_has(X86_FEATURE_PTI))
		return;

	__set_bit(kern_pcid(asid),
		  (unsigned long *)this_cpu_ptr(&cpu_tlbstate.user_pcid_flush_mask));
}


static inline void __native_flush_tlb(void)
{
	
	WARN_ON_ONCE(preemptible());

	invalidate_user_asid(this_cpu_read(cpu_tlbstate.loaded_mm_asid));

	
	native_write_cr3(__native_read_cr3());
}


static inline void __native_flush_tlb_global(void)
{
	unsigned long cr4, flags;

	if (static_cpu_has(X86_FEATURE_INVPCID)) {
		
		invpcid_flush_all();
		return;
	}

	
	raw_local_irq_save(flags);

	cr4 = this_cpu_read(cpu_tlbstate.cr4);
	
	native_write_cr4(cr4 ^ X86_CR4_PGE);
	
	native_write_cr4(cr4);

	raw_local_irq_restore(flags);
}


static inline void __native_flush_tlb_one_user(unsigned long addr)
{
	u32 loaded_mm_asid = this_cpu_read(cpu_tlbstate.loaded_mm_asid);

	asm volatile("invlpg (%0)" ::"r" (addr) : "memory");

	if (!static_cpu_has(X86_FEATURE_PTI))
		return;

	
	if (!this_cpu_has(X86_FEATURE_INVPCID_SINGLE))
		invalidate_user_asid(loaded_mm_asid);
	else
		invpcid_flush_one(user_pcid(loaded_mm_asid), addr);
}


static inline void __flush_tlb_all(void)
{
	
	VM_WARN_ON_ONCE(preemptible());

	if (boot_cpu_has(X86_FEATURE_PGE)) {
		__flush_tlb_global();
	} else {
		
		__flush_tlb();
	}
}


static inline void __flush_tlb_one_kernel(unsigned long addr)
{
	count_vm_tlb_event(NR_TLB_LOCAL_FLUSH_ONE);

	
	__flush_tlb_one_user(addr);

	if (!static_cpu_has(X86_FEATURE_PTI))
		return;

	
	invalidate_other_asid();
}

#define TLB_FLUSH_ALL	-1UL


struct flush_tlb_info {
	
	struct mm_struct	*mm;
	unsigned long		start;
	unsigned long		end;
	u64			new_tlb_gen;
	unsigned int		stride_shift;
	bool			freed_tables;
};

#define local_flush_tlb() __flush_tlb()

#define flush_tlb_mm(mm)						\
		flush_tlb_mm_range(mm, 0UL, TLB_FLUSH_ALL, 0UL, true)

#define flush_tlb_range(vma, start, end)				\
	flush_tlb_mm_range((vma)->vm_mm, start, end,			\
			   ((vma)->vm_flags & VM_HUGETLB)		\
				? huge_page_shift(hstate_vma(vma))	\
				: PAGE_SHIFT, false)

extern void flush_tlb_all(void);
extern void flush_tlb_mm_range(struct mm_struct *mm, unsigned long start,
				unsigned long end, unsigned int stride_shift,
				bool freed_tables);
extern void flush_tlb_kernel_range(unsigned long start, unsigned long end);

static inline void flush_tlb_page(struct vm_area_struct *vma, unsigned long a)
{
	flush_tlb_mm_range(vma->vm_mm, a, a + PAGE_SIZE, PAGE_SHIFT, false);
}

void native_flush_tlb_others(const struct cpumask *cpumask,
			     const struct flush_tlb_info *info);

static inline u64 inc_mm_tlb_gen(struct mm_struct *mm)
{
	
	return atomic64_inc_return(&mm->context.tlb_gen);
}

static inline void arch_tlbbatch_add_mm(struct arch_tlbflush_unmap_batch *batch,
					struct mm_struct *mm)
{
	inc_mm_tlb_gen(mm);
	cpumask_or(&batch->cpumask, &batch->cpumask, mm_cpumask(mm));
}

extern void arch_tlbbatch_flush(struct arch_tlbflush_unmap_batch *batch);

#ifndef CONFIG_PARAVIRT
#define flush_tlb_others(mask, info)	\
	native_flush_tlb_others(mask, info)

#define paravirt_tlb_remove_table(tlb, page) \
	tlb_remove_page(tlb, (void *)(page))
#endif

#endif 
