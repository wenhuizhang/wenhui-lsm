/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef V4L2_COMMON_H_
#define V4L2_COMMON_H_

#include <media/v4l2-dev.h>


#define v4l_printk(level, name, adapter, addr, fmt, arg...) \
	printk(level "%s %d-%04x: " fmt, name, i2c_adapter_id(adapter), addr , ## arg)

#define v4l_client_printk(level, client, fmt, arg...)			    \
	v4l_printk(level, (client)->dev.driver->name, (client)->adapter, \
		   (client)->addr, fmt , ## arg)

#define v4l_err(client, fmt, arg...) \
	v4l_client_printk(KERN_ERR, client, fmt , ## arg)

#define v4l_warn(client, fmt, arg...) \
	v4l_client_printk(KERN_WARNING, client, fmt , ## arg)

#define v4l_info(client, fmt, arg...) \
	v4l_client_printk(KERN_INFO, client, fmt , ## arg)


#define v4l_dbg(level, debug, client, fmt, arg...)			     \
	do {								     \
		if (debug >= (level))					     \
			v4l_client_printk(KERN_DEBUG, client, fmt , ## arg); \
	} while (0)


#define dev_dbg_lvl(__dev, __level, __debug, __fmt, __arg...)		\
	do {								\
		if (__debug >= (__level))				\
			dev_printk(KERN_DEBUG, __dev, __fmt, ##__arg);	\
	} while (0)




#define v4l2_printk(level, dev, fmt, arg...) \
	printk(level "%s: " fmt, (dev)->name , ## arg)

#define v4l2_err(dev, fmt, arg...) \
	v4l2_printk(KERN_ERR, dev, fmt , ## arg)

#define v4l2_warn(dev, fmt, arg...) \
	v4l2_printk(KERN_WARNING, dev, fmt , ## arg)

#define v4l2_info(dev, fmt, arg...) \
	v4l2_printk(KERN_INFO, dev, fmt , ## arg)


#define v4l2_dbg(level, debug, dev, fmt, arg...)			\
	do {								\
		if (debug >= (level))					\
			v4l2_printk(KERN_DEBUG, dev, fmt , ## arg);	\
	} while (0)



int v4l2_ctrl_query_fill(struct v4l2_queryctrl *qctrl,
			 s32 min, s32 max, s32 step, s32 def);





struct i2c_driver;
struct i2c_adapter;
struct i2c_client;
struct i2c_device_id;
struct v4l2_device;
struct v4l2_subdev;
struct v4l2_subdev_ops;


struct v4l2_subdev *v4l2_i2c_new_subdev(struct v4l2_device *v4l2_dev,
		struct i2c_adapter *adapter, const char *client_type,
		u8 addr, const unsigned short *probe_addrs);

struct i2c_board_info;


struct v4l2_subdev *v4l2_i2c_new_subdev_board(struct v4l2_device *v4l2_dev,
		struct i2c_adapter *adapter, struct i2c_board_info *info,
		const unsigned short *probe_addrs);


void v4l2_i2c_subdev_set_name(struct v4l2_subdev *sd, struct i2c_client *client,
			      const char *devname, const char *postfix);


void v4l2_i2c_subdev_init(struct v4l2_subdev *sd, struct i2c_client *client,
		const struct v4l2_subdev_ops *ops);


unsigned short v4l2_i2c_subdev_addr(struct v4l2_subdev *sd);


enum v4l2_i2c_tuner_type {
	ADDRS_RADIO,
	ADDRS_DEMOD,
	ADDRS_TV,
	ADDRS_TV_WITH_DEMOD,
};

const unsigned short *v4l2_i2c_tuner_addrs(enum v4l2_i2c_tuner_type type);




#if defined(CONFIG_SPI)

#include <linux/spi/spi.h>

struct spi_device;


struct v4l2_subdev *v4l2_spi_new_subdev(struct v4l2_device *v4l2_dev,
		struct spi_master *master, struct spi_board_info *info);


void v4l2_spi_subdev_init(struct v4l2_subdev *sd, struct spi_device *spi,
		const struct v4l2_subdev_ops *ops);
#endif






struct v4l2_priv_tun_config {
	int tuner;
	void *priv;
};
#define TUNER_SET_CONFIG           _IOW('d', 92, struct v4l2_priv_tun_config)

#define VIDIOC_INT_RESET		_IOW ('d', 102, u32)






void v4l_bound_align_image(unsigned int *width, unsigned int wmin,
			   unsigned int wmax, unsigned int walign,
			   unsigned int *height, unsigned int hmin,
			   unsigned int hmax, unsigned int halign,
			   unsigned int salign);


#define v4l2_find_nearest_size(array, array_size, width_field, height_field, \
			       width, height)				\
	({								\
		BUILD_BUG_ON(sizeof((array)->width_field) != sizeof(u32) || \
			     sizeof((array)->height_field) != sizeof(u32)); \
		(typeof(&(array)[0]))__v4l2_find_nearest_size(		\
			(array), array_size, sizeof(*(array)),		\
			offsetof(typeof(*(array)), width_field),	\
			offsetof(typeof(*(array)), height_field),	\
			width, height);					\
	})
const void *
__v4l2_find_nearest_size(const void *array, size_t array_size,
			 size_t entry_size, size_t width_offset,
			 size_t height_offset, s32 width, s32 height);


int v4l2_g_parm_cap(struct video_device *vdev,
		    struct v4l2_subdev *sd, struct v4l2_streamparm *a);


int v4l2_s_parm_cap(struct video_device *vdev,
		    struct v4l2_subdev *sd, struct v4l2_streamparm *a);


#define V4L2_FRACT_COMPARE(a, OP, b)			\
	((u64)(a).numerator * (b).denominator OP	\
	(u64)(b).numerator * (a).denominator)






struct v4l2_format_info {
	u32 format;
	u8 mem_planes;
	u8 comp_planes;
	u8 bpp[4];
	u8 hdiv;
	u8 vdiv;
	u8 block_w[4];
	u8 block_h[4];
};

const struct v4l2_format_info *v4l2_format_info(u32 format);

void v4l2_apply_frmsize_constraints(u32 *width, u32 *height,
				    const struct v4l2_frmsize_stepwise *frmsize);
int v4l2_fill_pixfmt(struct v4l2_pix_format *pixfmt, u32 pixelformat,
		     u32 width, u32 height);
int v4l2_fill_pixfmt_mp(struct v4l2_pix_format_mplane *pixfmt, u32 pixelformat,
			u32 width, u32 height);

#endif 
