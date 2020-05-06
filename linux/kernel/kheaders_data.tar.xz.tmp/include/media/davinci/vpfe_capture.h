/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef _VPFE_CAPTURE_H
#define _VPFE_CAPTURE_H

#ifdef __KERNEL__


#include <media/v4l2-dev.h>
#include <linux/videodev2.h>
#include <linux/clk.h>
#include <linux/i2c.h>
#include <media/v4l2-fh.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-device.h>
#include <media/videobuf-dma-contig.h>
#include <media/davinci/vpfe_types.h>

#define VPFE_CAPTURE_NUM_DECODERS        5


#define VPFE_MAJOR_RELEASE              0
#define VPFE_MINOR_RELEASE              0
#define VPFE_BUILD                      1
#define VPFE_CAPTURE_VERSION_CODE       ((VPFE_MAJOR_RELEASE << 16) | \
					(VPFE_MINOR_RELEASE << 8)  | \
					VPFE_BUILD)

#define CAPTURE_DRV_NAME		"vpfe-capture"

struct vpfe_pixel_format {
	struct v4l2_fmtdesc fmtdesc;
	
	int bpp;
};

struct vpfe_std_info {
	int active_pixels;
	int active_lines;
	
	int frame_format;
};

struct vpfe_route {
	u32 input;
	u32 output;
};

struct vpfe_subdev_info {
	
	char name[32];
	
	int grp_id;
	
	int num_inputs;
	
	struct v4l2_input *inputs;
	
	struct vpfe_route *routes;
	
	int can_route;
	
	struct vpfe_hw_if_param ccdc_if_params;
	
	struct i2c_board_info board_info;
};

struct vpfe_config {
	
	int num_subdevs;
	
	int i2c_adapter_id;
	
	struct vpfe_subdev_info *sub_devs;
	
	char *card_name;
	
	char *ccdc;
	
	struct clk *vpssclk;
	struct clk *slaveclk;
	
	void (*clr_intr)(int vdint);
};

struct vpfe_device {
	
	
	struct video_device video_dev;
	
	struct v4l2_subdev **sd;
	
	struct vpfe_config *cfg;
	
	struct v4l2_device v4l2_dev;
	
	struct device *pdev;
	
	u32 usrs;
	
	u32 field_id;
	
	u8 initialized;
	
	struct vpfe_hw_if_param vpfe_if_params;
	
	struct vpfe_subdev_info *current_subdev;
	
	int current_input;
	
	struct vpfe_std_info std_info;
	
	int std_index;
	
	unsigned int ccdc_irq0;
	unsigned int ccdc_irq1;
	
	u32 numbuffers;
	
	u8 *fbuffers[VIDEO_MAX_FRAME];
	
	struct videobuf_buffer *cur_frm;
	
	struct videobuf_buffer *next_frm;
	
	enum v4l2_memory memory;
	
	struct v4l2_format fmt;
	
	struct v4l2_rect crop;
	
	struct videobuf_queue buffer_queue;
	
	struct list_head dma_queue;
	
	spinlock_t irqlock;
	
	spinlock_t dma_queue_lock;
	
	struct mutex lock;
	
	u32 io_usrs;
	
	u8 started;
	
	u32 field_off;
};


struct vpfe_fh {
	struct v4l2_fh fh;
	struct vpfe_device *vpfe_dev;
	
	u8 io_allowed;
};

struct vpfe_config_params {
	u8 min_numbuffers;
	u8 numbuffers;
	u32 min_bufsize;
	u32 device_bufsize;
};

#endif				
#endif				
