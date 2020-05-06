/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_FUTEX_H
#define _LINUX_FUTEX_H

#include <linux/ktime.h>
#include <uapi/linux/futex.h>

struct inode;
struct mm_struct;
struct task_struct;



#define FUT_OFF_INODE    1 
#define FUT_OFF_MMSHARED 2 

union futex_key {
	struct {
		unsigned long pgoff;
		struct inode *inode;
		int offset;
	} shared;
	struct {
		unsigned long address;
		struct mm_struct *mm;
		int offset;
	} private;
	struct {
		unsigned long word;
		void *ptr;
		int offset;
	} both;
};

#define FUTEX_KEY_INIT (union futex_key) { .both = { .ptr = NULL } }

#ifdef CONFIG_FUTEX
extern void exit_robust_list(struct task_struct *curr);

long do_futex(u32 __user *uaddr, int op, u32 val, ktime_t *timeout,
	      u32 __user *uaddr2, u32 val2, u32 val3);
#else
static inline void exit_robust_list(struct task_struct *curr)
{
}

static inline long do_futex(u32 __user *uaddr, int op, u32 val,
			    ktime_t *timeout, u32 __user *uaddr2,
			    u32 val2, u32 val3)
{
	return -EINVAL;
}
#endif

#ifdef CONFIG_FUTEX_PI
extern void exit_pi_state_list(struct task_struct *curr);
#else
static inline void exit_pi_state_list(struct task_struct *curr)
{
}
#endif

#endif
