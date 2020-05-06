/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef PAGEBLOCK_FLAGS_H
#define PAGEBLOCK_FLAGS_H

#include <linux/types.h>

#define PB_migratetype_bits 3

enum pageblock_bits {
	PB_migrate,
	PB_migrate_end = PB_migrate + PB_migratetype_bits - 1,
			
	PB_migrate_skip,

	
	NR_PAGEBLOCK_BITS
};

#ifdef CONFIG_HUGETLB_PAGE

#ifdef CONFIG_HUGETLB_PAGE_SIZE_VARIABLE


extern unsigned int pageblock_order;

#else 


#define pageblock_order		HUGETLB_PAGE_ORDER

#endif 

#else 


#define pageblock_order		(MAX_ORDER-1)

#endif 

#define pageblock_nr_pages	(1UL << pageblock_order)


struct page;

unsigned long get_pfnblock_flags_mask(struct page *page,
				unsigned long pfn,
				unsigned long end_bitidx,
				unsigned long mask);

void set_pfnblock_flags_mask(struct page *page,
				unsigned long flags,
				unsigned long pfn,
				unsigned long end_bitidx,
				unsigned long mask);


#define get_pageblock_flags_group(page, start_bitidx, end_bitidx) \
	get_pfnblock_flags_mask(page, page_to_pfn(page),		\
			end_bitidx,					\
			(1 << (end_bitidx - start_bitidx + 1)) - 1)
#define set_pageblock_flags_group(page, flags, start_bitidx, end_bitidx) \
	set_pfnblock_flags_mask(page, flags, page_to_pfn(page),		\
			end_bitidx,					\
			(1 << (end_bitidx - start_bitidx + 1)) - 1)

#ifdef CONFIG_COMPACTION
#define get_pageblock_skip(page) \
			get_pageblock_flags_group(page, PB_migrate_skip,     \
							PB_migrate_skip)
#define clear_pageblock_skip(page) \
			set_pageblock_flags_group(page, 0, PB_migrate_skip,  \
							PB_migrate_skip)
#define set_pageblock_skip(page) \
			set_pageblock_flags_group(page, 1, PB_migrate_skip,  \
							PB_migrate_skip)
#else
static inline bool get_pageblock_skip(struct page *page)
{
	return false;
}
static inline void clear_pageblock_skip(struct page *page)
{
}
static inline void set_pageblock_skip(struct page *page)
{
}
#endif 

#endif	
