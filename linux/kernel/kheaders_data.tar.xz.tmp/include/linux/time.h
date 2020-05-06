/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TIME_H
#define _LINUX_TIME_H

# include <linux/cache.h>
# include <linux/seqlock.h>
# include <linux/math64.h>
# include <linux/time64.h>

extern struct timezone sys_tz;

int get_timespec64(struct timespec64 *ts,
		const struct __kernel_timespec __user *uts);
int put_timespec64(const struct timespec64 *ts,
		struct __kernel_timespec __user *uts);
int get_itimerspec64(struct itimerspec64 *it,
			const struct __kernel_itimerspec __user *uit);
int put_itimerspec64(const struct itimerspec64 *it,
			struct __kernel_itimerspec __user *uit);

extern time64_t mktime64(const unsigned int year, const unsigned int mon,
			const unsigned int day, const unsigned int hour,
			const unsigned int min, const unsigned int sec);


#ifdef CONFIG_ARCH_USES_GETTIMEOFFSET
extern u32 (*arch_gettimeoffset)(void);
#endif

struct itimerval;
extern int do_setitimer(int which, struct itimerval *value,
			struct itimerval *ovalue);
extern int do_getitimer(int which, struct itimerval *value);

extern long do_utimes(int dfd, const char __user *filename, struct timespec64 *times, int flags);


struct tm {
	
	int tm_sec;
	
	int tm_min;
	
	int tm_hour;
	
	int tm_mday;
	
	int tm_mon;
	
	long tm_year;
	
	int tm_wday;
	
	int tm_yday;
};

void time64_to_tm(time64_t totalsecs, int offset, struct tm *result);

# include <linux/time32.h>

static inline bool itimerspec64_valid(const struct itimerspec64 *its)
{
	if (!timespec64_valid(&(its->it_interval)) ||
		!timespec64_valid(&(its->it_value)))
		return false;

	return true;
}


#define time_after32(a, b)	((s32)((u32)(b) - (u32)(a)) < 0)
#define time_before32(b, a)	time_after32(a, b)
#endif
