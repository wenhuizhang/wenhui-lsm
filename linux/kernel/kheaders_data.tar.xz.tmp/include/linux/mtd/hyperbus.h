/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 */

#ifndef __LINUX_MTD_HYPERBUS_H__
#define __LINUX_MTD_HYPERBUS_H__

#include <linux/mtd/map.h>

enum hyperbus_memtype {
	HYPERFLASH,
	HYPERRAM,
};



struct hyperbus_device {
	struct map_info map;
	struct device_node *np;
	struct mtd_info *mtd;
	struct hyperbus_ctlr *ctlr;
	enum hyperbus_memtype memtype;
};



struct hyperbus_ops {
	u16 (*read16)(struct hyperbus_device *hbdev, unsigned long addr);
	void (*write16)(struct hyperbus_device *hbdev,
			unsigned long addr, u16 val);
	void (*copy_from)(struct hyperbus_device *hbdev, void *to,
			  unsigned long from, ssize_t len);
	void (*copy_to)(struct hyperbus_device *dev, unsigned long to,
			const void *from, ssize_t len);
	int (*calibrate)(struct hyperbus_device *dev);
};


struct hyperbus_ctlr {
	struct device *dev;
	bool calibrated;

	const struct hyperbus_ops *ops;
};


int hyperbus_register_device(struct hyperbus_device *hbdev);


int hyperbus_unregister_device(struct hyperbus_device *hbdev);

#endif 
