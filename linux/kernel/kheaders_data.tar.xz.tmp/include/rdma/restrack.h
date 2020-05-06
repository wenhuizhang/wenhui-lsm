/* SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB */


#ifndef _RDMA_RESTRACK_H_
#define _RDMA_RESTRACK_H_

#include <linux/typecheck.h>
#include <linux/sched.h>
#include <linux/kref.h>
#include <linux/completion.h>
#include <linux/sched/task.h>
#include <uapi/rdma/rdma_netlink.h>
#include <linux/xarray.h>

struct ib_device;
struct sk_buff;


enum rdma_restrack_type {
	
	RDMA_RESTRACK_PD,
	
	RDMA_RESTRACK_CQ,
	
	RDMA_RESTRACK_QP,
	
	RDMA_RESTRACK_CM_ID,
	
	RDMA_RESTRACK_MR,
	
	RDMA_RESTRACK_CTX,
	
	RDMA_RESTRACK_COUNTER,
	
	RDMA_RESTRACK_MAX
};


struct rdma_restrack_entry {
	
	bool			valid;
	
	struct kref		kref;
	
	struct completion	comp;
	
	struct task_struct	*task;
	
	const char		*kern_name;
	
	enum rdma_restrack_type	type;
	
	bool			user;
	
	u32 id;
};

int rdma_restrack_count(struct ib_device *dev,
			enum rdma_restrack_type type);

void rdma_restrack_kadd(struct rdma_restrack_entry *res);
void rdma_restrack_uadd(struct rdma_restrack_entry *res);


void rdma_restrack_del(struct rdma_restrack_entry *res);


static inline bool rdma_is_kernel_res(struct rdma_restrack_entry *res)
{
	return !res->user;
}


int __must_check rdma_restrack_get(struct rdma_restrack_entry *res);


int rdma_restrack_put(struct rdma_restrack_entry *res);


void rdma_restrack_set_task(struct rdma_restrack_entry *res,
			    const char *caller);


int rdma_nl_put_driver_u32(struct sk_buff *msg, const char *name, u32 value);
int rdma_nl_put_driver_u32_hex(struct sk_buff *msg, const char *name,
			       u32 value);
int rdma_nl_put_driver_u64(struct sk_buff *msg, const char *name, u64 value);
int rdma_nl_put_driver_u64_hex(struct sk_buff *msg, const char *name,
			       u64 value);
struct rdma_restrack_entry *rdma_restrack_get_byid(struct ib_device *dev,
						   enum rdma_restrack_type type,
						   u32 id);
#endif 
