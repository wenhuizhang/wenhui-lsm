/* SPDX-License-Identifier: ((GPL-2.0 WITH Linux-syscall-note) OR BSD-3-Clause) */


#ifndef __INCLUDE_UAPI_SOUND_SOF_USER_HEADER_H__
#define __INCLUDE_UAPI_SOUND_SOF_USER_HEADER_H__

#include <linux/types.h>


struct sof_abi_hdr {
	__u32 magic;		
	__u32 type;		
	__u32 size;		
	__u32 abi;		
	__u32 reserved[4];	
	__u32 data[0];		
}  __packed;

#endif
