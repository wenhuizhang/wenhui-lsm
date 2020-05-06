/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __PXA310_U2D__
#define __PXA310_U2D__

#include <linux/usb/ulpi.h>

struct pxa3xx_u2d_platform_data {

#define ULPI_SER_6PIN	(1 << 0)
#define ULPI_SER_3PIN	(1 << 1)
	unsigned int ulpi_mode;

	int (*init)(struct device *);
	void (*exit)(struct device *);
};



int pxa3xx_u2d_start_hc(struct usb_bus *host);

void pxa3xx_u2d_stop_hc(struct usb_bus *host);

extern void pxa3xx_set_u2d_info(struct pxa3xx_u2d_platform_data *info);

#endif 
