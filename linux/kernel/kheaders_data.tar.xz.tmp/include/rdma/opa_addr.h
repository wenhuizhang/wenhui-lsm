

#ifndef OPA_ADDR_H
#define OPA_ADDR_H

#include <rdma/opa_smi.h>

#define	OPA_SPECIAL_OUI		(0x00066AULL)
#define OPA_MAKE_ID(x)          (cpu_to_be64(OPA_SPECIAL_OUI << 40 | (x)))
#define OPA_TO_IB_UCAST_LID(x) (((x) >= be16_to_cpu(IB_MULTICAST_LID_BASE)) \
				? 0 : x)
#define OPA_GID_INDEX		0x1

#define OPA_MCAST_NR 0x4 
#define OPA_COLLECTIVE_NR 0x1 


static inline bool ib_is_opa_gid(const union ib_gid *gid)
{
	return ((be64_to_cpu(gid->global.interface_id) >> 40) ==
		OPA_SPECIAL_OUI);
}


static inline u32 opa_get_lid_from_gid(const union ib_gid *gid)
{
	return be64_to_cpu(gid->global.interface_id) & 0xFFFFFFFF;
}


static inline bool opa_is_extended_lid(__be32 dlid, __be32 slid)
{
	if ((be32_to_cpu(dlid) >=
	     be16_to_cpu(IB_MULTICAST_LID_BASE)) ||
	    (be32_to_cpu(slid) >=
	     be16_to_cpu(IB_MULTICAST_LID_BASE)))
		return true;

	return false;
}


static inline u32 opa_get_mcast_base(u32 nr_top_bits)
{
	return (be32_to_cpu(OPA_LID_PERMISSIVE) << (32 - nr_top_bits));
}


static inline bool rdma_is_valid_unicast_lid(struct rdma_ah_attr *attr)
{
	if (attr->type == RDMA_AH_ATTR_TYPE_IB) {
		if (!rdma_ah_get_dlid(attr) ||
		    rdma_ah_get_dlid(attr) >=
		    be16_to_cpu(IB_MULTICAST_LID_BASE))
			return false;
	} else if (attr->type == RDMA_AH_ATTR_TYPE_OPA) {
		if (!rdma_ah_get_dlid(attr) ||
		    rdma_ah_get_dlid(attr) >=
		    opa_get_mcast_base(OPA_MCAST_NR))
			return false;
	}
	return true;
}
#endif 
