/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_MMU_NOTIFIER_H
#define _LINUX_MMU_NOTIFIER_H

#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/mm_types.h>
#include <linux/srcu.h>

struct mmu_notifier;
struct mmu_notifier_ops;


enum mmu_notifier_event {
	MMU_NOTIFY_UNMAP = 0,
	MMU_NOTIFY_CLEAR,
	MMU_NOTIFY_PROTECTION_VMA,
	MMU_NOTIFY_PROTECTION_PAGE,
	MMU_NOTIFY_SOFT_DIRTY,
};

#ifdef CONFIG_MMU_NOTIFIER


struct mmu_notifier_mm {
	
	struct hlist_head list;
	
	spinlock_t lock;
};

#define MMU_NOTIFIER_RANGE_BLOCKABLE (1 << 0)

struct mmu_notifier_range {
	struct vm_area_struct *vma;
	struct mm_struct *mm;
	unsigned long start;
	unsigned long end;
	unsigned flags;
	enum mmu_notifier_event event;
};

struct mmu_notifier_ops {
	
	void (*release)(struct mmu_notifier *mn,
			struct mm_struct *mm);

	
	int (*clear_flush_young)(struct mmu_notifier *mn,
				 struct mm_struct *mm,
				 unsigned long start,
				 unsigned long end);

	
	int (*clear_young)(struct mmu_notifier *mn,
			   struct mm_struct *mm,
			   unsigned long start,
			   unsigned long end);

	
	int (*test_young)(struct mmu_notifier *mn,
			  struct mm_struct *mm,
			  unsigned long address);

	
	void (*change_pte)(struct mmu_notifier *mn,
			   struct mm_struct *mm,
			   unsigned long address,
			   pte_t pte);

	
	int (*invalidate_range_start)(struct mmu_notifier *mn,
				      const struct mmu_notifier_range *range);
	void (*invalidate_range_end)(struct mmu_notifier *mn,
				     const struct mmu_notifier_range *range);

	
	void (*invalidate_range)(struct mmu_notifier *mn, struct mm_struct *mm,
				 unsigned long start, unsigned long end);
};


struct mmu_notifier {
	struct hlist_node hlist;
	const struct mmu_notifier_ops *ops;
};

static inline int mm_has_notifiers(struct mm_struct *mm)
{
	return unlikely(mm->mmu_notifier_mm);
}

extern int mmu_notifier_register(struct mmu_notifier *mn,
				 struct mm_struct *mm);
extern int __mmu_notifier_register(struct mmu_notifier *mn,
				   struct mm_struct *mm);
extern void mmu_notifier_unregister(struct mmu_notifier *mn,
				    struct mm_struct *mm);
extern void mmu_notifier_unregister_no_release(struct mmu_notifier *mn,
					       struct mm_struct *mm);
extern void __mmu_notifier_mm_destroy(struct mm_struct *mm);
extern void __mmu_notifier_release(struct mm_struct *mm);
extern int __mmu_notifier_clear_flush_young(struct mm_struct *mm,
					  unsigned long start,
					  unsigned long end);
extern int __mmu_notifier_clear_young(struct mm_struct *mm,
				      unsigned long start,
				      unsigned long end);
extern int __mmu_notifier_test_young(struct mm_struct *mm,
				     unsigned long address);
extern void __mmu_notifier_change_pte(struct mm_struct *mm,
				      unsigned long address, pte_t pte);
extern int __mmu_notifier_invalidate_range_start(struct mmu_notifier_range *r);
extern void __mmu_notifier_invalidate_range_end(struct mmu_notifier_range *r,
				  bool only_end);
extern void __mmu_notifier_invalidate_range(struct mm_struct *mm,
				  unsigned long start, unsigned long end);
extern bool
mmu_notifier_range_update_to_read_only(const struct mmu_notifier_range *range);

static inline bool
mmu_notifier_range_blockable(const struct mmu_notifier_range *range)
{
	return (range->flags & MMU_NOTIFIER_RANGE_BLOCKABLE);
}

static inline void mmu_notifier_release(struct mm_struct *mm)
{
	if (mm_has_notifiers(mm))
		__mmu_notifier_release(mm);
}

static inline int mmu_notifier_clear_flush_young(struct mm_struct *mm,
					  unsigned long start,
					  unsigned long end)
{
	if (mm_has_notifiers(mm))
		return __mmu_notifier_clear_flush_young(mm, start, end);
	return 0;
}

static inline int mmu_notifier_clear_young(struct mm_struct *mm,
					   unsigned long start,
					   unsigned long end)
{
	if (mm_has_notifiers(mm))
		return __mmu_notifier_clear_young(mm, start, end);
	return 0;
}

static inline int mmu_notifier_test_young(struct mm_struct *mm,
					  unsigned long address)
{
	if (mm_has_notifiers(mm))
		return __mmu_notifier_test_young(mm, address);
	return 0;
}

static inline void mmu_notifier_change_pte(struct mm_struct *mm,
					   unsigned long address, pte_t pte)
{
	if (mm_has_notifiers(mm))
		__mmu_notifier_change_pte(mm, address, pte);
}

static inline void
mmu_notifier_invalidate_range_start(struct mmu_notifier_range *range)
{
	if (mm_has_notifiers(range->mm)) {
		range->flags |= MMU_NOTIFIER_RANGE_BLOCKABLE;
		__mmu_notifier_invalidate_range_start(range);
	}
}

static inline int
mmu_notifier_invalidate_range_start_nonblock(struct mmu_notifier_range *range)
{
	if (mm_has_notifiers(range->mm)) {
		range->flags &= ~MMU_NOTIFIER_RANGE_BLOCKABLE;
		return __mmu_notifier_invalidate_range_start(range);
	}
	return 0;
}

static inline void
mmu_notifier_invalidate_range_end(struct mmu_notifier_range *range)
{
	if (mm_has_notifiers(range->mm))
		__mmu_notifier_invalidate_range_end(range, false);
}

static inline void
mmu_notifier_invalidate_range_only_end(struct mmu_notifier_range *range)
{
	if (mm_has_notifiers(range->mm))
		__mmu_notifier_invalidate_range_end(range, true);
}

static inline void mmu_notifier_invalidate_range(struct mm_struct *mm,
				  unsigned long start, unsigned long end)
{
	if (mm_has_notifiers(mm))
		__mmu_notifier_invalidate_range(mm, start, end);
}

static inline void mmu_notifier_mm_init(struct mm_struct *mm)
{
	mm->mmu_notifier_mm = NULL;
}

static inline void mmu_notifier_mm_destroy(struct mm_struct *mm)
{
	if (mm_has_notifiers(mm))
		__mmu_notifier_mm_destroy(mm);
}


static inline void mmu_notifier_range_init(struct mmu_notifier_range *range,
					   enum mmu_notifier_event event,
					   unsigned flags,
					   struct vm_area_struct *vma,
					   struct mm_struct *mm,
					   unsigned long start,
					   unsigned long end)
{
	range->vma = vma;
	range->event = event;
	range->mm = mm;
	range->start = start;
	range->end = end;
	range->flags = flags;
}

#define ptep_clear_flush_young_notify(__vma, __address, __ptep)		\
({									\
	int __young;							\
	struct vm_area_struct *___vma = __vma;				\
	unsigned long ___address = __address;				\
	__young = ptep_clear_flush_young(___vma, ___address, __ptep);	\
	__young |= mmu_notifier_clear_flush_young(___vma->vm_mm,	\
						  ___address,		\
						  ___address +		\
							PAGE_SIZE);	\
	__young;							\
})

#define pmdp_clear_flush_young_notify(__vma, __address, __pmdp)		\
({									\
	int __young;							\
	struct vm_area_struct *___vma = __vma;				\
	unsigned long ___address = __address;				\
	__young = pmdp_clear_flush_young(___vma, ___address, __pmdp);	\
	__young |= mmu_notifier_clear_flush_young(___vma->vm_mm,	\
						  ___address,		\
						  ___address +		\
							PMD_SIZE);	\
	__young;							\
})

#define ptep_clear_young_notify(__vma, __address, __ptep)		\
({									\
	int __young;							\
	struct vm_area_struct *___vma = __vma;				\
	unsigned long ___address = __address;				\
	__young = ptep_test_and_clear_young(___vma, ___address, __ptep);\
	__young |= mmu_notifier_clear_young(___vma->vm_mm, ___address,	\
					    ___address + PAGE_SIZE);	\
	__young;							\
})

#define pmdp_clear_young_notify(__vma, __address, __pmdp)		\
({									\
	int __young;							\
	struct vm_area_struct *___vma = __vma;				\
	unsigned long ___address = __address;				\
	__young = pmdp_test_and_clear_young(___vma, ___address, __pmdp);\
	__young |= mmu_notifier_clear_young(___vma->vm_mm, ___address,	\
					    ___address + PMD_SIZE);	\
	__young;							\
})

#define	ptep_clear_flush_notify(__vma, __address, __ptep)		\
({									\
	unsigned long ___addr = __address & PAGE_MASK;			\
	struct mm_struct *___mm = (__vma)->vm_mm;			\
	pte_t ___pte;							\
									\
	___pte = ptep_clear_flush(__vma, __address, __ptep);		\
	mmu_notifier_invalidate_range(___mm, ___addr,			\
					___addr + PAGE_SIZE);		\
									\
	___pte;								\
})

#define pmdp_huge_clear_flush_notify(__vma, __haddr, __pmd)		\
({									\
	unsigned long ___haddr = __haddr & HPAGE_PMD_MASK;		\
	struct mm_struct *___mm = (__vma)->vm_mm;			\
	pmd_t ___pmd;							\
									\
	___pmd = pmdp_huge_clear_flush(__vma, __haddr, __pmd);		\
	mmu_notifier_invalidate_range(___mm, ___haddr,			\
				      ___haddr + HPAGE_PMD_SIZE);	\
									\
	___pmd;								\
})

#define pudp_huge_clear_flush_notify(__vma, __haddr, __pud)		\
({									\
	unsigned long ___haddr = __haddr & HPAGE_PUD_MASK;		\
	struct mm_struct *___mm = (__vma)->vm_mm;			\
	pud_t ___pud;							\
									\
	___pud = pudp_huge_clear_flush(__vma, __haddr, __pud);		\
	mmu_notifier_invalidate_range(___mm, ___haddr,			\
				      ___haddr + HPAGE_PUD_SIZE);	\
									\
	___pud;								\
})


#define set_pte_at_notify(__mm, __address, __ptep, __pte)		\
({									\
	struct mm_struct *___mm = __mm;					\
	unsigned long ___address = __address;				\
	pte_t ___pte = __pte;						\
									\
	mmu_notifier_change_pte(___mm, ___address, ___pte);		\
	set_pte_at(___mm, ___address, __ptep, ___pte);			\
})

extern void mmu_notifier_call_srcu(struct rcu_head *rcu,
				   void (*func)(struct rcu_head *rcu));

#else 

struct mmu_notifier_range {
	unsigned long start;
	unsigned long end;
};

static inline void _mmu_notifier_range_init(struct mmu_notifier_range *range,
					    unsigned long start,
					    unsigned long end)
{
	range->start = start;
	range->end = end;
}

#define mmu_notifier_range_init(range,event,flags,vma,mm,start,end)  \
	_mmu_notifier_range_init(range, start, end)

static inline bool
mmu_notifier_range_blockable(const struct mmu_notifier_range *range)
{
	return true;
}

static inline int mm_has_notifiers(struct mm_struct *mm)
{
	return 0;
}

static inline void mmu_notifier_release(struct mm_struct *mm)
{
}

static inline int mmu_notifier_clear_flush_young(struct mm_struct *mm,
					  unsigned long start,
					  unsigned long end)
{
	return 0;
}

static inline int mmu_notifier_test_young(struct mm_struct *mm,
					  unsigned long address)
{
	return 0;
}

static inline void mmu_notifier_change_pte(struct mm_struct *mm,
					   unsigned long address, pte_t pte)
{
}

static inline void
mmu_notifier_invalidate_range_start(struct mmu_notifier_range *range)
{
}

static inline int
mmu_notifier_invalidate_range_start_nonblock(struct mmu_notifier_range *range)
{
	return 0;
}

static inline
void mmu_notifier_invalidate_range_end(struct mmu_notifier_range *range)
{
}

static inline void
mmu_notifier_invalidate_range_only_end(struct mmu_notifier_range *range)
{
}

static inline void mmu_notifier_invalidate_range(struct mm_struct *mm,
				  unsigned long start, unsigned long end)
{
}

static inline void mmu_notifier_mm_init(struct mm_struct *mm)
{
}

static inline void mmu_notifier_mm_destroy(struct mm_struct *mm)
{
}

#define mmu_notifier_range_update_to_read_only(r) false

#define ptep_clear_flush_young_notify ptep_clear_flush_young
#define pmdp_clear_flush_young_notify pmdp_clear_flush_young
#define ptep_clear_young_notify ptep_test_and_clear_young
#define pmdp_clear_young_notify pmdp_test_and_clear_young
#define	ptep_clear_flush_notify ptep_clear_flush
#define pmdp_huge_clear_flush_notify pmdp_huge_clear_flush
#define pudp_huge_clear_flush_notify pudp_huge_clear_flush
#define set_pte_at_notify set_pte_at

#endif 

#endif 
