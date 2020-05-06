#ifndef __LINUX_ERSPAN_H
#define __LINUX_ERSPAN_H



#include <uapi/linux/erspan.h>

#define ERSPAN_VERSION	0x1	
#define VER_MASK	0xf000
#define VLAN_MASK	0x0fff
#define COS_MASK	0xe000
#define EN_MASK		0x1800
#define T_MASK		0x0400
#define ID_MASK		0x03ff
#define INDEX_MASK	0xfffff

#define ERSPAN_VERSION2	0x2	
#define BSO_MASK	EN_MASK
#define SGT_MASK	0xffff0000
#define P_MASK		0x8000
#define FT_MASK		0x7c00
#define HWID_MASK	0x03f0
#define DIR_MASK	0x0008
#define GRA_MASK	0x0006
#define O_MASK		0x0001

#define HWID_OFFSET    4
#define DIR_OFFSET     3

enum erspan_encap_type {
	ERSPAN_ENCAP_NOVLAN = 0x0,	
	ERSPAN_ENCAP_ISL = 0x1,		
	ERSPAN_ENCAP_8021Q = 0x2,	
	ERSPAN_ENCAP_INFRAME = 0x3,	
};

#define ERSPAN_V1_MDSIZE	4
#define ERSPAN_V2_MDSIZE	8

struct erspan_base_hdr {
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8	vlan_upper:4,
		ver:4;
	__u8	vlan:8;
	__u8	session_id_upper:2,
		t:1,
		en:2,
		cos:3;
	__u8	session_id:8;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u8	ver: 4,
		vlan_upper:4;
	__u8	vlan:8;
	__u8	cos:3,
		en:2,
		t:1,
		session_id_upper:2;
	__u8	session_id:8;
#else
#error "Please fix <asm/byteorder.h>"
#endif
};

static inline void set_session_id(struct erspan_base_hdr *ershdr, u16 id)
{
	ershdr->session_id = id & 0xff;
	ershdr->session_id_upper = (id >> 8) & 0x3;
}

static inline u16 get_session_id(const struct erspan_base_hdr *ershdr)
{
	return (ershdr->session_id_upper << 8) + ershdr->session_id;
}

static inline void set_vlan(struct erspan_base_hdr *ershdr, u16 vlan)
{
	ershdr->vlan = vlan & 0xff;
	ershdr->vlan_upper = (vlan >> 8) & 0xf;
}

static inline u16 get_vlan(const struct erspan_base_hdr *ershdr)
{
	return (ershdr->vlan_upper << 8) + ershdr->vlan;
}

static inline void set_hwid(struct erspan_md2 *md2, u8 hwid)
{
	md2->hwid = hwid & 0xf;
	md2->hwid_upper = (hwid >> 4) & 0x3;
}

static inline u8 get_hwid(const struct erspan_md2 *md2)
{
	return (md2->hwid_upper << 4) + md2->hwid;
}

static inline int erspan_hdr_len(int version)
{
	return sizeof(struct erspan_base_hdr) +
	       (version == 1 ? ERSPAN_V1_MDSIZE : ERSPAN_V2_MDSIZE);
}

static inline u8 tos_to_cos(u8 tos)
{
	u8 dscp, cos;

	dscp = tos >> 2;
	cos = dscp >> 3;
	return cos;
}

static inline void erspan_build_header(struct sk_buff *skb,
				u32 id, u32 index,
				bool truncate, bool is_ipv4)
{
	struct ethhdr *eth = (struct ethhdr *)skb->data;
	enum erspan_encap_type enc_type;
	struct erspan_base_hdr *ershdr;
	struct qtag_prefix {
		__be16 eth_type;
		__be16 tci;
	} *qp;
	u16 vlan_tci = 0;
	u8 tos;
	__be32 *idx;

	tos = is_ipv4 ? ip_hdr(skb)->tos :
			(ipv6_hdr(skb)->priority << 4) +
			(ipv6_hdr(skb)->flow_lbl[0] >> 4);

	enc_type = ERSPAN_ENCAP_NOVLAN;

	
	if (eth->h_proto == htons(ETH_P_8021Q)) {
		qp = (struct qtag_prefix *)(skb->data + 2 * ETH_ALEN);
		vlan_tci = ntohs(qp->tci);
		enc_type = ERSPAN_ENCAP_INFRAME;
	}

	skb_push(skb, sizeof(*ershdr) + ERSPAN_V1_MDSIZE);
	ershdr = (struct erspan_base_hdr *)skb->data;
	memset(ershdr, 0, sizeof(*ershdr) + ERSPAN_V1_MDSIZE);

	
	ershdr->ver = ERSPAN_VERSION;
	ershdr->cos = tos_to_cos(tos);
	ershdr->en = enc_type;
	ershdr->t = truncate;
	set_vlan(ershdr, vlan_tci);
	set_session_id(ershdr, id);

	
	idx = (__be32 *)(ershdr + 1);
	*idx = htonl(index & INDEX_MASK);
}


static inline __be32 erspan_get_timestamp(void)
{
	u64 h_usecs;
	ktime_t kt;

	kt = ktime_get_real();
	h_usecs = ktime_divns(kt, 100 * NSEC_PER_USEC);

	
	return htonl((u32)h_usecs);
}


enum erspan_bso {
	BSO_NOERROR = 0x0,
	BSO_SHORT = 0x1,
	BSO_OVERSIZED = 0x2,
	BSO_BAD = 0x3,
};

static inline u8 erspan_detect_bso(struct sk_buff *skb)
{
	
	if (skb->len < ETH_ZLEN)
		return BSO_SHORT;

	if (skb->len > ETH_FRAME_LEN)
		return BSO_OVERSIZED;

	return BSO_NOERROR;
}

static inline void erspan_build_header_v2(struct sk_buff *skb,
					  u32 id, u8 direction, u16 hwid,
					  bool truncate, bool is_ipv4)
{
	struct ethhdr *eth = (struct ethhdr *)skb->data;
	struct erspan_base_hdr *ershdr;
	struct erspan_md2 *md2;
	struct qtag_prefix {
		__be16 eth_type;
		__be16 tci;
	} *qp;
	u16 vlan_tci = 0;
	u8 gra = 0; 
	u8 bso = 0; 
	u8 sgt = 0;
	u8 tos;

	tos = is_ipv4 ? ip_hdr(skb)->tos :
			(ipv6_hdr(skb)->priority << 4) +
			(ipv6_hdr(skb)->flow_lbl[0] >> 4);

	
	if (eth->h_proto == htons(ETH_P_8021Q)) {
		qp = (struct qtag_prefix *)(skb->data + 2 * ETH_ALEN);
		vlan_tci = ntohs(qp->tci);
	}

	bso = erspan_detect_bso(skb);
	skb_push(skb, sizeof(*ershdr) + ERSPAN_V2_MDSIZE);
	ershdr = (struct erspan_base_hdr *)skb->data;
	memset(ershdr, 0, sizeof(*ershdr) + ERSPAN_V2_MDSIZE);

	
	ershdr->ver = ERSPAN_VERSION2;
	ershdr->cos = tos_to_cos(tos);
	ershdr->en = bso;
	ershdr->t = truncate;
	set_vlan(ershdr, vlan_tci);
	set_session_id(ershdr, id);

	
	md2 = (struct erspan_md2 *)(ershdr + 1);
	md2->timestamp = erspan_get_timestamp();
	md2->sgt = htons(sgt);
	md2->p = 1;
	md2->ft = 0;
	md2->dir = direction;
	md2->gra = gra;
	md2->o = 0;
	set_hwid(md2, hwid);
}

#endif
