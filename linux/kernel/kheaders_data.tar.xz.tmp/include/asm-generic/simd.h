/* SPDX-License-Identifier: GPL-2.0 */

#include <linux/hardirq.h>


static __must_check inline bool may_use_simd(void)
{
	return !in_interrupt();
}
