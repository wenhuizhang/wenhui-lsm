

#ifndef _MEDIA_VIDEOBUF2_MEMOPS_H
#define _MEDIA_VIDEOBUF2_MEMOPS_H

#include <media/videobuf2-v4l2.h>
#include <linux/mm.h>
#include <linux/refcount.h>


struct vb2_vmarea_handler {
	refcount_t		*refcount;
	void			(*put)(void *arg);
	void			*arg;
};

extern const struct vm_operations_struct vb2_common_vm_ops;

struct frame_vector *vb2_create_framevec(unsigned long start,
					 unsigned long length);
void vb2_destroy_framevec(struct frame_vector *vec);

#endif
