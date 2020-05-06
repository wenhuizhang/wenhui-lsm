/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ROCKCHIP_BOOT_MODE_H
#define __ROCKCHIP_BOOT_MODE_H


#define REBOOT_FLAG		0x5242C300

#define BOOT_NORMAL		(REBOOT_FLAG + 0)

#define BOOT_BL_DOWNLOAD	(REBOOT_FLAG + 1)

#define BOOT_RECOVERY		(REBOOT_FLAG + 3)
 
#define BOOT_FASTBOOT		(REBOOT_FLAG + 9)

#endif
