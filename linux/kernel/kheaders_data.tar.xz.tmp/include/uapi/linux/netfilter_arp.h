/* SPDX-License-Identifier: GPL-1.0+ WITH Linux-syscall-note */
#ifndef __LINUX_ARP_NETFILTER_H
#define __LINUX_ARP_NETFILTER_H



#include <linux/netfilter.h>


#define NF_ARP		0


#define NF_ARP_IN	0
#define NF_ARP_OUT	1
#define NF_ARP_FORWARD	2

#ifndef __KERNEL__
#define NF_ARP_NUMHOOKS	3
#endif

#endif 
