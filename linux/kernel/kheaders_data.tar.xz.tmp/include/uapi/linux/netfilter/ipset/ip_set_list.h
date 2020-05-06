/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI__IP_SET_LIST_H
#define _UAPI__IP_SET_LIST_H

#include <linux/netfilter/ipset/ip_set.h>


enum {
	
	IPSET_ERR_NAME = IPSET_ERR_TYPE_SPECIFIC,
	
	IPSET_ERR_LOOP,
	
	IPSET_ERR_BEFORE,
	
	IPSET_ERR_NAMEREF,
	
	IPSET_ERR_LIST_FULL,
	
	IPSET_ERR_REF_EXIST,
};


#endif 
