/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef _ASM_GENERIC__TLB_H
#define _ASM_GENERIC__TLB_H

#include <linux/mmu_notifier.h>
#include <linux/swap.h>
#include <asm/pgalloc.h>
#include <asm/tlbflush.h>
#include <asm/cacheflush.h>


#ifndef nmi_uaccess_okay
# define nmi_uaccess_okay() true
#endif

#ifdef CONFIG_MMU



#ifdef CONFIG_HAVE_RCU_TABLE_FREE

struct mmu_table_batch {
	struct rcu_head		rcu;
	unsigned int		nr;
	void			*tables[0];
};

#define MAX_TABLE_BATCH		\
	((PAGE_SIZE - sizeof(struct mmu_table_batch)) / sizeof(void *))

extern void tlb_remove_table(struct mmu_gather *tlb, void *table);

#endif

#ifndef CONFIG_HAVE_MMU_GATHER_NO_GATHER

#define MMU_GATHER_BUNDLE	8

struct mmu_gather_batch {
	struct mmu_gather_batch	*next;
	unsigned int		nr;
	unsigned int		max;
	struct page		*pages[0];
};

#define MAX_GATHER_BATCH	\
	((PAGE_SIZE - sizeof(struct mmu_gather_batch)) / sizeof(void *))


#define MAX_GATHER_BATCH_COUNT	(10000UL/MAX_GATHER_BATCH)

extern bool __tlb_remove_page_size(struct mmu_gather *tlb, struct page *page,
				   int page_size);
#endif


struct mmu_gather {
	struct mm_struct	*mm;

#ifdef CONFIG_HAVE_RCU_TABLE_FREE
	struct mmu_table_batch	*batch;
#endif

	unsigned long		start;
	unsigned long		end;
	
	unsigned int		fullmm : 1;

	
	unsigned int		need_flush_all : 1;

	
	unsigned int		freed_tables : 1;

	
	unsigned int		cleared_ptes : 1;
	unsigned int		cleared_pmds : 1;
	unsigned int		cleared_puds : 1;
	unsigned int		cleared_p4ds : 1;

	
	unsigned int		vma_exec : 1;
	unsigned int		vma_huge : 1;

	unsigned int		batch_count;

#ifndef CONFIG_HAVE_MMU_GATHER_NO_GATHER
	struct mmu_gather_batch *active;
	struct mmu_gather_batch	local;
	struct page		*__pages[MMU_GATHER_BUNDLE];

#ifdef CONFIG_HAVE_MMU_GATHER_PAGE_SIZE
	unsigned int page_size;
#endif
#endif
};

void arch_tlb_gather_mmu(struct mmu_gather *tlb,
	struct mm_struct *mm, unsigned long start, unsigned long end);
void tlb_flush_mmu(struct mmu_gather *tlb);
void arch_tlb_finish_mmu(struct mmu_gather *tlb,
			 unsigned long start, unsigned long end, bool force);

static inline void __tlb_adjust_range(struct mmu_gather *tlb,
				      unsigned long address,
				      unsigned int range_size)
{
	tlb->start = min(tlb->start, address);
	tlb->end = max(tlb->end, address + range_size);
}

static inline void __tlb_reset_range(struct mmu_gather *tlb)
{
	if (tlb->fullmm) {
		tlb->start = tlb->end = ~0;
	} else {
		tlb->start = TASK_SIZE;
		tlb->end = 0;
	}
	tlb->freed_tables = 0;
	tlb->cleared_ptes = 0;
	tlb->cleared_pmds = 0;
	tlb->cleared_puds = 0;
	tlb->cleared_p4ds = 0;
	
}

#ifdef CONFIG_MMU_GATHER_NO_RANGE

#if defined(tlb_flush) || defined(tlb_start_vma) || defined(tlb_end_vma)
#error MMU_GATHER_NO_RANGE relies on default tlb_flush(), tlb_start_vma() and tlb_end_vma()
#endif


static inline void tlb_flush(struct mmu_gather *tlb)
{
	if (tlb->end)
		flush_tlb_mm(tlb->mm);
}

static inline void
tlb_update_vma_flags(struct mmu_gather *tlb, struct vm_area_struct *vma) { }

#define tlb_end_vma tlb_end_vma
static inline void tlb_end_vma(struct mmu_gather *tlb, struct vm_area_struct *vma) { }

#else 

#ifndef tlb_flush

#if defined(tlb_start_vma) || defined(tlb_end_vma)
#error Default tlb_flush() relies on default tlb_start_vma() and tlb_end_vma()
#endif


static inline void tlb_flush(struct mmu_gather *tlb)
{
	if (tlb->fullmm || tlb->need_flush_all) {
		flush_tlb_mm(tlb->mm);
	} else if (tlb->end) {
		struct vm_area_struct vma = {
			.vm_mm = tlb->mm,
			.vm_flags = (tlb->vma_exec ? VM_EXEC    : 0) |
				    (tlb->vma_huge ? VM_HUGETLB : 0),
		};

		flush_tlb_range(&vma, tlb->start, tlb->end);
	}
}

static inline void
tlb_update_vma_flags(struct mmu_gather *tlb, struct vm_area_struct *vma)
{
	
	tlb->vma_huge = !!(vma->vm_flags & VM_HUGETLB);
	tlb->vma_exec = !!(vma->vm_flags & VM_EXEC);
}

#else

static inline void
tlb_update_vma_flags(struct mmu_gather *tlb, struct vm_area_struct *vma) { }

#endif

#endif 

static inline void tlb_flush_mmu_tlbonly(struct mmu_gather *tlb)
{
	if (!tlb->end)
		return;

	tlb_flush(tlb);
	mmu_notifier_invalidate_range(tlb->mm, tlb->start, tlb->end);
	__tlb_reset_range(tlb);
}

static inline void tlb_remove_page_size(struct mmu_gather *tlb,
					struct page *page, int page_size)
{
	if (__tlb_remove_page_size(tlb, page, page_size))
		tlb_flush_mmu(tlb);
}

static inline bool __tlb_remove_page(struct mmu_gather *tlb, struct page *page)
{
	return __tlb_remove_page_size(tlb, page, PAGE_SIZE);
}


static inline void tlb_remove_page(struct mmu_gather *tlb, struct page *page)
{
	return tlb_remove_page_size(tlb, page, PAGE_SIZE);
}

static inline void tlb_change_page_size(struct mmu_gather *tlb,
						     unsigned int page_size)
{
#ifdef CONFIG_HAVE_MMU_GATHER_PAGE_SIZE
	if (tlb->page_size && tlb->page_size != page_size) {
		if (!tlb->fullmm)
			tlb_flush_mmu(tlb);
	}

	tlb->page_size = page_size;
#endif
}

static inline unsigned long tlb_get_unmap_shift(struct mmu_gather *tlb)
{
	if (tlb->cleared_ptes)
		return PAGE_SHIFT;
	if (tlb->cleared_pmds)
		return PMD_SHIFT;
	if (tlb->cleared_puds)
		return PUD_SHIFT;
	if (tlb->cleared_p4ds)
		return P4D_SHIFT;

	return PAGE_SHIFT;
}

static inline unsigned long tlb_get_unmap_size(struct mmu_gather *tlb)
{
	return 1UL << tlb_get_unmap_shift(tlb);
}


#ifndef tlb_start_vma
static inline void tlb_start_vma(struct mmu_gather *tlb, struct vm_area_struct *vma)
{
	if (tlb->fullmm)
		return;

	tlb_update_vma_flags(tlb, vma);
	flush_cache_range(vma, vma->vm_start, vma->vm_end);
}
#endif

#ifndef tlb_end_vma
static inline void tlb_end_vma(struct mmu_gather *tlb, struct vm_area_struct *vma)
{
	if (tlb->fullmm)
		return;

	
	tlb_flush_mmu_tlbonly(tlb);
}
#endif

#ifndef __tlb_remove_tlb_entry
#define __tlb_remove_tlb_entry(tlb, ptep, address) do { } while (0)
#endif


#define tlb_remove_tlb_entry(tlb, ptep, address)		\
	do {							\
		__tlb_adjust_range(tlb, address, PAGE_SIZE);	\
		tlb->cleared_ptes = 1;				\
		__tlb_remove_tlb_entry(tlb, ptep, address);	\
	} while (0)

#define tlb_remove_huge_tlb_entry(h, tlb, ptep, address)	\
	do {							\
		unsigned long _sz = huge_page_size(h);		\
		__tlb_adjust_range(tlb, address, _sz);		\
		if (_sz == PMD_SIZE)				\
			tlb->cleared_pmds = 1;			\
		else if (_sz == PUD_SIZE)			\
			tlb->cleared_puds = 1;			\
		__tlb_remove_tlb_entry(tlb, ptep, address);	\
	} while (0)


#ifndef __tlb_remove_pmd_tlb_entry
#define __tlb_remove_pmd_tlb_entry(tlb, pmdp, address) do {} while (0)
#endif

#define tlb_remove_pmd_tlb_entry(tlb, pmdp, address)			\
	do {								\
		__tlb_adjust_range(tlb, address, HPAGE_PMD_SIZE);	\
		tlb->cleared_pmds = 1;					\
		__tlb_remove_pmd_tlb_entry(tlb, pmdp, address);		\
	} while (0)


#ifndef __tlb_remove_pud_tlb_entry
#define __tlb_remove_pud_tlb_entry(tlb, pudp, address) do {} while (0)
#endif

#define tlb_remove_pud_tlb_entry(tlb, pudp, address)			\
	do {								\
		__tlb_adjust_range(tlb, address, HPAGE_PUD_SIZE);	\
		tlb->cleared_puds = 1;					\
		__tlb_remove_pud_tlb_entry(tlb, pudp, address);		\
	} while (0)



#ifndef pte_free_tlb
#define pte_free_tlb(tlb, ptep, address)			\
	do {							\
		__tlb_adjust_range(tlb, address, PAGE_SIZE);	\
		tlb->freed_tables = 1;				\
		tlb->cleared_pmds = 1;				\
		__pte_free_tlb(tlb, ptep, address);		\
	} while (0)
#endif

#ifndef pmd_free_tlb
#define pmd_free_tlb(tlb, pmdp, address)			\
	do {							\
		__tlb_adjust_range(tlb, address, PAGE_SIZE);	\
		tlb->freed_tables = 1;				\
		tlb->cleared_puds = 1;				\
		__pmd_free_tlb(tlb, pmdp, address);		\
	} while (0)
#endif

#ifndef __ARCH_HAS_4LEVEL_HACK
#ifndef pud_free_tlb
#define pud_free_tlb(tlb, pudp, address)			\
	do {							\
		__tlb_adjust_range(tlb, address, PAGE_SIZE);	\
		tlb->freed_tables = 1;				\
		tlb->cleared_p4ds = 1;				\
		__pud_free_tlb(tlb, pudp, address);		\
	} while (0)
#endif
#endif

#ifndef __ARCH_HAS_5LEVEL_HACK
#ifndef p4d_free_tlb
#define p4d_free_tlb(tlb, pudp, address)			\
	do {							\
		__tlb_adjust_range(tlb, address, PAGE_SIZE);	\
		tlb->freed_tables = 1;				\
		__p4d_free_tlb(tlb, pudp, address);		\
	} while (0)
#endif
#endif

#endif 

#endif 
