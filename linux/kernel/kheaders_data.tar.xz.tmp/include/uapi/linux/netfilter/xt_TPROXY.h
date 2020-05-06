/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _XT_TPROXY_H
#define _XT_TPROXY_H

#include <linux/types.h>
#include <linux/netfilter.h>


struct xt_tproxy_target_info {
	__u32 mark_mask;
	__u32 mark_value;
	__be32 laddr;
	__be16 lport;
};

struct xt_tproxy_target_info_v1 {
	__u32 mark_mask;
	__u32 mark_value;
	union nf_inet_addr laddr;
	__be16 lport;
};

#endif 
