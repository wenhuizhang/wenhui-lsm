/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_RECIPROCAL_DIV_H
#define _LINUX_RECIPROCAL_DIV_H

#include <linux/types.h>



struct reciprocal_value {
	u32 m;
	u8 sh1, sh2;
};


struct reciprocal_value reciprocal_value(u32 d);

static inline u32 reciprocal_divide(u32 a, struct reciprocal_value R)
{
	u32 t = (u32)(((u64)a * R.m) >> 32);
	return (t + ((a - t) >> R.sh1)) >> R.sh2;
}

struct reciprocal_value_adv {
	u32 m;
	u8 sh, exp;
	bool is_wide_m;
};


struct reciprocal_value_adv reciprocal_value_adv(u32 d, u8 prec);

#endif 
