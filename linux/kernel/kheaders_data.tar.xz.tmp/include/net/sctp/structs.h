/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef __sctp_structs_h__
#define __sctp_structs_h__

#include <linux/ktime.h>
#include <linux/generic-radix-tree.h>
#include <linux/rhashtable-types.h>
#include <linux/socket.h>	
#include <linux/in.h>		
#include <linux/in6.h>		
#include <linux/ipv6.h>
#include <asm/param.h>		
#include <linux/atomic.h>		
#include <linux/skbuff.h>	
#include <linux/workqueue.h>	
#include <linux/sctp.h>		
#include <net/sctp/auth.h>	
#include <net/ip.h>		


union sctp_addr {
	struct sockaddr_in v4;
	struct sockaddr_in6 v6;
	struct sockaddr sa;
};


struct sctp_globals;
struct sctp_endpoint;
struct sctp_association;
struct sctp_transport;
struct sctp_packet;
struct sctp_chunk;
struct sctp_inq;
struct sctp_outq;
struct sctp_bind_addr;
struct sctp_ulpq;
struct sctp_ep_common;
struct crypto_shash;
struct sctp_stream;


#include <net/sctp/tsnmap.h>
#include <net/sctp/ulpevent.h>
#include <net/sctp/ulpqueue.h>
#include <net/sctp/stream_interleave.h>



struct sctp_bind_bucket {
	unsigned short	port;
	signed char	fastreuse;
	signed char	fastreuseport;
	kuid_t		fastuid;
	struct hlist_node	node;
	struct hlist_head	owner;
	struct net	*net;
};

struct sctp_bind_hashbucket {
	spinlock_t	lock;
	struct hlist_head	chain;
};


struct sctp_hashbucket {
	rwlock_t	lock;
	struct hlist_head	chain;
} __attribute__((__aligned__(8)));



extern struct sctp_globals {
	
	struct list_head address_families;

	
	struct sctp_hashbucket *ep_hashtable;
	
	struct sctp_bind_hashbucket *port_hashtable;
	
	struct rhltable transport_hashtable;

	
	int ep_hashsize;
	int port_hashsize;

	
	__u16 max_instreams;
	__u16 max_outstreams;

	
        bool checksum_disable;
} sctp_globals;

#define sctp_max_instreams		(sctp_globals.max_instreams)
#define sctp_max_outstreams		(sctp_globals.max_outstreams)
#define sctp_address_families		(sctp_globals.address_families)
#define sctp_ep_hashsize		(sctp_globals.ep_hashsize)
#define sctp_ep_hashtable		(sctp_globals.ep_hashtable)
#define sctp_port_hashsize		(sctp_globals.port_hashsize)
#define sctp_port_hashtable		(sctp_globals.port_hashtable)
#define sctp_transport_hashtable	(sctp_globals.transport_hashtable)
#define sctp_checksum_disable		(sctp_globals.checksum_disable)


enum sctp_socket_type {
	SCTP_SOCKET_UDP = 0,
	SCTP_SOCKET_UDP_HIGH_BANDWIDTH,
	SCTP_SOCKET_TCP
};


struct sctp_sock {
	
	struct inet_sock inet;
	
	enum sctp_socket_type type;

	
	struct sctp_pf *pf;

	
	struct crypto_shash *hmac;
	char *sctp_hmac_alg;

	
	struct sctp_endpoint *ep;

	struct sctp_bind_bucket *bind_hash;
	
	__u16 default_stream;
	__u32 default_ppid;
	__u16 default_flags;
	__u32 default_context;
	__u32 default_timetolive;
	__u32 default_rcv_context;
	int max_burst;

	
	__u32 hbinterval;

	
	__u16 pathmaxrxt;

	__u32 flowlabel;
	__u8  dscp;

	int pf_retrans;

	
	__u32 pathmtu;

	
	__u32 sackdelay;
	__u32 sackfreq;

	
	__u32 param_flags;

	__u32 default_ss;

	struct sctp_rtoinfo rtoinfo;
	struct sctp_paddrparams paddrparam;
	struct sctp_assocparams assocparams;

	
	__u16 subscribe;
	struct sctp_initmsg initmsg;

	int user_frag;

	__u32 autoclose;
	__u32 adaptation_ind;
	__u32 pd_point;
	__u16	nodelay:1,
		reuse:1,
		disable_fragments:1,
		v4mapped:1,
		frag_interleave:1,
		recvrcvinfo:1,
		recvnxtinfo:1,
		data_ready_signalled:1;

	atomic_t pd_mode;
	
	struct sk_buff_head pd_lobby;

	
	struct list_head auto_asconf_list;
	int do_auto_asconf;
};

static inline struct sctp_sock *sctp_sk(const struct sock *sk)
{
       return (struct sctp_sock *)sk;
}

static inline struct sock *sctp_opt2sk(const struct sctp_sock *sp)
{
       return (struct sock *)sp;
}

#if IS_ENABLED(CONFIG_IPV6)
struct sctp6_sock {
       struct sctp_sock  sctp;
       struct ipv6_pinfo inet6;
};
#endif 





struct sctp_cookie {

	
	__u32 my_vtag;

	
	__u32 peer_vtag;

	

	
	__u32 my_ttag;

	
	__u32 peer_ttag;

	
	ktime_t expiration;

	
	__u16 sinit_num_ostreams;
	__u16 sinit_max_instreams;

	
	__u32 initial_tsn;

	
	union sctp_addr peer_addr;

	
	__u16		my_port;

	__u8 prsctp_capable;

	
	__u8 padding;  		

	__u32 adaptation_ind;

	__u8 auth_random[sizeof(struct sctp_paramhdr) +
			 SCTP_AUTH_RANDOM_LENGTH];
	__u8 auth_hmacs[SCTP_AUTH_NUM_HMACS * sizeof(__u16) + 2];
	__u8 auth_chunks[sizeof(struct sctp_paramhdr) + SCTP_AUTH_MAX_CHUNKS];

	
	__u32 raw_addr_list_len;
	struct sctp_init_chunk peer_init[0];
};



struct sctp_signed_cookie {
	__u8 signature[SCTP_SECRET_SIZE];
	__u32 __pad;		
	struct sctp_cookie c;
} __packed;


union sctp_addr_param {
	struct sctp_paramhdr p;
	struct sctp_ipv4addr_param v4;
	struct sctp_ipv6addr_param v6;
};


union sctp_params {
	void *v;
	struct sctp_paramhdr *p;
	struct sctp_cookie_preserve_param *life;
	struct sctp_hostname_param *dns;
	struct sctp_cookie_param *cookie;
	struct sctp_supported_addrs_param *sat;
	struct sctp_ipv4addr_param *v4;
	struct sctp_ipv6addr_param *v6;
	union sctp_addr_param *addr;
	struct sctp_adaptation_ind_param *aind;
	struct sctp_supported_ext_param *ext;
	struct sctp_random_param *random;
	struct sctp_chunks_param *chunks;
	struct sctp_hmac_algo_param *hmac_algo;
	struct sctp_addip_param *addip;
};


struct sctp_sender_hb_info {
	struct sctp_paramhdr param_hdr;
	union sctp_addr daddr;
	unsigned long sent_at;
	__u64 hb_nonce;
};

int sctp_stream_init(struct sctp_stream *stream, __u16 outcnt, __u16 incnt,
		     gfp_t gfp);
int sctp_stream_init_ext(struct sctp_stream *stream, __u16 sid);
void sctp_stream_free(struct sctp_stream *stream);
void sctp_stream_clear(struct sctp_stream *stream);
void sctp_stream_update(struct sctp_stream *stream, struct sctp_stream *new);


#define sctp_ssn_peek(stream, type, sid) \
	(sctp_stream_##type((stream), (sid))->ssn)


#define sctp_ssn_next(stream, type, sid) \
	(sctp_stream_##type((stream), (sid))->ssn++)


#define sctp_ssn_skip(stream, type, sid, ssn) \
	(sctp_stream_##type((stream), (sid))->ssn = ssn + 1)


#define sctp_mid_peek(stream, type, sid) \
	(sctp_stream_##type((stream), (sid))->mid)


#define sctp_mid_next(stream, type, sid) \
	(sctp_stream_##type((stream), (sid))->mid++)


#define sctp_mid_skip(stream, type, sid, mid) \
	(sctp_stream_##type((stream), (sid))->mid = mid + 1)


#define sctp_mid_uo_peek(stream, type, sid) \
	(sctp_stream_##type((stream), (sid))->mid_uo)


#define sctp_mid_uo_next(stream, type, sid) \
	(sctp_stream_##type((stream), (sid))->mid_uo++)


struct sctp_af {
	int		(*sctp_xmit)	(struct sk_buff *skb,
					 struct sctp_transport *);
	int		(*setsockopt)	(struct sock *sk,
					 int level,
					 int optname,
					 char __user *optval,
					 unsigned int optlen);
	int		(*getsockopt)	(struct sock *sk,
					 int level,
					 int optname,
					 char __user *optval,
					 int __user *optlen);
	int		(*compat_setsockopt)	(struct sock *sk,
					 int level,
					 int optname,
					 char __user *optval,
					 unsigned int optlen);
	int		(*compat_getsockopt)	(struct sock *sk,
					 int level,
					 int optname,
					 char __user *optval,
					 int __user *optlen);
	void		(*get_dst)	(struct sctp_transport *t,
					 union sctp_addr *saddr,
					 struct flowi *fl,
					 struct sock *sk);
	void		(*get_saddr)	(struct sctp_sock *sk,
					 struct sctp_transport *t,
					 struct flowi *fl);
	void		(*copy_addrlist) (struct list_head *,
					  struct net_device *);
	int		(*cmp_addr)	(const union sctp_addr *addr1,
					 const union sctp_addr *addr2);
	void		(*addr_copy)	(union sctp_addr *dst,
					 union sctp_addr *src);
	void		(*from_skb)	(union sctp_addr *,
					 struct sk_buff *skb,
					 int saddr);
	void		(*from_sk)	(union sctp_addr *,
					 struct sock *sk);
	void		(*from_addr_param) (union sctp_addr *,
					    union sctp_addr_param *,
					    __be16 port, int iif);
	int		(*to_addr_param) (const union sctp_addr *,
					  union sctp_addr_param *); 
	int		(*addr_valid)	(union sctp_addr *,
					 struct sctp_sock *,
					 const struct sk_buff *);
	enum sctp_scope	(*scope)(union sctp_addr *);
	void		(*inaddr_any)	(union sctp_addr *, __be16);
	int		(*is_any)	(const union sctp_addr *);
	int		(*available)	(union sctp_addr *,
					 struct sctp_sock *);
	int		(*skb_iif)	(const struct sk_buff *sk);
	int		(*is_ce)	(const struct sk_buff *sk);
	void		(*seq_dump_addr)(struct seq_file *seq,
					 union sctp_addr *addr);
	void		(*ecn_capable)(struct sock *sk);
	__u16		net_header_len;
	int		sockaddr_len;
	int		(*ip_options_len)(struct sock *sk);
	sa_family_t	sa_family;
	struct list_head list;
};

struct sctp_af *sctp_get_af_specific(sa_family_t);
int sctp_register_af(struct sctp_af *);


struct sctp_pf {
	void (*event_msgname)(struct sctp_ulpevent *, char *, int *);
	void (*skb_msgname)  (struct sk_buff *, char *, int *);
	int  (*af_supported) (sa_family_t, struct sctp_sock *);
	int  (*cmp_addr) (const union sctp_addr *,
			  const union sctp_addr *,
			  struct sctp_sock *);
	int  (*bind_verify) (struct sctp_sock *, union sctp_addr *);
	int  (*send_verify) (struct sctp_sock *, union sctp_addr *);
	int  (*supported_addrs)(const struct sctp_sock *, __be16 *);
	struct sock *(*create_accept_sk) (struct sock *sk,
					  struct sctp_association *asoc,
					  bool kern);
	int (*addr_to_user)(struct sctp_sock *sk, union sctp_addr *addr);
	void (*to_sk_saddr)(union sctp_addr *, struct sock *sk);
	void (*to_sk_daddr)(union sctp_addr *, struct sock *sk);
	void (*copy_ip_options)(struct sock *sk, struct sock *newsk);
	struct sctp_af *af;
};



struct sctp_datamsg {
	
	struct list_head chunks;
	
	refcount_t refcnt;
	
	unsigned long expires_at;
	
	int send_error;
	u8 send_failed:1,
	   can_delay:1,	
	   abandoned:1;	
};

struct sctp_datamsg *sctp_datamsg_from_user(struct sctp_association *,
					    struct sctp_sndrcvinfo *,
					    struct iov_iter *);
void sctp_datamsg_free(struct sctp_datamsg *);
void sctp_datamsg_put(struct sctp_datamsg *);
void sctp_chunk_fail(struct sctp_chunk *, int error);
int sctp_chunk_abandoned(struct sctp_chunk *);


struct sctp_chunk {
	struct list_head list;

	refcount_t refcnt;

	
	int sent_count;

	union {
		
		struct list_head transmitted_list;
		
		struct list_head stream_list;
	};

	
	struct list_head frag_list;

	
	struct sk_buff *skb;

	union {
		
		struct sk_buff *head_skb;
		
		struct sctp_shared_key *shkey;
	};

	

	
	union sctp_params param_hdr;
	union {
		__u8 *v;
		struct sctp_datahdr *data_hdr;
		struct sctp_inithdr *init_hdr;
		struct sctp_sackhdr *sack_hdr;
		struct sctp_heartbeathdr *hb_hdr;
		struct sctp_sender_hb_info *hbs_hdr;
		struct sctp_shutdownhdr *shutdown_hdr;
		struct sctp_signed_cookie *cookie_hdr;
		struct sctp_ecnehdr *ecne_hdr;
		struct sctp_cwrhdr *ecn_cwr_hdr;
		struct sctp_errhdr *err_hdr;
		struct sctp_addiphdr *addip_hdr;
		struct sctp_fwdtsn_hdr *fwdtsn_hdr;
		struct sctp_authhdr *auth_hdr;
		struct sctp_idatahdr *idata_hdr;
		struct sctp_ifwdtsn_hdr *ifwdtsn_hdr;
	} subh;

	__u8 *chunk_end;

	struct sctp_chunkhdr *chunk_hdr;
	struct sctphdr *sctp_hdr;

	
	struct sctp_sndrcvinfo sinfo;

	
	struct sctp_association *asoc;

	
	struct sctp_ep_common *rcvr;

	
	unsigned long sent_at;

	
	union sctp_addr source;
	
	union sctp_addr dest;

	
	struct sctp_datamsg *msg;

	
	struct sctp_transport *transport;

	
	struct sk_buff *auth_chunk;

#define SCTP_CAN_FRTX 0x0
#define SCTP_NEED_FRTX 0x1
#define SCTP_DONT_FRTX 0x2
	__u16	rtt_in_progress:1,	
		has_tsn:1,		
		has_ssn:1,		
#define has_mid has_ssn
		singleton:1,		
		end_of_packet:1,	
		ecn_ce_done:1,		
		pdiscard:1,		
		tsn_gap_acked:1,	
		data_accepted:1,	
		auth:1,			
		has_asconf:1,		
		tsn_missing_report:2,	
		fast_retransmit:2;	
};

#define sctp_chunk_retransmitted(chunk)	(chunk->sent_count > 1)
void sctp_chunk_hold(struct sctp_chunk *);
void sctp_chunk_put(struct sctp_chunk *);
int sctp_user_addto_chunk(struct sctp_chunk *chunk, int len,
			  struct iov_iter *from);
void sctp_chunk_free(struct sctp_chunk *);
void  *sctp_addto_chunk(struct sctp_chunk *, int len, const void *data);
struct sctp_chunk *sctp_chunkify(struct sk_buff *,
				 const struct sctp_association *,
				 struct sock *, gfp_t gfp);
void sctp_init_addrs(struct sctp_chunk *, union sctp_addr *,
		     union sctp_addr *);
const union sctp_addr *sctp_source(const struct sctp_chunk *chunk);

static inline __u16 sctp_chunk_stream_no(struct sctp_chunk *ch)
{
	return ntohs(ch->subh.data_hdr->stream);
}

enum {
	SCTP_ADDR_NEW,		
	SCTP_ADDR_SRC,		
	SCTP_ADDR_DEL,		
};


struct sctp_sockaddr_entry {
	struct list_head list;
	struct rcu_head	rcu;
	union sctp_addr a;
	__u8 state;
	__u8 valid;
};

#define SCTP_ADDRESS_TICK_DELAY	500


struct sctp_packet {
	
	__u16 source_port;
	__u16 destination_port;
	__u32 vtag;

	
	struct list_head chunk_list;

	
	size_t overhead;
	
	size_t size;
	
	size_t max_size;

	
	struct sctp_transport *transport;

	
	struct sctp_chunk *auth;

	u8  has_cookie_echo:1,	
	    has_sack:1,		
	    has_auth:1,		
	    has_data:1,		
	    ipfragok:1;		
};

void sctp_packet_init(struct sctp_packet *, struct sctp_transport *,
		      __u16 sport, __u16 dport);
void sctp_packet_config(struct sctp_packet *, __u32 vtag, int);
enum sctp_xmit sctp_packet_transmit_chunk(struct sctp_packet *packet,
					  struct sctp_chunk *chunk,
					  int one_packet, gfp_t gfp);
enum sctp_xmit sctp_packet_append_chunk(struct sctp_packet *packet,
					struct sctp_chunk *chunk);
int sctp_packet_transmit(struct sctp_packet *, gfp_t);
void sctp_packet_free(struct sctp_packet *);

static inline int sctp_packet_empty(struct sctp_packet *packet)
{
	return packet->size == packet->overhead;
}


struct sctp_transport {
	
	struct list_head transports;
	struct rhlist_head node;

	
	refcount_t refcnt;
		
	__u32	rto_pending:1,

		
		hb_sent:1,

		
		pmtu_pending:1,

		dst_pending_confirm:1,	

		
		sack_generation:1;
	u32 dst_cookie;

	struct flowi fl;

	
	union sctp_addr ipaddr;

	
	struct sctp_af *af_specific;

	
	struct sctp_association *asoc;

	
	
	unsigned long rto;

	__u32 rtt;		

	
	__u32 rttvar;

	
	__u32 srtt;

	
	
	__u32 cwnd;		  

	
	__u32 ssthresh;

	
	__u32 partial_bytes_acked;

	
	__u32 flight_size;

	__u32 burst_limited;	

	
	struct dst_entry *dst;
	
	union sctp_addr saddr;

	
	unsigned long hbinterval;

	
	unsigned long sackdelay;
	__u32 sackfreq;

	atomic_t mtu_info;

	
	ktime_t last_time_heard;

	
	unsigned long last_time_sent;

	
	unsigned long last_time_ecne_reduced;

	
	__u16 pathmaxrxt;

	__u32 flowlabel;
	__u8  dscp;

	
	int pf_retrans;
	
	__u32 pathmtu;

	
	__u32 param_flags;

	
	int init_sent_count;

	
	int state;

	

	
	unsigned short error_count;

	
	struct timer_list T3_rtx_timer;

	
	struct timer_list hb_timer;

	
	struct timer_list proto_unreach_timer;

	
	struct timer_list reconf_timer;

	
	struct list_head transmitted;

	
	struct sctp_packet packet;

	
	struct list_head send_ready;

	
	struct {
		
		__u32 next_tsn_at_change;

		
		char changeover_active;

		
		char cycling_changeover;

		
		char cacc_saw_newack;
	} cacc;

	
	__u64 hb_nonce;

	struct rcu_head rcu;
};

struct sctp_transport *sctp_transport_new(struct net *, const union sctp_addr *,
					  gfp_t);
void sctp_transport_set_owner(struct sctp_transport *,
			      struct sctp_association *);
void sctp_transport_route(struct sctp_transport *, union sctp_addr *,
			  struct sctp_sock *);
void sctp_transport_pmtu(struct sctp_transport *, struct sock *sk);
void sctp_transport_free(struct sctp_transport *);
void sctp_transport_reset_t3_rtx(struct sctp_transport *);
void sctp_transport_reset_hb_timer(struct sctp_transport *);
void sctp_transport_reset_reconf_timer(struct sctp_transport *transport);
int sctp_transport_hold(struct sctp_transport *);
void sctp_transport_put(struct sctp_transport *);
void sctp_transport_update_rto(struct sctp_transport *, __u32);
void sctp_transport_raise_cwnd(struct sctp_transport *, __u32, __u32);
void sctp_transport_lower_cwnd(struct sctp_transport *t,
			       enum sctp_lower_cwnd reason);
void sctp_transport_burst_limited(struct sctp_transport *);
void sctp_transport_burst_reset(struct sctp_transport *);
unsigned long sctp_transport_timeout(struct sctp_transport *);
void sctp_transport_reset(struct sctp_transport *t);
bool sctp_transport_update_pmtu(struct sctp_transport *t, u32 pmtu);
void sctp_transport_immediate_rtx(struct sctp_transport *);
void sctp_transport_dst_release(struct sctp_transport *t);
void sctp_transport_dst_confirm(struct sctp_transport *t);



struct sctp_inq {
	
	struct list_head in_chunk_list;
	
	struct sctp_chunk *in_progress;

	
	struct work_struct immediate;
};

void sctp_inq_init(struct sctp_inq *);
void sctp_inq_free(struct sctp_inq *);
void sctp_inq_push(struct sctp_inq *, struct sctp_chunk *packet);
struct sctp_chunk *sctp_inq_pop(struct sctp_inq *);
struct sctp_chunkhdr *sctp_inq_peek(struct sctp_inq *);
void sctp_inq_set_th_handler(struct sctp_inq *, work_func_t);


struct sctp_outq {
	struct sctp_association *asoc;

	
	struct list_head out_chunk_list;

	
	struct sctp_sched_ops *sched;

	unsigned int out_qlen;	

	
	unsigned int error;

	
	struct list_head control_chunk_list;

	
	struct list_head sacked;

	
	struct list_head retransmit;

	
	struct list_head abandoned;

	
	__u32 outstanding_bytes;

	
	char fast_rtx;

	
	char cork;
};

void sctp_outq_init(struct sctp_association *, struct sctp_outq *);
void sctp_outq_teardown(struct sctp_outq *);
void sctp_outq_free(struct sctp_outq*);
void sctp_outq_tail(struct sctp_outq *, struct sctp_chunk *chunk, gfp_t);
int sctp_outq_sack(struct sctp_outq *, struct sctp_chunk *);
int sctp_outq_is_empty(const struct sctp_outq *);
void sctp_outq_restart(struct sctp_outq *);

void sctp_retransmit(struct sctp_outq *q, struct sctp_transport *transport,
		     enum sctp_retransmit_reason reason);
void sctp_retransmit_mark(struct sctp_outq *, struct sctp_transport *, __u8);
void sctp_outq_uncork(struct sctp_outq *, gfp_t gfp);
void sctp_prsctp_prune(struct sctp_association *asoc,
		       struct sctp_sndrcvinfo *sinfo, int msg_len);
void sctp_generate_fwdtsn(struct sctp_outq *q, __u32 sack_ctsn);

static inline void sctp_outq_cork(struct sctp_outq *q)
{
	q->cork = 1;
}


struct sctp_input_cb {
	union {
		struct inet_skb_parm	h4;
#if IS_ENABLED(CONFIG_IPV6)
		struct inet6_skb_parm	h6;
#endif
	} header;
	struct sctp_chunk *chunk;
	struct sctp_af *af;
};
#define SCTP_INPUT_CB(__skb)	((struct sctp_input_cb *)&((__skb)->cb[0]))

struct sctp_output_cb {
	struct sk_buff *last;
};
#define SCTP_OUTPUT_CB(__skb)	((struct sctp_output_cb *)&((__skb)->cb[0]))

static inline const struct sk_buff *sctp_gso_headskb(const struct sk_buff *skb)
{
	const struct sctp_chunk *chunk = SCTP_INPUT_CB(skb)->chunk;

	return chunk->head_skb ? : skb;
}


struct sctp_bind_addr {

	
	__u16 port;

	
	struct list_head address_list;
};

void sctp_bind_addr_init(struct sctp_bind_addr *, __u16 port);
void sctp_bind_addr_free(struct sctp_bind_addr *);
int sctp_bind_addr_copy(struct net *net, struct sctp_bind_addr *dest,
			const struct sctp_bind_addr *src,
			enum sctp_scope scope, gfp_t gfp,
			int flags);
int sctp_bind_addr_dup(struct sctp_bind_addr *dest,
			const struct sctp_bind_addr *src,
			gfp_t gfp);
int sctp_add_bind_addr(struct sctp_bind_addr *, union sctp_addr *,
		       int new_size, __u8 addr_state, gfp_t gfp);
int sctp_del_bind_addr(struct sctp_bind_addr *, union sctp_addr *);
int sctp_bind_addr_match(struct sctp_bind_addr *, const union sctp_addr *,
			 struct sctp_sock *);
int sctp_bind_addr_conflict(struct sctp_bind_addr *, const union sctp_addr *,
			 struct sctp_sock *, struct sctp_sock *);
int sctp_bind_addr_state(const struct sctp_bind_addr *bp,
			 const union sctp_addr *addr);
int sctp_bind_addrs_check(struct sctp_sock *sp,
			  struct sctp_sock *sp2, int cnt2);
union sctp_addr *sctp_find_unmatch_addr(struct sctp_bind_addr	*bp,
					const union sctp_addr	*addrs,
					int			addrcnt,
					struct sctp_sock	*opt);
union sctp_params sctp_bind_addrs_to_raw(const struct sctp_bind_addr *bp,
					 int *addrs_len,
					 gfp_t gfp);
int sctp_raw_to_bind_addrs(struct sctp_bind_addr *bp, __u8 *raw, int len,
			   __u16 port, gfp_t gfp);

enum sctp_scope sctp_scope(const union sctp_addr *addr);
int sctp_in_scope(struct net *net, const union sctp_addr *addr,
		  const enum sctp_scope scope);
int sctp_is_any(struct sock *sk, const union sctp_addr *addr);
int sctp_is_ep_boundall(struct sock *sk);



enum sctp_endpoint_type {
	SCTP_EP_TYPE_SOCKET,
	SCTP_EP_TYPE_ASSOCIATION,
};



struct sctp_ep_common {
	
	struct hlist_node node;
	int hashent;

	
	enum sctp_endpoint_type type;

	
	refcount_t    refcnt;
	bool	    dead;

	
	struct sock *sk;

	
	struct sctp_inq	  inqueue;

	
	struct sctp_bind_addr bind_addr;
};




struct sctp_endpoint {
	
	struct sctp_ep_common base;

	
	
	struct list_head asocs;

	
	__u8 secret_key[SCTP_SECRET_SIZE];

 	
 	__u8 *digest;
 
	
	__u32 sndbuf_policy;

	
	__u32 rcvbuf_policy;

	
	struct crypto_shash **auth_hmacs;

	
	 struct sctp_hmac_algo_param *auth_hmacs_list;

	
	struct sctp_chunks_param *auth_chunk_list;

	
	struct list_head endpoint_shared_keys;
	__u16 active_key_id;
	__u8  auth_enable:1,
	      intl_enable:1,
	      prsctp_enable:1,
	      reconf_enable:1;

	__u8  strreset_enable;

	

	u32 secid;
	u32 peer_secid;
};


static inline struct sctp_endpoint *sctp_ep(struct sctp_ep_common *base)
{
	struct sctp_endpoint *ep;

	ep = container_of(base, struct sctp_endpoint, base);
	return ep;
}


struct sctp_endpoint *sctp_endpoint_new(struct sock *, gfp_t);
void sctp_endpoint_free(struct sctp_endpoint *);
void sctp_endpoint_put(struct sctp_endpoint *);
void sctp_endpoint_hold(struct sctp_endpoint *);
void sctp_endpoint_add_asoc(struct sctp_endpoint *, struct sctp_association *);
struct sctp_association *sctp_endpoint_lookup_assoc(
	const struct sctp_endpoint *ep,
	const union sctp_addr *paddr,
	struct sctp_transport **);
bool sctp_endpoint_is_peeled_off(struct sctp_endpoint *ep,
				 const union sctp_addr *paddr);
struct sctp_endpoint *sctp_endpoint_is_match(struct sctp_endpoint *,
					struct net *, const union sctp_addr *);
bool sctp_has_association(struct net *net, const union sctp_addr *laddr,
			  const union sctp_addr *paddr);

int sctp_verify_init(struct net *net, const struct sctp_endpoint *ep,
		     const struct sctp_association *asoc,
		     enum sctp_cid cid, struct sctp_init_chunk *peer_init,
		     struct sctp_chunk *chunk, struct sctp_chunk **err_chunk);
int sctp_process_init(struct sctp_association *, struct sctp_chunk *chunk,
		      const union sctp_addr *peer,
		      struct sctp_init_chunk *init, gfp_t gfp);
__u32 sctp_generate_tag(const struct sctp_endpoint *);
__u32 sctp_generate_tsn(const struct sctp_endpoint *);

struct sctp_inithdr_host {
	__u32 init_tag;
	__u32 a_rwnd;
	__u16 num_outbound_streams;
	__u16 num_inbound_streams;
	__u32 initial_tsn;
};

struct sctp_stream_priorities {
	
	struct list_head prio_sched;
	
	struct list_head active;
	
	struct sctp_stream_out_ext *next;
	__u16 prio;
};

struct sctp_stream_out_ext {
	__u64 abandoned_unsent[SCTP_PR_INDEX(MAX) + 1];
	__u64 abandoned_sent[SCTP_PR_INDEX(MAX) + 1];
	struct list_head outq; 
	union {
		struct {
			
			struct list_head prio_list;
			struct sctp_stream_priorities *prio_head;
		};
		
		struct {
			struct list_head rr_list;
		};
	};
};

struct sctp_stream_out {
	union {
		__u32 mid;
		__u16 ssn;
	};
	__u32 mid_uo;
	struct sctp_stream_out_ext *ext;
	__u8 state;
};

struct sctp_stream_in {
	union {
		__u32 mid;
		__u16 ssn;
	};
	__u32 mid_uo;
	__u32 fsn;
	__u32 fsn_uo;
	char pd_mode;
	char pd_mode_uo;
};

struct sctp_stream {
	GENRADIX(struct sctp_stream_out) out;
	GENRADIX(struct sctp_stream_in)	in;

	__u16 outcnt;
	__u16 incnt;
	
	struct sctp_stream_out *out_curr;
	union {
		
		struct {
			
			struct list_head prio_list;
		};
		
		struct {
			
			struct list_head rr_list;
			
			struct sctp_stream_out_ext *rr_next;
		};
	};
	struct sctp_stream_interleave *si;
};

static inline struct sctp_stream_out *sctp_stream_out(
	struct sctp_stream *stream,
	__u16 sid)
{
	return genradix_ptr(&stream->out, sid);
}

static inline struct sctp_stream_in *sctp_stream_in(
	struct sctp_stream *stream,
	__u16 sid)
{
	return genradix_ptr(&stream->in, sid);
}

#define SCTP_SO(s, i) sctp_stream_out((s), (i))
#define SCTP_SI(s, i) sctp_stream_in((s), (i))

#define SCTP_STREAM_CLOSED		0x00
#define SCTP_STREAM_OPEN		0x01

static inline __u16 sctp_datachk_len(const struct sctp_stream *stream)
{
	return stream->si->data_chunk_len;
}

static inline __u16 sctp_datahdr_len(const struct sctp_stream *stream)
{
	return stream->si->data_chunk_len - sizeof(struct sctp_chunkhdr);
}

static inline __u16 sctp_ftsnchk_len(const struct sctp_stream *stream)
{
	return stream->si->ftsn_chunk_len;
}

static inline __u16 sctp_ftsnhdr_len(const struct sctp_stream *stream)
{
	return stream->si->ftsn_chunk_len - sizeof(struct sctp_chunkhdr);
}


struct sctp_priv_assoc_stats {
	
	struct sockaddr_storage obs_rto_ipaddr;
	__u64 max_obs_rto;
	
	__u64 isacks;
	__u64 osacks;
	
	__u64 opackets;
	__u64 ipackets;
	
	__u64 rtxchunks;
	
	__u64 outofseqtsns;
	
	__u64 idupchunks;
	
	__u64 gapcnt;
	
	__u64 ouodchunks;
	__u64 iuodchunks;
	
	__u64 oodchunks;
	__u64 iodchunks;
	
	__u64 octrlchunks;
	__u64 ictrlchunks;
};





struct sctp_association {

	
	struct sctp_ep_common base;

	
	struct list_head asocs;

	
	sctp_assoc_t assoc_id;

	
	struct sctp_endpoint *ep;

	
	struct sctp_cookie c;

	
	struct {
		
		struct list_head transport_addr_list;

		
		__u32 rwnd;

		
		__u16 transport_count;

		
		__u16 port;

		
		struct sctp_transport *primary_path;

		
		union sctp_addr primary_addr;

		
		struct sctp_transport *active_path;

		
		struct sctp_transport *retran_path;

		
		struct sctp_transport *last_sent_to;

		
		struct sctp_transport *last_data_from;

		
		struct sctp_tsnmap tsn_map;

		
		__be16 addip_disabled_mask;

		
		__u16	ecn_capable:1,      
			ipv4_address:1,     
			ipv6_address:1,     
			hostname_address:1, 
			asconf_capable:1,   
			prsctp_capable:1,   
			reconf_capable:1,   
			intl_capable:1,     
			auth_capable:1,     
			
			sack_needed:1,     
			sack_generation:1,
			zero_window_announced:1;

		__u32	sack_cnt;

		__u32   adaptation_ind;	 

		struct sctp_inithdr_host i;
		void *cookie;
		int cookie_len;

		
		__u32 addip_serial;

		
		struct sctp_random_param *peer_random;
		struct sctp_chunks_param *peer_chunks;
		struct sctp_hmac_algo_param *peer_hmacs;
	} peer;

	
	enum sctp_state state;

	
	int overall_error_count;

	
	ktime_t cookie_life;

	
	unsigned long rto_initial;
	unsigned long rto_max;
	unsigned long rto_min;

	
	int max_burst;

	
	int max_retrans;

	
	int pf_retrans;

	
	__u16 max_init_attempts;

	
	__u16 init_retries;

	
	unsigned long max_init_timeo;

	
	unsigned long hbinterval;

	
	__u16 pathmaxrxt;

	__u32 flowlabel;
	__u8  dscp;

	
	__u8   pmtu_pending;

	
	__u32 pathmtu;

	
	__u32 param_flags;

	__u32 sackfreq;
	
	unsigned long sackdelay;

	unsigned long timeouts[SCTP_NUM_TIMEOUT_TYPES];
	struct timer_list timers[SCTP_NUM_TIMEOUT_TYPES];

	
	struct sctp_transport *shutdown_last_sent_to;

	
	struct sctp_transport *init_last_sent_to;

	
	int shutdown_retries;

	
	__u32 next_tsn;

	

	__u32 ctsn_ack_point;

	
	__u32 adv_peer_ack_point;

	
	__u32 highest_sacked;

	
	__u32 fast_recovery_exit;

	
	__u8 fast_recovery;

	
	__u16 unack_data;

	
	__u32 rtx_data_chunks;

	
	__u32 rwnd;

	
	__u32 a_rwnd;

	
	__u32 rwnd_over;

	
	__u32 rwnd_press;

	
	int sndbuf_used;

	
	atomic_t rmem_alloc;

	
	wait_queue_head_t	wait;

	
	__u32 frag_point;
	__u32 user_frag;

	
	int init_err_counter;

	
	int init_cycle;

	
	__u16 default_stream;
	__u16 default_flags;
	__u32 default_ppid;
	__u32 default_context;
	__u32 default_timetolive;

	
	__u32 default_rcv_context;

	
	struct sctp_stream stream;

	
	struct sctp_outq outqueue;

	
	struct sctp_ulpq ulpq;

	
	__u32 last_ecne_tsn;

	
	__u32 last_cwr_tsn;

	
	int numduptsns;

	



	
	struct sctp_chunk *addip_last_asconf;

	
	struct list_head asconf_ack_list;

	
	struct list_head addip_chunk_list;

	
	__u32 addip_serial;
	int src_out_of_asoc_ok;
	union sctp_addr *asconf_addr_del_pending;
	struct sctp_transport *new_transport;

	
	struct list_head endpoint_shared_keys;

	
	struct sctp_auth_bytes *asoc_shared_key;
	struct sctp_shared_key *shkey;

	
	__u16 default_hmac_id;

	__u16 active_key_id;

	__u8 need_ecne:1,	
	     temp:1,		
	     force_delay:1;

	__u8 strreset_enable;
	__u8 strreset_outstanding; 

	__u32 strreset_outseq; 
	__u32 strreset_inseq; 
	__u32 strreset_result[2]; 

	struct sctp_chunk *strreset_chunk; 

	struct sctp_priv_assoc_stats stats;

	int sent_cnt_removable;

	__u16 subscribe;

	__u64 abandoned_unsent[SCTP_PR_INDEX(MAX) + 1];
	__u64 abandoned_sent[SCTP_PR_INDEX(MAX) + 1];

	struct rcu_head rcu;
};



enum {
	SCTP_ASSOC_EYECATCHER = 0xa550c123,
};


static inline struct sctp_association *sctp_assoc(struct sctp_ep_common *base)
{
	struct sctp_association *asoc;

	asoc = container_of(base, struct sctp_association, base);
	return asoc;
}




struct sctp_association *
sctp_association_new(const struct sctp_endpoint *ep, const struct sock *sk,
		     enum sctp_scope scope, gfp_t gfp);
void sctp_association_free(struct sctp_association *);
void sctp_association_put(struct sctp_association *);
void sctp_association_hold(struct sctp_association *);

struct sctp_transport *sctp_assoc_choose_alter_transport(
	struct sctp_association *, struct sctp_transport *);
void sctp_assoc_update_retran_path(struct sctp_association *);
struct sctp_transport *sctp_assoc_lookup_paddr(const struct sctp_association *,
					  const union sctp_addr *);
int sctp_assoc_lookup_laddr(struct sctp_association *asoc,
			    const union sctp_addr *laddr);
struct sctp_transport *sctp_assoc_add_peer(struct sctp_association *,
				     const union sctp_addr *address,
				     const gfp_t gfp,
				     const int peer_state);
void sctp_assoc_del_peer(struct sctp_association *asoc,
			 const union sctp_addr *addr);
void sctp_assoc_rm_peer(struct sctp_association *asoc,
			 struct sctp_transport *peer);
void sctp_assoc_control_transport(struct sctp_association *asoc,
				  struct sctp_transport *transport,
				  enum sctp_transport_cmd command,
				  sctp_sn_error_t error);
struct sctp_transport *sctp_assoc_lookup_tsn(struct sctp_association *, __u32);
void sctp_assoc_migrate(struct sctp_association *, struct sock *);
int sctp_assoc_update(struct sctp_association *old,
		      struct sctp_association *new);

__u32 sctp_association_get_next_tsn(struct sctp_association *);

void sctp_assoc_update_frag_point(struct sctp_association *asoc);
void sctp_assoc_set_pmtu(struct sctp_association *asoc, __u32 pmtu);
void sctp_assoc_sync_pmtu(struct sctp_association *asoc);
void sctp_assoc_rwnd_increase(struct sctp_association *, unsigned int);
void sctp_assoc_rwnd_decrease(struct sctp_association *, unsigned int);
void sctp_assoc_set_primary(struct sctp_association *,
			    struct sctp_transport *);
void sctp_assoc_del_nonprimary_peers(struct sctp_association *,
				    struct sctp_transport *);
int sctp_assoc_set_bind_addr_from_ep(struct sctp_association *asoc,
				     enum sctp_scope scope, gfp_t gfp);
int sctp_assoc_set_bind_addr_from_cookie(struct sctp_association *,
					 struct sctp_cookie*,
					 gfp_t gfp);
int sctp_assoc_set_id(struct sctp_association *, gfp_t);
void sctp_assoc_clean_asconf_ack_cache(const struct sctp_association *asoc);
struct sctp_chunk *sctp_assoc_lookup_asconf_ack(
					const struct sctp_association *asoc,
					__be32 serial);
void sctp_asconf_queue_teardown(struct sctp_association *asoc);

int sctp_cmp_addr_exact(const union sctp_addr *ss1,
			const union sctp_addr *ss2);
struct sctp_chunk *sctp_get_ecne_prepend(struct sctp_association *asoc);


struct sctp_cmsgs {
	struct sctp_initmsg *init;
	struct sctp_sndrcvinfo *srinfo;
	struct sctp_sndinfo *sinfo;
	struct sctp_prinfo *prinfo;
	struct sctp_authinfo *authinfo;
	struct msghdr *addrs_msg;
};


struct sctp_dbg_objcnt_entry {
	char *label;
	atomic_t *counter;
};

#endif 
