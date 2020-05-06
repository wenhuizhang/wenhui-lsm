/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef _LINUX_PTR_RING_H
#define _LINUX_PTR_RING_H 1

#ifdef __KERNEL__
#include <linux/spinlock.h>
#include <linux/cache.h>
#include <linux/types.h>
#include <linux/compiler.h>
#include <linux/slab.h>
#include <asm/errno.h>
#endif

struct ptr_ring {
	int producer ____cacheline_aligned_in_smp;
	spinlock_t producer_lock;
	int consumer_head ____cacheline_aligned_in_smp; 
	int consumer_tail; 
	spinlock_t consumer_lock;
	
	
	int size ____cacheline_aligned_in_smp; 
	int batch; 
	void **queue;
};


static inline bool __ptr_ring_full(struct ptr_ring *r)
{
	return r->queue[r->producer];
}

static inline bool ptr_ring_full(struct ptr_ring *r)
{
	bool ret;

	spin_lock(&r->producer_lock);
	ret = __ptr_ring_full(r);
	spin_unlock(&r->producer_lock);

	return ret;
}

static inline bool ptr_ring_full_irq(struct ptr_ring *r)
{
	bool ret;

	spin_lock_irq(&r->producer_lock);
	ret = __ptr_ring_full(r);
	spin_unlock_irq(&r->producer_lock);

	return ret;
}

static inline bool ptr_ring_full_any(struct ptr_ring *r)
{
	unsigned long flags;
	bool ret;

	spin_lock_irqsave(&r->producer_lock, flags);
	ret = __ptr_ring_full(r);
	spin_unlock_irqrestore(&r->producer_lock, flags);

	return ret;
}

static inline bool ptr_ring_full_bh(struct ptr_ring *r)
{
	bool ret;

	spin_lock_bh(&r->producer_lock);
	ret = __ptr_ring_full(r);
	spin_unlock_bh(&r->producer_lock);

	return ret;
}


static inline int __ptr_ring_produce(struct ptr_ring *r, void *ptr)
{
	if (unlikely(!r->size) || r->queue[r->producer])
		return -ENOSPC;

	
	
	smp_wmb();

	WRITE_ONCE(r->queue[r->producer++], ptr);
	if (unlikely(r->producer >= r->size))
		r->producer = 0;
	return 0;
}


static inline int ptr_ring_produce(struct ptr_ring *r, void *ptr)
{
	int ret;

	spin_lock(&r->producer_lock);
	ret = __ptr_ring_produce(r, ptr);
	spin_unlock(&r->producer_lock);

	return ret;
}

static inline int ptr_ring_produce_irq(struct ptr_ring *r, void *ptr)
{
	int ret;

	spin_lock_irq(&r->producer_lock);
	ret = __ptr_ring_produce(r, ptr);
	spin_unlock_irq(&r->producer_lock);

	return ret;
}

static inline int ptr_ring_produce_any(struct ptr_ring *r, void *ptr)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&r->producer_lock, flags);
	ret = __ptr_ring_produce(r, ptr);
	spin_unlock_irqrestore(&r->producer_lock, flags);

	return ret;
}

static inline int ptr_ring_produce_bh(struct ptr_ring *r, void *ptr)
{
	int ret;

	spin_lock_bh(&r->producer_lock);
	ret = __ptr_ring_produce(r, ptr);
	spin_unlock_bh(&r->producer_lock);

	return ret;
}

static inline void *__ptr_ring_peek(struct ptr_ring *r)
{
	if (likely(r->size))
		return READ_ONCE(r->queue[r->consumer_head]);
	return NULL;
}


static inline bool __ptr_ring_empty(struct ptr_ring *r)
{
	if (likely(r->size))
		return !r->queue[READ_ONCE(r->consumer_head)];
	return true;
}

static inline bool ptr_ring_empty(struct ptr_ring *r)
{
	bool ret;

	spin_lock(&r->consumer_lock);
	ret = __ptr_ring_empty(r);
	spin_unlock(&r->consumer_lock);

	return ret;
}

static inline bool ptr_ring_empty_irq(struct ptr_ring *r)
{
	bool ret;

	spin_lock_irq(&r->consumer_lock);
	ret = __ptr_ring_empty(r);
	spin_unlock_irq(&r->consumer_lock);

	return ret;
}

static inline bool ptr_ring_empty_any(struct ptr_ring *r)
{
	unsigned long flags;
	bool ret;

	spin_lock_irqsave(&r->consumer_lock, flags);
	ret = __ptr_ring_empty(r);
	spin_unlock_irqrestore(&r->consumer_lock, flags);

	return ret;
}

static inline bool ptr_ring_empty_bh(struct ptr_ring *r)
{
	bool ret;

	spin_lock_bh(&r->consumer_lock);
	ret = __ptr_ring_empty(r);
	spin_unlock_bh(&r->consumer_lock);

	return ret;
}


static inline void __ptr_ring_discard_one(struct ptr_ring *r)
{
	
	
	int consumer_head = r->consumer_head;
	int head = consumer_head++;

	
	if (unlikely(consumer_head - r->consumer_tail >= r->batch ||
		     consumer_head >= r->size)) {
		
		while (likely(head >= r->consumer_tail))
			r->queue[head--] = NULL;
		r->consumer_tail = consumer_head;
	}
	if (unlikely(consumer_head >= r->size)) {
		consumer_head = 0;
		r->consumer_tail = 0;
	}
	
	WRITE_ONCE(r->consumer_head, consumer_head);
}

static inline void *__ptr_ring_consume(struct ptr_ring *r)
{
	void *ptr;

	
	ptr = __ptr_ring_peek(r);
	if (ptr)
		__ptr_ring_discard_one(r);

	return ptr;
}

static inline int __ptr_ring_consume_batched(struct ptr_ring *r,
					     void **array, int n)
{
	void *ptr;
	int i;

	for (i = 0; i < n; i++) {
		ptr = __ptr_ring_consume(r);
		if (!ptr)
			break;
		array[i] = ptr;
	}

	return i;
}


static inline void *ptr_ring_consume(struct ptr_ring *r)
{
	void *ptr;

	spin_lock(&r->consumer_lock);
	ptr = __ptr_ring_consume(r);
	spin_unlock(&r->consumer_lock);

	return ptr;
}

static inline void *ptr_ring_consume_irq(struct ptr_ring *r)
{
	void *ptr;

	spin_lock_irq(&r->consumer_lock);
	ptr = __ptr_ring_consume(r);
	spin_unlock_irq(&r->consumer_lock);

	return ptr;
}

static inline void *ptr_ring_consume_any(struct ptr_ring *r)
{
	unsigned long flags;
	void *ptr;

	spin_lock_irqsave(&r->consumer_lock, flags);
	ptr = __ptr_ring_consume(r);
	spin_unlock_irqrestore(&r->consumer_lock, flags);

	return ptr;
}

static inline void *ptr_ring_consume_bh(struct ptr_ring *r)
{
	void *ptr;

	spin_lock_bh(&r->consumer_lock);
	ptr = __ptr_ring_consume(r);
	spin_unlock_bh(&r->consumer_lock);

	return ptr;
}

static inline int ptr_ring_consume_batched(struct ptr_ring *r,
					   void **array, int n)
{
	int ret;

	spin_lock(&r->consumer_lock);
	ret = __ptr_ring_consume_batched(r, array, n);
	spin_unlock(&r->consumer_lock);

	return ret;
}

static inline int ptr_ring_consume_batched_irq(struct ptr_ring *r,
					       void **array, int n)
{
	int ret;

	spin_lock_irq(&r->consumer_lock);
	ret = __ptr_ring_consume_batched(r, array, n);
	spin_unlock_irq(&r->consumer_lock);

	return ret;
}

static inline int ptr_ring_consume_batched_any(struct ptr_ring *r,
					       void **array, int n)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&r->consumer_lock, flags);
	ret = __ptr_ring_consume_batched(r, array, n);
	spin_unlock_irqrestore(&r->consumer_lock, flags);

	return ret;
}

static inline int ptr_ring_consume_batched_bh(struct ptr_ring *r,
					      void **array, int n)
{
	int ret;

	spin_lock_bh(&r->consumer_lock);
	ret = __ptr_ring_consume_batched(r, array, n);
	spin_unlock_bh(&r->consumer_lock);

	return ret;
}


#define __PTR_RING_PEEK_CALL(r, f) ((f)(__ptr_ring_peek(r)))

#define PTR_RING_PEEK_CALL(r, f) ({ \
	typeof((f)(NULL)) __PTR_RING_PEEK_CALL_v; \
	\
	spin_lock(&(r)->consumer_lock); \
	__PTR_RING_PEEK_CALL_v = __PTR_RING_PEEK_CALL(r, f); \
	spin_unlock(&(r)->consumer_lock); \
	__PTR_RING_PEEK_CALL_v; \
})

#define PTR_RING_PEEK_CALL_IRQ(r, f) ({ \
	typeof((f)(NULL)) __PTR_RING_PEEK_CALL_v; \
	\
	spin_lock_irq(&(r)->consumer_lock); \
	__PTR_RING_PEEK_CALL_v = __PTR_RING_PEEK_CALL(r, f); \
	spin_unlock_irq(&(r)->consumer_lock); \
	__PTR_RING_PEEK_CALL_v; \
})

#define PTR_RING_PEEK_CALL_BH(r, f) ({ \
	typeof((f)(NULL)) __PTR_RING_PEEK_CALL_v; \
	\
	spin_lock_bh(&(r)->consumer_lock); \
	__PTR_RING_PEEK_CALL_v = __PTR_RING_PEEK_CALL(r, f); \
	spin_unlock_bh(&(r)->consumer_lock); \
	__PTR_RING_PEEK_CALL_v; \
})

#define PTR_RING_PEEK_CALL_ANY(r, f) ({ \
	typeof((f)(NULL)) __PTR_RING_PEEK_CALL_v; \
	unsigned long __PTR_RING_PEEK_CALL_f;\
	\
	spin_lock_irqsave(&(r)->consumer_lock, __PTR_RING_PEEK_CALL_f); \
	__PTR_RING_PEEK_CALL_v = __PTR_RING_PEEK_CALL(r, f); \
	spin_unlock_irqrestore(&(r)->consumer_lock, __PTR_RING_PEEK_CALL_f); \
	__PTR_RING_PEEK_CALL_v; \
})


static inline void **__ptr_ring_init_queue_alloc(unsigned int size, gfp_t gfp)
{
	if (size > KMALLOC_MAX_SIZE / sizeof(void *))
		return NULL;
	return kvmalloc_array(size, sizeof(void *), gfp | __GFP_ZERO);
}

static inline void __ptr_ring_set_size(struct ptr_ring *r, int size)
{
	r->size = size;
	r->batch = SMP_CACHE_BYTES * 2 / sizeof(*(r->queue));
	
	if (r->batch > r->size / 2 || !r->batch)
		r->batch = 1;
}

static inline int ptr_ring_init(struct ptr_ring *r, int size, gfp_t gfp)
{
	r->queue = __ptr_ring_init_queue_alloc(size, gfp);
	if (!r->queue)
		return -ENOMEM;

	__ptr_ring_set_size(r, size);
	r->producer = r->consumer_head = r->consumer_tail = 0;
	spin_lock_init(&r->producer_lock);
	spin_lock_init(&r->consumer_lock);

	return 0;
}


static inline void ptr_ring_unconsume(struct ptr_ring *r, void **batch, int n,
				      void (*destroy)(void *))
{
	unsigned long flags;
	int head;

	spin_lock_irqsave(&r->consumer_lock, flags);
	spin_lock(&r->producer_lock);

	if (!r->size)
		goto done;

	
	head = r->consumer_head - 1;
	while (likely(head >= r->consumer_tail))
		r->queue[head--] = NULL;
	r->consumer_tail = r->consumer_head;

	
	while (n) {
		head = r->consumer_head - 1;
		if (head < 0)
			head = r->size - 1;
		if (r->queue[head]) {
			
			goto done;
		}
		r->queue[head] = batch[--n];
		r->consumer_tail = head;
		
		WRITE_ONCE(r->consumer_head, head);
	}

done:
	
	while (n)
		destroy(batch[--n]);
	spin_unlock(&r->producer_lock);
	spin_unlock_irqrestore(&r->consumer_lock, flags);
}

static inline void **__ptr_ring_swap_queue(struct ptr_ring *r, void **queue,
					   int size, gfp_t gfp,
					   void (*destroy)(void *))
{
	int producer = 0;
	void **old;
	void *ptr;

	while ((ptr = __ptr_ring_consume(r)))
		if (producer < size)
			queue[producer++] = ptr;
		else if (destroy)
			destroy(ptr);

	if (producer >= size)
		producer = 0;
	__ptr_ring_set_size(r, size);
	r->producer = producer;
	r->consumer_head = 0;
	r->consumer_tail = 0;
	old = r->queue;
	r->queue = queue;

	return old;
}


static inline int ptr_ring_resize(struct ptr_ring *r, int size, gfp_t gfp,
				  void (*destroy)(void *))
{
	unsigned long flags;
	void **queue = __ptr_ring_init_queue_alloc(size, gfp);
	void **old;

	if (!queue)
		return -ENOMEM;

	spin_lock_irqsave(&(r)->consumer_lock, flags);
	spin_lock(&(r)->producer_lock);

	old = __ptr_ring_swap_queue(r, queue, size, gfp, destroy);

	spin_unlock(&(r)->producer_lock);
	spin_unlock_irqrestore(&(r)->consumer_lock, flags);

	kvfree(old);

	return 0;
}


static inline int ptr_ring_resize_multiple(struct ptr_ring **rings,
					   unsigned int nrings,
					   int size,
					   gfp_t gfp, void (*destroy)(void *))
{
	unsigned long flags;
	void ***queues;
	int i;

	queues = kmalloc_array(nrings, sizeof(*queues), gfp);
	if (!queues)
		goto noqueues;

	for (i = 0; i < nrings; ++i) {
		queues[i] = __ptr_ring_init_queue_alloc(size, gfp);
		if (!queues[i])
			goto nomem;
	}

	for (i = 0; i < nrings; ++i) {
		spin_lock_irqsave(&(rings[i])->consumer_lock, flags);
		spin_lock(&(rings[i])->producer_lock);
		queues[i] = __ptr_ring_swap_queue(rings[i], queues[i],
						  size, gfp, destroy);
		spin_unlock(&(rings[i])->producer_lock);
		spin_unlock_irqrestore(&(rings[i])->consumer_lock, flags);
	}

	for (i = 0; i < nrings; ++i)
		kvfree(queues[i]);

	kfree(queues);

	return 0;

nomem:
	while (--i >= 0)
		kvfree(queues[i]);

	kfree(queues);

noqueues:
	return -ENOMEM;
}

static inline void ptr_ring_cleanup(struct ptr_ring *r, void (*destroy)(void *))
{
	void *ptr;

	if (destroy)
		while ((ptr = ptr_ring_consume(r)))
			destroy(ptr);
	kvfree(r->queue);
}

#endif 
