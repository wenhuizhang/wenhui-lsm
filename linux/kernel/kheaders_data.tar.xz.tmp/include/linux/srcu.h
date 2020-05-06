/* SPDX-License-Identifier: GPL-2.0+ */


#ifndef _LINUX_SRCU_H
#define _LINUX_SRCU_H

#include <linux/mutex.h>
#include <linux/rcupdate.h>
#include <linux/workqueue.h>
#include <linux/rcu_segcblist.h>

struct srcu_struct;

#ifdef CONFIG_DEBUG_LOCK_ALLOC

int __init_srcu_struct(struct srcu_struct *ssp, const char *name,
		       struct lock_class_key *key);

#define init_srcu_struct(ssp) \
({ \
	static struct lock_class_key __srcu_key; \
	\
	__init_srcu_struct((ssp), #ssp, &__srcu_key); \
})

#define __SRCU_DEP_MAP_INIT(srcu_name)	.dep_map = { .name = #srcu_name },
#else 

int init_srcu_struct(struct srcu_struct *ssp);

#define __SRCU_DEP_MAP_INIT(srcu_name)
#endif 

#ifdef CONFIG_TINY_SRCU
#include <linux/srcutiny.h>
#elif defined(CONFIG_TREE_SRCU)
#include <linux/srcutree.h>
#elif defined(CONFIG_SRCU)
#error "Unknown SRCU implementation specified to kernel configuration"
#else

struct srcu_struct { };
#endif

void call_srcu(struct srcu_struct *ssp, struct rcu_head *head,
		void (*func)(struct rcu_head *head));
void cleanup_srcu_struct(struct srcu_struct *ssp);
int __srcu_read_lock(struct srcu_struct *ssp) __acquires(ssp);
void __srcu_read_unlock(struct srcu_struct *ssp, int idx) __releases(ssp);
void synchronize_srcu(struct srcu_struct *ssp);

#ifdef CONFIG_DEBUG_LOCK_ALLOC


static inline int srcu_read_lock_held(const struct srcu_struct *ssp)
{
	if (!debug_lockdep_rcu_enabled())
		return 1;
	return lock_is_held(&ssp->dep_map);
}

#else 

static inline int srcu_read_lock_held(const struct srcu_struct *ssp)
{
	return 1;
}

#endif 


#define srcu_dereference_check(p, ssp, c) \
	__rcu_dereference_check((p), (c) || srcu_read_lock_held(ssp), __rcu)


#define srcu_dereference(p, ssp) srcu_dereference_check((p), (ssp), 0)


#define srcu_dereference_notrace(p, ssp) srcu_dereference_check((p), (ssp), 1)


static inline int srcu_read_lock(struct srcu_struct *ssp) __acquires(ssp)
{
	int retval;

	retval = __srcu_read_lock(ssp);
	rcu_lock_acquire(&(ssp)->dep_map);
	return retval;
}


static inline notrace int
srcu_read_lock_notrace(struct srcu_struct *ssp) __acquires(ssp)
{
	int retval;

	retval = __srcu_read_lock(ssp);
	return retval;
}


static inline void srcu_read_unlock(struct srcu_struct *ssp, int idx)
	__releases(ssp)
{
	WARN_ON_ONCE(idx & ~0x1);
	rcu_lock_release(&(ssp)->dep_map);
	__srcu_read_unlock(ssp, idx);
}


static inline notrace void
srcu_read_unlock_notrace(struct srcu_struct *ssp, int idx) __releases(ssp)
{
	__srcu_read_unlock(ssp, idx);
}


static inline void smp_mb__after_srcu_read_unlock(void)
{
	
}

#endif
