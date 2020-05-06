/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef _MEDIA_V4L2_MEM2MEM_H
#define _MEDIA_V4L2_MEM2MEM_H

#include <media/videobuf2-v4l2.h>


struct v4l2_m2m_ops {
	void (*device_run)(void *priv);
	int (*job_ready)(void *priv);
	void (*job_abort)(void *priv);
};

struct video_device;
struct v4l2_m2m_dev;



struct v4l2_m2m_queue_ctx {
	struct vb2_queue	q;

	struct list_head	rdy_queue;
	spinlock_t		rdy_spinlock;
	u8			num_rdy;
	bool			buffered;
};


struct v4l2_m2m_ctx {
	
	struct mutex			*q_lock;

	
	struct v4l2_m2m_dev		*m2m_dev;

	struct v4l2_m2m_queue_ctx	cap_q_ctx;

	struct v4l2_m2m_queue_ctx	out_q_ctx;

	
	struct list_head		queue;
	unsigned long			job_flags;
	wait_queue_head_t		finished;

	void				*priv;
};


struct v4l2_m2m_buffer {
	struct vb2_v4l2_buffer	vb;
	struct list_head	list;
};


void *v4l2_m2m_get_curr_priv(struct v4l2_m2m_dev *m2m_dev);


struct vb2_queue *v4l2_m2m_get_vq(struct v4l2_m2m_ctx *m2m_ctx,
				       enum v4l2_buf_type type);


void v4l2_m2m_try_schedule(struct v4l2_m2m_ctx *m2m_ctx);


void v4l2_m2m_job_finish(struct v4l2_m2m_dev *m2m_dev,
			 struct v4l2_m2m_ctx *m2m_ctx);

static inline void
v4l2_m2m_buf_done(struct vb2_v4l2_buffer *buf, enum vb2_buffer_state state)
{
	vb2_buffer_done(&buf->vb2_buf, state);
}


int v4l2_m2m_reqbufs(struct file *file, struct v4l2_m2m_ctx *m2m_ctx,
		     struct v4l2_requestbuffers *reqbufs);


int v4l2_m2m_querybuf(struct file *file, struct v4l2_m2m_ctx *m2m_ctx,
		      struct v4l2_buffer *buf);


int v4l2_m2m_qbuf(struct file *file, struct v4l2_m2m_ctx *m2m_ctx,
		  struct v4l2_buffer *buf);


int v4l2_m2m_dqbuf(struct file *file, struct v4l2_m2m_ctx *m2m_ctx,
		   struct v4l2_buffer *buf);


int v4l2_m2m_prepare_buf(struct file *file, struct v4l2_m2m_ctx *m2m_ctx,
			 struct v4l2_buffer *buf);


int v4l2_m2m_create_bufs(struct file *file, struct v4l2_m2m_ctx *m2m_ctx,
			 struct v4l2_create_buffers *create);


int v4l2_m2m_expbuf(struct file *file, struct v4l2_m2m_ctx *m2m_ctx,
		   struct v4l2_exportbuffer *eb);


int v4l2_m2m_streamon(struct file *file, struct v4l2_m2m_ctx *m2m_ctx,
		      enum v4l2_buf_type type);


int v4l2_m2m_streamoff(struct file *file, struct v4l2_m2m_ctx *m2m_ctx,
		       enum v4l2_buf_type type);


__poll_t v4l2_m2m_poll(struct file *file, struct v4l2_m2m_ctx *m2m_ctx,
			   struct poll_table_struct *wait);


int v4l2_m2m_mmap(struct file *file, struct v4l2_m2m_ctx *m2m_ctx,
		  struct vm_area_struct *vma);


struct v4l2_m2m_dev *v4l2_m2m_init(const struct v4l2_m2m_ops *m2m_ops);

#if defined(CONFIG_MEDIA_CONTROLLER)
void v4l2_m2m_unregister_media_controller(struct v4l2_m2m_dev *m2m_dev);
int v4l2_m2m_register_media_controller(struct v4l2_m2m_dev *m2m_dev,
			struct video_device *vdev, int function);
#else
static inline void
v4l2_m2m_unregister_media_controller(struct v4l2_m2m_dev *m2m_dev)
{
}

static inline int
v4l2_m2m_register_media_controller(struct v4l2_m2m_dev *m2m_dev,
		struct video_device *vdev, int function)
{
	return 0;
}
#endif


void v4l2_m2m_release(struct v4l2_m2m_dev *m2m_dev);


struct v4l2_m2m_ctx *v4l2_m2m_ctx_init(struct v4l2_m2m_dev *m2m_dev,
		void *drv_priv,
		int (*queue_init)(void *priv, struct vb2_queue *src_vq, struct vb2_queue *dst_vq));

static inline void v4l2_m2m_set_src_buffered(struct v4l2_m2m_ctx *m2m_ctx,
					     bool buffered)
{
	m2m_ctx->out_q_ctx.buffered = buffered;
}

static inline void v4l2_m2m_set_dst_buffered(struct v4l2_m2m_ctx *m2m_ctx,
					     bool buffered)
{
	m2m_ctx->cap_q_ctx.buffered = buffered;
}


void v4l2_m2m_ctx_release(struct v4l2_m2m_ctx *m2m_ctx);


void v4l2_m2m_buf_queue(struct v4l2_m2m_ctx *m2m_ctx,
			struct vb2_v4l2_buffer *vbuf);


static inline
unsigned int v4l2_m2m_num_src_bufs_ready(struct v4l2_m2m_ctx *m2m_ctx)
{
	return m2m_ctx->out_q_ctx.num_rdy;
}


static inline
unsigned int v4l2_m2m_num_dst_bufs_ready(struct v4l2_m2m_ctx *m2m_ctx)
{
	return m2m_ctx->cap_q_ctx.num_rdy;
}


struct vb2_v4l2_buffer *v4l2_m2m_next_buf(struct v4l2_m2m_queue_ctx *q_ctx);


static inline struct vb2_v4l2_buffer *
v4l2_m2m_next_src_buf(struct v4l2_m2m_ctx *m2m_ctx)
{
	return v4l2_m2m_next_buf(&m2m_ctx->out_q_ctx);
}


static inline struct vb2_v4l2_buffer *
v4l2_m2m_next_dst_buf(struct v4l2_m2m_ctx *m2m_ctx)
{
	return v4l2_m2m_next_buf(&m2m_ctx->cap_q_ctx);
}


struct vb2_v4l2_buffer *v4l2_m2m_last_buf(struct v4l2_m2m_queue_ctx *q_ctx);


static inline struct vb2_v4l2_buffer *
v4l2_m2m_last_src_buf(struct v4l2_m2m_ctx *m2m_ctx)
{
	return v4l2_m2m_last_buf(&m2m_ctx->out_q_ctx);
}


static inline struct vb2_v4l2_buffer *
v4l2_m2m_last_dst_buf(struct v4l2_m2m_ctx *m2m_ctx)
{
	return v4l2_m2m_last_buf(&m2m_ctx->cap_q_ctx);
}


#define v4l2_m2m_for_each_dst_buf(m2m_ctx, b)	\
	list_for_each_entry(b, &m2m_ctx->cap_q_ctx.rdy_queue, list)


#define v4l2_m2m_for_each_src_buf(m2m_ctx, b)	\
	list_for_each_entry(b, &m2m_ctx->out_q_ctx.rdy_queue, list)


#define v4l2_m2m_for_each_dst_buf_safe(m2m_ctx, b, n)	\
	list_for_each_entry_safe(b, n, &m2m_ctx->cap_q_ctx.rdy_queue, list)


#define v4l2_m2m_for_each_src_buf_safe(m2m_ctx, b, n)	\
	list_for_each_entry_safe(b, n, &m2m_ctx->out_q_ctx.rdy_queue, list)


static inline
struct vb2_queue *v4l2_m2m_get_src_vq(struct v4l2_m2m_ctx *m2m_ctx)
{
	return &m2m_ctx->out_q_ctx.q;
}


static inline
struct vb2_queue *v4l2_m2m_get_dst_vq(struct v4l2_m2m_ctx *m2m_ctx)
{
	return &m2m_ctx->cap_q_ctx.q;
}


struct vb2_v4l2_buffer *v4l2_m2m_buf_remove(struct v4l2_m2m_queue_ctx *q_ctx);


static inline struct vb2_v4l2_buffer *
v4l2_m2m_src_buf_remove(struct v4l2_m2m_ctx *m2m_ctx)
{
	return v4l2_m2m_buf_remove(&m2m_ctx->out_q_ctx);
}


static inline struct vb2_v4l2_buffer *
v4l2_m2m_dst_buf_remove(struct v4l2_m2m_ctx *m2m_ctx)
{
	return v4l2_m2m_buf_remove(&m2m_ctx->cap_q_ctx);
}


void v4l2_m2m_buf_remove_by_buf(struct v4l2_m2m_queue_ctx *q_ctx,
				struct vb2_v4l2_buffer *vbuf);


static inline void v4l2_m2m_src_buf_remove_by_buf(struct v4l2_m2m_ctx *m2m_ctx,
						  struct vb2_v4l2_buffer *vbuf)
{
	v4l2_m2m_buf_remove_by_buf(&m2m_ctx->out_q_ctx, vbuf);
}


static inline void v4l2_m2m_dst_buf_remove_by_buf(struct v4l2_m2m_ctx *m2m_ctx,
						  struct vb2_v4l2_buffer *vbuf)
{
	v4l2_m2m_buf_remove_by_buf(&m2m_ctx->cap_q_ctx, vbuf);
}

struct vb2_v4l2_buffer *
v4l2_m2m_buf_remove_by_idx(struct v4l2_m2m_queue_ctx *q_ctx, unsigned int idx);

static inline struct vb2_v4l2_buffer *
v4l2_m2m_src_buf_remove_by_idx(struct v4l2_m2m_ctx *m2m_ctx, unsigned int idx)
{
	return v4l2_m2m_buf_remove_by_idx(&m2m_ctx->out_q_ctx, idx);
}

static inline struct vb2_v4l2_buffer *
v4l2_m2m_dst_buf_remove_by_idx(struct v4l2_m2m_ctx *m2m_ctx, unsigned int idx)
{
	return v4l2_m2m_buf_remove_by_idx(&m2m_ctx->cap_q_ctx, idx);
}


void v4l2_m2m_buf_copy_metadata(const struct vb2_v4l2_buffer *out_vb,
				struct vb2_v4l2_buffer *cap_vb,
				bool copy_frame_flags);



void v4l2_m2m_request_queue(struct media_request *req);



int v4l2_m2m_ioctl_reqbufs(struct file *file, void *priv,
				struct v4l2_requestbuffers *rb);
int v4l2_m2m_ioctl_create_bufs(struct file *file, void *fh,
				struct v4l2_create_buffers *create);
int v4l2_m2m_ioctl_querybuf(struct file *file, void *fh,
				struct v4l2_buffer *buf);
int v4l2_m2m_ioctl_expbuf(struct file *file, void *fh,
				struct v4l2_exportbuffer *eb);
int v4l2_m2m_ioctl_qbuf(struct file *file, void *fh,
				struct v4l2_buffer *buf);
int v4l2_m2m_ioctl_dqbuf(struct file *file, void *fh,
				struct v4l2_buffer *buf);
int v4l2_m2m_ioctl_prepare_buf(struct file *file, void *fh,
			       struct v4l2_buffer *buf);
int v4l2_m2m_ioctl_streamon(struct file *file, void *fh,
				enum v4l2_buf_type type);
int v4l2_m2m_ioctl_streamoff(struct file *file, void *fh,
				enum v4l2_buf_type type);
int v4l2_m2m_ioctl_try_encoder_cmd(struct file *file, void *fh,
				   struct v4l2_encoder_cmd *ec);
int v4l2_m2m_ioctl_try_decoder_cmd(struct file *file, void *fh,
				   struct v4l2_decoder_cmd *dc);
int v4l2_m2m_fop_mmap(struct file *file, struct vm_area_struct *vma);
__poll_t v4l2_m2m_fop_poll(struct file *file, poll_table *wait);

#endif 

