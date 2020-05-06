/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __MACB_PDATA_H__
#define __MACB_PDATA_H__

#include <linux/clk.h>


struct macb_platform_data {
	struct clk	*pclk;
	struct clk	*hclk;
};

#endif 
