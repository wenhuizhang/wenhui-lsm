/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef __LINUX_IF_PPPOX_H
#define __LINUX_IF_PPPOX_H

#include <linux/if.h>
#include <linux/netdevice.h>
#include <linux/ppp_channel.h>
#include <linux/skbuff.h>
#include <linux/workqueue.h>
#include <uapi/linux/if_pppox.h>

static inline struct pppoe_hdr *pppoe_hdr(const struct sk_buff *skb)
{
	return (struct pppoe_hdr *)skb_network_header(skb);
}

struct pppoe_opt {
	struct net_device      *dev;	  
	int			ifindex;  
	struct pppoe_addr	pa;	  
	struct sockaddr_pppox	relay;	  
	struct work_struct      padt_work;
};

struct pptp_opt {
	struct pptp_addr src_addr;
	struct pptp_addr dst_addr;
	u32 ack_sent, ack_recv;
	u32 seq_sent, seq_recv;
	int ppp_flags;
};
#include <net/sock.h>

struct pppox_sock {
	
	struct sock sk;
	struct ppp_channel chan;
	struct pppox_sock	*next;	  
	union {
		struct pppoe_opt pppoe;
		struct pptp_opt  pptp;
	} proto;
	__be16			num;
};
#define pppoe_dev	proto.pppoe.dev
#define pppoe_ifindex	proto.pppoe.ifindex
#define pppoe_pa	proto.pppoe.pa
#define pppoe_relay	proto.pppoe.relay

static inline struct pppox_sock *pppox_sk(struct sock *sk)
{
	return (struct pppox_sock *)sk;
}

static inline struct sock *sk_pppox(struct pppox_sock *po)
{
	return (struct sock *)po;
}

struct module;

struct pppox_proto {
	int		(*create)(struct net *net, struct socket *sock, int kern);
	int		(*ioctl)(struct socket *sock, unsigned int cmd,
				 unsigned long arg);
	struct module	*owner;
};

extern int register_pppox_proto(int proto_num, const struct pppox_proto *pp);
extern void unregister_pppox_proto(int proto_num);
extern void pppox_unbind_sock(struct sock *sk);
extern int pppox_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg);
extern int pppox_compat_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg);

#define PPPOEIOCSFWD32    _IOW(0xB1 ,0, compat_size_t)


enum {
    PPPOX_NONE		= 0,  
    PPPOX_CONNECTED	= 1,  
    PPPOX_BOUND		= 2,  
    PPPOX_RELAY		= 4,  
    PPPOX_DEAD		= 16  
};

#endif 
