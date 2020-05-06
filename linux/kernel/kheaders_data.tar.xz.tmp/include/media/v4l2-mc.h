/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef _V4L2_MC_H
#define _V4L2_MC_H

#include <media/media-device.h>
#include <media/v4l2-dev.h>
#include <linux/types.h>


struct pci_dev;
struct usb_device;

#ifdef CONFIG_MEDIA_CONTROLLER

int v4l2_mc_create_media_graph(struct media_device *mdev);


int v4l_enable_media_source(struct video_device *vdev);


void v4l_disable_media_source(struct video_device *vdev);


int v4l_vb2q_enable_media_source(struct vb2_queue *q);



int v4l2_pipeline_pm_use(struct media_entity *entity, int use);



int v4l2_pipeline_link_notify(struct media_link *link, u32 flags,
			      unsigned int notification);

#else 

static inline int v4l2_mc_create_media_graph(struct media_device *mdev)
{
	return 0;
}

static inline int v4l_enable_media_source(struct video_device *vdev)
{
	return 0;
}

static inline void v4l_disable_media_source(struct video_device *vdev)
{
}

static inline int v4l_vb2q_enable_media_source(struct vb2_queue *q)
{
	return 0;
}

static inline int v4l2_pipeline_pm_use(struct media_entity *entity, int use)
{
	return 0;
}

static inline int v4l2_pipeline_link_notify(struct media_link *link, u32 flags,
					    unsigned int notification)
{
	return 0;
}

#endif 
#endif 
