/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef __UAPI_TC_CT_H
#define __UAPI_TC_CT_H

#include <linux/types.h>
#include <linux/pkt_cls.h>

enum {
	TCA_CT_UNSPEC,
	TCA_CT_PARMS,
	TCA_CT_TM,
	TCA_CT_ACTION,		
	TCA_CT_ZONE,		
	TCA_CT_MARK,		
	TCA_CT_MARK_MASK,	
	TCA_CT_LABELS,		
	TCA_CT_LABELS_MASK,	
	TCA_CT_NAT_IPV4_MIN,	
	TCA_CT_NAT_IPV4_MAX,	
	TCA_CT_NAT_IPV6_MIN,	
	TCA_CT_NAT_IPV6_MAX,	
	TCA_CT_NAT_PORT_MIN,	
	TCA_CT_NAT_PORT_MAX,	
	TCA_CT_PAD,
	__TCA_CT_MAX
};

#define TCA_CT_MAX (__TCA_CT_MAX - 1)

#define TCA_CT_ACT_COMMIT	(1 << 0)
#define TCA_CT_ACT_FORCE	(1 << 1)
#define TCA_CT_ACT_CLEAR	(1 << 2)
#define TCA_CT_ACT_NAT		(1 << 3)
#define TCA_CT_ACT_NAT_SRC	(1 << 4)
#define TCA_CT_ACT_NAT_DST	(1 << 5)

struct tc_ct {
	tc_gen;
};

#endif 
