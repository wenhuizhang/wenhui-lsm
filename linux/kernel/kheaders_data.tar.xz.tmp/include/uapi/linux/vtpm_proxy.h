/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPI_LINUX_VTPM_PROXY_H
#define _UAPI_LINUX_VTPM_PROXY_H

#include <linux/types.h>
#include <linux/ioctl.h>


enum vtpm_proxy_flags {
	VTPM_PROXY_FLAG_TPM2	= 1,
};


struct vtpm_proxy_new_dev {
	__u32 flags;         
	__u32 tpm_num;       
	__u32 fd;            
	__u32 major;         
	__u32 minor;         
};

#define VTPM_PROXY_IOC_NEW_DEV	_IOWR(0xa1, 0x00, struct vtpm_proxy_new_dev)


#define TPM2_CC_SET_LOCALITY	0x20001000
#define TPM_ORD_SET_LOCALITY	0x20001000

#endif 
