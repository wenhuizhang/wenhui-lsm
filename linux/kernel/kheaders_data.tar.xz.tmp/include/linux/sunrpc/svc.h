/* SPDX-License-Identifier: GPL-2.0 */



#ifndef SUNRPC_SVC_H
#define SUNRPC_SVC_H

#include <linux/in.h>
#include <linux/in6.h>
#include <linux/sunrpc/types.h>
#include <linux/sunrpc/xdr.h>
#include <linux/sunrpc/auth.h>
#include <linux/sunrpc/svcauth.h>
#include <linux/wait.h>
#include <linux/mm.h>


struct svc_pool_stats {
	atomic_long_t	packets;
	unsigned long	sockets_queued;
	atomic_long_t	threads_woken;
	atomic_long_t	threads_timedout;
};


struct svc_pool {
	unsigned int		sp_id;	    	
	spinlock_t		sp_lock;	
	struct list_head	sp_sockets;	
	unsigned int		sp_nrthreads;	
	struct list_head	sp_all_threads;	
	struct svc_pool_stats	sp_stats;	
#define	SP_TASK_PENDING		(0)		
#define SP_CONGESTED		(1)
	unsigned long		sp_flags;
} ____cacheline_aligned_in_smp;

struct svc_serv;

struct svc_serv_ops {
	
	void		(*svo_shutdown)(struct svc_serv *, struct net *);

	
	int		(*svo_function)(void *);

	
	void		(*svo_enqueue_xprt)(struct svc_xprt *);

	
	int		(*svo_setup)(struct svc_serv *, struct svc_pool *, int);

	
	struct module	*svo_module;
};


struct svc_serv {
	struct svc_program *	sv_program;	
	struct svc_stat *	sv_stats;	
	spinlock_t		sv_lock;
	unsigned int		sv_nrthreads;	
	unsigned int		sv_maxconn;	

	unsigned int		sv_max_payload;	
	unsigned int		sv_max_mesg;	
	unsigned int		sv_xdrsize;	
	struct list_head	sv_permsocks;	
	struct list_head	sv_tempsocks;	
	int			sv_tmpcnt;	
	struct timer_list	sv_temptimer;	

	char *			sv_name;	

	unsigned int		sv_nrpools;	
	struct svc_pool *	sv_pools;	
	const struct svc_serv_ops *sv_ops;	
#if defined(CONFIG_SUNRPC_BACKCHANNEL)
	struct list_head	sv_cb_list;	
	spinlock_t		sv_cb_lock;	
	wait_queue_head_t	sv_cb_waitq;	
	bool			sv_bc_enabled;	
#endif 
};


static inline void svc_get(struct svc_serv *serv)
{
	serv->sv_nrthreads++;
}


#define RPCSVC_MAXPAYLOAD	(1*1024*1024u)
#define RPCSVC_MAXPAYLOAD_TCP	RPCSVC_MAXPAYLOAD
#define RPCSVC_MAXPAYLOAD_UDP	(32*1024u)

extern u32 svc_max_payload(const struct svc_rqst *rqstp);


#define RPCSVC_MAXPAGES		((RPCSVC_MAXPAYLOAD+PAGE_SIZE-1)/PAGE_SIZE \
				+ 2 + 1)

static inline u32 svc_getnl(struct kvec *iov)
{
	__be32 val, *vp;
	vp = iov->iov_base;
	val = *vp++;
	iov->iov_base = (void*)vp;
	iov->iov_len -= sizeof(__be32);
	return ntohl(val);
}

static inline void svc_putnl(struct kvec *iov, u32 val)
{
	__be32 *vp = iov->iov_base + iov->iov_len;
	*vp = htonl(val);
	iov->iov_len += sizeof(__be32);
}

static inline __be32 svc_getu32(struct kvec *iov)
{
	__be32 val, *vp;
	vp = iov->iov_base;
	val = *vp++;
	iov->iov_base = (void*)vp;
	iov->iov_len -= sizeof(__be32);
	return val;
}

static inline void svc_ungetu32(struct kvec *iov)
{
	__be32 *vp = (__be32 *)iov->iov_base;
	iov->iov_base = (void *)(vp - 1);
	iov->iov_len += sizeof(*vp);
}

static inline void svc_putu32(struct kvec *iov, __be32 val)
{
	__be32 *vp = iov->iov_base + iov->iov_len;
	*vp = val;
	iov->iov_len += sizeof(__be32);
}


struct svc_rqst {
	struct list_head	rq_all;		
	struct rcu_head		rq_rcu_head;	
	struct svc_xprt *	rq_xprt;	

	struct sockaddr_storage	rq_addr;	
	size_t			rq_addrlen;
	struct sockaddr_storage	rq_daddr;	
	size_t			rq_daddrlen;

	struct svc_serv *	rq_server;	
	struct svc_pool *	rq_pool;	
	const struct svc_procedure *rq_procinfo;
	struct auth_ops *	rq_authop;	
	struct svc_cred		rq_cred;	
	void *			rq_xprt_ctxt;	
	struct svc_deferred_req*rq_deferred;	

	size_t			rq_xprt_hlen;	
	struct xdr_buf		rq_arg;
	struct xdr_buf		rq_res;
	struct page		*rq_pages[RPCSVC_MAXPAGES + 1];
	struct page *		*rq_respages;	
	struct page *		*rq_next_page; 
	struct page *		*rq_page_end;  

	struct kvec		rq_vec[RPCSVC_MAXPAGES]; 

	__be32			rq_xid;		
	u32			rq_prog;	
	u32			rq_vers;	
	u32			rq_proc;	
	u32			rq_prot;	
	int			rq_cachetype;	
#define	RQ_SECURE	(0)			
#define	RQ_LOCAL	(1)			
#define	RQ_USEDEFERRAL	(2)			
#define	RQ_DROPME	(3)			
#define	RQ_SPLICE_OK	(4)			
#define	RQ_VICTIM	(5)			
#define	RQ_BUSY		(6)			
#define	RQ_DATA		(7)			
#define RQ_AUTHERR	(8)			
	unsigned long		rq_flags;	
	ktime_t			rq_qtime;	

	void *			rq_argp;	
	void *			rq_resp;	
	void *			rq_auth_data;	
	int			rq_auth_slack;	
	int			rq_reserved;	
	ktime_t			rq_stime;	

	struct cache_req	rq_chandle;	
	
	struct auth_domain *	rq_client;	
	struct auth_domain *	rq_gssclient;	
	struct svc_cacherep *	rq_cacherep;	
	struct task_struct	*rq_task;	
	spinlock_t		rq_lock;	
	struct net		*rq_bc_net;	
};

#define SVC_NET(rqst) (rqst->rq_xprt ? rqst->rq_xprt->xpt_net : rqst->rq_bc_net)


static inline struct sockaddr_in *svc_addr_in(const struct svc_rqst *rqst)
{
	return (struct sockaddr_in *) &rqst->rq_addr;
}

static inline struct sockaddr_in6 *svc_addr_in6(const struct svc_rqst *rqst)
{
	return (struct sockaddr_in6 *) &rqst->rq_addr;
}

static inline struct sockaddr *svc_addr(const struct svc_rqst *rqst)
{
	return (struct sockaddr *) &rqst->rq_addr;
}

static inline struct sockaddr_in *svc_daddr_in(const struct svc_rqst *rqst)
{
	return (struct sockaddr_in *) &rqst->rq_daddr;
}

static inline struct sockaddr_in6 *svc_daddr_in6(const struct svc_rqst *rqst)
{
	return (struct sockaddr_in6 *) &rqst->rq_daddr;
}

static inline struct sockaddr *svc_daddr(const struct svc_rqst *rqst)
{
	return (struct sockaddr *) &rqst->rq_daddr;
}


static inline int
xdr_argsize_check(struct svc_rqst *rqstp, __be32 *p)
{
	char *cp = (char *)p;
	struct kvec *vec = &rqstp->rq_arg.head[0];
	return cp >= (char*)vec->iov_base
		&& cp <= (char*)vec->iov_base + vec->iov_len;
}

static inline int
xdr_ressize_check(struct svc_rqst *rqstp, __be32 *p)
{
	struct kvec *vec = &rqstp->rq_res.head[0];
	char *cp = (char*)p;

	vec->iov_len = cp - (char*)vec->iov_base;

	return vec->iov_len <= PAGE_SIZE;
}

static inline void svc_free_res_pages(struct svc_rqst *rqstp)
{
	while (rqstp->rq_next_page != rqstp->rq_respages) {
		struct page **pp = --rqstp->rq_next_page;
		if (*pp) {
			put_page(*pp);
			*pp = NULL;
		}
	}
}

struct svc_deferred_req {
	u32			prot;	
	struct svc_xprt		*xprt;
	struct sockaddr_storage	addr;	
	size_t			addrlen;
	struct sockaddr_storage	daddr;	
	size_t			daddrlen;
	struct cache_deferred_req handle;
	size_t			xprt_hlen;
	int			argslen;
	__be32			args[0];
};

struct svc_process_info {
	union {
		int  (*dispatch)(struct svc_rqst *, __be32 *);
		struct {
			unsigned int lovers;
			unsigned int hivers;
		} mismatch;
	};
};


struct svc_program {
	struct svc_program *	pg_next;	
	u32			pg_prog;	
	unsigned int		pg_lovers;	
	unsigned int		pg_hivers;	
	unsigned int		pg_nvers;	
	const struct svc_version **pg_vers;	
	char *			pg_name;	
	char *			pg_class;	
	struct svc_stat *	pg_stats;	
	int			(*pg_authenticate)(struct svc_rqst *);
	__be32			(*pg_init_request)(struct svc_rqst *,
						   const struct svc_program *,
						   struct svc_process_info *);
	int			(*pg_rpcbind_set)(struct net *net,
						  const struct svc_program *,
						  u32 version, int family,
						  unsigned short proto,
						  unsigned short port);
};


struct svc_version {
	u32			vs_vers;	
	u32			vs_nproc;	
	const struct svc_procedure *vs_proc;	
	unsigned int		*vs_count;	
	u32			vs_xdrsize;	

	
	bool			vs_hidden;

	
	bool			vs_rpcb_optnl;

	
	bool			vs_need_cong_ctrl;

	
	int			(*vs_dispatch)(struct svc_rqst *, __be32 *);
};


struct svc_procedure {
	
	__be32			(*pc_func)(struct svc_rqst *);
	
	int			(*pc_decode)(struct svc_rqst *, __be32 *data);
	
	int			(*pc_encode)(struct svc_rqst *, __be32 *data);
	
	void			(*pc_release)(struct svc_rqst *);
	unsigned int		pc_argsize;	
	unsigned int		pc_ressize;	
	unsigned int		pc_cachetype;	
	unsigned int		pc_xdrressize;	
};


enum {
	SVC_POOL_AUTO = -1,	
	SVC_POOL_GLOBAL,	
	SVC_POOL_PERCPU,	
	SVC_POOL_PERNODE	
};

struct svc_pool_map {
	int count;			
	int mode;			
	unsigned int npools;
	unsigned int *pool_to;		
	unsigned int *to_pool;		
};

extern struct svc_pool_map svc_pool_map;


int svc_rpcb_setup(struct svc_serv *serv, struct net *net);
void svc_rpcb_cleanup(struct svc_serv *serv, struct net *net);
int svc_bind(struct svc_serv *serv, struct net *net);
struct svc_serv *svc_create(struct svc_program *, unsigned int,
			    const struct svc_serv_ops *);
struct svc_rqst *svc_rqst_alloc(struct svc_serv *serv,
					struct svc_pool *pool, int node);
struct svc_rqst *svc_prepare_thread(struct svc_serv *serv,
					struct svc_pool *pool, int node);
void		   svc_rqst_free(struct svc_rqst *);
void		   svc_exit_thread(struct svc_rqst *);
unsigned int	   svc_pool_map_get(void);
void		   svc_pool_map_put(void);
struct svc_serv *  svc_create_pooled(struct svc_program *, unsigned int,
			const struct svc_serv_ops *);
int		   svc_set_num_threads(struct svc_serv *, struct svc_pool *, int);
int		   svc_set_num_threads_sync(struct svc_serv *, struct svc_pool *, int);
int		   svc_pool_stats_open(struct svc_serv *serv, struct file *file);
void		   svc_destroy(struct svc_serv *);
void		   svc_shutdown_net(struct svc_serv *, struct net *);
int		   svc_process(struct svc_rqst *);
int		   bc_svc_process(struct svc_serv *, struct rpc_rqst *,
			struct svc_rqst *);
int		   svc_register(const struct svc_serv *, struct net *, const int,
				const unsigned short, const unsigned short);

void		   svc_wake_up(struct svc_serv *);
void		   svc_reserve(struct svc_rqst *rqstp, int space);
struct svc_pool *  svc_pool_for_cpu(struct svc_serv *serv, int cpu);
char *		   svc_print_addr(struct svc_rqst *, char *, size_t);
unsigned int	   svc_fill_write_vector(struct svc_rqst *rqstp,
					 struct page **pages,
					 struct kvec *first, size_t total);
char		  *svc_fill_symlink_pathname(struct svc_rqst *rqstp,
					     struct kvec *first, void *p,
					     size_t total);
__be32		   svc_return_autherr(struct svc_rqst *rqstp, __be32 auth_err);
__be32		   svc_generic_init_request(struct svc_rqst *rqstp,
					    const struct svc_program *progp,
					    struct svc_process_info *procinfo);
int		   svc_generic_rpcbind_set(struct net *net,
					   const struct svc_program *progp,
					   u32 version, int family,
					   unsigned short proto,
					   unsigned short port);
int		   svc_rpcbind_set_version(struct net *net,
					   const struct svc_program *progp,
					   u32 version, int family,
					   unsigned short proto,
					   unsigned short port);

#define	RPC_MAX_ADDRBUFLEN	(63U)


static inline void svc_reserve_auth(struct svc_rqst *rqstp, int space)
{
	svc_reserve(rqstp, space + rqstp->rq_auth_slack);
}

#endif 
