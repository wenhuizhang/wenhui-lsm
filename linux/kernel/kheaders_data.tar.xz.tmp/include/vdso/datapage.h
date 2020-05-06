/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __VDSO_DATAPAGE_H
#define __VDSO_DATAPAGE_H

#ifndef __ASSEMBLY__

#include <linux/bits.h>
#include <linux/time.h>
#include <linux/types.h>

#define VDSO_BASES	(CLOCK_TAI + 1)
#define VDSO_HRES	(BIT(CLOCK_REALTIME)		| \
			 BIT(CLOCK_MONOTONIC)		| \
			 BIT(CLOCK_BOOTTIME)		| \
			 BIT(CLOCK_TAI))
#define VDSO_COARSE	(BIT(CLOCK_REALTIME_COARSE)	| \
			 BIT(CLOCK_MONOTONIC_COARSE))
#define VDSO_RAW	(BIT(CLOCK_MONOTONIC_RAW))

#define CS_HRES_COARSE	0
#define CS_RAW		1
#define CS_BASES	(CS_RAW + 1)


struct vdso_timestamp {
	u64	sec;
	u64	nsec;
};


struct vdso_data {
	u32			seq;

	s32			clock_mode;
	u64			cycle_last;
	u64			mask;
	u32			mult;
	u32			shift;

	struct vdso_timestamp	basetime[VDSO_BASES];

	s32			tz_minuteswest;
	s32			tz_dsttime;
	u32			hrtimer_res;
	u32			__unused;
};


extern struct vdso_data _vdso_data[CS_BASES] __attribute__((visibility("hidden")));

#endif 

#endif 
