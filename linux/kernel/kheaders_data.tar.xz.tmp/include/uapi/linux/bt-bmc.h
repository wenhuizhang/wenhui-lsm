/* SPDX-License-Identifier: GPL-2.0+ WITH Linux-syscall-note */


#ifndef _UAPI_LINUX_BT_BMC_H
#define _UAPI_LINUX_BT_BMC_H

#include <linux/ioctl.h>

#define __BT_BMC_IOCTL_MAGIC	0xb1
#define BT_BMC_IOCTL_SMS_ATN	_IO(__BT_BMC_IOCTL_MAGIC, 0x00)

#endif 
