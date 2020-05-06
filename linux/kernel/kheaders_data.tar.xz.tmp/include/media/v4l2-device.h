/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef _V4L2_DEVICE_H
#define _V4L2_DEVICE_H

#include <media/media-device.h>
#include <media/v4l2-subdev.h>
#include <media/v4l2-dev.h>

#define V4L2_DEVICE_NAME_SIZE (20 + 16)

struct v4l2_ctrl_handler;


struct v4l2_device {
	struct device *dev;
	struct media_device *mdev;
	struct list_head subdevs;
	spinlock_t lock;
	char name[V4L2_DEVICE_NAME_SIZE];
	void (*notify)(struct v4l2_subdev *sd,
			unsigned int notification, void *arg);
	struct v4l2_ctrl_handler *ctrl_handler;
	struct v4l2_prio_state prio;
	struct kref ref;
	void (*release)(struct v4l2_device *v4l2_dev);
};


static inline void v4l2_device_get(struct v4l2_device *v4l2_dev)
{
	kref_get(&v4l2_dev->ref);
}


int v4l2_device_put(struct v4l2_device *v4l2_dev);


int __must_check v4l2_device_register(struct device *dev,
				      struct v4l2_device *v4l2_dev);


int v4l2_device_set_name(struct v4l2_device *v4l2_dev, const char *basename,
			 atomic_t *instance);


void v4l2_device_disconnect(struct v4l2_device *v4l2_dev);


void v4l2_device_unregister(struct v4l2_device *v4l2_dev);


int __must_check v4l2_device_register_subdev(struct v4l2_device *v4l2_dev,
					     struct v4l2_subdev *sd);


void v4l2_device_unregister_subdev(struct v4l2_subdev *sd);


int __must_check
v4l2_device_register_subdev_nodes(struct v4l2_device *v4l2_dev);


static inline void v4l2_subdev_notify(struct v4l2_subdev *sd,
				      unsigned int notification, void *arg)
{
	if (sd && sd->v4l2_dev && sd->v4l2_dev->notify)
		sd->v4l2_dev->notify(sd, notification, arg);
}


static inline bool v4l2_device_supports_requests(struct v4l2_device *v4l2_dev)
{
	return v4l2_dev->mdev && v4l2_dev->mdev->ops &&
	       v4l2_dev->mdev->ops->req_queue;
}




#define v4l2_device_for_each_subdev(sd, v4l2_dev)			\
	list_for_each_entry(sd, &(v4l2_dev)->subdevs, list)


#define __v4l2_device_call_subdevs_p(v4l2_dev, sd, cond, o, f, args...)	\
	do {								\
		list_for_each_entry((sd), &(v4l2_dev)->subdevs, list)	\
			if ((cond) && (sd)->ops->o && (sd)->ops->o->f)	\
				(sd)->ops->o->f((sd) , ##args);		\
	} while (0)


#define __v4l2_device_call_subdevs(v4l2_dev, cond, o, f, args...)	\
	do {								\
		struct v4l2_subdev *__sd;				\
									\
		__v4l2_device_call_subdevs_p(v4l2_dev, __sd, cond, o,	\
						f , ##args);		\
	} while (0)


#define __v4l2_device_call_subdevs_until_err_p(v4l2_dev, sd, cond, o, f, args...) \
({									\
	long __err = 0;							\
									\
	list_for_each_entry((sd), &(v4l2_dev)->subdevs, list) {		\
		if ((cond) && (sd)->ops->o && (sd)->ops->o->f)		\
			__err = (sd)->ops->o->f((sd) , ##args);		\
		if (__err && __err != -ENOIOCTLCMD)			\
			break;						\
	}								\
	(__err == -ENOIOCTLCMD) ? 0 : __err;				\
})


#define __v4l2_device_call_subdevs_until_err(v4l2_dev, cond, o, f, args...) \
({									\
	struct v4l2_subdev *__sd;					\
	__v4l2_device_call_subdevs_until_err_p(v4l2_dev, __sd, cond, o,	\
						f , ##args);		\
})


#define v4l2_device_call_all(v4l2_dev, grpid, o, f, args...)		\
	do {								\
		struct v4l2_subdev *__sd;				\
									\
		__v4l2_device_call_subdevs_p(v4l2_dev, __sd,		\
			!(grpid) || __sd->grp_id == (grpid), o, f ,	\
			##args);					\
	} while (0)


#define v4l2_device_call_until_err(v4l2_dev, grpid, o, f, args...)	\
({									\
	struct v4l2_subdev *__sd;					\
	__v4l2_device_call_subdevs_until_err_p(v4l2_dev, __sd,		\
			!(grpid) || __sd->grp_id == (grpid), o, f ,	\
			##args);					\
})


#define v4l2_device_mask_call_all(v4l2_dev, grpmsk, o, f, args...)	\
	do {								\
		struct v4l2_subdev *__sd;				\
									\
		__v4l2_device_call_subdevs_p(v4l2_dev, __sd,		\
			!(grpmsk) || (__sd->grp_id & (grpmsk)), o, f ,	\
			##args);					\
	} while (0)


#define v4l2_device_mask_call_until_err(v4l2_dev, grpmsk, o, f, args...) \
({									\
	struct v4l2_subdev *__sd;					\
	__v4l2_device_call_subdevs_until_err_p(v4l2_dev, __sd,		\
			!(grpmsk) || (__sd->grp_id & (grpmsk)), o, f ,	\
			##args);					\
})



#define v4l2_device_has_op(v4l2_dev, grpid, o, f)			\
({									\
	struct v4l2_subdev *__sd;					\
	bool __result = false;						\
	list_for_each_entry(__sd, &(v4l2_dev)->subdevs, list) {		\
		if ((grpid) && __sd->grp_id != (grpid))			\
			continue;					\
		if (v4l2_subdev_has_op(__sd, o, f)) {			\
			__result = true;				\
			break;						\
		}							\
	}								\
	__result;							\
})


#define v4l2_device_mask_has_op(v4l2_dev, grpmsk, o, f)			\
({									\
	struct v4l2_subdev *__sd;					\
	bool __result = false;						\
	list_for_each_entry(__sd, &(v4l2_dev)->subdevs, list) {		\
		if ((grpmsk) && !(__sd->grp_id & (grpmsk)))		\
			continue;					\
		if (v4l2_subdev_has_op(__sd, o, f)) {			\
			__result = true;				\
			break;						\
		}							\
	}								\
	__result;							\
})

#endif
