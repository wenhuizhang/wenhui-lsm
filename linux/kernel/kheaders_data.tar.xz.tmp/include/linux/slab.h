/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _LINUX_SLAB_H
#define	_LINUX_SLAB_H

#include <linux/gfp.h>
#include <linux/overflow.h>
#include <linux/types.h>
#include <linux/workqueue.h>
#include <linux/percpu-refcount.h>




#define SLAB_CONSISTENCY_CHECKS	((slab_flags_t __force)0x00000100U)

#define SLAB_RED_ZONE		((slab_flags_t __force)0x00000400U)

#define SLAB_POISON		((slab_flags_t __force)0x00000800U)

#define SLAB_HWCACHE_ALIGN	((slab_flags_t __force)0x00002000U)

#define SLAB_CACHE_DMA		((slab_flags_t __force)0x00004000U)

#define SLAB_CACHE_DMA32	((slab_flags_t __force)0x00008000U)

#define SLAB_STORE_USER		((slab_flags_t __force)0x00010000U)

#define SLAB_PANIC		((slab_flags_t __force)0x00040000U)


#define SLAB_TYPESAFE_BY_RCU	((slab_flags_t __force)0x00080000U)

#define SLAB_MEM_SPREAD		((slab_flags_t __force)0x00100000U)

#define SLAB_TRACE		((slab_flags_t __force)0x00200000U)


#ifdef CONFIG_DEBUG_OBJECTS
# define SLAB_DEBUG_OBJECTS	((slab_flags_t __force)0x00400000U)
#else
# define SLAB_DEBUG_OBJECTS	0
#endif


#define SLAB_NOLEAKTRACE	((slab_flags_t __force)0x00800000U)


#ifdef CONFIG_FAILSLAB
# define SLAB_FAILSLAB		((slab_flags_t __force)0x02000000U)
#else
# define SLAB_FAILSLAB		0
#endif

#ifdef CONFIG_MEMCG_KMEM
# define SLAB_ACCOUNT		((slab_flags_t __force)0x04000000U)
#else
# define SLAB_ACCOUNT		0
#endif

#ifdef CONFIG_KASAN
#define SLAB_KASAN		((slab_flags_t __force)0x08000000U)
#else
#define SLAB_KASAN		0
#endif



#define SLAB_RECLAIM_ACCOUNT	((slab_flags_t __force)0x00020000U)
#define SLAB_TEMPORARY		SLAB_RECLAIM_ACCOUNT	


#define SLAB_DEACTIVATED	((slab_flags_t __force)0x10000000U)


#define ZERO_SIZE_PTR ((void *)16)

#define ZERO_OR_NULL_PTR(x) ((unsigned long)(x) <= \
				(unsigned long)ZERO_SIZE_PTR)

#include <linux/kasan.h>

struct mem_cgroup;

void __init kmem_cache_init(void);
bool slab_is_available(void);

extern bool usercopy_fallback;

struct kmem_cache *kmem_cache_create(const char *name, unsigned int size,
			unsigned int align, slab_flags_t flags,
			void (*ctor)(void *));
struct kmem_cache *kmem_cache_create_usercopy(const char *name,
			unsigned int size, unsigned int align,
			slab_flags_t flags,
			unsigned int useroffset, unsigned int usersize,
			void (*ctor)(void *));
void kmem_cache_destroy(struct kmem_cache *);
int kmem_cache_shrink(struct kmem_cache *);

void memcg_create_kmem_cache(struct mem_cgroup *, struct kmem_cache *);
void memcg_deactivate_kmem_caches(struct mem_cgroup *, struct mem_cgroup *);


#define KMEM_CACHE(__struct, __flags)					\
		kmem_cache_create(#__struct, sizeof(struct __struct),	\
			__alignof__(struct __struct), (__flags), NULL)


#define KMEM_CACHE_USERCOPY(__struct, __flags, __field)			\
		kmem_cache_create_usercopy(#__struct,			\
			sizeof(struct __struct),			\
			__alignof__(struct __struct), (__flags),	\
			offsetof(struct __struct, __field),		\
			sizeof_field(struct __struct, __field), NULL)


void * __must_check __krealloc(const void *, size_t, gfp_t);
void * __must_check krealloc(const void *, size_t, gfp_t);
void kfree(const void *);
void kzfree(const void *);
size_t __ksize(const void *);
size_t ksize(const void *);

#ifdef CONFIG_HAVE_HARDENED_USERCOPY_ALLOCATOR
void __check_heap_object(const void *ptr, unsigned long n, struct page *page,
			bool to_user);
#else
static inline void __check_heap_object(const void *ptr, unsigned long n,
				       struct page *page, bool to_user) { }
#endif


#if defined(ARCH_DMA_MINALIGN) && ARCH_DMA_MINALIGN > 8
#define ARCH_KMALLOC_MINALIGN ARCH_DMA_MINALIGN
#define KMALLOC_MIN_SIZE ARCH_DMA_MINALIGN
#define KMALLOC_SHIFT_LOW ilog2(ARCH_DMA_MINALIGN)
#else
#define ARCH_KMALLOC_MINALIGN __alignof__(unsigned long long)
#endif


#ifndef ARCH_SLAB_MINALIGN
#define ARCH_SLAB_MINALIGN __alignof__(unsigned long long)
#endif


#define __assume_kmalloc_alignment __assume_aligned(ARCH_KMALLOC_MINALIGN)
#define __assume_slab_alignment __assume_aligned(ARCH_SLAB_MINALIGN)
#define __assume_page_alignment __assume_aligned(PAGE_SIZE)



#ifdef CONFIG_SLAB

#define KMALLOC_SHIFT_HIGH	((MAX_ORDER + PAGE_SHIFT - 1) <= 25 ? \
				(MAX_ORDER + PAGE_SHIFT - 1) : 25)
#define KMALLOC_SHIFT_MAX	KMALLOC_SHIFT_HIGH
#ifndef KMALLOC_SHIFT_LOW
#define KMALLOC_SHIFT_LOW	5
#endif
#endif

#ifdef CONFIG_SLUB

#define KMALLOC_SHIFT_HIGH	(PAGE_SHIFT + 1)
#define KMALLOC_SHIFT_MAX	(MAX_ORDER + PAGE_SHIFT - 1)
#ifndef KMALLOC_SHIFT_LOW
#define KMALLOC_SHIFT_LOW	3
#endif
#endif

#ifdef CONFIG_SLOB

#define KMALLOC_SHIFT_HIGH	PAGE_SHIFT
#define KMALLOC_SHIFT_MAX	(MAX_ORDER + PAGE_SHIFT - 1)
#ifndef KMALLOC_SHIFT_LOW
#define KMALLOC_SHIFT_LOW	3
#endif
#endif


#define KMALLOC_MAX_SIZE	(1UL << KMALLOC_SHIFT_MAX)

#define KMALLOC_MAX_CACHE_SIZE	(1UL << KMALLOC_SHIFT_HIGH)

#define KMALLOC_MAX_ORDER	(KMALLOC_SHIFT_MAX - PAGE_SHIFT)


#ifndef KMALLOC_MIN_SIZE
#define KMALLOC_MIN_SIZE (1 << KMALLOC_SHIFT_LOW)
#endif


#define SLAB_OBJ_MIN_SIZE      (KMALLOC_MIN_SIZE < 16 ? \
                               (KMALLOC_MIN_SIZE) : 16)


enum kmalloc_cache_type {
	KMALLOC_NORMAL = 0,
	KMALLOC_RECLAIM,
#ifdef CONFIG_ZONE_DMA
	KMALLOC_DMA,
#endif
	NR_KMALLOC_TYPES
};

#ifndef CONFIG_SLOB
extern struct kmem_cache *
kmalloc_caches[NR_KMALLOC_TYPES][KMALLOC_SHIFT_HIGH + 1];

static __always_inline enum kmalloc_cache_type kmalloc_type(gfp_t flags)
{
#ifdef CONFIG_ZONE_DMA
	
	if (likely((flags & (__GFP_DMA | __GFP_RECLAIMABLE)) == 0))
		return KMALLOC_NORMAL;

	
	return flags & __GFP_DMA ? KMALLOC_DMA : KMALLOC_RECLAIM;
#else
	return flags & __GFP_RECLAIMABLE ? KMALLOC_RECLAIM : KMALLOC_NORMAL;
#endif
}


static __always_inline unsigned int kmalloc_index(size_t size)
{
	if (!size)
		return 0;

	if (size <= KMALLOC_MIN_SIZE)
		return KMALLOC_SHIFT_LOW;

	if (KMALLOC_MIN_SIZE <= 32 && size > 64 && size <= 96)
		return 1;
	if (KMALLOC_MIN_SIZE <= 64 && size > 128 && size <= 192)
		return 2;
	if (size <=          8) return 3;
	if (size <=         16) return 4;
	if (size <=         32) return 5;
	if (size <=         64) return 6;
	if (size <=        128) return 7;
	if (size <=        256) return 8;
	if (size <=        512) return 9;
	if (size <=       1024) return 10;
	if (size <=   2 * 1024) return 11;
	if (size <=   4 * 1024) return 12;
	if (size <=   8 * 1024) return 13;
	if (size <=  16 * 1024) return 14;
	if (size <=  32 * 1024) return 15;
	if (size <=  64 * 1024) return 16;
	if (size <= 128 * 1024) return 17;
	if (size <= 256 * 1024) return 18;
	if (size <= 512 * 1024) return 19;
	if (size <= 1024 * 1024) return 20;
	if (size <=  2 * 1024 * 1024) return 21;
	if (size <=  4 * 1024 * 1024) return 22;
	if (size <=  8 * 1024 * 1024) return 23;
	if (size <=  16 * 1024 * 1024) return 24;
	if (size <=  32 * 1024 * 1024) return 25;
	if (size <=  64 * 1024 * 1024) return 26;
	BUG();

	
	return -1;
}
#endif 

void *__kmalloc(size_t size, gfp_t flags) __assume_kmalloc_alignment __malloc;
void *kmem_cache_alloc(struct kmem_cache *, gfp_t flags) __assume_slab_alignment __malloc;
void kmem_cache_free(struct kmem_cache *, void *);


void kmem_cache_free_bulk(struct kmem_cache *, size_t, void **);
int kmem_cache_alloc_bulk(struct kmem_cache *, gfp_t, size_t, void **);


static __always_inline void kfree_bulk(size_t size, void **p)
{
	kmem_cache_free_bulk(NULL, size, p);
}

#ifdef CONFIG_NUMA
void *__kmalloc_node(size_t size, gfp_t flags, int node) __assume_kmalloc_alignment __malloc;
void *kmem_cache_alloc_node(struct kmem_cache *, gfp_t flags, int node) __assume_slab_alignment __malloc;
#else
static __always_inline void *__kmalloc_node(size_t size, gfp_t flags, int node)
{
	return __kmalloc(size, flags);
}

static __always_inline void *kmem_cache_alloc_node(struct kmem_cache *s, gfp_t flags, int node)
{
	return kmem_cache_alloc(s, flags);
}
#endif

#ifdef CONFIG_TRACING
extern void *kmem_cache_alloc_trace(struct kmem_cache *, gfp_t, size_t) __assume_slab_alignment __malloc;

#ifdef CONFIG_NUMA
extern void *kmem_cache_alloc_node_trace(struct kmem_cache *s,
					   gfp_t gfpflags,
					   int node, size_t size) __assume_slab_alignment __malloc;
#else
static __always_inline void *
kmem_cache_alloc_node_trace(struct kmem_cache *s,
			      gfp_t gfpflags,
			      int node, size_t size)
{
	return kmem_cache_alloc_trace(s, gfpflags, size);
}
#endif 

#else 
static __always_inline void *kmem_cache_alloc_trace(struct kmem_cache *s,
		gfp_t flags, size_t size)
{
	void *ret = kmem_cache_alloc(s, flags);

	ret = kasan_kmalloc(s, ret, size, flags);
	return ret;
}

static __always_inline void *
kmem_cache_alloc_node_trace(struct kmem_cache *s,
			      gfp_t gfpflags,
			      int node, size_t size)
{
	void *ret = kmem_cache_alloc_node(s, gfpflags, node);

	ret = kasan_kmalloc(s, ret, size, gfpflags);
	return ret;
}
#endif 

extern void *kmalloc_order(size_t size, gfp_t flags, unsigned int order) __assume_page_alignment __malloc;

#ifdef CONFIG_TRACING
extern void *kmalloc_order_trace(size_t size, gfp_t flags, unsigned int order) __assume_page_alignment __malloc;
#else
static __always_inline void *
kmalloc_order_trace(size_t size, gfp_t flags, unsigned int order)
{
	return kmalloc_order(size, flags, order);
}
#endif

static __always_inline void *kmalloc_large(size_t size, gfp_t flags)
{
	unsigned int order = get_order(size);
	return kmalloc_order_trace(size, flags, order);
}


static __always_inline void *kmalloc(size_t size, gfp_t flags)
{
	if (__builtin_constant_p(size)) {
#ifndef CONFIG_SLOB
		unsigned int index;
#endif
		if (size > KMALLOC_MAX_CACHE_SIZE)
			return kmalloc_large(size, flags);
#ifndef CONFIG_SLOB
		index = kmalloc_index(size);

		if (!index)
			return ZERO_SIZE_PTR;

		return kmem_cache_alloc_trace(
				kmalloc_caches[kmalloc_type(flags)][index],
				flags, size);
#endif
	}
	return __kmalloc(size, flags);
}


static __always_inline unsigned int kmalloc_size(unsigned int n)
{
#ifndef CONFIG_SLOB
	if (n > 2)
		return 1U << n;

	if (n == 1 && KMALLOC_MIN_SIZE <= 32)
		return 96;

	if (n == 2 && KMALLOC_MIN_SIZE <= 64)
		return 192;
#endif
	return 0;
}

static __always_inline void *kmalloc_node(size_t size, gfp_t flags, int node)
{
#ifndef CONFIG_SLOB
	if (__builtin_constant_p(size) &&
		size <= KMALLOC_MAX_CACHE_SIZE) {
		unsigned int i = kmalloc_index(size);

		if (!i)
			return ZERO_SIZE_PTR;

		return kmem_cache_alloc_node_trace(
				kmalloc_caches[kmalloc_type(flags)][i],
						flags, node, size);
	}
#endif
	return __kmalloc_node(size, flags, node);
}

struct memcg_cache_array {
	struct rcu_head rcu;
	struct kmem_cache *entries[0];
};


struct memcg_cache_params {
	struct kmem_cache *root_cache;
	union {
		struct {
			struct memcg_cache_array __rcu *memcg_caches;
			struct list_head __root_caches_node;
			struct list_head children;
			bool dying;
		};
		struct {
			struct mem_cgroup *memcg;
			struct list_head children_node;
			struct list_head kmem_caches_node;
			struct percpu_ref refcnt;

			void (*work_fn)(struct kmem_cache *);
			union {
				struct rcu_head rcu_head;
				struct work_struct work;
			};
		};
	};
};

int memcg_update_all_caches(int num_memcgs);


static inline void *kmalloc_array(size_t n, size_t size, gfp_t flags)
{
	size_t bytes;

	if (unlikely(check_mul_overflow(n, size, &bytes)))
		return NULL;
	if (__builtin_constant_p(n) && __builtin_constant_p(size))
		return kmalloc(bytes, flags);
	return __kmalloc(bytes, flags);
}


static inline void *kcalloc(size_t n, size_t size, gfp_t flags)
{
	return kmalloc_array(n, size, flags | __GFP_ZERO);
}


extern void *__kmalloc_track_caller(size_t, gfp_t, unsigned long);
#define kmalloc_track_caller(size, flags) \
	__kmalloc_track_caller(size, flags, _RET_IP_)

static inline void *kmalloc_array_node(size_t n, size_t size, gfp_t flags,
				       int node)
{
	size_t bytes;

	if (unlikely(check_mul_overflow(n, size, &bytes)))
		return NULL;
	if (__builtin_constant_p(n) && __builtin_constant_p(size))
		return kmalloc_node(bytes, flags, node);
	return __kmalloc_node(bytes, flags, node);
}

static inline void *kcalloc_node(size_t n, size_t size, gfp_t flags, int node)
{
	return kmalloc_array_node(n, size, flags | __GFP_ZERO, node);
}


#ifdef CONFIG_NUMA
extern void *__kmalloc_node_track_caller(size_t, gfp_t, int, unsigned long);
#define kmalloc_node_track_caller(size, flags, node) \
	__kmalloc_node_track_caller(size, flags, node, \
			_RET_IP_)

#else 

#define kmalloc_node_track_caller(size, flags, node) \
	kmalloc_track_caller(size, flags)

#endif 


static inline void *kmem_cache_zalloc(struct kmem_cache *k, gfp_t flags)
{
	return kmem_cache_alloc(k, flags | __GFP_ZERO);
}


static inline void *kzalloc(size_t size, gfp_t flags)
{
	return kmalloc(size, flags | __GFP_ZERO);
}


static inline void *kzalloc_node(size_t size, gfp_t flags, int node)
{
	return kmalloc_node(size, flags | __GFP_ZERO, node);
}

unsigned int kmem_cache_size(struct kmem_cache *s);
void __init kmem_cache_init_late(void);

#if defined(CONFIG_SMP) && defined(CONFIG_SLAB)
int slab_prepare_cpu(unsigned int cpu);
int slab_dead_cpu(unsigned int cpu);
#else
#define slab_prepare_cpu	NULL
#define slab_dead_cpu		NULL
#endif

#endif	
