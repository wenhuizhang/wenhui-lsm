/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _SUNRPC_XDR_H_
#define _SUNRPC_XDR_H_

#ifdef __KERNEL__

#include <linux/uio.h>
#include <asm/byteorder.h>
#include <asm/unaligned.h>
#include <linux/scatterlist.h>

struct bio_vec;
struct rpc_rqst;


#define XDR_QUADLEN(l)		(((l) + 3) >> 2)


#define XDR_MAX_NETOBJ		1024
struct xdr_netobj {
	unsigned int		len;
	u8 *			data;
};


struct xdr_buf {
	struct kvec	head[1],	
			tail[1];	

	struct bio_vec	*bvec;
	struct page **	pages;		
	unsigned int	page_base,	
			page_len,	
			flags;		
#define XDRBUF_READ		0x01		
#define XDRBUF_WRITE		0x02		
#define XDRBUF_SPARSE_PAGES	0x04		

	unsigned int	buflen,		
			len;		
};

static inline void
xdr_buf_init(struct xdr_buf *buf, void *start, size_t len)
{
	buf->head[0].iov_base = start;
	buf->head[0].iov_len = len;
	buf->tail[0].iov_len = 0;
	buf->pages = NULL;
	buf->page_len = 0;
	buf->flags = 0;
	buf->len = 0;
	buf->buflen = len;
}



#define	xdr_zero	cpu_to_be32(0)
#define	xdr_one		cpu_to_be32(1)
#define	xdr_two		cpu_to_be32(2)

#define	rpc_auth_null	cpu_to_be32(RPC_AUTH_NULL)
#define	rpc_auth_unix	cpu_to_be32(RPC_AUTH_UNIX)
#define	rpc_auth_short	cpu_to_be32(RPC_AUTH_SHORT)
#define	rpc_auth_gss	cpu_to_be32(RPC_AUTH_GSS)

#define	rpc_call	cpu_to_be32(RPC_CALL)
#define	rpc_reply	cpu_to_be32(RPC_REPLY)

#define	rpc_msg_accepted	cpu_to_be32(RPC_MSG_ACCEPTED)

#define	rpc_success		cpu_to_be32(RPC_SUCCESS)
#define	rpc_prog_unavail	cpu_to_be32(RPC_PROG_UNAVAIL)
#define	rpc_prog_mismatch	cpu_to_be32(RPC_PROG_MISMATCH)
#define	rpc_proc_unavail	cpu_to_be32(RPC_PROC_UNAVAIL)
#define	rpc_garbage_args	cpu_to_be32(RPC_GARBAGE_ARGS)
#define	rpc_system_err		cpu_to_be32(RPC_SYSTEM_ERR)
#define	rpc_drop_reply		cpu_to_be32(RPC_DROP_REPLY)

#define	rpc_mismatch		cpu_to_be32(RPC_MISMATCH)
#define	rpc_auth_error		cpu_to_be32(RPC_AUTH_ERROR)

#define	rpc_auth_ok		cpu_to_be32(RPC_AUTH_OK)
#define	rpc_autherr_badcred	cpu_to_be32(RPC_AUTH_BADCRED)
#define	rpc_autherr_rejectedcred cpu_to_be32(RPC_AUTH_REJECTEDCRED)
#define	rpc_autherr_badverf	cpu_to_be32(RPC_AUTH_BADVERF)
#define	rpc_autherr_rejectedverf cpu_to_be32(RPC_AUTH_REJECTEDVERF)
#define	rpc_autherr_tooweak	cpu_to_be32(RPC_AUTH_TOOWEAK)
#define	rpcsec_gsserr_credproblem	cpu_to_be32(RPCSEC_GSS_CREDPROBLEM)
#define	rpcsec_gsserr_ctxproblem	cpu_to_be32(RPCSEC_GSS_CTXPROBLEM)


__be32 *xdr_encode_opaque_fixed(__be32 *p, const void *ptr, unsigned int len);
__be32 *xdr_encode_opaque(__be32 *p, const void *ptr, unsigned int len);
__be32 *xdr_encode_string(__be32 *p, const char *s);
__be32 *xdr_decode_string_inplace(__be32 *p, char **sp, unsigned int *lenp,
			unsigned int maxlen);
__be32 *xdr_encode_netobj(__be32 *p, const struct xdr_netobj *);
__be32 *xdr_decode_netobj(__be32 *p, struct xdr_netobj *);

void	xdr_inline_pages(struct xdr_buf *, unsigned int,
			 struct page **, unsigned int, unsigned int);
void	xdr_terminate_string(struct xdr_buf *, const u32);
size_t	xdr_buf_pagecount(struct xdr_buf *buf);
int	xdr_alloc_bvec(struct xdr_buf *buf, gfp_t gfp);
void	xdr_free_bvec(struct xdr_buf *buf);

static inline __be32 *xdr_encode_array(__be32 *p, const void *s, unsigned int len)
{
	return xdr_encode_opaque(p, s, len);
}


static inline __be32 *
xdr_encode_hyper(__be32 *p, __u64 val)
{
	put_unaligned_be64(val, p);
	return p + 2;
}

static inline __be32 *
xdr_decode_hyper(__be32 *p, __u64 *valp)
{
	*valp = get_unaligned_be64(p);
	return p + 2;
}

static inline __be32 *
xdr_decode_opaque_fixed(__be32 *p, void *ptr, unsigned int len)
{
	memcpy(ptr, p, len);
	return p + XDR_QUADLEN(len);
}

static inline void xdr_netobj_dup(struct xdr_netobj *dst,
				  struct xdr_netobj *src, gfp_t gfp_mask)
{
	dst->data = kmemdup(src->data, src->len, gfp_mask);
	dst->len = src->len;
}


static inline int
xdr_adjust_iovec(struct kvec *iov, __be32 *p)
{
	return iov->iov_len = ((u8 *) p - (u8 *) iov->iov_base);
}


extern void xdr_shift_buf(struct xdr_buf *, size_t);
extern void xdr_buf_from_iov(struct kvec *, struct xdr_buf *);
extern int xdr_buf_subsegment(struct xdr_buf *, struct xdr_buf *, unsigned int, unsigned int);
extern int xdr_buf_read_netobj(struct xdr_buf *, struct xdr_netobj *, unsigned int);
extern int read_bytes_from_xdr_buf(struct xdr_buf *, unsigned int, void *, unsigned int);
extern int write_bytes_to_xdr_buf(struct xdr_buf *, unsigned int, void *, unsigned int);


struct xdr_skb_reader {
	struct sk_buff	*skb;
	unsigned int	offset;
	size_t		count;
	__wsum		csum;
};

typedef size_t (*xdr_skb_read_actor)(struct xdr_skb_reader *desc, void *to, size_t len);

extern int csum_partial_copy_to_xdr(struct xdr_buf *, struct sk_buff *);

extern int xdr_encode_word(struct xdr_buf *, unsigned int, u32);
extern int xdr_decode_word(struct xdr_buf *, unsigned int, u32 *);

struct xdr_array2_desc;
typedef int (*xdr_xcode_elem_t)(struct xdr_array2_desc *desc, void *elem);
struct xdr_array2_desc {
	unsigned int elem_size;
	unsigned int array_len;
	unsigned int array_maxlen;
	xdr_xcode_elem_t xcode;
};

extern int xdr_decode_array2(struct xdr_buf *buf, unsigned int base,
			     struct xdr_array2_desc *desc);
extern int xdr_encode_array2(struct xdr_buf *buf, unsigned int base,
			     struct xdr_array2_desc *desc);
extern void _copy_from_pages(char *p, struct page **pages, size_t pgbase,
			     size_t len);


struct xdr_stream {
	__be32 *p;		
	struct xdr_buf *buf;	

	__be32 *end;		
	struct kvec *iov;	
	struct kvec scratch;	
	struct page **page_ptr;	
	unsigned int nwords;	

	struct rpc_rqst *rqst;	
};


typedef void	(*kxdreproc_t)(struct rpc_rqst *rqstp, struct xdr_stream *xdr,
		const void *obj);
typedef int	(*kxdrdproc_t)(struct rpc_rqst *rqstp, struct xdr_stream *xdr,
		void *obj);

extern void xdr_init_encode(struct xdr_stream *xdr, struct xdr_buf *buf,
			    __be32 *p, struct rpc_rqst *rqst);
extern __be32 *xdr_reserve_space(struct xdr_stream *xdr, size_t nbytes);
extern void xdr_commit_encode(struct xdr_stream *xdr);
extern void xdr_truncate_encode(struct xdr_stream *xdr, size_t len);
extern int xdr_restrict_buflen(struct xdr_stream *xdr, int newbuflen);
extern void xdr_write_pages(struct xdr_stream *xdr, struct page **pages,
		unsigned int base, unsigned int len);
extern unsigned int xdr_stream_pos(const struct xdr_stream *xdr);
extern void xdr_init_decode(struct xdr_stream *xdr, struct xdr_buf *buf,
			    __be32 *p, struct rpc_rqst *rqst);
extern void xdr_init_decode_pages(struct xdr_stream *xdr, struct xdr_buf *buf,
		struct page **pages, unsigned int len);
extern void xdr_set_scratch_buffer(struct xdr_stream *xdr, void *buf, size_t buflen);
extern __be32 *xdr_inline_decode(struct xdr_stream *xdr, size_t nbytes);
extern unsigned int xdr_read_pages(struct xdr_stream *xdr, unsigned int len);
extern void xdr_enter_page(struct xdr_stream *xdr, unsigned int len);
extern int xdr_process_buf(struct xdr_buf *buf, unsigned int offset, unsigned int len, int (*actor)(struct scatterlist *, void *), void *data);


static inline size_t
xdr_stream_remaining(const struct xdr_stream *xdr)
{
	return xdr->nwords << 2;
}

ssize_t xdr_stream_decode_opaque(struct xdr_stream *xdr, void *ptr,
		size_t size);
ssize_t xdr_stream_decode_opaque_dup(struct xdr_stream *xdr, void **ptr,
		size_t maxlen, gfp_t gfp_flags);
ssize_t xdr_stream_decode_string(struct xdr_stream *xdr, char *str,
		size_t size);
ssize_t xdr_stream_decode_string_dup(struct xdr_stream *xdr, char **str,
		size_t maxlen, gfp_t gfp_flags);

static inline size_t
xdr_align_size(size_t n)
{
	const size_t mask = sizeof(__u32) - 1;

	return (n + mask) & ~mask;
}


static inline ssize_t
xdr_stream_encode_u32(struct xdr_stream *xdr, __u32 n)
{
	const size_t len = sizeof(n);
	__be32 *p = xdr_reserve_space(xdr, len);

	if (unlikely(!p))
		return -EMSGSIZE;
	*p = cpu_to_be32(n);
	return len;
}


static inline ssize_t
xdr_stream_encode_u64(struct xdr_stream *xdr, __u64 n)
{
	const size_t len = sizeof(n);
	__be32 *p = xdr_reserve_space(xdr, len);

	if (unlikely(!p))
		return -EMSGSIZE;
	xdr_encode_hyper(p, n);
	return len;
}


static inline ssize_t
xdr_stream_encode_opaque_inline(struct xdr_stream *xdr, void **ptr, size_t len)
{
	size_t count = sizeof(__u32) + xdr_align_size(len);
	__be32 *p = xdr_reserve_space(xdr, count);

	if (unlikely(!p)) {
		*ptr = NULL;
		return -EMSGSIZE;
	}
	xdr_encode_opaque(p, NULL, len);
	*ptr = ++p;
	return count;
}


static inline ssize_t
xdr_stream_encode_opaque_fixed(struct xdr_stream *xdr, const void *ptr, size_t len)
{
	__be32 *p = xdr_reserve_space(xdr, len);

	if (unlikely(!p))
		return -EMSGSIZE;
	xdr_encode_opaque_fixed(p, ptr, len);
	return xdr_align_size(len);
}


static inline ssize_t
xdr_stream_encode_opaque(struct xdr_stream *xdr, const void *ptr, size_t len)
{
	size_t count = sizeof(__u32) + xdr_align_size(len);
	__be32 *p = xdr_reserve_space(xdr, count);

	if (unlikely(!p))
		return -EMSGSIZE;
	xdr_encode_opaque(p, ptr, len);
	return count;
}


static inline ssize_t
xdr_stream_encode_uint32_array(struct xdr_stream *xdr,
		const __u32 *array, size_t array_size)
{
	ssize_t ret = (array_size+1) * sizeof(__u32);
	__be32 *p = xdr_reserve_space(xdr, ret);

	if (unlikely(!p))
		return -EMSGSIZE;
	*p++ = cpu_to_be32(array_size);
	for (; array_size > 0; p++, array++, array_size--)
		*p = cpu_to_be32p(array);
	return ret;
}


static inline ssize_t
xdr_stream_decode_u32(struct xdr_stream *xdr, __u32 *ptr)
{
	const size_t count = sizeof(*ptr);
	__be32 *p = xdr_inline_decode(xdr, count);

	if (unlikely(!p))
		return -EBADMSG;
	*ptr = be32_to_cpup(p);
	return 0;
}


static inline ssize_t
xdr_stream_decode_opaque_fixed(struct xdr_stream *xdr, void *ptr, size_t len)
{
	__be32 *p = xdr_inline_decode(xdr, len);

	if (unlikely(!p))
		return -EBADMSG;
	xdr_decode_opaque_fixed(p, ptr, len);
	return len;
}


static inline ssize_t
xdr_stream_decode_opaque_inline(struct xdr_stream *xdr, void **ptr, size_t maxlen)
{
	__be32 *p;
	__u32 len;

	*ptr = NULL;
	if (unlikely(xdr_stream_decode_u32(xdr, &len) < 0))
		return -EBADMSG;
	if (len != 0) {
		p = xdr_inline_decode(xdr, len);
		if (unlikely(!p))
			return -EBADMSG;
		if (unlikely(len > maxlen))
			return -EMSGSIZE;
		*ptr = p;
	}
	return len;
}


static inline ssize_t
xdr_stream_decode_uint32_array(struct xdr_stream *xdr,
		__u32 *array, size_t array_size)
{
	__be32 *p;
	__u32 len;
	ssize_t retval;

	if (unlikely(xdr_stream_decode_u32(xdr, &len) < 0))
		return -EBADMSG;
	p = xdr_inline_decode(xdr, len * sizeof(*p));
	if (unlikely(!p))
		return -EBADMSG;
	if (array == NULL)
		return len;
	if (len <= array_size) {
		if (len < array_size)
			memset(array+len, 0, (array_size-len)*sizeof(*array));
		array_size = len;
		retval = len;
	} else
		retval = -EMSGSIZE;
	for (; array_size > 0; p++, array++, array_size--)
		*array = be32_to_cpup(p);
	return retval;
}
#endif 

#endif 
