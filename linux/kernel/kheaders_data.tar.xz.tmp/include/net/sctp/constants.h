/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef __sctp_constants_h__
#define __sctp_constants_h__

#include <linux/sctp.h>
#include <linux/ipv6.h> 
#include <net/tcp_states.h>  


enum { SCTP_MAX_STREAM = 0xffff };
enum { SCTP_DEFAULT_OUTSTREAMS = 10 };
enum { SCTP_DEFAULT_INSTREAMS = SCTP_MAX_STREAM };


#define SCTP_CID_BASE_MAX		SCTP_CID_SHUTDOWN_COMPLETE

#define SCTP_NUM_BASE_CHUNK_TYPES	(SCTP_CID_BASE_MAX + 1)

#define SCTP_NUM_ADDIP_CHUNK_TYPES	2

#define SCTP_NUM_PRSCTP_CHUNK_TYPES	1

#define SCTP_NUM_RECONF_CHUNK_TYPES	1

#define SCTP_NUM_AUTH_CHUNK_TYPES	1

#define SCTP_NUM_CHUNK_TYPES		(SCTP_NUM_BASE_CHUNK_TYPES + \
					 SCTP_NUM_ADDIP_CHUNK_TYPES +\
					 SCTP_NUM_PRSCTP_CHUNK_TYPES +\
					 SCTP_NUM_RECONF_CHUNK_TYPES +\
					 SCTP_NUM_AUTH_CHUNK_TYPES)


enum sctp_event_type {
	SCTP_EVENT_T_CHUNK = 1,
	SCTP_EVENT_T_TIMEOUT,
	SCTP_EVENT_T_OTHER,
	SCTP_EVENT_T_PRIMITIVE
};



enum sctp_event_timeout {
	SCTP_EVENT_TIMEOUT_NONE = 0,
	SCTP_EVENT_TIMEOUT_T1_COOKIE,
	SCTP_EVENT_TIMEOUT_T1_INIT,
	SCTP_EVENT_TIMEOUT_T2_SHUTDOWN,
	SCTP_EVENT_TIMEOUT_T3_RTX,
	SCTP_EVENT_TIMEOUT_T4_RTO,
	SCTP_EVENT_TIMEOUT_T5_SHUTDOWN_GUARD,
	SCTP_EVENT_TIMEOUT_HEARTBEAT,
	SCTP_EVENT_TIMEOUT_RECONF,
	SCTP_EVENT_TIMEOUT_SACK,
	SCTP_EVENT_TIMEOUT_AUTOCLOSE,
};

#define SCTP_EVENT_TIMEOUT_MAX		SCTP_EVENT_TIMEOUT_AUTOCLOSE
#define SCTP_NUM_TIMEOUT_TYPES		(SCTP_EVENT_TIMEOUT_MAX + 1)

enum sctp_event_other {
	SCTP_EVENT_NO_PENDING_TSN = 0,
	SCTP_EVENT_ICMP_PROTO_UNREACH,
};

#define SCTP_EVENT_OTHER_MAX		SCTP_EVENT_ICMP_PROTO_UNREACH
#define SCTP_NUM_OTHER_TYPES		(SCTP_EVENT_OTHER_MAX + 1)


enum sctp_event_primitive {
	SCTP_PRIMITIVE_ASSOCIATE = 0,
	SCTP_PRIMITIVE_SHUTDOWN,
	SCTP_PRIMITIVE_ABORT,
	SCTP_PRIMITIVE_SEND,
	SCTP_PRIMITIVE_REQUESTHEARTBEAT,
	SCTP_PRIMITIVE_ASCONF,
	SCTP_PRIMITIVE_RECONF,
};

#define SCTP_EVENT_PRIMITIVE_MAX	SCTP_PRIMITIVE_RECONF
#define SCTP_NUM_PRIMITIVE_TYPES	(SCTP_EVENT_PRIMITIVE_MAX + 1)



union sctp_subtype {
	enum sctp_cid chunk;
	enum sctp_event_timeout timeout;
	enum sctp_event_other other;
	enum sctp_event_primitive primitive;
};

#define SCTP_SUBTYPE_CONSTRUCTOR(_name, _type, _elt) \
static inline union sctp_subtype	\
SCTP_ST_## _name (_type _arg)		\
{ union sctp_subtype _retval; _retval._elt = _arg; return _retval; }

SCTP_SUBTYPE_CONSTRUCTOR(CHUNK,		enum sctp_cid,		chunk)
SCTP_SUBTYPE_CONSTRUCTOR(TIMEOUT,	enum sctp_event_timeout, timeout)
SCTP_SUBTYPE_CONSTRUCTOR(OTHER,		enum sctp_event_other,	other)
SCTP_SUBTYPE_CONSTRUCTOR(PRIMITIVE,	enum sctp_event_primitive, primitive)


#define sctp_chunk_is_data(a) (a->chunk_hdr->type == SCTP_CID_DATA || \
			       a->chunk_hdr->type == SCTP_CID_I_DATA)


enum sctp_ierror {
	SCTP_IERROR_NO_ERROR	        = 0,
	SCTP_IERROR_BASE		= 1000,
	SCTP_IERROR_NO_COOKIE,
	SCTP_IERROR_BAD_SIG,
	SCTP_IERROR_STALE_COOKIE,
	SCTP_IERROR_NOMEM,
	SCTP_IERROR_MALFORMED,
	SCTP_IERROR_BAD_TAG,
	SCTP_IERROR_BIG_GAP,
	SCTP_IERROR_DUP_TSN,
	SCTP_IERROR_HIGH_TSN,
	SCTP_IERROR_IGNORE_TSN,
	SCTP_IERROR_NO_DATA,
	SCTP_IERROR_BAD_STREAM,
	SCTP_IERROR_BAD_PORTS,
	SCTP_IERROR_AUTH_BAD_HMAC,
	SCTP_IERROR_AUTH_BAD_KEYID,
	SCTP_IERROR_PROTO_VIOLATION,
	SCTP_IERROR_ERROR,
	SCTP_IERROR_ABORT,
};




enum sctp_state {

	SCTP_STATE_CLOSED		= 0,
	SCTP_STATE_COOKIE_WAIT		= 1,
	SCTP_STATE_COOKIE_ECHOED	= 2,
	SCTP_STATE_ESTABLISHED		= 3,
	SCTP_STATE_SHUTDOWN_PENDING	= 4,
	SCTP_STATE_SHUTDOWN_SENT	= 5,
	SCTP_STATE_SHUTDOWN_RECEIVED	= 6,
	SCTP_STATE_SHUTDOWN_ACK_SENT	= 7,

};

#define SCTP_STATE_MAX			SCTP_STATE_SHUTDOWN_ACK_SENT
#define SCTP_STATE_NUM_STATES		(SCTP_STATE_MAX + 1)


enum sctp_sock_state {
	SCTP_SS_CLOSED         = TCP_CLOSE,
	SCTP_SS_LISTENING      = TCP_LISTEN,
	SCTP_SS_ESTABLISHING   = TCP_SYN_SENT,
	SCTP_SS_ESTABLISHED    = TCP_ESTABLISHED,
	SCTP_SS_CLOSING        = TCP_CLOSE_WAIT,
};


const char *sctp_cname(const union sctp_subtype id);	
const char *sctp_oname(const union sctp_subtype id);	
const char *sctp_tname(const union sctp_subtype id);	
const char *sctp_pname(const union sctp_subtype id);	


extern const char *const sctp_state_tbl[];
extern const char *const sctp_evttype_tbl[];
extern const char *const sctp_status_tbl[];


enum { SCTP_MAX_CHUNK_LEN = ((1<<16) - sizeof(__u32)) };


enum { SCTP_ARBITRARY_COOKIE_ECHO_LEN = 200 };


#define SCTP_TSN_MAP_INITIAL BITS_PER_LONG
#define SCTP_TSN_MAP_INCREMENT SCTP_TSN_MAP_INITIAL
#define SCTP_TSN_MAP_SIZE 4096


enum { SCTP_MAX_DUP_TSNS = 16 };
enum { SCTP_MAX_GABS = 16 };


#define SCTP_DEFAULT_TIMEOUT_HEARTBEAT	(30*1000)


#define SCTP_DEFAULT_TIMEOUT_SACK	(200)


#define SCTP_RTO_INITIAL	(3 * 1000)
#define SCTP_RTO_MIN		(1 * 1000)
#define SCTP_RTO_MAX		(60 * 1000)

#define SCTP_RTO_ALPHA          3   
#define SCTP_RTO_BETA           2   


#define SCTP_DEFAULT_MAX_BURST		4

#define SCTP_CLOCK_GRANULARITY	1	

#define SCTP_DEFAULT_COOKIE_LIFE	(60 * 1000) 

#define SCTP_DEFAULT_MINWINDOW	1500	
#define SCTP_DEFAULT_MAXWINDOW	65535	
#define SCTP_DEFAULT_RWND_SHIFT  4	
#define SCTP_DEFAULT_MAXSEGMENT 1500	
#define SCTP_DEFAULT_MINSEGMENT 512	

#define SCTP_SECRET_SIZE 32		

#define SCTP_SIGNATURE_SIZE 20	        

#define SCTP_COOKIE_MULTIPLE 32 


enum sctp_xmit {
	SCTP_XMIT_OK,
	SCTP_XMIT_PMTU_FULL,
	SCTP_XMIT_RWND_FULL,
	SCTP_XMIT_DELAY,
};


enum sctp_transport_cmd {
	SCTP_TRANSPORT_UP,
	SCTP_TRANSPORT_DOWN,
	SCTP_TRANSPORT_PF,
};


enum sctp_scope {
	SCTP_SCOPE_GLOBAL,		
	SCTP_SCOPE_PRIVATE,		
	SCTP_SCOPE_LINK,		
	SCTP_SCOPE_LOOPBACK,		
	SCTP_SCOPE_UNUSABLE,		
};

enum {
	SCTP_SCOPE_POLICY_DISABLE,	
	SCTP_SCOPE_POLICY_ENABLE,	
	SCTP_SCOPE_POLICY_PRIVATE,	
	SCTP_SCOPE_POLICY_LINK,		
};

#define SCTP_SCOPE_POLICY_MAX	SCTP_SCOPE_POLICY_LINK


#define IS_IPV4_UNUSABLE_ADDRESS(a)	    \
	((htonl(INADDR_BROADCAST) == a) ||  \
	 ipv4_is_multicast(a) ||	    \
	 ipv4_is_zeronet(a) ||		    \
	 ipv4_is_test_198(a) ||		    \
	 ipv4_is_anycast_6to4(a))


#define SCTP_ADDR6_ALLOWED	0x00000001	
#define SCTP_ADDR4_PEERSUPP	0x00000002	
#define SCTP_ADDR6_PEERSUPP	0x00000004	


enum sctp_retransmit_reason {
	SCTP_RTXR_T3_RTX,
	SCTP_RTXR_FAST_RTX,
	SCTP_RTXR_PMTUD,
	SCTP_RTXR_T1_RTX,
};


enum sctp_lower_cwnd {
	SCTP_LOWER_CWND_T3_RTX,
	SCTP_LOWER_CWND_FAST_RTX,
	SCTP_LOWER_CWND_ECNE,
	SCTP_LOWER_CWND_INACTIVE,
};





enum {
	SCTP_AUTH_HMAC_ID_RESERVED_0,
	SCTP_AUTH_HMAC_ID_SHA1,
	SCTP_AUTH_HMAC_ID_RESERVED_2,
#if defined (CONFIG_CRYPTO_SHA256) || defined (CONFIG_CRYPTO_SHA256_MODULE)
	SCTP_AUTH_HMAC_ID_SHA256,
#endif
	__SCTP_AUTH_HMAC_MAX
};

#define SCTP_AUTH_HMAC_ID_MAX	__SCTP_AUTH_HMAC_MAX - 1
#define SCTP_AUTH_NUM_HMACS 	__SCTP_AUTH_HMAC_MAX
#define SCTP_SHA1_SIG_SIZE 20
#define SCTP_SHA256_SIG_SIZE 32


#define SCTP_NUM_NOAUTH_CHUNKS	4
#define SCTP_AUTH_MAX_CHUNKS	(SCTP_NUM_CHUNK_TYPES - SCTP_NUM_NOAUTH_CHUNKS)


#define SCTP_AUTH_RANDOM_LENGTH 32

#endif 
