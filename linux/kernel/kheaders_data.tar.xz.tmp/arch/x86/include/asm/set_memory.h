/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SET_MEMORY_H
#define _ASM_X86_SET_MEMORY_H

#include <asm/page.h>
#include <asm-generic/set_memory.h>



int _set_memory_uc(unsigned long addr, int numpages);
int _set_memory_wc(unsigned long addr, int numpages);
int _set_memory_wt(unsigned long addr, int numpages);
int _set_memory_wb(unsigned long addr, int numpages);
int set_memory_uc(unsigned long addr, int numpages);
int set_memory_wc(unsigned long addr, int numpages);
int set_memory_wt(unsigned long addr, int numpages);
int set_memory_wb(unsigned long addr, int numpages);
int set_memory_np(unsigned long addr, int numpages);
int set_memory_4k(unsigned long addr, int numpages);
int set_memory_encrypted(unsigned long addr, int numpages);
int set_memory_decrypted(unsigned long addr, int numpages);
int set_memory_np_noalias(unsigned long addr, int numpages);

int set_memory_array_uc(unsigned long *addr, int addrinarray);
int set_memory_array_wc(unsigned long *addr, int addrinarray);
int set_memory_array_wt(unsigned long *addr, int addrinarray);
int set_memory_array_wb(unsigned long *addr, int addrinarray);

int set_pages_array_uc(struct page **pages, int addrinarray);
int set_pages_array_wc(struct page **pages, int addrinarray);
int set_pages_array_wt(struct page **pages, int addrinarray);
int set_pages_array_wb(struct page **pages, int addrinarray);



int set_pages_uc(struct page *page, int numpages);
int set_pages_wb(struct page *page, int numpages);
int set_pages_x(struct page *page, int numpages);
int set_pages_nx(struct page *page, int numpages);
int set_pages_ro(struct page *page, int numpages);
int set_pages_rw(struct page *page, int numpages);

int set_direct_map_invalid_noflush(struct page *page);
int set_direct_map_default_noflush(struct page *page);

extern int kernel_set_to_readonly;
void set_kernel_text_rw(void);
void set_kernel_text_ro(void);

#ifdef CONFIG_X86_64
static inline int set_mce_nospec(unsigned long pfn)
{
	unsigned long decoy_addr;
	int rc;

	
	decoy_addr = (pfn << PAGE_SHIFT) + (PAGE_OFFSET ^ BIT(63));

	rc = set_memory_uc(decoy_addr, 1);
	if (rc)
		pr_warn("Could not invalidate pfn=0x%lx from 1:1 map\n", pfn);
	return rc;
}
#define set_mce_nospec set_mce_nospec


static inline int clear_mce_nospec(unsigned long pfn)
{
	return set_memory_wb((unsigned long) pfn_to_kaddr(pfn), 1);
}
#define clear_mce_nospec clear_mce_nospec
#else

#endif

#endif 
