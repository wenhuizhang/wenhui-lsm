/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * dvb-vb2.h - DVB driver helper framework for streaming I/O
 *
 * Copyright (C) 2015 Samsung Electronics
 *
 * Author: jh1009.sung@samsung.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 */

#ifndef _DVB_VB2_H
#define _DVB_VB2_H

#include <linux/mutex.h>
#include <linux/poll.h>
#include <linux/dvb/dmx.h>
#include <media/videobuf2-core.h>
#include <media/videobuf2-dma-contig.h>
#include <media/videobuf2-vmalloc.h>


enum dvb_buf_type {
	DVB_BUF_TYPE_CAPTURE        = 1,
};


enum dvb_vb2_states {
	DVB_VB2_STATE_NONE	= 0x0,
	DVB_VB2_STATE_INIT	= 0x1,
	DVB_VB2_STATE_REQBUFS	= 0x2,
	DVB_VB2_STATE_STREAMON	= 0x4,
};

#define DVB_VB2_NAME_MAX (20)


struct dvb_buffer {
	struct vb2_buffer	vb;
	struct list_head	list;
};


struct dvb_vb2_ctx {
	struct vb2_queue	vb_q;
	struct mutex		mutex;
	spinlock_t		slock;
	struct list_head	dvb_q;
	struct dvb_buffer	*buf;
	int	offset;
	int	remain;
	int	state;
	int	buf_siz;
	int	buf_cnt;
	int	nonblocking;

	enum dmx_buffer_flags flags;
	u32	count;

	char	name[DVB_VB2_NAME_MAX + 1];
};

#ifndef CONFIG_DVB_MMAP
static inline int dvb_vb2_init(struct dvb_vb2_ctx *ctx,
			       const char *name, int non_blocking)
{
	return 0;
};
static inline int dvb_vb2_release(struct dvb_vb2_ctx *ctx)
{
	return 0;
};
#define dvb_vb2_is_streaming(ctx) (0)
#define dvb_vb2_fill_buffer(ctx, file, wait, flags) (0)

static inline __poll_t dvb_vb2_poll(struct dvb_vb2_ctx *ctx,
				    struct file *file,
				    poll_table *wait)
{
	return 0;
}
#else

int dvb_vb2_init(struct dvb_vb2_ctx *ctx, const char *name, int non_blocking);


int dvb_vb2_release(struct dvb_vb2_ctx *ctx);


int dvb_vb2_is_streaming(struct dvb_vb2_ctx *ctx);


int dvb_vb2_fill_buffer(struct dvb_vb2_ctx *ctx,
			const unsigned char *src, int len,
			enum dmx_buffer_flags *buffer_flags);


__poll_t dvb_vb2_poll(struct dvb_vb2_ctx *ctx, struct file *file,
		      poll_table *wait);
#endif


int dvb_vb2_stream_on(struct dvb_vb2_ctx *ctx);

int dvb_vb2_stream_off(struct dvb_vb2_ctx *ctx);


int dvb_vb2_reqbufs(struct dvb_vb2_ctx *ctx, struct dmx_requestbuffers *req);


int dvb_vb2_querybuf(struct dvb_vb2_ctx *ctx, struct dmx_buffer *b);


int dvb_vb2_expbuf(struct dvb_vb2_ctx *ctx, struct dmx_exportbuffer *exp);


int dvb_vb2_qbuf(struct dvb_vb2_ctx *ctx, struct dmx_buffer *b);


int dvb_vb2_dqbuf(struct dvb_vb2_ctx *ctx, struct dmx_buffer *b);


int dvb_vb2_mmap(struct dvb_vb2_ctx *ctx, struct vm_area_struct *vma);

#endif 
