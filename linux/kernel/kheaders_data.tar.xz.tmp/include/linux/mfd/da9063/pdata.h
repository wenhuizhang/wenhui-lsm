/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef __MFD_DA9063_PDATA_H__
#define __MFD_DA9063_PDATA_H__



enum {
	DA9063_GPIO11_LED,
	DA9063_GPIO14_LED,
	DA9063_GPIO15_LED,

	DA9063_LED_NUM
};
#define DA9063_LED_ID_MASK		0x3


#define DA9063_LED_HIGH_LEVEL_ACTIVE	0x0
#define DA9063_LED_LOW_LEVEL_ACTIVE	0x4




#define DA9063_FLG_FORCE_IN0_MANUAL_MODE	0x0010
#define DA9063_FLG_FORCE_IN0_AUTO_MODE		0x0020
#define DA9063_FLG_FORCE_IN1_MANUAL_MODE	0x0040
#define DA9063_FLG_FORCE_IN1_AUTO_MODE		0x0080
#define DA9063_FLG_FORCE_IN2_MANUAL_MODE	0x0100
#define DA9063_FLG_FORCE_IN2_AUTO_MODE		0x0200
#define DA9063_FLG_FORCE_IN3_MANUAL_MODE	0x0400
#define DA9063_FLG_FORCE_IN3_AUTO_MODE		0x0800


#define DA9063_FLG_NO_CACHE			0x0008

struct da9063;


struct da9063_pdata {
	int				(*init)(struct da9063 *da9063);
	int				irq_base;
	bool				key_power;
	unsigned			flags;
	struct da9063_regulators_pdata	*regulators_pdata;
	struct led_platform_data	*leds_pdata;
};

#endif	
