/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TRACE_CLOCK_H
#define _LINUX_TRACE_CLOCK_H


#include <linux/compiler.h>
#include <linux/types.h>

#include <asm/trace_clock.h>

extern u64 notrace trace_clock_local(void);
extern u64 notrace trace_clock(void);
extern u64 notrace trace_clock_jiffies(void);
extern u64 notrace trace_clock_global(void);
extern u64 notrace trace_clock_counter(void);

#endif 
