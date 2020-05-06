/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_RING_BUFFER_H
#define _LINUX_RING_BUFFER_H

#include <linux/mm.h>
#include <linux/seq_file.h>
#include <linux/poll.h>

struct ring_buffer;
struct ring_buffer_iter;


struct ring_buffer_event {
	u32		type_len:5, time_delta:27;

	u32		array[];
};


enum ring_buffer_type {
	RINGBUF_TYPE_DATA_TYPE_LEN_MAX = 28,
	RINGBUF_TYPE_PADDING,
	RINGBUF_TYPE_TIME_EXTEND,
	RINGBUF_TYPE_TIME_STAMP,
};

unsigned ring_buffer_event_length(struct ring_buffer_event *event);
void *ring_buffer_event_data(struct ring_buffer_event *event);
u64 ring_buffer_event_time_stamp(struct ring_buffer_event *event);


void ring_buffer_discard_commit(struct ring_buffer *buffer,
				struct ring_buffer_event *event);


struct ring_buffer *
__ring_buffer_alloc(unsigned long size, unsigned flags, struct lock_class_key *key);


#define ring_buffer_alloc(size, flags)			\
({							\
	static struct lock_class_key __key;		\
	__ring_buffer_alloc((size), (flags), &__key);	\
})

int ring_buffer_wait(struct ring_buffer *buffer, int cpu, int full);
__poll_t ring_buffer_poll_wait(struct ring_buffer *buffer, int cpu,
			  struct file *filp, poll_table *poll_table);


#define RING_BUFFER_ALL_CPUS -1

void ring_buffer_free(struct ring_buffer *buffer);

int ring_buffer_resize(struct ring_buffer *buffer, unsigned long size, int cpu);

void ring_buffer_change_overwrite(struct ring_buffer *buffer, int val);

struct ring_buffer_event *ring_buffer_lock_reserve(struct ring_buffer *buffer,
						   unsigned long length);
int ring_buffer_unlock_commit(struct ring_buffer *buffer,
			      struct ring_buffer_event *event);
int ring_buffer_write(struct ring_buffer *buffer,
		      unsigned long length, void *data);

void ring_buffer_nest_start(struct ring_buffer *buffer);
void ring_buffer_nest_end(struct ring_buffer *buffer);

struct ring_buffer_event *
ring_buffer_peek(struct ring_buffer *buffer, int cpu, u64 *ts,
		 unsigned long *lost_events);
struct ring_buffer_event *
ring_buffer_consume(struct ring_buffer *buffer, int cpu, u64 *ts,
		    unsigned long *lost_events);

struct ring_buffer_iter *
ring_buffer_read_prepare(struct ring_buffer *buffer, int cpu, gfp_t flags);
void ring_buffer_read_prepare_sync(void);
void ring_buffer_read_start(struct ring_buffer_iter *iter);
void ring_buffer_read_finish(struct ring_buffer_iter *iter);

struct ring_buffer_event *
ring_buffer_iter_peek(struct ring_buffer_iter *iter, u64 *ts);
struct ring_buffer_event *
ring_buffer_read(struct ring_buffer_iter *iter, u64 *ts);
void ring_buffer_iter_reset(struct ring_buffer_iter *iter);
int ring_buffer_iter_empty(struct ring_buffer_iter *iter);

unsigned long ring_buffer_size(struct ring_buffer *buffer, int cpu);

void ring_buffer_reset_cpu(struct ring_buffer *buffer, int cpu);
void ring_buffer_reset(struct ring_buffer *buffer);

#ifdef CONFIG_RING_BUFFER_ALLOW_SWAP
int ring_buffer_swap_cpu(struct ring_buffer *buffer_a,
			 struct ring_buffer *buffer_b, int cpu);
#else
static inline int
ring_buffer_swap_cpu(struct ring_buffer *buffer_a,
		     struct ring_buffer *buffer_b, int cpu)
{
	return -ENODEV;
}
#endif

bool ring_buffer_empty(struct ring_buffer *buffer);
bool ring_buffer_empty_cpu(struct ring_buffer *buffer, int cpu);

void ring_buffer_record_disable(struct ring_buffer *buffer);
void ring_buffer_record_enable(struct ring_buffer *buffer);
void ring_buffer_record_off(struct ring_buffer *buffer);
void ring_buffer_record_on(struct ring_buffer *buffer);
bool ring_buffer_record_is_on(struct ring_buffer *buffer);
bool ring_buffer_record_is_set_on(struct ring_buffer *buffer);
void ring_buffer_record_disable_cpu(struct ring_buffer *buffer, int cpu);
void ring_buffer_record_enable_cpu(struct ring_buffer *buffer, int cpu);

u64 ring_buffer_oldest_event_ts(struct ring_buffer *buffer, int cpu);
unsigned long ring_buffer_bytes_cpu(struct ring_buffer *buffer, int cpu);
unsigned long ring_buffer_entries(struct ring_buffer *buffer);
unsigned long ring_buffer_overruns(struct ring_buffer *buffer);
unsigned long ring_buffer_entries_cpu(struct ring_buffer *buffer, int cpu);
unsigned long ring_buffer_overrun_cpu(struct ring_buffer *buffer, int cpu);
unsigned long ring_buffer_commit_overrun_cpu(struct ring_buffer *buffer, int cpu);
unsigned long ring_buffer_dropped_events_cpu(struct ring_buffer *buffer, int cpu);
unsigned long ring_buffer_read_events_cpu(struct ring_buffer *buffer, int cpu);

u64 ring_buffer_time_stamp(struct ring_buffer *buffer, int cpu);
void ring_buffer_normalize_time_stamp(struct ring_buffer *buffer,
				      int cpu, u64 *ts);
void ring_buffer_set_clock(struct ring_buffer *buffer,
			   u64 (*clock)(void));
void ring_buffer_set_time_stamp_abs(struct ring_buffer *buffer, bool abs);
bool ring_buffer_time_stamp_abs(struct ring_buffer *buffer);

size_t ring_buffer_nr_pages(struct ring_buffer *buffer, int cpu);
size_t ring_buffer_nr_dirty_pages(struct ring_buffer *buffer, int cpu);

void *ring_buffer_alloc_read_page(struct ring_buffer *buffer, int cpu);
void ring_buffer_free_read_page(struct ring_buffer *buffer, int cpu, void *data);
int ring_buffer_read_page(struct ring_buffer *buffer, void **data_page,
			  size_t len, int cpu, int full);

struct trace_seq;

int ring_buffer_print_entry_header(struct trace_seq *s);
int ring_buffer_print_page_header(struct trace_seq *s);

enum ring_buffer_flags {
	RB_FL_OVERWRITE		= 1 << 0,
};

#ifdef CONFIG_RING_BUFFER
int trace_rb_cpu_prepare(unsigned int cpu, struct hlist_node *node);
#else
#define trace_rb_cpu_prepare	NULL
#endif

#endif 
