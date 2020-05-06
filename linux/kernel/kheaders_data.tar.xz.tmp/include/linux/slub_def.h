/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SLUB_DEF_H
#define _LINUX_SLUB_DEF_H


#include <linux/kobject.h>

enum stat_item {
	ALLOC_FASTPATH,		
	ALLOC_SLOWPATH,		
	FREE_FASTPATH,		
	FREE_SLOWPATH,		
	FREE_FROZEN,		
	FREE_ADD_PARTIAL,	
	FREE_REMOVE_PARTIAL,	
	ALLOC_FROM_PARTIAL,	
	ALLOC_SLAB,		
	ALLOC_REFILL,		
	ALLOC_NODE_MISMATCH,	
	FREE_SLAB,		
	CPUSLAB_FLUSH,		
	DEACTIVATE_FULL,	
	DEACTIVATE_EMPTY,	
	DEACTIVATE_TO_HEAD,	
	DEACTIVATE_TO_TAIL,	
	DEACTIVATE_REMOTE_FREES,
	DEACTIVATE_BYPASS,	
	ORDER_FALLBACK,		
	CMPXCHG_DOUBLE_CPU_FAIL,
	CMPXCHG_DOUBLE_FAIL,	
	CPU_PARTIAL_ALLOC,	
	CPU_PARTIAL_FREE,	
	CPU_PARTIAL_NODE,	
	CPU_PARTIAL_DRAIN,	
	NR_SLUB_STAT_ITEMS };

struct kmem_cache_cpu {
	void **freelist;	
	unsigned long tid;	
	struct page *page;	
#ifdef CONFIG_SLUB_CPU_PARTIAL
	struct page *partial;	
#endif
#ifdef CONFIG_SLUB_STATS
	unsigned stat[NR_SLUB_STAT_ITEMS];
#endif
};

#ifdef CONFIG_SLUB_CPU_PARTIAL
#define slub_percpu_partial(c)		((c)->partial)

#define slub_set_percpu_partial(c, p)		\
({						\
	slub_percpu_partial(c) = (p)->next;	\
})

#define slub_percpu_partial_read_once(c)     READ_ONCE(slub_percpu_partial(c))
#else
#define slub_percpu_partial(c)			NULL

#define slub_set_percpu_partial(c, p)

#define slub_percpu_partial_read_once(c)	NULL
#endif // CONFIG_SLUB_CPU_PARTIAL


struct kmem_cache_order_objects {
	unsigned int x;
};


struct kmem_cache {
	struct kmem_cache_cpu __percpu *cpu_slab;
	
	slab_flags_t flags;
	unsigned long min_partial;
	unsigned int size;	
	unsigned int object_size;
	unsigned int offset;	
#ifdef CONFIG_SLUB_CPU_PARTIAL
	
	unsigned int cpu_partial;
#endif
	struct kmem_cache_order_objects oo;

	
	struct kmem_cache_order_objects max;
	struct kmem_cache_order_objects min;
	gfp_t allocflags;	
	int refcount;		
	void (*ctor)(void *);
	unsigned int inuse;		
	unsigned int align;		
	unsigned int red_left_pad;	
	const char *name;	
	struct list_head list;	
#ifdef CONFIG_SYSFS
	struct kobject kobj;	
	struct work_struct kobj_remove_work;
#endif
#ifdef CONFIG_MEMCG
	struct memcg_cache_params memcg_params;
	
	unsigned int max_attr_size;
#ifdef CONFIG_SYSFS
	struct kset *memcg_kset;
#endif
#endif

#ifdef CONFIG_SLAB_FREELIST_HARDENED
	unsigned long random;
#endif

#ifdef CONFIG_NUMA
	
	unsigned int remote_node_defrag_ratio;
#endif

#ifdef CONFIG_SLAB_FREELIST_RANDOM
	unsigned int *random_seq;
#endif

#ifdef CONFIG_KASAN
	struct kasan_cache kasan_info;
#endif

	unsigned int useroffset;	
	unsigned int usersize;		

	struct kmem_cache_node *node[MAX_NUMNODES];
};

#ifdef CONFIG_SLUB_CPU_PARTIAL
#define slub_cpu_partial(s)		((s)->cpu_partial)
#define slub_set_cpu_partial(s, n)		\
({						\
	slub_cpu_partial(s) = (n);		\
})
#else
#define slub_cpu_partial(s)		(0)
#define slub_set_cpu_partial(s, n)
#endif 

#ifdef CONFIG_SYSFS
#define SLAB_SUPPORTS_SYSFS
void sysfs_slab_unlink(struct kmem_cache *);
void sysfs_slab_release(struct kmem_cache *);
#else
static inline void sysfs_slab_unlink(struct kmem_cache *s)
{
}
static inline void sysfs_slab_release(struct kmem_cache *s)
{
}
#endif

void object_err(struct kmem_cache *s, struct page *page,
		u8 *object, char *reason);

void *fixup_red_left(struct kmem_cache *s, void *p);

static inline void *nearest_obj(struct kmem_cache *cache, struct page *page,
				void *x) {
	void *object = x - (x - page_address(page)) % cache->size;
	void *last_object = page_address(page) +
		(page->objects - 1) * cache->size;
	void *result = (unlikely(object > last_object)) ? last_object : object;

	result = fixup_red_left(cache, result);
	return result;
}

#endif 
