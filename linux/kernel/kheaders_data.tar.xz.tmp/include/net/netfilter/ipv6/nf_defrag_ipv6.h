/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NF_DEFRAG_IPV6_H
#define _NF_DEFRAG_IPV6_H

struct net;
int nf_defrag_ipv6_enable(struct net *);

int nf_ct_frag6_init(void);
void nf_ct_frag6_cleanup(void);
int nf_ct_frag6_gather(struct net *net, struct sk_buff *skb, u32 user);

struct inet_frags_ctl;

#endif 
