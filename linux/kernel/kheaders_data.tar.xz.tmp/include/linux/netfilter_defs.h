/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_NETFILTER_CORE_H_
#define __LINUX_NETFILTER_CORE_H_

#include <uapi/linux/netfilter.h>


#define NF_ARP_NUMHOOKS 3


#define NF_DN_NUMHOOKS 7

#if IS_ENABLED(CONFIG_DECNET)

#define NF_MAX_HOOKS	NF_DN_NUMHOOKS
#else
#define NF_MAX_HOOKS	NF_INET_NUMHOOKS
#endif

#endif
