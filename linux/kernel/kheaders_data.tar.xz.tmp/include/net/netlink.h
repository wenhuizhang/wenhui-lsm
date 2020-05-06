/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __NET_NETLINK_H
#define __NET_NETLINK_H

#include <linux/types.h>
#include <linux/netlink.h>
#include <linux/jiffies.h>
#include <linux/in6.h>



 
enum {
	NLA_UNSPEC,
	NLA_U8,
	NLA_U16,
	NLA_U32,
	NLA_U64,
	NLA_STRING,
	NLA_FLAG,
	NLA_MSECS,
	NLA_NESTED,
	NLA_NESTED_ARRAY,
	NLA_NUL_STRING,
	NLA_BINARY,
	NLA_S8,
	NLA_S16,
	NLA_S32,
	NLA_S64,
	NLA_BITFIELD32,
	NLA_REJECT,
	NLA_EXACT_LEN,
	NLA_EXACT_LEN_WARN,
	NLA_MIN_LEN,
	__NLA_TYPE_MAX,
};

#define NLA_TYPE_MAX (__NLA_TYPE_MAX - 1)

enum nla_policy_validation {
	NLA_VALIDATE_NONE,
	NLA_VALIDATE_RANGE,
	NLA_VALIDATE_MIN,
	NLA_VALIDATE_MAX,
	NLA_VALIDATE_FUNCTION,
};


struct nla_policy {
	u8		type;
	u8		validation_type;
	u16		len;
	union {
		const void *validation_data;
		struct {
			s16 min, max;
		};
		int (*validate)(const struct nlattr *attr,
				struct netlink_ext_ack *extack);
		
		u16 strict_start_type;
	};
};

#define NLA_POLICY_EXACT_LEN(_len)	{ .type = NLA_EXACT_LEN, .len = _len }
#define NLA_POLICY_EXACT_LEN_WARN(_len)	{ .type = NLA_EXACT_LEN_WARN, \
					  .len = _len }
#define NLA_POLICY_MIN_LEN(_len)	{ .type = NLA_MIN_LEN, .len = _len }

#define NLA_POLICY_ETH_ADDR		NLA_POLICY_EXACT_LEN(ETH_ALEN)
#define NLA_POLICY_ETH_ADDR_COMPAT	NLA_POLICY_EXACT_LEN_WARN(ETH_ALEN)

#define _NLA_POLICY_NESTED(maxattr, policy) \
	{ .type = NLA_NESTED, .validation_data = policy, .len = maxattr }
#define _NLA_POLICY_NESTED_ARRAY(maxattr, policy) \
	{ .type = NLA_NESTED_ARRAY, .validation_data = policy, .len = maxattr }
#define NLA_POLICY_NESTED(policy) \
	_NLA_POLICY_NESTED(ARRAY_SIZE(policy) - 1, policy)
#define NLA_POLICY_NESTED_ARRAY(policy) \
	_NLA_POLICY_NESTED_ARRAY(ARRAY_SIZE(policy) - 1, policy)

#define __NLA_ENSURE(condition) BUILD_BUG_ON_ZERO(!(condition))
#define NLA_ENSURE_INT_TYPE(tp)				\
	(__NLA_ENSURE(tp == NLA_S8 || tp == NLA_U8 ||	\
		      tp == NLA_S16 || tp == NLA_U16 ||	\
		      tp == NLA_S32 || tp == NLA_U32 ||	\
		      tp == NLA_S64 || tp == NLA_U64) + tp)
#define NLA_ENSURE_NO_VALIDATION_PTR(tp)		\
	(__NLA_ENSURE(tp != NLA_BITFIELD32 &&		\
		      tp != NLA_REJECT &&		\
		      tp != NLA_NESTED &&		\
		      tp != NLA_NESTED_ARRAY) + tp)

#define NLA_POLICY_RANGE(tp, _min, _max) {		\
	.type = NLA_ENSURE_INT_TYPE(tp),		\
	.validation_type = NLA_VALIDATE_RANGE,		\
	.min = _min,					\
	.max = _max					\
}

#define NLA_POLICY_MIN(tp, _min) {			\
	.type = NLA_ENSURE_INT_TYPE(tp),		\
	.validation_type = NLA_VALIDATE_MIN,		\
	.min = _min,					\
}

#define NLA_POLICY_MAX(tp, _max) {			\
	.type = NLA_ENSURE_INT_TYPE(tp),		\
	.validation_type = NLA_VALIDATE_MAX,		\
	.max = _max,					\
}

#define NLA_POLICY_VALIDATE_FN(tp, fn, ...) {		\
	.type = NLA_ENSURE_NO_VALIDATION_PTR(tp),	\
	.validation_type = NLA_VALIDATE_FUNCTION,	\
	.validate = fn,					\
	.len = __VA_ARGS__ + 0,				\
}


struct nl_info {
	struct nlmsghdr		*nlh;
	struct net		*nl_net;
	u32			portid;
	u8			skip_notify:1,
				skip_notify_kernel:1;
};


enum netlink_validation {
	NL_VALIDATE_LIBERAL = 0,
	NL_VALIDATE_TRAILING = BIT(0),
	NL_VALIDATE_MAXTYPE = BIT(1),
	NL_VALIDATE_UNSPEC = BIT(2),
	NL_VALIDATE_STRICT_ATTRS = BIT(3),
	NL_VALIDATE_NESTED = BIT(4),
};

#define NL_VALIDATE_DEPRECATED_STRICT (NL_VALIDATE_TRAILING |\
				       NL_VALIDATE_MAXTYPE)
#define NL_VALIDATE_STRICT (NL_VALIDATE_TRAILING |\
			    NL_VALIDATE_MAXTYPE |\
			    NL_VALIDATE_UNSPEC |\
			    NL_VALIDATE_STRICT_ATTRS |\
			    NL_VALIDATE_NESTED)

int netlink_rcv_skb(struct sk_buff *skb,
		    int (*cb)(struct sk_buff *, struct nlmsghdr *,
			      struct netlink_ext_ack *));
int nlmsg_notify(struct sock *sk, struct sk_buff *skb, u32 portid,
		 unsigned int group, int report, gfp_t flags);

int __nla_validate(const struct nlattr *head, int len, int maxtype,
		   const struct nla_policy *policy, unsigned int validate,
		   struct netlink_ext_ack *extack);
int __nla_parse(struct nlattr **tb, int maxtype, const struct nlattr *head,
		int len, const struct nla_policy *policy, unsigned int validate,
		struct netlink_ext_ack *extack);
int nla_policy_len(const struct nla_policy *, int);
struct nlattr *nla_find(const struct nlattr *head, int len, int attrtype);
size_t nla_strlcpy(char *dst, const struct nlattr *nla, size_t dstsize);
char *nla_strdup(const struct nlattr *nla, gfp_t flags);
int nla_memcpy(void *dest, const struct nlattr *src, int count);
int nla_memcmp(const struct nlattr *nla, const void *data, size_t size);
int nla_strcmp(const struct nlattr *nla, const char *str);
struct nlattr *__nla_reserve(struct sk_buff *skb, int attrtype, int attrlen);
struct nlattr *__nla_reserve_64bit(struct sk_buff *skb, int attrtype,
				   int attrlen, int padattr);
void *__nla_reserve_nohdr(struct sk_buff *skb, int attrlen);
struct nlattr *nla_reserve(struct sk_buff *skb, int attrtype, int attrlen);
struct nlattr *nla_reserve_64bit(struct sk_buff *skb, int attrtype,
				 int attrlen, int padattr);
void *nla_reserve_nohdr(struct sk_buff *skb, int attrlen);
void __nla_put(struct sk_buff *skb, int attrtype, int attrlen,
	       const void *data);
void __nla_put_64bit(struct sk_buff *skb, int attrtype, int attrlen,
		     const void *data, int padattr);
void __nla_put_nohdr(struct sk_buff *skb, int attrlen, const void *data);
int nla_put(struct sk_buff *skb, int attrtype, int attrlen, const void *data);
int nla_put_64bit(struct sk_buff *skb, int attrtype, int attrlen,
		  const void *data, int padattr);
int nla_put_nohdr(struct sk_buff *skb, int attrlen, const void *data);
int nla_append(struct sk_buff *skb, int attrlen, const void *data);




static inline int nlmsg_msg_size(int payload)
{
	return NLMSG_HDRLEN + payload;
}


static inline int nlmsg_total_size(int payload)
{
	return NLMSG_ALIGN(nlmsg_msg_size(payload));
}


static inline int nlmsg_padlen(int payload)
{
	return nlmsg_total_size(payload) - nlmsg_msg_size(payload);
}


static inline void *nlmsg_data(const struct nlmsghdr *nlh)
{
	return (unsigned char *) nlh + NLMSG_HDRLEN;
}


static inline int nlmsg_len(const struct nlmsghdr *nlh)
{
	return nlh->nlmsg_len - NLMSG_HDRLEN;
}


static inline struct nlattr *nlmsg_attrdata(const struct nlmsghdr *nlh,
					    int hdrlen)
{
	unsigned char *data = nlmsg_data(nlh);
	return (struct nlattr *) (data + NLMSG_ALIGN(hdrlen));
}


static inline int nlmsg_attrlen(const struct nlmsghdr *nlh, int hdrlen)
{
	return nlmsg_len(nlh) - NLMSG_ALIGN(hdrlen);
}


static inline int nlmsg_ok(const struct nlmsghdr *nlh, int remaining)
{
	return (remaining >= (int) sizeof(struct nlmsghdr) &&
		nlh->nlmsg_len >= sizeof(struct nlmsghdr) &&
		nlh->nlmsg_len <= remaining);
}


static inline struct nlmsghdr *
nlmsg_next(const struct nlmsghdr *nlh, int *remaining)
{
	int totlen = NLMSG_ALIGN(nlh->nlmsg_len);

	*remaining -= totlen;

	return (struct nlmsghdr *) ((unsigned char *) nlh + totlen);
}


static inline int nla_parse(struct nlattr **tb, int maxtype,
			    const struct nlattr *head, int len,
			    const struct nla_policy *policy,
			    struct netlink_ext_ack *extack)
{
	return __nla_parse(tb, maxtype, head, len, policy,
			   NL_VALIDATE_STRICT, extack);
}


static inline int nla_parse_deprecated(struct nlattr **tb, int maxtype,
				       const struct nlattr *head, int len,
				       const struct nla_policy *policy,
				       struct netlink_ext_ack *extack)
{
	return __nla_parse(tb, maxtype, head, len, policy,
			   NL_VALIDATE_LIBERAL, extack);
}


static inline int nla_parse_deprecated_strict(struct nlattr **tb, int maxtype,
					      const struct nlattr *head,
					      int len,
					      const struct nla_policy *policy,
					      struct netlink_ext_ack *extack)
{
	return __nla_parse(tb, maxtype, head, len, policy,
			   NL_VALIDATE_DEPRECATED_STRICT, extack);
}


static inline int __nlmsg_parse(const struct nlmsghdr *nlh, int hdrlen,
				struct nlattr *tb[], int maxtype,
				const struct nla_policy *policy,
				unsigned int validate,
				struct netlink_ext_ack *extack)
{
	if (nlh->nlmsg_len < nlmsg_msg_size(hdrlen)) {
		NL_SET_ERR_MSG(extack, "Invalid header length");
		return -EINVAL;
	}

	return __nla_parse(tb, maxtype, nlmsg_attrdata(nlh, hdrlen),
			   nlmsg_attrlen(nlh, hdrlen), policy, validate,
			   extack);
}


static inline int nlmsg_parse(const struct nlmsghdr *nlh, int hdrlen,
			      struct nlattr *tb[], int maxtype,
			      const struct nla_policy *policy,
			      struct netlink_ext_ack *extack)
{
	return __nlmsg_parse(nlh, hdrlen, tb, maxtype, policy,
			     NL_VALIDATE_STRICT, extack);
}


static inline int nlmsg_parse_deprecated(const struct nlmsghdr *nlh, int hdrlen,
					 struct nlattr *tb[], int maxtype,
					 const struct nla_policy *policy,
					 struct netlink_ext_ack *extack)
{
	return __nlmsg_parse(nlh, hdrlen, tb, maxtype, policy,
			     NL_VALIDATE_LIBERAL, extack);
}


static inline int
nlmsg_parse_deprecated_strict(const struct nlmsghdr *nlh, int hdrlen,
			      struct nlattr *tb[], int maxtype,
			      const struct nla_policy *policy,
			      struct netlink_ext_ack *extack)
{
	return __nlmsg_parse(nlh, hdrlen, tb, maxtype, policy,
			     NL_VALIDATE_DEPRECATED_STRICT, extack);
}


static inline struct nlattr *nlmsg_find_attr(const struct nlmsghdr *nlh,
					     int hdrlen, int attrtype)
{
	return nla_find(nlmsg_attrdata(nlh, hdrlen),
			nlmsg_attrlen(nlh, hdrlen), attrtype);
}


static inline int nla_validate_deprecated(const struct nlattr *head, int len,
					  int maxtype,
					  const struct nla_policy *policy,
					  struct netlink_ext_ack *extack)
{
	return __nla_validate(head, len, maxtype, policy, NL_VALIDATE_LIBERAL,
			      extack);
}


static inline int nla_validate(const struct nlattr *head, int len, int maxtype,
			       const struct nla_policy *policy,
			       struct netlink_ext_ack *extack)
{
	return __nla_validate(head, len, maxtype, policy, NL_VALIDATE_STRICT,
			      extack);
}


static inline int nlmsg_validate_deprecated(const struct nlmsghdr *nlh,
					    int hdrlen, int maxtype,
					    const struct nla_policy *policy,
					    struct netlink_ext_ack *extack)
{
	if (nlh->nlmsg_len < nlmsg_msg_size(hdrlen))
		return -EINVAL;

	return __nla_validate(nlmsg_attrdata(nlh, hdrlen),
			      nlmsg_attrlen(nlh, hdrlen), maxtype,
			      policy, NL_VALIDATE_LIBERAL, extack);
}




static inline int nlmsg_report(const struct nlmsghdr *nlh)
{
	return !!(nlh->nlmsg_flags & NLM_F_ECHO);
}


#define nlmsg_for_each_attr(pos, nlh, hdrlen, rem) \
	nla_for_each_attr(pos, nlmsg_attrdata(nlh, hdrlen), \
			  nlmsg_attrlen(nlh, hdrlen), rem)


static inline struct nlmsghdr *nlmsg_put(struct sk_buff *skb, u32 portid, u32 seq,
					 int type, int payload, int flags)
{
	if (unlikely(skb_tailroom(skb) < nlmsg_total_size(payload)))
		return NULL;

	return __nlmsg_put(skb, portid, seq, type, payload, flags);
}


static inline struct nlmsghdr *nlmsg_put_answer(struct sk_buff *skb,
						struct netlink_callback *cb,
						int type, int payload,
						int flags)
{
	return nlmsg_put(skb, NETLINK_CB(cb->skb).portid, cb->nlh->nlmsg_seq,
			 type, payload, flags);
}


static inline struct sk_buff *nlmsg_new(size_t payload, gfp_t flags)
{
	return alloc_skb(nlmsg_total_size(payload), flags);
}


static inline void nlmsg_end(struct sk_buff *skb, struct nlmsghdr *nlh)
{
	nlh->nlmsg_len = skb_tail_pointer(skb) - (unsigned char *)nlh;
}


static inline void *nlmsg_get_pos(struct sk_buff *skb)
{
	return skb_tail_pointer(skb);
}


static inline void nlmsg_trim(struct sk_buff *skb, const void *mark)
{
	if (mark) {
		WARN_ON((unsigned char *) mark < skb->data);
		skb_trim(skb, (unsigned char *) mark - skb->data);
	}
}


static inline void nlmsg_cancel(struct sk_buff *skb, struct nlmsghdr *nlh)
{
	nlmsg_trim(skb, nlh);
}


static inline void nlmsg_free(struct sk_buff *skb)
{
	kfree_skb(skb);
}


static inline int nlmsg_multicast(struct sock *sk, struct sk_buff *skb,
				  u32 portid, unsigned int group, gfp_t flags)
{
	int err;

	NETLINK_CB(skb).dst_group = group;

	err = netlink_broadcast(sk, skb, portid, group, flags);
	if (err > 0)
		err = 0;

	return err;
}


static inline int nlmsg_unicast(struct sock *sk, struct sk_buff *skb, u32 portid)
{
	int err;

	err = netlink_unicast(sk, skb, portid, MSG_DONTWAIT);
	if (err > 0)
		err = 0;

	return err;
}


#define nlmsg_for_each_msg(pos, head, len, rem) \
	for (pos = head, rem = len; \
	     nlmsg_ok(pos, rem); \
	     pos = nlmsg_next(pos, &(rem)))


static inline void
nl_dump_check_consistent(struct netlink_callback *cb,
			 struct nlmsghdr *nlh)
{
	if (cb->prev_seq && cb->seq != cb->prev_seq)
		nlh->nlmsg_flags |= NLM_F_DUMP_INTR;
	cb->prev_seq = cb->seq;
}




static inline int nla_attr_size(int payload)
{
	return NLA_HDRLEN + payload;
}


static inline int nla_total_size(int payload)
{
	return NLA_ALIGN(nla_attr_size(payload));
}


static inline int nla_padlen(int payload)
{
	return nla_total_size(payload) - nla_attr_size(payload);
}


static inline int nla_type(const struct nlattr *nla)
{
	return nla->nla_type & NLA_TYPE_MASK;
}


static inline void *nla_data(const struct nlattr *nla)
{
	return (char *) nla + NLA_HDRLEN;
}


static inline int nla_len(const struct nlattr *nla)
{
	return nla->nla_len - NLA_HDRLEN;
}


static inline int nla_ok(const struct nlattr *nla, int remaining)
{
	return remaining >= (int) sizeof(*nla) &&
	       nla->nla_len >= sizeof(*nla) &&
	       nla->nla_len <= remaining;
}


static inline struct nlattr *nla_next(const struct nlattr *nla, int *remaining)
{
	unsigned int totlen = NLA_ALIGN(nla->nla_len);

	*remaining -= totlen;
	return (struct nlattr *) ((char *) nla + totlen);
}


static inline struct nlattr *
nla_find_nested(const struct nlattr *nla, int attrtype)
{
	return nla_find(nla_data(nla), nla_len(nla), attrtype);
}


static inline int nla_parse_nested(struct nlattr *tb[], int maxtype,
				   const struct nlattr *nla,
				   const struct nla_policy *policy,
				   struct netlink_ext_ack *extack)
{
	if (!(nla->nla_type & NLA_F_NESTED)) {
		NL_SET_ERR_MSG_ATTR(extack, nla, "NLA_F_NESTED is missing");
		return -EINVAL;
	}

	return __nla_parse(tb, maxtype, nla_data(nla), nla_len(nla), policy,
			   NL_VALIDATE_STRICT, extack);
}


static inline int nla_parse_nested_deprecated(struct nlattr *tb[], int maxtype,
					      const struct nlattr *nla,
					      const struct nla_policy *policy,
					      struct netlink_ext_ack *extack)
{
	return __nla_parse(tb, maxtype, nla_data(nla), nla_len(nla), policy,
			   NL_VALIDATE_LIBERAL, extack);
}


static inline int nla_put_u8(struct sk_buff *skb, int attrtype, u8 value)
{
	
	u8 tmp = value;

	return nla_put(skb, attrtype, sizeof(u8), &tmp);
}


static inline int nla_put_u16(struct sk_buff *skb, int attrtype, u16 value)
{
	u16 tmp = value;

	return nla_put(skb, attrtype, sizeof(u16), &tmp);
}


static inline int nla_put_be16(struct sk_buff *skb, int attrtype, __be16 value)
{
	__be16 tmp = value;

	return nla_put(skb, attrtype, sizeof(__be16), &tmp);
}


static inline int nla_put_net16(struct sk_buff *skb, int attrtype, __be16 value)
{
	__be16 tmp = value;

	return nla_put_be16(skb, attrtype | NLA_F_NET_BYTEORDER, tmp);
}


static inline int nla_put_le16(struct sk_buff *skb, int attrtype, __le16 value)
{
	__le16 tmp = value;

	return nla_put(skb, attrtype, sizeof(__le16), &tmp);
}


static inline int nla_put_u32(struct sk_buff *skb, int attrtype, u32 value)
{
	u32 tmp = value;

	return nla_put(skb, attrtype, sizeof(u32), &tmp);
}


static inline int nla_put_be32(struct sk_buff *skb, int attrtype, __be32 value)
{
	__be32 tmp = value;

	return nla_put(skb, attrtype, sizeof(__be32), &tmp);
}


static inline int nla_put_net32(struct sk_buff *skb, int attrtype, __be32 value)
{
	__be32 tmp = value;

	return nla_put_be32(skb, attrtype | NLA_F_NET_BYTEORDER, tmp);
}


static inline int nla_put_le32(struct sk_buff *skb, int attrtype, __le32 value)
{
	__le32 tmp = value;

	return nla_put(skb, attrtype, sizeof(__le32), &tmp);
}


static inline int nla_put_u64_64bit(struct sk_buff *skb, int attrtype,
				    u64 value, int padattr)
{
	u64 tmp = value;

	return nla_put_64bit(skb, attrtype, sizeof(u64), &tmp, padattr);
}


static inline int nla_put_be64(struct sk_buff *skb, int attrtype, __be64 value,
			       int padattr)
{
	__be64 tmp = value;

	return nla_put_64bit(skb, attrtype, sizeof(__be64), &tmp, padattr);
}


static inline int nla_put_net64(struct sk_buff *skb, int attrtype, __be64 value,
				int padattr)
{
	__be64 tmp = value;

	return nla_put_be64(skb, attrtype | NLA_F_NET_BYTEORDER, tmp,
			    padattr);
}


static inline int nla_put_le64(struct sk_buff *skb, int attrtype, __le64 value,
			       int padattr)
{
	__le64 tmp = value;

	return nla_put_64bit(skb, attrtype, sizeof(__le64), &tmp, padattr);
}


static inline int nla_put_s8(struct sk_buff *skb, int attrtype, s8 value)
{
	s8 tmp = value;

	return nla_put(skb, attrtype, sizeof(s8), &tmp);
}


static inline int nla_put_s16(struct sk_buff *skb, int attrtype, s16 value)
{
	s16 tmp = value;

	return nla_put(skb, attrtype, sizeof(s16), &tmp);
}


static inline int nla_put_s32(struct sk_buff *skb, int attrtype, s32 value)
{
	s32 tmp = value;

	return nla_put(skb, attrtype, sizeof(s32), &tmp);
}


static inline int nla_put_s64(struct sk_buff *skb, int attrtype, s64 value,
			      int padattr)
{
	s64 tmp = value;

	return nla_put_64bit(skb, attrtype, sizeof(s64), &tmp, padattr);
}


static inline int nla_put_string(struct sk_buff *skb, int attrtype,
				 const char *str)
{
	return nla_put(skb, attrtype, strlen(str) + 1, str);
}


static inline int nla_put_flag(struct sk_buff *skb, int attrtype)
{
	return nla_put(skb, attrtype, 0, NULL);
}


static inline int nla_put_msecs(struct sk_buff *skb, int attrtype,
				unsigned long njiffies, int padattr)
{
	u64 tmp = jiffies_to_msecs(njiffies);

	return nla_put_64bit(skb, attrtype, sizeof(u64), &tmp, padattr);
}


static inline int nla_put_in_addr(struct sk_buff *skb, int attrtype,
				  __be32 addr)
{
	__be32 tmp = addr;

	return nla_put_be32(skb, attrtype, tmp);
}


static inline int nla_put_in6_addr(struct sk_buff *skb, int attrtype,
				   const struct in6_addr *addr)
{
	return nla_put(skb, attrtype, sizeof(*addr), addr);
}


static inline u32 nla_get_u32(const struct nlattr *nla)
{
	return *(u32 *) nla_data(nla);
}


static inline __be32 nla_get_be32(const struct nlattr *nla)
{
	return *(__be32 *) nla_data(nla);
}


static inline __le32 nla_get_le32(const struct nlattr *nla)
{
	return *(__le32 *) nla_data(nla);
}


static inline u16 nla_get_u16(const struct nlattr *nla)
{
	return *(u16 *) nla_data(nla);
}


static inline __be16 nla_get_be16(const struct nlattr *nla)
{
	return *(__be16 *) nla_data(nla);
}


static inline __le16 nla_get_le16(const struct nlattr *nla)
{
	return *(__le16 *) nla_data(nla);
}


static inline u8 nla_get_u8(const struct nlattr *nla)
{
	return *(u8 *) nla_data(nla);
}


static inline u64 nla_get_u64(const struct nlattr *nla)
{
	u64 tmp;

	nla_memcpy(&tmp, nla, sizeof(tmp));

	return tmp;
}


static inline __be64 nla_get_be64(const struct nlattr *nla)
{
	__be64 tmp;

	nla_memcpy(&tmp, nla, sizeof(tmp));

	return tmp;
}


static inline __le64 nla_get_le64(const struct nlattr *nla)
{
	return *(__le64 *) nla_data(nla);
}


static inline s32 nla_get_s32(const struct nlattr *nla)
{
	return *(s32 *) nla_data(nla);
}


static inline s16 nla_get_s16(const struct nlattr *nla)
{
	return *(s16 *) nla_data(nla);
}


static inline s8 nla_get_s8(const struct nlattr *nla)
{
	return *(s8 *) nla_data(nla);
}


static inline s64 nla_get_s64(const struct nlattr *nla)
{
	s64 tmp;

	nla_memcpy(&tmp, nla, sizeof(tmp));

	return tmp;
}


static inline int nla_get_flag(const struct nlattr *nla)
{
	return !!nla;
}


static inline unsigned long nla_get_msecs(const struct nlattr *nla)
{
	u64 msecs = nla_get_u64(nla);

	return msecs_to_jiffies((unsigned long) msecs);
}


static inline __be32 nla_get_in_addr(const struct nlattr *nla)
{
	return *(__be32 *) nla_data(nla);
}


static inline struct in6_addr nla_get_in6_addr(const struct nlattr *nla)
{
	struct in6_addr tmp;

	nla_memcpy(&tmp, nla, sizeof(tmp));
	return tmp;
}


static inline struct nla_bitfield32 nla_get_bitfield32(const struct nlattr *nla)
{
	struct nla_bitfield32 tmp;

	nla_memcpy(&tmp, nla, sizeof(tmp));
	return tmp;
}


static inline void *nla_memdup(const struct nlattr *src, gfp_t gfp)
{
	return kmemdup(nla_data(src), nla_len(src), gfp);
}


static inline struct nlattr *nla_nest_start_noflag(struct sk_buff *skb,
						   int attrtype)
{
	struct nlattr *start = (struct nlattr *)skb_tail_pointer(skb);

	if (nla_put(skb, attrtype, 0, NULL) < 0)
		return NULL;

	return start;
}


static inline struct nlattr *nla_nest_start(struct sk_buff *skb, int attrtype)
{
	return nla_nest_start_noflag(skb, attrtype | NLA_F_NESTED);
}


static inline int nla_nest_end(struct sk_buff *skb, struct nlattr *start)
{
	start->nla_len = skb_tail_pointer(skb) - (unsigned char *)start;
	return skb->len;
}


static inline void nla_nest_cancel(struct sk_buff *skb, struct nlattr *start)
{
	nlmsg_trim(skb, start);
}


static inline int __nla_validate_nested(const struct nlattr *start, int maxtype,
					const struct nla_policy *policy,
					unsigned int validate,
					struct netlink_ext_ack *extack)
{
	return __nla_validate(nla_data(start), nla_len(start), maxtype, policy,
			      validate, extack);
}

static inline int
nl80211_validate_nested(const struct nlattr *start, int maxtype,
			const struct nla_policy *policy,
			struct netlink_ext_ack *extack)
{
	return __nla_validate_nested(start, maxtype, policy,
				     NL_VALIDATE_STRICT, extack);
}

static inline int
nla_validate_nested_deprecated(const struct nlattr *start, int maxtype,
			       const struct nla_policy *policy,
			       struct netlink_ext_ack *extack)
{
	return __nla_validate_nested(start, maxtype, policy,
				     NL_VALIDATE_LIBERAL, extack);
}


static inline bool nla_need_padding_for_64bit(struct sk_buff *skb)
{
#ifndef CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS
	
	if (IS_ALIGNED((unsigned long)skb_tail_pointer(skb), 8))
		return true;
#endif
	return false;
}


static inline int nla_align_64bit(struct sk_buff *skb, int padattr)
{
	if (nla_need_padding_for_64bit(skb) &&
	    !nla_reserve(skb, padattr, 0))
		return -EMSGSIZE;

	return 0;
}


static inline int nla_total_size_64bit(int payload)
{
	return NLA_ALIGN(nla_attr_size(payload))
#ifndef CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS
		+ NLA_ALIGN(nla_attr_size(0))
#endif
		;
}


#define nla_for_each_attr(pos, head, len, rem) \
	for (pos = head, rem = len; \
	     nla_ok(pos, rem); \
	     pos = nla_next(pos, &(rem)))


#define nla_for_each_nested(pos, nla, rem) \
	nla_for_each_attr(pos, nla_data(nla), nla_len(nla), rem)


static inline bool nla_is_last(const struct nlattr *nla, int rem)
{
	return nla->nla_len == rem;
}

#endif
