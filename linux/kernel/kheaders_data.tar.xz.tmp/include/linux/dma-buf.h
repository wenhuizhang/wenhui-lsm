/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __DMA_BUF_H__
#define __DMA_BUF_H__

#include <linux/file.h>
#include <linux/err.h>
#include <linux/scatterlist.h>
#include <linux/list.h>
#include <linux/dma-mapping.h>
#include <linux/fs.h>
#include <linux/dma-fence.h>
#include <linux/wait.h>

struct device;
struct dma_buf;
struct dma_buf_attachment;


struct dma_buf_ops {
	
	bool cache_sgt_mapping;

	
	int (*attach)(struct dma_buf *, struct dma_buf_attachment *);

	
	void (*detach)(struct dma_buf *, struct dma_buf_attachment *);

	
	struct sg_table * (*map_dma_buf)(struct dma_buf_attachment *,
					 enum dma_data_direction);
	
	void (*unmap_dma_buf)(struct dma_buf_attachment *,
			      struct sg_table *,
			      enum dma_data_direction);

	

	
	void (*release)(struct dma_buf *);

	
	int (*begin_cpu_access)(struct dma_buf *, enum dma_data_direction);

	
	int (*end_cpu_access)(struct dma_buf *, enum dma_data_direction);

	
	int (*mmap)(struct dma_buf *, struct vm_area_struct *vma);

	
	void *(*map)(struct dma_buf *, unsigned long);

	
	void (*unmap)(struct dma_buf *, unsigned long, void *);

	void *(*vmap)(struct dma_buf *);
	void (*vunmap)(struct dma_buf *, void *vaddr);
};


struct dma_buf {
	size_t size;
	struct file *file;
	struct list_head attachments;
	const struct dma_buf_ops *ops;
	struct mutex lock;
	unsigned vmapping_counter;
	void *vmap_ptr;
	const char *exp_name;
	const char *name;
	struct module *owner;
	struct list_head list_node;
	void *priv;
	struct reservation_object *resv;

	
	wait_queue_head_t poll;

	struct dma_buf_poll_cb_t {
		struct dma_fence_cb cb;
		wait_queue_head_t *poll;

		__poll_t active;
	} cb_excl, cb_shared;
};


struct dma_buf_attachment {
	struct dma_buf *dmabuf;
	struct device *dev;
	struct list_head node;
	struct sg_table *sgt;
	enum dma_data_direction dir;
	void *priv;
};


struct dma_buf_export_info {
	const char *exp_name;
	struct module *owner;
	const struct dma_buf_ops *ops;
	size_t size;
	int flags;
	struct reservation_object *resv;
	void *priv;
};


#define DEFINE_DMA_BUF_EXPORT_INFO(name)	\
	struct dma_buf_export_info name = { .exp_name = KBUILD_MODNAME, \
					 .owner = THIS_MODULE }


static inline void get_dma_buf(struct dma_buf *dmabuf)
{
	get_file(dmabuf->file);
}

struct dma_buf_attachment *dma_buf_attach(struct dma_buf *dmabuf,
							struct device *dev);
void dma_buf_detach(struct dma_buf *dmabuf,
				struct dma_buf_attachment *dmabuf_attach);

struct dma_buf *dma_buf_export(const struct dma_buf_export_info *exp_info);

int dma_buf_fd(struct dma_buf *dmabuf, int flags);
struct dma_buf *dma_buf_get(int fd);
void dma_buf_put(struct dma_buf *dmabuf);

struct sg_table *dma_buf_map_attachment(struct dma_buf_attachment *,
					enum dma_data_direction);
void dma_buf_unmap_attachment(struct dma_buf_attachment *, struct sg_table *,
				enum dma_data_direction);
int dma_buf_begin_cpu_access(struct dma_buf *dma_buf,
			     enum dma_data_direction dir);
int dma_buf_end_cpu_access(struct dma_buf *dma_buf,
			   enum dma_data_direction dir);
void *dma_buf_kmap(struct dma_buf *, unsigned long);
void dma_buf_kunmap(struct dma_buf *, unsigned long, void *);

int dma_buf_mmap(struct dma_buf *, struct vm_area_struct *,
		 unsigned long);
void *dma_buf_vmap(struct dma_buf *);
void dma_buf_vunmap(struct dma_buf *, void *vaddr);
#endif 
