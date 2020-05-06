/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _LINUX_ATOMIC_H
#define _LINUX_ATOMIC_H
#include <linux/types.h>

#include <asm/atomic.h>
#include <asm/barrier.h>




#ifndef __atomic_acquire_fence
#define __atomic_acquire_fence		smp_mb__after_atomic
#endif

#ifndef __atomic_release_fence
#define __atomic_release_fence		smp_mb__before_atomic
#endif

#ifndef __atomic_pre_full_fence
#define __atomic_pre_full_fence		smp_mb__before_atomic
#endif

#ifndef __atomic_post_full_fence
#define __atomic_post_full_fence	smp_mb__after_atomic
#endif

#define __atomic_op_acquire(op, args...)				\
({									\
	typeof(op##_relaxed(args)) __ret  = op##_relaxed(args);		\
	__atomic_acquire_fence();					\
	__ret;								\
})

#define __atomic_op_release(op, args...)				\
({									\
	__atomic_release_fence();					\
	op##_relaxed(args);						\
})

#define __atomic_op_fence(op, args...)					\
({									\
	typeof(op##_relaxed(args)) __ret;				\
	__atomic_pre_full_fence();					\
	__ret = op##_relaxed(args);					\
	__atomic_post_full_fence();					\
	__ret;								\
})

#include <linux/atomic-fallback.h>

#include <asm-generic/atomic-long.h>

#endif 
