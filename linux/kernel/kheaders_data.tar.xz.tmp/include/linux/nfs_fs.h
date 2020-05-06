/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _LINUX_NFS_FS_H
#define _LINUX_NFS_FS_H

#include <uapi/linux/nfs_fs.h>



#ifdef CONFIG_NFS_DEBUG
# define NFS_DEBUG
#endif

#include <linux/in.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/rbtree.h>
#include <linux/refcount.h>
#include <linux/rwsem.h>
#include <linux/wait.h>

#include <linux/sunrpc/debug.h>
#include <linux/sunrpc/auth.h>
#include <linux/sunrpc/clnt.h>

#include <linux/nfs.h>
#include <linux/nfs2.h>
#include <linux/nfs3.h>
#include <linux/nfs4.h>
#include <linux/nfs_xdr.h>
#include <linux/nfs_fs_sb.h>

#include <linux/mempool.h>


#define NFS_RPC_SWAPFLAGS		(RPC_TASK_SWAPPER|RPC_TASK_ROOTCREDS)


struct nfs_access_entry {
	struct rb_node		rb_node;
	struct list_head	lru;
	const struct cred *	cred;
	__u32			mask;
	struct rcu_head		rcu_head;
};

struct nfs_lock_context {
	refcount_t count;
	struct list_head list;
	struct nfs_open_context *open_context;
	fl_owner_t lockowner;
	atomic_t io_count;
	struct rcu_head	rcu_head;
};

struct nfs4_state;
struct nfs_open_context {
	struct nfs_lock_context lock_context;
	fl_owner_t flock_owner;
	struct dentry *dentry;
	const struct cred *cred;
	struct rpc_cred *ll_cred;	
	struct nfs4_state *state;
	fmode_t mode;

	unsigned long flags;
#define NFS_CONTEXT_RESEND_WRITES	(1)
#define NFS_CONTEXT_BAD			(2)
#define NFS_CONTEXT_UNLOCK	(3)
	int error;

	struct list_head list;
	struct nfs4_threshold	*mdsthreshold;
	struct rcu_head	rcu_head;
};

struct nfs_open_dir_context {
	struct list_head list;
	const struct cred *cred;
	unsigned long attr_gencount;
	__u64 dir_cookie;
	__u64 dup_cookie;
	signed char duped;
};


struct nfs_delegation;

struct posix_acl;


struct nfs_inode {
	
	__u64 fileid;

	
	struct nfs_fh		fh;

	
	unsigned long		flags;			
	unsigned long		cache_validity;		

	
	unsigned long		read_cache_jiffies;
	unsigned long		attrtimeo;
	unsigned long		attrtimeo_timestamp;

	unsigned long		attr_gencount;
	
	unsigned long		cache_change_attribute;

	struct rb_root		access_cache;
	struct list_head	access_cache_entry_lru;
	struct list_head	access_cache_inode_lru;

	
	__be32			cookieverf[2];

	atomic_long_t		nrequests;
	struct nfs_mds_commit_info commit_info;

	
	struct list_head	open_files;

	
	
	struct rw_semaphore	rmdir_sem;
	struct mutex		commit_mutex;

#if IS_ENABLED(CONFIG_NFS_V4)
	struct nfs4_cached_acl	*nfs4_acl;
        
	struct list_head	open_states;
	struct nfs_delegation __rcu *delegation;
	struct rw_semaphore	rwsem;

	
	struct pnfs_layout_hdr *layout;
#endif 
	
	__u64 write_io;
	__u64 read_io;
#ifdef CONFIG_NFS_FSCACHE
	struct fscache_cookie	*fscache;
#endif
	struct inode		vfs_inode;
};

struct nfs4_copy_state {
	struct list_head	copies;
	nfs4_stateid		stateid;
	struct completion	completion;
	uint64_t		count;
	struct nfs_writeverf	verf;
	int			error;
	int			flags;
	struct nfs4_state	*parent_state;
};


#define NFS_ACCESS_READ        0x0001
#define NFS_ACCESS_LOOKUP      0x0002
#define NFS_ACCESS_MODIFY      0x0004
#define NFS_ACCESS_EXTEND      0x0008
#define NFS_ACCESS_DELETE      0x0010
#define NFS_ACCESS_EXECUTE     0x0020


#define NFS_INO_INVALID_DATA	BIT(1)		
#define NFS_INO_INVALID_ATIME	BIT(2)		
#define NFS_INO_INVALID_ACCESS	BIT(3)		
#define NFS_INO_INVALID_ACL	BIT(4)		
#define NFS_INO_REVAL_PAGECACHE	BIT(5)		
#define NFS_INO_REVAL_FORCED	BIT(6)		
#define NFS_INO_INVALID_LABEL	BIT(7)		
#define NFS_INO_INVALID_CHANGE	BIT(8)		
#define NFS_INO_INVALID_CTIME	BIT(9)		
#define NFS_INO_INVALID_MTIME	BIT(10)		
#define NFS_INO_INVALID_SIZE	BIT(11)		
#define NFS_INO_INVALID_OTHER	BIT(12)		
#define NFS_INO_DATA_INVAL_DEFER	\
				BIT(13)		

#define NFS_INO_INVALID_ATTR	(NFS_INO_INVALID_CHANGE \
		| NFS_INO_INVALID_CTIME \
		| NFS_INO_INVALID_MTIME \
		| NFS_INO_INVALID_SIZE \
		| NFS_INO_INVALID_OTHER)	


#define NFS_INO_ADVISE_RDPLUS	(0)		
#define NFS_INO_STALE		(1)		
#define NFS_INO_ACL_LRU_SET	(2)		
#define NFS_INO_INVALIDATING	(3)		
#define NFS_INO_FSCACHE		(5)		
#define NFS_INO_FSCACHE_LOCK	(6)		
#define NFS_INO_LAYOUTCOMMIT	(9)		
#define NFS_INO_LAYOUTCOMMITTING (10)		
#define NFS_INO_LAYOUTSTATS	(11)		
#define NFS_INO_ODIRECT		(12)		

static inline struct nfs_inode *NFS_I(const struct inode *inode)
{
	return container_of(inode, struct nfs_inode, vfs_inode);
}

static inline struct nfs_server *NFS_SB(const struct super_block *s)
{
	return (struct nfs_server *)(s->s_fs_info);
}

static inline struct nfs_fh *NFS_FH(const struct inode *inode)
{
	return &NFS_I(inode)->fh;
}

static inline struct nfs_server *NFS_SERVER(const struct inode *inode)
{
	return NFS_SB(inode->i_sb);
}

static inline struct rpc_clnt *NFS_CLIENT(const struct inode *inode)
{
	return NFS_SERVER(inode)->client;
}

static inline const struct nfs_rpc_ops *NFS_PROTO(const struct inode *inode)
{
	return NFS_SERVER(inode)->nfs_client->rpc_ops;
}

static inline unsigned NFS_MINATTRTIMEO(const struct inode *inode)
{
	struct nfs_server *nfss = NFS_SERVER(inode);
	return S_ISDIR(inode->i_mode) ? nfss->acdirmin : nfss->acregmin;
}

static inline unsigned NFS_MAXATTRTIMEO(const struct inode *inode)
{
	struct nfs_server *nfss = NFS_SERVER(inode);
	return S_ISDIR(inode->i_mode) ? nfss->acdirmax : nfss->acregmax;
}

static inline int NFS_STALE(const struct inode *inode)
{
	return test_bit(NFS_INO_STALE, &NFS_I(inode)->flags);
}

static inline struct fscache_cookie *nfs_i_fscache(struct inode *inode)
{
#ifdef CONFIG_NFS_FSCACHE
	return NFS_I(inode)->fscache;
#else
	return NULL;
#endif
}

static inline __u64 NFS_FILEID(const struct inode *inode)
{
	return NFS_I(inode)->fileid;
}

static inline void set_nfs_fileid(struct inode *inode, __u64 fileid)
{
	NFS_I(inode)->fileid = fileid;
}

static inline void nfs_mark_for_revalidate(struct inode *inode)
{
	struct nfs_inode *nfsi = NFS_I(inode);

	spin_lock(&inode->i_lock);
	nfsi->cache_validity |= NFS_INO_REVAL_PAGECACHE
		| NFS_INO_INVALID_ACCESS
		| NFS_INO_INVALID_ACL
		| NFS_INO_INVALID_CHANGE
		| NFS_INO_INVALID_CTIME;
	if (S_ISDIR(inode->i_mode))
		nfsi->cache_validity |= NFS_INO_INVALID_DATA;
	spin_unlock(&inode->i_lock);
}

static inline int nfs_server_capable(struct inode *inode, int cap)
{
	return NFS_SERVER(inode)->caps & cap;
}

static inline void nfs_set_verifier(struct dentry * dentry, unsigned long verf)
{
	dentry->d_time = verf;
}


static inline unsigned long nfs_save_change_attribute(struct inode *dir)
{
	return NFS_I(dir)->cache_change_attribute;
}


static inline int nfs_verify_change_attribute(struct inode *dir, unsigned long chattr)
{
	return chattr == NFS_I(dir)->cache_change_attribute;
}


extern int nfs_sync_mapping(struct address_space *mapping);
extern void nfs_zap_mapping(struct inode *inode, struct address_space *mapping);
extern void nfs_zap_caches(struct inode *);
extern void nfs_invalidate_atime(struct inode *);
extern struct inode *nfs_fhget(struct super_block *, struct nfs_fh *,
				struct nfs_fattr *, struct nfs4_label *);
struct inode *nfs_ilookup(struct super_block *sb, struct nfs_fattr *, struct nfs_fh *);
extern int nfs_refresh_inode(struct inode *, struct nfs_fattr *);
extern int nfs_post_op_update_inode(struct inode *inode, struct nfs_fattr *fattr);
extern int nfs_post_op_update_inode_force_wcc(struct inode *inode, struct nfs_fattr *fattr);
extern int nfs_post_op_update_inode_force_wcc_locked(struct inode *inode, struct nfs_fattr *fattr);
extern int nfs_getattr(const struct path *, struct kstat *, u32, unsigned int);
extern void nfs_access_add_cache(struct inode *, struct nfs_access_entry *);
extern void nfs_access_set_mask(struct nfs_access_entry *, u32);
extern int nfs_permission(struct inode *, int);
extern int nfs_open(struct inode *, struct file *);
extern int nfs_attribute_cache_expired(struct inode *inode);
extern int nfs_revalidate_inode(struct nfs_server *server, struct inode *inode);
extern int __nfs_revalidate_inode(struct nfs_server *, struct inode *);
extern bool nfs_mapping_need_revalidate_inode(struct inode *inode);
extern int nfs_revalidate_mapping(struct inode *inode, struct address_space *mapping);
extern int nfs_revalidate_mapping_rcu(struct inode *inode);
extern int nfs_setattr(struct dentry *, struct iattr *);
extern void nfs_setattr_update_inode(struct inode *inode, struct iattr *attr, struct nfs_fattr *);
extern void nfs_setsecurity(struct inode *inode, struct nfs_fattr *fattr,
				struct nfs4_label *label);
extern struct nfs_open_context *get_nfs_open_context(struct nfs_open_context *ctx);
extern void put_nfs_open_context(struct nfs_open_context *ctx);
extern struct nfs_open_context *nfs_find_open_context(struct inode *inode, const struct cred *cred, fmode_t mode);
extern struct nfs_open_context *alloc_nfs_open_context(struct dentry *dentry, fmode_t f_mode, struct file *filp);
extern void nfs_inode_attach_open_context(struct nfs_open_context *ctx);
extern void nfs_file_set_open_context(struct file *filp, struct nfs_open_context *ctx);
extern void nfs_file_clear_open_context(struct file *flip);
extern struct nfs_lock_context *nfs_get_lock_context(struct nfs_open_context *ctx);
extern void nfs_put_lock_context(struct nfs_lock_context *l_ctx);
extern u64 nfs_compat_user_ino64(u64 fileid);
extern void nfs_fattr_init(struct nfs_fattr *fattr);
extern void nfs_fattr_set_barrier(struct nfs_fattr *fattr);
extern unsigned long nfs_inc_attr_generation_counter(void);

extern struct nfs_fattr *nfs_alloc_fattr(void);

static inline void nfs_free_fattr(const struct nfs_fattr *fattr)
{
	kfree(fattr);
}

extern struct nfs_fh *nfs_alloc_fhandle(void);

static inline void nfs_free_fhandle(const struct nfs_fh *fh)
{
	kfree(fh);
}

#ifdef NFS_DEBUG
extern u32 _nfs_display_fhandle_hash(const struct nfs_fh *fh);
static inline u32 nfs_display_fhandle_hash(const struct nfs_fh *fh)
{
	return _nfs_display_fhandle_hash(fh);
}
extern void _nfs_display_fhandle(const struct nfs_fh *fh, const char *caption);
#define nfs_display_fhandle(fh, caption)			\
	do {							\
		if (unlikely(nfs_debug & NFSDBG_FACILITY))	\
			_nfs_display_fhandle(fh, caption);	\
	} while (0)
#else
static inline u32 nfs_display_fhandle_hash(const struct nfs_fh *fh)
{
	return 0;
}
static inline void nfs_display_fhandle(const struct nfs_fh *fh,
				       const char *caption)
{
}
#endif


extern int  nfs_root_data(char **root_device, char **root_data); 

extern __be32 root_nfs_parse_addr(char *name); 


extern const struct file_operations nfs_file_operations;
#if IS_ENABLED(CONFIG_NFS_V4)
extern const struct file_operations nfs4_file_operations;
#endif 
extern const struct address_space_operations nfs_file_aops;
extern const struct address_space_operations nfs_dir_aops;

static inline struct nfs_open_context *nfs_file_open_context(struct file *filp)
{
	return filp->private_data;
}

static inline const struct cred *nfs_file_cred(struct file *file)
{
	if (file != NULL) {
		struct nfs_open_context *ctx =
			nfs_file_open_context(file);
		if (ctx)
			return ctx->cred;
	}
	return NULL;
}


extern ssize_t nfs_direct_IO(struct kiocb *, struct iov_iter *);
extern ssize_t nfs_file_direct_read(struct kiocb *iocb,
			struct iov_iter *iter);
extern ssize_t nfs_file_direct_write(struct kiocb *iocb,
			struct iov_iter *iter);


extern const struct file_operations nfs_dir_operations;
extern const struct dentry_operations nfs_dentry_operations;

extern void nfs_force_lookup_revalidate(struct inode *dir);
extern int nfs_instantiate(struct dentry *dentry, struct nfs_fh *fh,
			struct nfs_fattr *fattr, struct nfs4_label *label);
extern int nfs_may_open(struct inode *inode, const struct cred *cred, int openflags);
extern void nfs_access_zap_cache(struct inode *inode);


extern const struct inode_operations nfs_symlink_inode_operations;


#ifdef CONFIG_SYSCTL
extern int nfs_register_sysctl(void);
extern void nfs_unregister_sysctl(void);
#else
#define nfs_register_sysctl() 0
#define nfs_unregister_sysctl() do { } while(0)
#endif


extern const struct inode_operations nfs_mountpoint_inode_operations;
extern const struct inode_operations nfs_referral_inode_operations;
extern int nfs_mountpoint_expiry_timeout;
extern void nfs_release_automount_timer(void);


extern void nfs_complete_unlink(struct dentry *dentry, struct inode *);


extern int  nfs_congestion_kb;
extern int  nfs_writepage(struct page *page, struct writeback_control *wbc);
extern int  nfs_writepages(struct address_space *, struct writeback_control *);
extern int  nfs_flush_incompatible(struct file *file, struct page *page);
extern int  nfs_updatepage(struct file *, struct page *, unsigned int, unsigned int);


extern int nfs_sync_inode(struct inode *inode);
extern int nfs_wb_all(struct inode *inode);
extern int nfs_wb_page(struct inode *inode, struct page *page);
extern int nfs_wb_page_cancel(struct inode *inode, struct page* page);
extern int  nfs_commit_inode(struct inode *, int);
extern struct nfs_commit_data *nfs_commitdata_alloc(bool never_fail);
extern void nfs_commit_free(struct nfs_commit_data *data);

static inline int
nfs_have_writebacks(struct inode *inode)
{
	return atomic_long_read(&NFS_I(inode)->nrequests) != 0;
}


extern int  nfs_readpage(struct file *, struct page *);
extern int  nfs_readpages(struct file *, struct address_space *,
		struct list_head *, unsigned);
extern int  nfs_readpage_async(struct nfs_open_context *, struct inode *,
			       struct page *);



static inline loff_t nfs_size_to_loff_t(__u64 size)
{
	return min_t(u64, size, OFFSET_MAX);
}

static inline ino_t
nfs_fileid_to_ino_t(u64 fileid)
{
	ino_t ino = (ino_t) fileid;
	if (sizeof(ino_t) < sizeof(u64))
		ino ^= fileid >> (sizeof(u64)-sizeof(ino_t)) * 8;
	return ino;
}

#define NFS_JUKEBOX_RETRY_TIME (5 * HZ)


# undef ifdebug
# ifdef NFS_DEBUG
#  define ifdebug(fac)		if (unlikely(nfs_debug & NFSDBG_##fac))
#  define NFS_IFDEBUG(x)	x
# else
#  define ifdebug(fac)		if (0)
#  define NFS_IFDEBUG(x)
# endif
#endif
