/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef __LINUX_USB_PD_BDO_H
#define __LINUX_USB_PD_BDO_H


#define BDO_MODE_RECV		(0 << 28)
#define BDO_MODE_TRANSMIT	(1 << 28)
#define BDO_MODE_COUNTERS	(2 << 28)
#define BDO_MODE_CARRIER0	(3 << 28)
#define BDO_MODE_CARRIER1	(4 << 28)
#define BDO_MODE_CARRIER2	(5 << 28)
#define BDO_MODE_CARRIER3	(6 << 28)
#define BDO_MODE_EYE		(7 << 28)
#define BDO_MODE_TESTDATA	(8 << 28)

#define BDO_MODE_MASK(mode)	((mode) & 0xf0000000)

#endif
