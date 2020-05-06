/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _XT_IPCOMP_H
#define _XT_IPCOMP_H

#include <linux/types.h>

struct xt_ipcomp {
	__u32 spis[2];	
	__u8 invflags;	
	__u8 hdrres;	
};


#define XT_IPCOMP_INV_SPI	0x01	
#define XT_IPCOMP_INV_MASK	0x01	

#endif 
