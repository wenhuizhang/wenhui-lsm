/* SPDX-License-Identifier: GPL-2.0 */
#undef TRACE_SYSTEM
#define TRACE_SYSTEM timer

#if !defined(_TRACE_TIMER_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_TIMER_H

#include <linux/tracepoint.h>
#include <linux/hrtimer.h>
#include <linux/timer.h>

DECLARE_EVENT_CLASS(timer_class,

	TP_PROTO(struct timer_list *timer),

	TP_ARGS(timer),

	TP_STRUCT__entry(
		__field( void *,	timer	)
	),

	TP_fast_assign(
		__entry->timer	= timer;
	),

	TP_printk("timer=%p", __entry->timer)
);


DEFINE_EVENT(timer_class, timer_init,

	TP_PROTO(struct timer_list *timer),

	TP_ARGS(timer)
);

#define decode_timer_flags(flags)			\
	__print_flags(flags, "|",			\
		{  TIMER_MIGRATING,	"M" },		\
		{  TIMER_DEFERRABLE,	"D" },		\
		{  TIMER_PINNED,	"P" },		\
		{  TIMER_IRQSAFE,	"I" })


TRACE_EVENT(timer_start,

	TP_PROTO(struct timer_list *timer,
		unsigned long expires,
		unsigned int flags),

	TP_ARGS(timer, expires, flags),

	TP_STRUCT__entry(
		__field( void *,	timer		)
		__field( void *,	function	)
		__field( unsigned long,	expires		)
		__field( unsigned long,	now		)
		__field( unsigned int,	flags		)
	),

	TP_fast_assign(
		__entry->timer		= timer;
		__entry->function	= timer->function;
		__entry->expires	= expires;
		__entry->now		= jiffies;
		__entry->flags		= flags;
	),

	TP_printk("timer=%p function=%ps expires=%lu [timeout=%ld] cpu=%u idx=%u flags=%s",
		  __entry->timer, __entry->function, __entry->expires,
		  (long)__entry->expires - __entry->now,
		  __entry->flags & TIMER_CPUMASK,
		  __entry->flags >> TIMER_ARRAYSHIFT,
		  decode_timer_flags(__entry->flags & TIMER_TRACE_FLAGMASK))
);


TRACE_EVENT(timer_expire_entry,

	TP_PROTO(struct timer_list *timer, unsigned long baseclk),

	TP_ARGS(timer, baseclk),

	TP_STRUCT__entry(
		__field( void *,	timer	)
		__field( unsigned long,	now	)
		__field( void *,	function)
		__field( unsigned long,	baseclk	)
	),

	TP_fast_assign(
		__entry->timer		= timer;
		__entry->now		= jiffies;
		__entry->function	= timer->function;
		__entry->baseclk	= baseclk;
	),

	TP_printk("timer=%p function=%ps now=%lu baseclk=%lu",
		  __entry->timer, __entry->function, __entry->now,
		  __entry->baseclk)
);


DEFINE_EVENT(timer_class, timer_expire_exit,

	TP_PROTO(struct timer_list *timer),

	TP_ARGS(timer)
);


DEFINE_EVENT(timer_class, timer_cancel,

	TP_PROTO(struct timer_list *timer),

	TP_ARGS(timer)
);

#define decode_clockid(type)						\
	__print_symbolic(type,						\
		{ CLOCK_REALTIME,	"CLOCK_REALTIME"	},	\
		{ CLOCK_MONOTONIC,	"CLOCK_MONOTONIC"	},	\
		{ CLOCK_BOOTTIME,	"CLOCK_BOOTTIME"	},	\
		{ CLOCK_TAI,		"CLOCK_TAI"		})

#define decode_hrtimer_mode(mode)					\
	__print_symbolic(mode,						\
		{ HRTIMER_MODE_ABS,		"ABS"		},	\
		{ HRTIMER_MODE_REL,		"REL"		},	\
		{ HRTIMER_MODE_ABS_PINNED,	"ABS|PINNED"	},	\
		{ HRTIMER_MODE_REL_PINNED,	"REL|PINNED"	},	\
		{ HRTIMER_MODE_ABS_SOFT,	"ABS|SOFT"	},	\
		{ HRTIMER_MODE_REL_SOFT,	"REL|SOFT"	},	\
		{ HRTIMER_MODE_ABS_PINNED_SOFT,	"ABS|PINNED|SOFT" },	\
		{ HRTIMER_MODE_REL_PINNED_SOFT,	"REL|PINNED|SOFT" })


TRACE_EVENT(hrtimer_init,

	TP_PROTO(struct hrtimer *hrtimer, clockid_t clockid,
		 enum hrtimer_mode mode),

	TP_ARGS(hrtimer, clockid, mode),

	TP_STRUCT__entry(
		__field( void *,		hrtimer		)
		__field( clockid_t,		clockid		)
		__field( enum hrtimer_mode,	mode		)
	),

	TP_fast_assign(
		__entry->hrtimer	= hrtimer;
		__entry->clockid	= clockid;
		__entry->mode		= mode;
	),

	TP_printk("hrtimer=%p clockid=%s mode=%s", __entry->hrtimer,
		  decode_clockid(__entry->clockid),
		  decode_hrtimer_mode(__entry->mode))
);


TRACE_EVENT(hrtimer_start,

	TP_PROTO(struct hrtimer *hrtimer, enum hrtimer_mode mode),

	TP_ARGS(hrtimer, mode),

	TP_STRUCT__entry(
		__field( void *,	hrtimer		)
		__field( void *,	function	)
		__field( s64,		expires		)
		__field( s64,		softexpires	)
		__field( enum hrtimer_mode,	mode	)
	),

	TP_fast_assign(
		__entry->hrtimer	= hrtimer;
		__entry->function	= hrtimer->function;
		__entry->expires	= hrtimer_get_expires(hrtimer);
		__entry->softexpires	= hrtimer_get_softexpires(hrtimer);
		__entry->mode		= mode;
	),

	TP_printk("hrtimer=%p function=%ps expires=%llu softexpires=%llu "
		  "mode=%s", __entry->hrtimer, __entry->function,
		  (unsigned long long) __entry->expires,
		  (unsigned long long) __entry->softexpires,
		  decode_hrtimer_mode(__entry->mode))
);


TRACE_EVENT(hrtimer_expire_entry,

	TP_PROTO(struct hrtimer *hrtimer, ktime_t *now),

	TP_ARGS(hrtimer, now),

	TP_STRUCT__entry(
		__field( void *,	hrtimer	)
		__field( s64,		now	)
		__field( void *,	function)
	),

	TP_fast_assign(
		__entry->hrtimer	= hrtimer;
		__entry->now		= *now;
		__entry->function	= hrtimer->function;
	),

	TP_printk("hrtimer=%p function=%ps now=%llu",
		  __entry->hrtimer, __entry->function,
		  (unsigned long long) __entry->now)
);

DECLARE_EVENT_CLASS(hrtimer_class,

	TP_PROTO(struct hrtimer *hrtimer),

	TP_ARGS(hrtimer),

	TP_STRUCT__entry(
		__field( void *,	hrtimer	)
	),

	TP_fast_assign(
		__entry->hrtimer	= hrtimer;
	),

	TP_printk("hrtimer=%p", __entry->hrtimer)
);


DEFINE_EVENT(hrtimer_class, hrtimer_expire_exit,

	TP_PROTO(struct hrtimer *hrtimer),

	TP_ARGS(hrtimer)
);


DEFINE_EVENT(hrtimer_class, hrtimer_cancel,

	TP_PROTO(struct hrtimer *hrtimer),

	TP_ARGS(hrtimer)
);


TRACE_EVENT(itimer_state,

	TP_PROTO(int which, const struct itimerval *const value,
		 unsigned long long expires),

	TP_ARGS(which, value, expires),

	TP_STRUCT__entry(
		__field(	int,			which		)
		__field(	unsigned long long,	expires		)
		__field(	long,			value_sec	)
		__field(	long,			value_usec	)
		__field(	long,			interval_sec	)
		__field(	long,			interval_usec	)
	),

	TP_fast_assign(
		__entry->which		= which;
		__entry->expires	= expires;
		__entry->value_sec	= value->it_value.tv_sec;
		__entry->value_usec	= value->it_value.tv_usec;
		__entry->interval_sec	= value->it_interval.tv_sec;
		__entry->interval_usec	= value->it_interval.tv_usec;
	),

	TP_printk("which=%d expires=%llu it_value=%ld.%ld it_interval=%ld.%ld",
		  __entry->which, __entry->expires,
		  __entry->value_sec, __entry->value_usec,
		  __entry->interval_sec, __entry->interval_usec)
);


TRACE_EVENT(itimer_expire,

	TP_PROTO(int which, struct pid *pid, unsigned long long now),

	TP_ARGS(which, pid, now),

	TP_STRUCT__entry(
		__field( int ,			which	)
		__field( pid_t,			pid	)
		__field( unsigned long long,	now	)
	),

	TP_fast_assign(
		__entry->which	= which;
		__entry->now	= now;
		__entry->pid	= pid_nr(pid);
	),

	TP_printk("which=%d pid=%d now=%llu", __entry->which,
		  (int) __entry->pid, __entry->now)
);

#ifdef CONFIG_NO_HZ_COMMON

#define TICK_DEP_NAMES					\
		tick_dep_mask_name(NONE)		\
		tick_dep_name(POSIX_TIMER)		\
		tick_dep_name(PERF_EVENTS)		\
		tick_dep_name(SCHED)			\
		tick_dep_name_end(CLOCK_UNSTABLE)

#undef tick_dep_name
#undef tick_dep_mask_name
#undef tick_dep_name_end


#define tick_dep_name(sdep) TRACE_DEFINE_ENUM(TICK_DEP_BIT_##sdep); \
	TRACE_DEFINE_ENUM(TICK_DEP_MASK_##sdep);
#define tick_dep_name_end(sdep)  TRACE_DEFINE_ENUM(TICK_DEP_BIT_##sdep); \
	TRACE_DEFINE_ENUM(TICK_DEP_MASK_##sdep);

#define tick_dep_mask_name(sdep) TRACE_DEFINE_ENUM(TICK_DEP_MASK_##sdep);

TICK_DEP_NAMES

#undef tick_dep_name
#undef tick_dep_mask_name
#undef tick_dep_name_end

#define tick_dep_name(sdep) { TICK_DEP_MASK_##sdep, #sdep },
#define tick_dep_mask_name(sdep) { TICK_DEP_MASK_##sdep, #sdep },
#define tick_dep_name_end(sdep) { TICK_DEP_MASK_##sdep, #sdep }

#define show_tick_dep_name(val)				\
	__print_symbolic(val, TICK_DEP_NAMES)

TRACE_EVENT(tick_stop,

	TP_PROTO(int success, int dependency),

	TP_ARGS(success, dependency),

	TP_STRUCT__entry(
		__field( int ,		success	)
		__field( int ,		dependency )
	),

	TP_fast_assign(
		__entry->success	= success;
		__entry->dependency	= dependency;
	),

	TP_printk("success=%d dependency=%s",  __entry->success, \
			show_tick_dep_name(__entry->dependency))
);
#endif

#endif 


#include <trace/define_trace.h>
