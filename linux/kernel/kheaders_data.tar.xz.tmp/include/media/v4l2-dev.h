/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _V4L2_DEV_H
#define _V4L2_DEV_H

#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/mutex.h>
#include <linux/videodev2.h>

#include <media/media-entity.h>

#define VIDEO_MAJOR	81


enum vfl_devnode_type {
	VFL_TYPE_GRABBER	= 0,
	VFL_TYPE_VBI,
	VFL_TYPE_RADIO,
	VFL_TYPE_SUBDEV,
	VFL_TYPE_SDR,
	VFL_TYPE_TOUCH,
	VFL_TYPE_MAX 
};


enum vfl_devnode_direction {
	VFL_DIR_RX,
	VFL_DIR_TX,
	VFL_DIR_M2M,
};

struct v4l2_ioctl_callbacks;
struct video_device;
struct v4l2_device;
struct v4l2_ctrl_handler;


enum v4l2_video_device_flags {
	V4L2_FL_REGISTERED		= 0,
	V4L2_FL_USES_V4L2_FH		= 1,
	V4L2_FL_QUIRK_INVERTED_CROP	= 2,
};




struct v4l2_prio_state {
	atomic_t prios[4];
};


void v4l2_prio_init(struct v4l2_prio_state *global);


int v4l2_prio_change(struct v4l2_prio_state *global, enum v4l2_priority *local,
		     enum v4l2_priority new);


void v4l2_prio_open(struct v4l2_prio_state *global, enum v4l2_priority *local);


void v4l2_prio_close(struct v4l2_prio_state *global, enum v4l2_priority local);


enum v4l2_priority v4l2_prio_max(struct v4l2_prio_state *global);


int v4l2_prio_check(struct v4l2_prio_state *global, enum v4l2_priority local);


struct v4l2_file_operations {
	struct module *owner;
	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
	__poll_t (*poll) (struct file *, struct poll_table_struct *);
	long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
#ifdef CONFIG_COMPAT
	long (*compat_ioctl32) (struct file *, unsigned int, unsigned long);
#endif
	unsigned long (*get_unmapped_area) (struct file *, unsigned long,
				unsigned long, unsigned long, unsigned long);
	int (*mmap) (struct file *, struct vm_area_struct *);
	int (*open) (struct file *);
	int (*release) (struct file *);
};





struct video_device
{
#if defined(CONFIG_MEDIA_CONTROLLER)
	struct media_entity entity;
	struct media_intf_devnode *intf_devnode;
	struct media_pipeline pipe;
#endif
	const struct v4l2_file_operations *fops;

	u32 device_caps;

	
	struct device dev;
	struct cdev *cdev;

	struct v4l2_device *v4l2_dev;
	struct device *dev_parent;

	struct v4l2_ctrl_handler *ctrl_handler;

	struct vb2_queue *queue;

	struct v4l2_prio_state *prio;

	
	char name[32];
	enum vfl_devnode_type vfl_type;
	enum vfl_devnode_direction vfl_dir;
	int minor;
	u16 num;
	unsigned long flags;
	int index;

	
	spinlock_t		fh_lock;
	struct list_head	fh_list;

	int dev_debug;

	v4l2_std_id tvnorms;

	
	void (*release)(struct video_device *vdev);
	const struct v4l2_ioctl_ops *ioctl_ops;
	DECLARE_BITMAP(valid_ioctls, BASE_VIDIOC_PRIVATE);

	struct mutex *lock;
};


#define media_entity_to_video_device(__entity) \
	container_of(__entity, struct video_device, entity)


#define to_video_device(cd) container_of(cd, struct video_device, dev)


int __must_check __video_register_device(struct video_device *vdev,
					 enum vfl_devnode_type type,
					 int nr, int warn_if_nr_in_use,
					 struct module *owner);


static inline int __must_check video_register_device(struct video_device *vdev,
						     enum vfl_devnode_type type,
						     int nr)
{
	return __video_register_device(vdev, type, nr, 1, vdev->fops->owner);
}


static inline int __must_check
video_register_device_no_warn(struct video_device *vdev,
			      enum vfl_devnode_type type, int nr)
{
	return __video_register_device(vdev, type, nr, 0, vdev->fops->owner);
}


void video_unregister_device(struct video_device *vdev);


struct video_device * __must_check video_device_alloc(void);


void video_device_release(struct video_device *vdev);


void video_device_release_empty(struct video_device *vdev);


static inline void v4l2_disable_ioctl(struct video_device *vdev,
				      unsigned int cmd)
{
	if (_IOC_NR(cmd) < BASE_VIDIOC_PRIVATE)
		set_bit(_IOC_NR(cmd), vdev->valid_ioctls);
}


static inline void *video_get_drvdata(struct video_device *vdev)
{
	return dev_get_drvdata(&vdev->dev);
}


static inline void video_set_drvdata(struct video_device *vdev, void *data)
{
	dev_set_drvdata(&vdev->dev, data);
}


struct video_device *video_devdata(struct file *file);


static inline void *video_drvdata(struct file *file)
{
	return video_get_drvdata(video_devdata(file));
}


static inline const char *video_device_node_name(struct video_device *vdev)
{
	return dev_name(&vdev->dev);
}


static inline int video_is_registered(struct video_device *vdev)
{
	return test_bit(V4L2_FL_REGISTERED, &vdev->flags);
}

#endif 
