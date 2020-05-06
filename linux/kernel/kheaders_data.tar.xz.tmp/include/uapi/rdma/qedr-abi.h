/* SPDX-License-Identifier: ((GPL-2.0 WITH Linux-syscall-note) OR Linux-OpenIB) */

#ifndef __QEDR_USER_H__
#define __QEDR_USER_H__

#include <linux/types.h>

#define QEDR_ABI_VERSION		(8)



struct qedr_alloc_ucontext_resp {
	__aligned_u64 db_pa;
	__u32 db_size;

	__u32 max_send_wr;
	__u32 max_recv_wr;
	__u32 max_srq_wr;
	__u32 sges_per_send_wr;
	__u32 sges_per_recv_wr;
	__u32 sges_per_srq_wr;
	__u32 max_cqes;
	__u8 dpm_enabled;
	__u8 wids_enabled;
	__u16 wid_count;
	__u32 reserved;
};

struct qedr_alloc_pd_ureq {
	__aligned_u64 rsvd1;
};

struct qedr_alloc_pd_uresp {
	__u32 pd_id;
	__u32 reserved;
};

struct qedr_create_cq_ureq {
	__aligned_u64 addr;
	__aligned_u64 len;
};

struct qedr_create_cq_uresp {
	__u32 db_offset;
	__u16 icid;
	__u16 reserved;
};

struct qedr_create_qp_ureq {
	__u32 qp_handle_hi;
	__u32 qp_handle_lo;

	
	
	__aligned_u64 sq_addr;

	
	__aligned_u64 sq_len;

	
	
	__aligned_u64 rq_addr;

	
	__aligned_u64 rq_len;
};

struct qedr_create_qp_uresp {
	__u32 qp_id;
	__u32 atomic_supported;

	
	__u32 sq_db_offset;
	__u16 sq_icid;

	
	__u32 rq_db_offset;
	__u16 rq_icid;

	__u32 rq_db2_offset;
	__u32 reserved;
};

struct qedr_create_srq_ureq {
	
	__aligned_u64 prod_pair_addr;

	
	__aligned_u64 srq_addr;

	
	__aligned_u64 srq_len;
};

struct qedr_create_srq_uresp {
	__u16 srq_id;
	__u16 reserved0;
	__u32 reserved1;
};

#endif 
