/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _XT_ADDRTYPE_H
#define _XT_ADDRTYPE_H

#include <linux/types.h>

enum {
	XT_ADDRTYPE_INVERT_SOURCE	= 0x0001,
	XT_ADDRTYPE_INVERT_DEST		= 0x0002,
	XT_ADDRTYPE_LIMIT_IFACE_IN	= 0x0004,
	XT_ADDRTYPE_LIMIT_IFACE_OUT	= 0x0008,
};



enum {
	XT_ADDRTYPE_UNSPEC = 1 << 0,
	XT_ADDRTYPE_UNICAST = 1 << 1,	
	XT_ADDRTYPE_LOCAL  = 1 << 2,	
	XT_ADDRTYPE_BROADCAST = 1 << 3,
	XT_ADDRTYPE_ANYCAST = 1 << 4,
	XT_ADDRTYPE_MULTICAST = 1 << 5,
	XT_ADDRTYPE_BLACKHOLE = 1 << 6,
	XT_ADDRTYPE_UNREACHABLE = 1 << 7,
	XT_ADDRTYPE_PROHIBIT = 1 << 8,
	XT_ADDRTYPE_THROW = 1 << 9,
	XT_ADDRTYPE_NAT = 1 << 10,
	XT_ADDRTYPE_XRESOLVE = 1 << 11,
};

struct xt_addrtype_info_v1 {
	__u16	source;		
	__u16	dest;		
	__u32	flags;
};


struct xt_addrtype_info {
	__u16	source;		
	__u16	dest;		
	__u32	invert_source;
	__u32	invert_dest;
};

#endif
