/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef _LINUX_NETDEV_FEATURES_H
#define _LINUX_NETDEV_FEATURES_H

#include <linux/types.h>
#include <linux/bitops.h>
#include <asm/byteorder.h>

typedef u64 netdev_features_t;

enum {
	NETIF_F_SG_BIT,			
	NETIF_F_IP_CSUM_BIT,		
	__UNUSED_NETIF_F_1,
	NETIF_F_HW_CSUM_BIT,		
	NETIF_F_IPV6_CSUM_BIT,		
	NETIF_F_HIGHDMA_BIT,		
	NETIF_F_FRAGLIST_BIT,		
	NETIF_F_HW_VLAN_CTAG_TX_BIT,	
	NETIF_F_HW_VLAN_CTAG_RX_BIT,	
	NETIF_F_HW_VLAN_CTAG_FILTER_BIT,
	NETIF_F_VLAN_CHALLENGED_BIT,	
	NETIF_F_GSO_BIT,		
	NETIF_F_LLTX_BIT,		
					
	NETIF_F_NETNS_LOCAL_BIT,	
	NETIF_F_GRO_BIT,		
	NETIF_F_LRO_BIT,		

	NETIF_F_GSO_SHIFT,		
	NETIF_F_TSO_BIT			
		= NETIF_F_GSO_SHIFT,
	NETIF_F_GSO_ROBUST_BIT,		
	NETIF_F_TSO_ECN_BIT,		
	NETIF_F_TSO_MANGLEID_BIT,	
	NETIF_F_TSO6_BIT,		
	NETIF_F_FSO_BIT,		
	NETIF_F_GSO_GRE_BIT,		
	NETIF_F_GSO_GRE_CSUM_BIT,	
	NETIF_F_GSO_IPXIP4_BIT,		
	NETIF_F_GSO_IPXIP6_BIT,		
	NETIF_F_GSO_UDP_TUNNEL_BIT,	
	NETIF_F_GSO_UDP_TUNNEL_CSUM_BIT,
	NETIF_F_GSO_PARTIAL_BIT,	
	NETIF_F_GSO_TUNNEL_REMCSUM_BIT, 
	NETIF_F_GSO_SCTP_BIT,		
	NETIF_F_GSO_ESP_BIT,		
	NETIF_F_GSO_UDP_BIT,		
	NETIF_F_GSO_UDP_L4_BIT,		
	NETIF_F_GSO_LAST =		
		NETIF_F_GSO_UDP_L4_BIT,

	NETIF_F_FCOE_CRC_BIT,		
	NETIF_F_SCTP_CRC_BIT,		
	NETIF_F_FCOE_MTU_BIT,		
	NETIF_F_NTUPLE_BIT,		
	NETIF_F_RXHASH_BIT,		
	NETIF_F_RXCSUM_BIT,		
	NETIF_F_NOCACHE_COPY_BIT,	
	NETIF_F_LOOPBACK_BIT,		
	NETIF_F_RXFCS_BIT,		
	NETIF_F_RXALL_BIT,		
	NETIF_F_HW_VLAN_STAG_TX_BIT,	
	NETIF_F_HW_VLAN_STAG_RX_BIT,	
	NETIF_F_HW_VLAN_STAG_FILTER_BIT,
	NETIF_F_HW_L2FW_DOFFLOAD_BIT,	

	NETIF_F_HW_TC_BIT,		
	NETIF_F_HW_ESP_BIT,		
	NETIF_F_HW_ESP_TX_CSUM_BIT,	
	NETIF_F_RX_UDP_TUNNEL_PORT_BIT, 
	NETIF_F_HW_TLS_TX_BIT,		
	NETIF_F_HW_TLS_RX_BIT,		

	NETIF_F_GRO_HW_BIT,		
	NETIF_F_HW_TLS_RECORD_BIT,	

	

	NETDEV_FEATURE_COUNT
};


#define __NETIF_F_BIT(bit)	((netdev_features_t)1 << (bit))
#define __NETIF_F(name)		__NETIF_F_BIT(NETIF_F_##name##_BIT)

#define NETIF_F_FCOE_CRC	__NETIF_F(FCOE_CRC)
#define NETIF_F_FCOE_MTU	__NETIF_F(FCOE_MTU)
#define NETIF_F_FRAGLIST	__NETIF_F(FRAGLIST)
#define NETIF_F_FSO		__NETIF_F(FSO)
#define NETIF_F_GRO		__NETIF_F(GRO)
#define NETIF_F_GRO_HW		__NETIF_F(GRO_HW)
#define NETIF_F_GSO		__NETIF_F(GSO)
#define NETIF_F_GSO_ROBUST	__NETIF_F(GSO_ROBUST)
#define NETIF_F_HIGHDMA		__NETIF_F(HIGHDMA)
#define NETIF_F_HW_CSUM		__NETIF_F(HW_CSUM)
#define NETIF_F_HW_VLAN_CTAG_FILTER __NETIF_F(HW_VLAN_CTAG_FILTER)
#define NETIF_F_HW_VLAN_CTAG_RX	__NETIF_F(HW_VLAN_CTAG_RX)
#define NETIF_F_HW_VLAN_CTAG_TX	__NETIF_F(HW_VLAN_CTAG_TX)
#define NETIF_F_IP_CSUM		__NETIF_F(IP_CSUM)
#define NETIF_F_IPV6_CSUM	__NETIF_F(IPV6_CSUM)
#define NETIF_F_LLTX		__NETIF_F(LLTX)
#define NETIF_F_LOOPBACK	__NETIF_F(LOOPBACK)
#define NETIF_F_LRO		__NETIF_F(LRO)
#define NETIF_F_NETNS_LOCAL	__NETIF_F(NETNS_LOCAL)
#define NETIF_F_NOCACHE_COPY	__NETIF_F(NOCACHE_COPY)
#define NETIF_F_NTUPLE		__NETIF_F(NTUPLE)
#define NETIF_F_RXCSUM		__NETIF_F(RXCSUM)
#define NETIF_F_RXHASH		__NETIF_F(RXHASH)
#define NETIF_F_SCTP_CRC	__NETIF_F(SCTP_CRC)
#define NETIF_F_SG		__NETIF_F(SG)
#define NETIF_F_TSO6		__NETIF_F(TSO6)
#define NETIF_F_TSO_ECN		__NETIF_F(TSO_ECN)
#define NETIF_F_TSO		__NETIF_F(TSO)
#define NETIF_F_VLAN_CHALLENGED	__NETIF_F(VLAN_CHALLENGED)
#define NETIF_F_RXFCS		__NETIF_F(RXFCS)
#define NETIF_F_RXALL		__NETIF_F(RXALL)
#define NETIF_F_GSO_GRE		__NETIF_F(GSO_GRE)
#define NETIF_F_GSO_GRE_CSUM	__NETIF_F(GSO_GRE_CSUM)
#define NETIF_F_GSO_IPXIP4	__NETIF_F(GSO_IPXIP4)
#define NETIF_F_GSO_IPXIP6	__NETIF_F(GSO_IPXIP6)
#define NETIF_F_GSO_UDP_TUNNEL	__NETIF_F(GSO_UDP_TUNNEL)
#define NETIF_F_GSO_UDP_TUNNEL_CSUM __NETIF_F(GSO_UDP_TUNNEL_CSUM)
#define NETIF_F_TSO_MANGLEID	__NETIF_F(TSO_MANGLEID)
#define NETIF_F_GSO_PARTIAL	 __NETIF_F(GSO_PARTIAL)
#define NETIF_F_GSO_TUNNEL_REMCSUM __NETIF_F(GSO_TUNNEL_REMCSUM)
#define NETIF_F_GSO_SCTP	__NETIF_F(GSO_SCTP)
#define NETIF_F_GSO_ESP		__NETIF_F(GSO_ESP)
#define NETIF_F_GSO_UDP		__NETIF_F(GSO_UDP)
#define NETIF_F_HW_VLAN_STAG_FILTER __NETIF_F(HW_VLAN_STAG_FILTER)
#define NETIF_F_HW_VLAN_STAG_RX	__NETIF_F(HW_VLAN_STAG_RX)
#define NETIF_F_HW_VLAN_STAG_TX	__NETIF_F(HW_VLAN_STAG_TX)
#define NETIF_F_HW_L2FW_DOFFLOAD	__NETIF_F(HW_L2FW_DOFFLOAD)
#define NETIF_F_HW_TC		__NETIF_F(HW_TC)
#define NETIF_F_HW_ESP		__NETIF_F(HW_ESP)
#define NETIF_F_HW_ESP_TX_CSUM	__NETIF_F(HW_ESP_TX_CSUM)
#define	NETIF_F_RX_UDP_TUNNEL_PORT  __NETIF_F(RX_UDP_TUNNEL_PORT)
#define NETIF_F_HW_TLS_RECORD	__NETIF_F(HW_TLS_RECORD)
#define NETIF_F_GSO_UDP_L4	__NETIF_F(GSO_UDP_L4)
#define NETIF_F_HW_TLS_TX	__NETIF_F(HW_TLS_TX)
#define NETIF_F_HW_TLS_RX	__NETIF_F(HW_TLS_RX)


static inline int find_next_netdev_feature(u64 feature, unsigned long start)
{
	
	feature &= ~0ULL >> (-start & ((sizeof(feature) * 8) - 1));

	return fls64(feature) - 1;
}


#define for_each_netdev_feature(mask_addr, bit)				\
	for ((bit) = find_next_netdev_feature((mask_addr),		\
					      NETDEV_FEATURE_COUNT);	\
	     (bit) >= 0;						\
	     (bit) = find_next_netdev_feature((mask_addr), (bit) - 1))



#define NETIF_F_NEVER_CHANGE	(NETIF_F_VLAN_CHALLENGED | \
				 NETIF_F_LLTX | NETIF_F_NETNS_LOCAL)


#define NETIF_F_ETHTOOL_BITS	((__NETIF_F_BIT(NETDEV_FEATURE_COUNT - 1) | \
		(__NETIF_F_BIT(NETDEV_FEATURE_COUNT - 1) - 1)) & \
		~NETIF_F_NEVER_CHANGE)


#define NETIF_F_GSO_MASK	(__NETIF_F_BIT(NETIF_F_GSO_LAST + 1) - \
		__NETIF_F_BIT(NETIF_F_GSO_SHIFT))


#define NETIF_F_CSUM_MASK	(NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM | \
				 NETIF_F_HW_CSUM)

#define NETIF_F_ALL_TSO 	(NETIF_F_TSO | NETIF_F_TSO6 | \
				 NETIF_F_TSO_ECN | NETIF_F_TSO_MANGLEID)

#define NETIF_F_ALL_FCOE	(NETIF_F_FCOE_CRC | NETIF_F_FCOE_MTU | \
				 NETIF_F_FSO)


#define NETIF_F_GSO_SOFTWARE	(NETIF_F_ALL_TSO | \
				 NETIF_F_GSO_SCTP)


#define NETIF_F_ONE_FOR_ALL	(NETIF_F_GSO_SOFTWARE | NETIF_F_GSO_ROBUST | \
				 NETIF_F_SG | NETIF_F_HIGHDMA |		\
				 NETIF_F_FRAGLIST | NETIF_F_VLAN_CHALLENGED)


#define NETIF_F_ALL_FOR_ALL	(NETIF_F_NOCACHE_COPY | NETIF_F_FSO)


#define NETIF_F_UPPER_DISABLES	NETIF_F_LRO


#define NETIF_F_SOFT_FEATURES	(NETIF_F_GSO | NETIF_F_GRO)

#define NETIF_F_VLAN_FEATURES	(NETIF_F_HW_VLAN_CTAG_FILTER | \
				 NETIF_F_HW_VLAN_CTAG_RX | \
				 NETIF_F_HW_VLAN_CTAG_TX | \
				 NETIF_F_HW_VLAN_STAG_FILTER | \
				 NETIF_F_HW_VLAN_STAG_RX | \
				 NETIF_F_HW_VLAN_STAG_TX)

#define NETIF_F_GSO_ENCAP_ALL	(NETIF_F_GSO_GRE |			\
				 NETIF_F_GSO_GRE_CSUM |			\
				 NETIF_F_GSO_IPXIP4 |			\
				 NETIF_F_GSO_IPXIP6 |			\
				 NETIF_F_GSO_UDP_TUNNEL |		\
				 NETIF_F_GSO_UDP_TUNNEL_CSUM)

#endif	
