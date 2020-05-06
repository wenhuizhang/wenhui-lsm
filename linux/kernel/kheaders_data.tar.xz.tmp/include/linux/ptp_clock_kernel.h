/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef _PTP_CLOCK_KERNEL_H_
#define _PTP_CLOCK_KERNEL_H_

#include <linux/device.h>
#include <linux/pps_kernel.h>
#include <linux/ptp_clock.h>


struct ptp_clock_request {
	enum {
		PTP_CLK_REQ_EXTTS,
		PTP_CLK_REQ_PEROUT,
		PTP_CLK_REQ_PPS,
	} type;
	union {
		struct ptp_extts_request extts;
		struct ptp_perout_request perout;
	};
};

struct system_device_crosststamp;


struct ptp_system_timestamp {
	struct timespec64 pre_ts;
	struct timespec64 post_ts;
};



struct ptp_clock_info {
	struct module *owner;
	char name[16];
	s32 max_adj;
	int n_alarm;
	int n_ext_ts;
	int n_per_out;
	int n_pins;
	int pps;
	struct ptp_pin_desc *pin_config;
	int (*adjfine)(struct ptp_clock_info *ptp, long scaled_ppm);
	int (*adjfreq)(struct ptp_clock_info *ptp, s32 delta);
	int (*adjtime)(struct ptp_clock_info *ptp, s64 delta);
	int (*gettime64)(struct ptp_clock_info *ptp, struct timespec64 *ts);
	int (*gettimex64)(struct ptp_clock_info *ptp, struct timespec64 *ts,
			  struct ptp_system_timestamp *sts);
	int (*getcrosststamp)(struct ptp_clock_info *ptp,
			      struct system_device_crosststamp *cts);
	int (*settime64)(struct ptp_clock_info *p, const struct timespec64 *ts);
	int (*enable)(struct ptp_clock_info *ptp,
		      struct ptp_clock_request *request, int on);
	int (*verify)(struct ptp_clock_info *ptp, unsigned int pin,
		      enum ptp_pin_function func, unsigned int chan);
	long (*do_aux_work)(struct ptp_clock_info *ptp);
};

struct ptp_clock;

enum ptp_clock_events {
	PTP_CLOCK_ALARM,
	PTP_CLOCK_EXTTS,
	PTP_CLOCK_PPS,
	PTP_CLOCK_PPSUSR,
};



struct ptp_clock_event {
	int type;
	int index;
	union {
		u64 timestamp;
		struct pps_event_time pps_times;
	};
};

#if IS_REACHABLE(CONFIG_PTP_1588_CLOCK)



extern struct ptp_clock *ptp_clock_register(struct ptp_clock_info *info,
					    struct device *parent);



extern int ptp_clock_unregister(struct ptp_clock *ptp);



extern void ptp_clock_event(struct ptp_clock *ptp,
			    struct ptp_clock_event *event);



extern int ptp_clock_index(struct ptp_clock *ptp);



extern s32 scaled_ppm_to_ppb(long ppm);



int ptp_find_pin(struct ptp_clock *ptp,
		 enum ptp_pin_function func, unsigned int chan);



int ptp_schedule_worker(struct ptp_clock *ptp, unsigned long delay);

#else
static inline struct ptp_clock *ptp_clock_register(struct ptp_clock_info *info,
						   struct device *parent)
{ return NULL; }
static inline int ptp_clock_unregister(struct ptp_clock *ptp)
{ return 0; }
static inline void ptp_clock_event(struct ptp_clock *ptp,
				   struct ptp_clock_event *event)
{ }
static inline int ptp_clock_index(struct ptp_clock *ptp)
{ return -1; }
static inline int ptp_find_pin(struct ptp_clock *ptp,
			       enum ptp_pin_function func, unsigned int chan)
{ return -1; }
static inline int ptp_schedule_worker(struct ptp_clock *ptp,
				      unsigned long delay)
{ return -EOPNOTSUPP; }

#endif

static inline void ptp_read_system_prets(struct ptp_system_timestamp *sts)
{
	if (sts)
		ktime_get_real_ts64(&sts->pre_ts);
}

static inline void ptp_read_system_postts(struct ptp_system_timestamp *sts)
{
	if (sts)
		ktime_get_real_ts64(&sts->post_ts);
}

#endif
