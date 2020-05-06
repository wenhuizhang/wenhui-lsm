/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _ASM_GENERIC_MM_HOOKS_H
#define _ASM_GENERIC_MM_HOOKS_H

static inline int arch_dup_mmap(struct mm_struct *oldmm,
				struct mm_struct *mm)
{
	return 0;
}

static inline void arch_exit_mmap(struct mm_struct *mm)
{
}

static inline void arch_unmap(struct mm_struct *mm,
			unsigned long start, unsigned long end)
{
}

static inline void arch_bprm_mm_init(struct mm_struct *mm,
				     struct vm_area_struct *vma)
{
}

static inline bool arch_vma_access_permitted(struct vm_area_struct *vma,
		bool write, bool execute, bool foreign)
{
	
	return true;
}
#endif	
