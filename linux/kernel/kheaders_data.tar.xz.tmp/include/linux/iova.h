/* SPDX-License-Identifier: GPL-2.0-only */


#ifndef _IOVA_H_
#define _IOVA_H_

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/rbtree.h>
#include <linux/atomic.h>
#include <linux/dma-mapping.h>


struct iova {
	struct rb_node	node;
	unsigned long	pfn_hi; 
	unsigned long	pfn_lo; 
};

struct iova_magazine;
struct iova_cpu_rcache;

#define IOVA_RANGE_CACHE_MAX_SIZE 6	
#define MAX_GLOBAL_MAGS 32	

struct iova_rcache {
	spinlock_t lock;
	unsigned long depot_size;
	struct iova_magazine *depot[MAX_GLOBAL_MAGS];
	struct iova_cpu_rcache __percpu *cpu_rcaches;
};

struct iova_domain;


typedef void (* iova_flush_cb)(struct iova_domain *domain);


typedef void (* iova_entry_dtor)(unsigned long data);


#define IOVA_FQ_SIZE	256


#define IOVA_FQ_TIMEOUT	10


struct iova_fq_entry {
	unsigned long iova_pfn;
	unsigned long pages;
	unsigned long data;
	u64 counter; 
};


struct iova_fq {
	struct iova_fq_entry entries[IOVA_FQ_SIZE];
	unsigned head, tail;
	spinlock_t lock;
};


struct iova_domain {
	spinlock_t	iova_rbtree_lock; 
	struct rb_root	rbroot;		
	struct rb_node	*cached_node;	
	struct rb_node	*cached32_node; 
	unsigned long	granule;	
	unsigned long	start_pfn;	
	unsigned long	dma_32bit_pfn;
	unsigned long	max32_alloc_size; 
	struct iova_fq __percpu *fq;	

	atomic64_t	fq_flush_start_cnt;	

	atomic64_t	fq_flush_finish_cnt;	

	struct iova	anchor;		
	struct iova_rcache rcaches[IOVA_RANGE_CACHE_MAX_SIZE];	

	iova_flush_cb	flush_cb;	

	iova_entry_dtor entry_dtor;	

	struct timer_list fq_timer;		
	atomic_t fq_timer_on;			
};

static inline unsigned long iova_size(struct iova *iova)
{
	return iova->pfn_hi - iova->pfn_lo + 1;
}

static inline unsigned long iova_shift(struct iova_domain *iovad)
{
	return __ffs(iovad->granule);
}

static inline unsigned long iova_mask(struct iova_domain *iovad)
{
	return iovad->granule - 1;
}

static inline size_t iova_offset(struct iova_domain *iovad, dma_addr_t iova)
{
	return iova & iova_mask(iovad);
}

static inline size_t iova_align(struct iova_domain *iovad, size_t size)
{
	return ALIGN(size, iovad->granule);
}

static inline dma_addr_t iova_dma_addr(struct iova_domain *iovad, struct iova *iova)
{
	return (dma_addr_t)iova->pfn_lo << iova_shift(iovad);
}

static inline unsigned long iova_pfn(struct iova_domain *iovad, dma_addr_t iova)
{
	return iova >> iova_shift(iovad);
}

#if IS_ENABLED(CONFIG_IOMMU_IOVA)
int iova_cache_get(void);
void iova_cache_put(void);

struct iova *alloc_iova_mem(void);
void free_iova_mem(struct iova *iova);
void free_iova(struct iova_domain *iovad, unsigned long pfn);
void __free_iova(struct iova_domain *iovad, struct iova *iova);
struct iova *alloc_iova(struct iova_domain *iovad, unsigned long size,
	unsigned long limit_pfn,
	bool size_aligned);
void free_iova_fast(struct iova_domain *iovad, unsigned long pfn,
		    unsigned long size);
void queue_iova(struct iova_domain *iovad,
		unsigned long pfn, unsigned long pages,
		unsigned long data);
unsigned long alloc_iova_fast(struct iova_domain *iovad, unsigned long size,
			      unsigned long limit_pfn, bool flush_rcache);
struct iova *reserve_iova(struct iova_domain *iovad, unsigned long pfn_lo,
	unsigned long pfn_hi);
void copy_reserved_iova(struct iova_domain *from, struct iova_domain *to);
void init_iova_domain(struct iova_domain *iovad, unsigned long granule,
	unsigned long start_pfn);
bool has_iova_flush_queue(struct iova_domain *iovad);
int init_iova_flush_queue(struct iova_domain *iovad,
			  iova_flush_cb flush_cb, iova_entry_dtor entry_dtor);
struct iova *find_iova(struct iova_domain *iovad, unsigned long pfn);
void put_iova_domain(struct iova_domain *iovad);
struct iova *split_and_remove_iova(struct iova_domain *iovad,
	struct iova *iova, unsigned long pfn_lo, unsigned long pfn_hi);
void free_cpu_cached_iovas(unsigned int cpu, struct iova_domain *iovad);
#else
static inline int iova_cache_get(void)
{
	return -ENOTSUPP;
}

static inline void iova_cache_put(void)
{
}

static inline struct iova *alloc_iova_mem(void)
{
	return NULL;
}

static inline void free_iova_mem(struct iova *iova)
{
}

static inline void free_iova(struct iova_domain *iovad, unsigned long pfn)
{
}

static inline void __free_iova(struct iova_domain *iovad, struct iova *iova)
{
}

static inline struct iova *alloc_iova(struct iova_domain *iovad,
				      unsigned long size,
				      unsigned long limit_pfn,
				      bool size_aligned)
{
	return NULL;
}

static inline void free_iova_fast(struct iova_domain *iovad,
				  unsigned long pfn,
				  unsigned long size)
{
}

static inline void queue_iova(struct iova_domain *iovad,
			      unsigned long pfn, unsigned long pages,
			      unsigned long data)
{
}

static inline unsigned long alloc_iova_fast(struct iova_domain *iovad,
					    unsigned long size,
					    unsigned long limit_pfn,
					    bool flush_rcache)
{
	return 0;
}

static inline struct iova *reserve_iova(struct iova_domain *iovad,
					unsigned long pfn_lo,
					unsigned long pfn_hi)
{
	return NULL;
}

static inline void copy_reserved_iova(struct iova_domain *from,
				      struct iova_domain *to)
{
}

static inline void init_iova_domain(struct iova_domain *iovad,
				    unsigned long granule,
				    unsigned long start_pfn)
{
}

static inline bool has_iova_flush_queue(struct iova_domain *iovad)
{
	return false;
}

static inline int init_iova_flush_queue(struct iova_domain *iovad,
					iova_flush_cb flush_cb,
					iova_entry_dtor entry_dtor)
{
	return -ENODEV;
}

static inline struct iova *find_iova(struct iova_domain *iovad,
				     unsigned long pfn)
{
	return NULL;
}

static inline void put_iova_domain(struct iova_domain *iovad)
{
}

static inline struct iova *split_and_remove_iova(struct iova_domain *iovad,
						 struct iova *iova,
						 unsigned long pfn_lo,
						 unsigned long pfn_hi)
{
	return NULL;
}

static inline void free_cpu_cached_iovas(unsigned int cpu,
					 struct iova_domain *iovad)
{
}
#endif

#endif
