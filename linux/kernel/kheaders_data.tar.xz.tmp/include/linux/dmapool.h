

#ifndef LINUX_DMAPOOL_H
#define	LINUX_DMAPOOL_H

#include <linux/scatterlist.h>
#include <asm/io.h>

struct device;

#ifdef CONFIG_HAS_DMA

struct dma_pool *dma_pool_create(const char *name, struct device *dev, 
			size_t size, size_t align, size_t allocation);

void dma_pool_destroy(struct dma_pool *pool);

void *dma_pool_alloc(struct dma_pool *pool, gfp_t mem_flags,
		     dma_addr_t *handle);
void dma_pool_free(struct dma_pool *pool, void *vaddr, dma_addr_t addr);


struct dma_pool *dmam_pool_create(const char *name, struct device *dev,
				  size_t size, size_t align, size_t allocation);
void dmam_pool_destroy(struct dma_pool *pool);

#else 
static inline struct dma_pool *dma_pool_create(const char *name,
	struct device *dev, size_t size, size_t align, size_t allocation)
{ return NULL; }
static inline void dma_pool_destroy(struct dma_pool *pool) { }
static inline void *dma_pool_alloc(struct dma_pool *pool, gfp_t mem_flags,
				   dma_addr_t *handle) { return NULL; }
static inline void dma_pool_free(struct dma_pool *pool, void *vaddr,
				 dma_addr_t addr) { }
static inline struct dma_pool *dmam_pool_create(const char *name,
	struct device *dev, size_t size, size_t align, size_t allocation)
{ return NULL; }
static inline void dmam_pool_destroy(struct dma_pool *pool) { }
#endif 

static inline void *dma_pool_zalloc(struct dma_pool *pool, gfp_t mem_flags,
				    dma_addr_t *handle)
{
	return dma_pool_alloc(pool, mem_flags | __GFP_ZERO, handle);
}

#endif

