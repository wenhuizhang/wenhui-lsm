/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PGTABLE_32_H
#define _ASM_X86_PGTABLE_32_H

#include <asm/pgtable_32_types.h>


#ifndef __ASSEMBLY__
#include <asm/processor.h>
#include <linux/threads.h>
#include <asm/paravirt.h>

#include <linux/bitops.h>
#include <linux/list.h>
#include <linux/spinlock.h>

struct mm_struct;
struct vm_area_struct;

extern pgd_t swapper_pg_dir[1024];
extern pgd_t initial_page_table[1024];
extern pmd_t initial_pg_pmd[];

static inline void pgtable_cache_init(void) { }
static inline void check_pgt_cache(void) { }
void paging_init(void);
void sync_initial_page_table(void);


#undef TEST_ACCESS_OK

#ifdef CONFIG_X86_PAE
# include <asm/pgtable-3level.h>
#else
# include <asm/pgtable-2level.h>
#endif

#if defined(CONFIG_HIGHPTE)
#define pte_offset_map(dir, address)					\
	((pte_t *)kmap_atomic(pmd_page(*(dir))) +		\
	 pte_index((address)))
#define pte_unmap(pte) kunmap_atomic((pte))
#else
#define pte_offset_map(dir, address)					\
	((pte_t *)page_address(pmd_page(*(dir))) + pte_index((address)))
#define pte_unmap(pte) do { } while (0)
#endif


#define kpte_clear_flush(ptep, vaddr)		\
do {						\
	pte_clear(&init_mm, (vaddr), (ptep));	\
	__flush_tlb_one_kernel((vaddr));		\
} while (0)

#endif 


#ifdef CONFIG_FLATMEM
#define kern_addr_valid(addr)	(1)
#else
#define kern_addr_valid(kaddr)	(0)
#endif


#if PTRS_PER_PMD > 1
#define PAGE_TABLE_SIZE(pages) (((pages) / PTRS_PER_PMD) + PTRS_PER_PGD)
#else
#define PAGE_TABLE_SIZE(pages) ((pages) / PTRS_PER_PGD)
#endif


#define LOWMEM_PAGES ((((_ULL(2)<<31) - __PAGE_OFFSET) >> PAGE_SHIFT))

#endif 
