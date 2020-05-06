/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_MPX_H
#define _ASM_X86_MPX_H

#include <linux/types.h>
#include <linux/mm_types.h>

#include <asm/ptrace.h>
#include <asm/insn.h>


#define MPX_INVALID_BOUNDS_DIR	((void __user *)-1)
#define MPX_BNDCFG_ENABLE_FLAG	0x1
#define MPX_BD_ENTRY_VALID_FLAG	0x1


#define MPX_BD_SIZE_BYTES_64	(1UL<<31)
#define MPX_BD_ENTRY_BYTES_64	8
#define MPX_BD_NR_ENTRIES_64	(MPX_BD_SIZE_BYTES_64/MPX_BD_ENTRY_BYTES_64)


#define MPX_BD_SIZE_BYTES_32	(1UL<<22)
#define MPX_BD_ENTRY_BYTES_32	4
#define MPX_BD_NR_ENTRIES_32	(MPX_BD_SIZE_BYTES_32/MPX_BD_ENTRY_BYTES_32)


#define MPX_BT_SIZE_BYTES_64	(1UL<<22)
#define MPX_BT_ENTRY_BYTES_64	32
#define MPX_BT_NR_ENTRIES_64	(MPX_BT_SIZE_BYTES_64/MPX_BT_ENTRY_BYTES_64)


#define MPX_BT_SIZE_BYTES_32	(1UL<<14)
#define MPX_BT_ENTRY_BYTES_32	16
#define MPX_BT_NR_ENTRIES_32	(MPX_BT_SIZE_BYTES_32/MPX_BT_ENTRY_BYTES_32)

#define MPX_BNDSTA_TAIL		2
#define MPX_BNDCFG_TAIL		12
#define MPX_BNDSTA_ADDR_MASK	(~((1UL<<MPX_BNDSTA_TAIL)-1))
#define MPX_BNDCFG_ADDR_MASK	(~((1UL<<MPX_BNDCFG_TAIL)-1))
#define MPX_BNDSTA_ERROR_CODE	0x3

struct mpx_fault_info {
	void __user *addr;
	void __user *lower;
	void __user *upper;
};

#ifdef CONFIG_X86_INTEL_MPX

extern int mpx_fault_info(struct mpx_fault_info *info, struct pt_regs *regs);
extern int mpx_handle_bd_fault(void);

static inline int kernel_managing_mpx_tables(struct mm_struct *mm)
{
	return (mm->context.bd_addr != MPX_INVALID_BOUNDS_DIR);
}

static inline void mpx_mm_init(struct mm_struct *mm)
{
	
	mm->context.bd_addr = MPX_INVALID_BOUNDS_DIR;
}

extern void mpx_notify_unmap(struct mm_struct *mm, unsigned long start, unsigned long end);
extern unsigned long mpx_unmapped_area_check(unsigned long addr, unsigned long len, unsigned long flags);

#else
static inline int mpx_fault_info(struct mpx_fault_info *info, struct pt_regs *regs)
{
	return -EINVAL;
}
static inline int mpx_handle_bd_fault(void)
{
	return -EINVAL;
}
static inline int kernel_managing_mpx_tables(struct mm_struct *mm)
{
	return 0;
}
static inline void mpx_mm_init(struct mm_struct *mm)
{
}
static inline void mpx_notify_unmap(struct mm_struct *mm,
				    unsigned long start, unsigned long end)
{
}

static inline unsigned long mpx_unmapped_area_check(unsigned long addr,
		unsigned long len, unsigned long flags)
{
	return addr;
}
#endif 

#endif 