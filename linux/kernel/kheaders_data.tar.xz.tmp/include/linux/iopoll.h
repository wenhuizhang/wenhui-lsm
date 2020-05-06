/* SPDX-License-Identifier: GPL-2.0-only */


#ifndef _LINUX_IOPOLL_H
#define _LINUX_IOPOLL_H

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/ktime.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/io.h>


#define readx_poll_timeout(op, addr, val, cond, sleep_us, timeout_us)	\
({ \
	u64 __timeout_us = (timeout_us); \
	unsigned long __sleep_us = (sleep_us); \
	ktime_t __timeout = ktime_add_us(ktime_get(), __timeout_us); \
	might_sleep_if((__sleep_us) != 0); \
	for (;;) { \
		(val) = op(addr); \
		if (cond) \
			break; \
		if (__timeout_us && \
		    ktime_compare(ktime_get(), __timeout) > 0) { \
			(val) = op(addr); \
			break; \
		} \
		if (__sleep_us) \
			usleep_range((__sleep_us >> 2) + 1, __sleep_us); \
	} \
	(cond) ? 0 : -ETIMEDOUT; \
})


#define readx_poll_timeout_atomic(op, addr, val, cond, delay_us, timeout_us) \
({ \
	u64 __timeout_us = (timeout_us); \
	unsigned long __delay_us = (delay_us); \
	ktime_t __timeout = ktime_add_us(ktime_get(), __timeout_us); \
	for (;;) { \
		(val) = op(addr); \
		if (cond) \
			break; \
		if (__timeout_us && \
		    ktime_compare(ktime_get(), __timeout) > 0) { \
			(val) = op(addr); \
			break; \
		} \
		if (__delay_us) \
			udelay(__delay_us);	\
	} \
	(cond) ? 0 : -ETIMEDOUT; \
})


#define readb_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readb, addr, val, cond, delay_us, timeout_us)

#define readb_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readb, addr, val, cond, delay_us, timeout_us)

#define readw_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readw, addr, val, cond, delay_us, timeout_us)

#define readw_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readw, addr, val, cond, delay_us, timeout_us)

#define readl_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readl, addr, val, cond, delay_us, timeout_us)

#define readl_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readl, addr, val, cond, delay_us, timeout_us)

#define readq_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readq, addr, val, cond, delay_us, timeout_us)

#define readq_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readq, addr, val, cond, delay_us, timeout_us)

#define readb_relaxed_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readb_relaxed, addr, val, cond, delay_us, timeout_us)

#define readb_relaxed_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readb_relaxed, addr, val, cond, delay_us, timeout_us)

#define readw_relaxed_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readw_relaxed, addr, val, cond, delay_us, timeout_us)

#define readw_relaxed_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readw_relaxed, addr, val, cond, delay_us, timeout_us)

#define readl_relaxed_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readl_relaxed, addr, val, cond, delay_us, timeout_us)

#define readl_relaxed_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readl_relaxed, addr, val, cond, delay_us, timeout_us)

#define readq_relaxed_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readq_relaxed, addr, val, cond, delay_us, timeout_us)

#define readq_relaxed_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readq_relaxed, addr, val, cond, delay_us, timeout_us)

#endif 
