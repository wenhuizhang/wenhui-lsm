/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NET_EVENT_H
#define _NET_EVENT_H



struct dst_entry;
struct neighbour;

struct netevent_redirect {
	struct dst_entry *old;
	struct dst_entry *new;
	struct neighbour *neigh;
	const void *daddr;
};

enum netevent_notif_type {
	NETEVENT_NEIGH_UPDATE = 1, 
	NETEVENT_REDIRECT,	   
	NETEVENT_DELAY_PROBE_TIME_UPDATE, 
	NETEVENT_IPV4_MPATH_HASH_UPDATE, 
	NETEVENT_IPV6_MPATH_HASH_UPDATE, 
	NETEVENT_IPV4_FWD_UPDATE_PRIORITY_UPDATE, 
};

int register_netevent_notifier(struct notifier_block *nb);
int unregister_netevent_notifier(struct notifier_block *nb);
int call_netevent_notifiers(unsigned long val, void *v);

#endif
