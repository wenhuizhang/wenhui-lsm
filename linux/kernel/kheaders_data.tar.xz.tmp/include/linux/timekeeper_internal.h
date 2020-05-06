/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _LINUX_TIMEKEEPER_INTERNAL_H
#define _LINUX_TIMEKEEPER_INTERNAL_H

#include <linux/clocksource.h>
#include <linux/jiffies.h>
#include <linux/time.h>


struct tk_read_base {
	struct clocksource	*clock;
	u64			mask;
	u64			cycle_last;
	u32			mult;
	u32			shift;
	u64			xtime_nsec;
	ktime_t			base;
	u64			base_real;
};


struct timekeeper {
	struct tk_read_base	tkr_mono;
	struct tk_read_base	tkr_raw;
	u64			xtime_sec;
	unsigned long		ktime_sec;
	struct timespec64	wall_to_monotonic;
	ktime_t			offs_real;
	ktime_t			offs_boot;
	ktime_t			offs_tai;
	s32			tai_offset;
	unsigned int		clock_was_set_seq;
	u8			cs_was_changed_seq;
	ktime_t			next_leap_ktime;
	u64			raw_sec;
	struct timespec64	monotonic_to_boot;

	
	u64			cycle_interval;
	u64			xtime_interval;
	s64			xtime_remainder;
	u64			raw_interval;
	
	u64			ntp_tick;
	
	s64			ntp_error;
	u32			ntp_error_shift;
	u32			ntp_err_mult;
	
	u32			skip_second_overflow;
#ifdef CONFIG_DEBUG_TIMEKEEPING
	long			last_warning;
	
	int			underflow_seen;
	int			overflow_seen;
#endif
};

#ifdef CONFIG_GENERIC_TIME_VSYSCALL

extern void update_vsyscall(struct timekeeper *tk);
extern void update_vsyscall_tz(void);

#else

static inline void update_vsyscall(struct timekeeper *tk)
{
}
static inline void update_vsyscall_tz(void)
{
}
#endif

#endif 
