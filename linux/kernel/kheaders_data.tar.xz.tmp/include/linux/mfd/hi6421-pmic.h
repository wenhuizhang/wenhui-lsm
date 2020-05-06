/* SPDX-License-Identifier: GPL-2.0-only */


#ifndef	__HI6421_PMIC_H
#define	__HI6421_PMIC_H


#define HI6421_REG_TO_BUS_ADDR(x)	(x << 2)


#define HI6421_REG_MAX			0xFF


#define	HI6421_OCP_DEB_CTRL_REG		HI6421_REG_TO_BUS_ADDR(0x51)
#define	HI6421_OCP_DEB_SEL_MASK		0x0C
#define HI6421_OCP_DEB_SEL_8MS		0x00
#define HI6421_OCP_DEB_SEL_16MS		0x04
#define HI6421_OCP_DEB_SEL_32MS		0x08
#define HI6421_OCP_DEB_SEL_64MS		0x0C
#define HI6421_OCP_EN_DEBOUNCE_MASK	0x02
#define HI6421_OCP_EN_DEBOUNCE_ENABLE	0x02
#define HI6421_OCP_AUTO_STOP_MASK	0x01
#define HI6421_OCP_AUTO_STOP_ENABLE	0x01

struct hi6421_pmic {
	struct regmap		*regmap;
};

enum hi6421_type {
	HI6421 = 0,
	HI6421_V530,
};

#endif		
