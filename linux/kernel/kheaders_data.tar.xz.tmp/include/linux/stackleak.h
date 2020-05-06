/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_STACKLEAK_H
#define _LINUX_STACKLEAK_H

#include <linux/sched.h>
#include <linux/sched/task_stack.h>


#define STACKLEAK_POISON -0xBEEF
#define STACKLEAK_SEARCH_DEPTH 128

#ifdef CONFIG_GCC_PLUGIN_STACKLEAK
#include <asm/stacktrace.h>

static inline void stackleak_task_init(struct task_struct *t)
{
	t->lowest_stack = (unsigned long)end_of_stack(t) + sizeof(unsigned long);
# ifdef CONFIG_STACKLEAK_METRICS
	t->prev_lowest_stack = t->lowest_stack;
# endif
}

#ifdef CONFIG_STACKLEAK_RUNTIME_DISABLE
int stack_erasing_sysctl(struct ctl_table *table, int write,
			void __user *buffer, size_t *lenp, loff_t *ppos);
#endif

#else 
static inline void stackleak_task_init(struct task_struct *t) { }
#endif

#endif
