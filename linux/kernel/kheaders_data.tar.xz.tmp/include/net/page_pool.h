/* SPDX-License-Identifier: GPL-2.0
 *
 * page_pool.h
 *	Author:	Jesper Dangaard Brouer <netoptimizer@brouer.com>
 *	Copyright (C) 2016 Red Hat, Inc.
 */


#ifndef _NET_PAGE_POOL_H
#define _NET_PAGE_POOL_H

#include <linux/mm.h> 
#include <linux/ptr_ring.h>
#include <linux/dma-direction.h>

#define PP_FLAG_DMA_MAP 1 
#define PP_FLAG_ALL	PP_FLAG_DMA_MAP


#define PP_ALLOC_CACHE_SIZE	128
#define PP_ALLOC_CACHE_REFILL	64
struct pp_alloc_cache {
	u32 count;
	void *cache[PP_ALLOC_CACHE_SIZE];
};

struct page_pool_params {
	unsigned int	flags;
	unsigned int	order;
	unsigned int	pool_size;
	int		nid;  
	struct device	*dev; 
	enum dma_data_direction dma_dir; 
};

struct page_pool {
	struct page_pool_params p;

        u32 pages_state_hold_cnt;

	
	struct pp_alloc_cache alloc ____cacheline_aligned_in_smp;

	
	struct ptr_ring ring;

	atomic_t pages_state_release_cnt;

	
	refcount_t user_cnt;
};

struct page *page_pool_alloc_pages(struct page_pool *pool, gfp_t gfp);

static inline struct page *page_pool_dev_alloc_pages(struct page_pool *pool)
{
	gfp_t gfp = (GFP_ATOMIC | __GFP_NOWARN);

	return page_pool_alloc_pages(pool, gfp);
}


static
inline enum dma_data_direction page_pool_get_dma_dir(struct page_pool *pool)
{
	return pool->p.dma_dir;
}

struct page_pool *page_pool_create(const struct page_pool_params *params);

void __page_pool_free(struct page_pool *pool);
static inline void page_pool_free(struct page_pool *pool)
{
	
#ifdef CONFIG_PAGE_POOL
	__page_pool_free(pool);
#endif
}


static inline void page_pool_destroy(struct page_pool *pool)
{
	if (!pool)
		return;

	page_pool_free(pool);
}


void __page_pool_put_page(struct page_pool *pool,
			  struct page *page, bool allow_direct);

static inline void page_pool_put_page(struct page_pool *pool,
				      struct page *page, bool allow_direct)
{
	
#ifdef CONFIG_PAGE_POOL
	__page_pool_put_page(pool, page, allow_direct);
#endif
}

static inline void page_pool_recycle_direct(struct page_pool *pool,
					    struct page *page)
{
	__page_pool_put_page(pool, page, true);
}


bool __page_pool_request_shutdown(struct page_pool *pool);
static inline bool page_pool_request_shutdown(struct page_pool *pool)
{
	bool safe_to_remove = false;

#ifdef CONFIG_PAGE_POOL
	safe_to_remove = __page_pool_request_shutdown(pool);
#endif
	return safe_to_remove;
}


void page_pool_unmap_page(struct page_pool *pool, struct page *page);
static inline void page_pool_release_page(struct page_pool *pool,
					  struct page *page)
{
#ifdef CONFIG_PAGE_POOL
	page_pool_unmap_page(pool, page);
#endif
}

static inline dma_addr_t page_pool_get_dma_addr(struct page *page)
{
	return page->dma_addr;
}

static inline bool is_page_pool_compiled_in(void)
{
#ifdef CONFIG_PAGE_POOL
	return true;
#else
	return false;
#endif
}

static inline void page_pool_get(struct page_pool *pool)
{
	refcount_inc(&pool->user_cnt);
}

static inline bool page_pool_put(struct page_pool *pool)
{
	return refcount_dec_and_test(&pool->user_cnt);
}

#endif 
