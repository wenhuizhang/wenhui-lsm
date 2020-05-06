/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef __LINUX_OMAPFB_H__
#define __LINUX_OMAPFB_H__

#include <uapi/linux/omapfb.h>


struct omap_lcd_config {
	char panel_name[16];
	char ctrl_name[16];
	s16  nreset_gpio;
	u8   data_lines;
};

struct omapfb_platform_data {
	struct omap_lcd_config		lcd;
};

void __init omapfb_set_lcd_config(const struct omap_lcd_config *config);

#endif 
