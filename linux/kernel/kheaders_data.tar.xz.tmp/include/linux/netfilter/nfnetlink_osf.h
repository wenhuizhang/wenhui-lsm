/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NFOSF_H
#define _NFOSF_H

#include <uapi/linux/netfilter/nfnetlink_osf.h>

enum osf_fmatch_states {
	
	FMATCH_WRONG = 0,
	
	FMATCH_OK,
	
	FMATCH_OPT_WRONG,
};

extern struct list_head nf_osf_fingers[2];

struct nf_osf_finger {
	struct rcu_head			rcu_head;
	struct list_head		finger_entry;
	struct nf_osf_user_finger	finger;
};

struct nf_osf_data {
	const char *genre;
	const char *version;
};

bool nf_osf_match(const struct sk_buff *skb, u_int8_t family,
		  int hooknum, struct net_device *in, struct net_device *out,
		  const struct nf_osf_info *info, struct net *net,
		  const struct list_head *nf_osf_fingers);

bool nf_osf_find(const struct sk_buff *skb,
		 const struct list_head *nf_osf_fingers,
		 const int ttl_check, struct nf_osf_data *data);

#endif 
