/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __GPIO_BACKLIGHT_H__
#define __GPIO_BACKLIGHT_H__

struct device;

struct gpio_backlight_platform_data {
	struct device *fbdev;
	int gpio;
	int def_value;
	const char *name;
};

#endif
