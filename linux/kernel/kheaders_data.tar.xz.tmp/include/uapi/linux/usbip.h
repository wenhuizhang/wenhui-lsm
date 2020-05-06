/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPI_LINUX_USBIP_H
#define _UAPI_LINUX_USBIP_H


enum usbip_device_status {
	
	SDEV_ST_AVAILABLE = 0x01,
	
	SDEV_ST_USED,
	
	SDEV_ST_ERROR,

	
	VDEV_ST_NULL,
	
	VDEV_ST_NOTASSIGNED,
	VDEV_ST_USED,
	VDEV_ST_ERROR
};
#endif 
