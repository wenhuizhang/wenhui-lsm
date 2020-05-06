/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _LINUX_FSCRYPT_H
#define _LINUX_FSCRYPT_H

#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/slab.h>

#define FS_CRYPTO_BLOCK_SIZE		16

struct fscrypt_ctx;
struct fscrypt_info;

struct fscrypt_str {
	unsigned char *name;
	u32 len;
};

struct fscrypt_name {
	const struct qstr *usr_fname;
	struct fscrypt_str disk_name;
	u32 hash;
	u32 minor_hash;
	struct fscrypt_str crypto_buf;
	bool is_ciphertext_name;
};

#define FSTR_INIT(n, l)		{ .name = n, .len = l }
#define FSTR_TO_QSTR(f)		QSTR_INIT((f)->name, (f)->len)
#define fname_name(p)		((p)->disk_name.name)
#define fname_len(p)		((p)->disk_name.len)


#define FSCRYPT_SET_CONTEXT_MAX_SIZE	28

#ifdef CONFIG_FS_ENCRYPTION

#define FS_CFLG_OWN_PAGES (1U << 1)


struct fscrypt_operations {
	unsigned int flags;
	const char *key_prefix;
	int (*get_context)(struct inode *, void *, size_t);
	int (*set_context)(struct inode *, const void *, size_t, void *);
	bool (*dummy_context)(struct inode *);
	bool (*empty_dir)(struct inode *);
	unsigned int max_namelen;
};


struct fscrypt_ctx {
	union {
		struct {
			struct bio *bio;
			struct work_struct work;
		};
		struct list_head free_list;	
	};
	u8 flags;				
};

static inline bool fscrypt_has_encryption_key(const struct inode *inode)
{
	
	return READ_ONCE(inode->i_crypt_info) != NULL;
}

static inline bool fscrypt_dummy_context_enabled(struct inode *inode)
{
	return inode->i_sb->s_cop->dummy_context &&
		inode->i_sb->s_cop->dummy_context(inode);
}


static inline void fscrypt_handle_d_move(struct dentry *dentry)
{
	dentry->d_flags &= ~DCACHE_ENCRYPTED_NAME;
}


extern void fscrypt_enqueue_decrypt_work(struct work_struct *);
extern struct fscrypt_ctx *fscrypt_get_ctx(gfp_t);
extern void fscrypt_release_ctx(struct fscrypt_ctx *);

extern struct page *fscrypt_encrypt_pagecache_blocks(struct page *page,
						     unsigned int len,
						     unsigned int offs,
						     gfp_t gfp_flags);
extern int fscrypt_encrypt_block_inplace(const struct inode *inode,
					 struct page *page, unsigned int len,
					 unsigned int offs, u64 lblk_num,
					 gfp_t gfp_flags);

extern int fscrypt_decrypt_pagecache_blocks(struct page *page, unsigned int len,
					    unsigned int offs);
extern int fscrypt_decrypt_block_inplace(const struct inode *inode,
					 struct page *page, unsigned int len,
					 unsigned int offs, u64 lblk_num);

static inline bool fscrypt_is_bounce_page(struct page *page)
{
	return page->mapping == NULL;
}

static inline struct page *fscrypt_pagecache_page(struct page *bounce_page)
{
	return (struct page *)page_private(bounce_page);
}

extern void fscrypt_free_bounce_page(struct page *bounce_page);


extern int fscrypt_ioctl_set_policy(struct file *, const void __user *);
extern int fscrypt_ioctl_get_policy(struct file *, void __user *);
extern int fscrypt_has_permitted_context(struct inode *, struct inode *);
extern int fscrypt_inherit_context(struct inode *, struct inode *,
					void *, bool);

extern int fscrypt_get_encryption_info(struct inode *);
extern void fscrypt_put_encryption_info(struct inode *);
extern void fscrypt_free_inode(struct inode *);


extern int fscrypt_setup_filename(struct inode *, const struct qstr *,
				int lookup, struct fscrypt_name *);

static inline void fscrypt_free_filename(struct fscrypt_name *fname)
{
	kfree(fname->crypto_buf.name);
}

extern int fscrypt_fname_alloc_buffer(const struct inode *, u32,
				struct fscrypt_str *);
extern void fscrypt_fname_free_buffer(struct fscrypt_str *);
extern int fscrypt_fname_disk_to_usr(struct inode *, u32, u32,
			const struct fscrypt_str *, struct fscrypt_str *);

#define FSCRYPT_FNAME_MAX_UNDIGESTED_SIZE	32


#define FSCRYPT_FNAME_DIGEST(name, len)	\
	((name) + round_down((len) - FS_CRYPTO_BLOCK_SIZE - 1, \
			     FS_CRYPTO_BLOCK_SIZE))

#define FSCRYPT_FNAME_DIGEST_SIZE	FS_CRYPTO_BLOCK_SIZE


struct fscrypt_digested_name {
	u32 hash;
	u32 minor_hash;
	u8 digest[FSCRYPT_FNAME_DIGEST_SIZE];
};


static inline bool fscrypt_match_name(const struct fscrypt_name *fname,
				      const u8 *de_name, u32 de_name_len)
{
	if (unlikely(!fname->disk_name.name)) {
		const struct fscrypt_digested_name *n =
			(const void *)fname->crypto_buf.name;
		if (WARN_ON_ONCE(fname->usr_fname->name[0] != '_'))
			return false;
		if (de_name_len <= FSCRYPT_FNAME_MAX_UNDIGESTED_SIZE)
			return false;
		return !memcmp(FSCRYPT_FNAME_DIGEST(de_name, de_name_len),
			       n->digest, FSCRYPT_FNAME_DIGEST_SIZE);
	}

	if (de_name_len != fname->disk_name.len)
		return false;
	return !memcmp(de_name, fname->disk_name.name, fname->disk_name.len);
}


extern void fscrypt_decrypt_bio(struct bio *);
extern void fscrypt_enqueue_decrypt_bio(struct fscrypt_ctx *ctx,
					struct bio *bio);
extern int fscrypt_zeroout_range(const struct inode *, pgoff_t, sector_t,
				 unsigned int);


extern int fscrypt_file_open(struct inode *inode, struct file *filp);
extern int __fscrypt_prepare_link(struct inode *inode, struct inode *dir,
				  struct dentry *dentry);
extern int __fscrypt_prepare_rename(struct inode *old_dir,
				    struct dentry *old_dentry,
				    struct inode *new_dir,
				    struct dentry *new_dentry,
				    unsigned int flags);
extern int __fscrypt_prepare_lookup(struct inode *dir, struct dentry *dentry,
				    struct fscrypt_name *fname);
extern int __fscrypt_prepare_symlink(struct inode *dir, unsigned int len,
				     unsigned int max_len,
				     struct fscrypt_str *disk_link);
extern int __fscrypt_encrypt_symlink(struct inode *inode, const char *target,
				     unsigned int len,
				     struct fscrypt_str *disk_link);
extern const char *fscrypt_get_symlink(struct inode *inode, const void *caddr,
				       unsigned int max_size,
				       struct delayed_call *done);
static inline void fscrypt_set_ops(struct super_block *sb,
				   const struct fscrypt_operations *s_cop)
{
	sb->s_cop = s_cop;
}
#else  

static inline bool fscrypt_has_encryption_key(const struct inode *inode)
{
	return false;
}

static inline bool fscrypt_dummy_context_enabled(struct inode *inode)
{
	return false;
}

static inline void fscrypt_handle_d_move(struct dentry *dentry)
{
}


static inline void fscrypt_enqueue_decrypt_work(struct work_struct *work)
{
}

static inline struct fscrypt_ctx *fscrypt_get_ctx(gfp_t gfp_flags)
{
	return ERR_PTR(-EOPNOTSUPP);
}

static inline void fscrypt_release_ctx(struct fscrypt_ctx *ctx)
{
	return;
}

static inline struct page *fscrypt_encrypt_pagecache_blocks(struct page *page,
							    unsigned int len,
							    unsigned int offs,
							    gfp_t gfp_flags)
{
	return ERR_PTR(-EOPNOTSUPP);
}

static inline int fscrypt_encrypt_block_inplace(const struct inode *inode,
						struct page *page,
						unsigned int len,
						unsigned int offs, u64 lblk_num,
						gfp_t gfp_flags)
{
	return -EOPNOTSUPP;
}

static inline int fscrypt_decrypt_pagecache_blocks(struct page *page,
						   unsigned int len,
						   unsigned int offs)
{
	return -EOPNOTSUPP;
}

static inline int fscrypt_decrypt_block_inplace(const struct inode *inode,
						struct page *page,
						unsigned int len,
						unsigned int offs, u64 lblk_num)
{
	return -EOPNOTSUPP;
}

static inline bool fscrypt_is_bounce_page(struct page *page)
{
	return false;
}

static inline struct page *fscrypt_pagecache_page(struct page *bounce_page)
{
	WARN_ON_ONCE(1);
	return ERR_PTR(-EINVAL);
}

static inline void fscrypt_free_bounce_page(struct page *bounce_page)
{
}


static inline int fscrypt_ioctl_set_policy(struct file *filp,
					   const void __user *arg)
{
	return -EOPNOTSUPP;
}

static inline int fscrypt_ioctl_get_policy(struct file *filp, void __user *arg)
{
	return -EOPNOTSUPP;
}

static inline int fscrypt_has_permitted_context(struct inode *parent,
						struct inode *child)
{
	return 0;
}

static inline int fscrypt_inherit_context(struct inode *parent,
					  struct inode *child,
					  void *fs_data, bool preload)
{
	return -EOPNOTSUPP;
}


static inline int fscrypt_get_encryption_info(struct inode *inode)
{
	return -EOPNOTSUPP;
}

static inline void fscrypt_put_encryption_info(struct inode *inode)
{
	return;
}

static inline void fscrypt_free_inode(struct inode *inode)
{
}

 
static inline int fscrypt_setup_filename(struct inode *dir,
					 const struct qstr *iname,
					 int lookup, struct fscrypt_name *fname)
{
	if (IS_ENCRYPTED(dir))
		return -EOPNOTSUPP;

	memset(fname, 0, sizeof(*fname));
	fname->usr_fname = iname;
	fname->disk_name.name = (unsigned char *)iname->name;
	fname->disk_name.len = iname->len;
	return 0;
}

static inline void fscrypt_free_filename(struct fscrypt_name *fname)
{
	return;
}

static inline int fscrypt_fname_alloc_buffer(const struct inode *inode,
					     u32 max_encrypted_len,
					     struct fscrypt_str *crypto_str)
{
	return -EOPNOTSUPP;
}

static inline void fscrypt_fname_free_buffer(struct fscrypt_str *crypto_str)
{
	return;
}

static inline int fscrypt_fname_disk_to_usr(struct inode *inode,
					    u32 hash, u32 minor_hash,
					    const struct fscrypt_str *iname,
					    struct fscrypt_str *oname)
{
	return -EOPNOTSUPP;
}

static inline bool fscrypt_match_name(const struct fscrypt_name *fname,
				      const u8 *de_name, u32 de_name_len)
{
	
	if (de_name_len != fname->disk_name.len)
		return false;
	return !memcmp(de_name, fname->disk_name.name, fname->disk_name.len);
}


static inline void fscrypt_decrypt_bio(struct bio *bio)
{
}

static inline void fscrypt_enqueue_decrypt_bio(struct fscrypt_ctx *ctx,
					       struct bio *bio)
{
}

static inline int fscrypt_zeroout_range(const struct inode *inode, pgoff_t lblk,
					sector_t pblk, unsigned int len)
{
	return -EOPNOTSUPP;
}



static inline int fscrypt_file_open(struct inode *inode, struct file *filp)
{
	if (IS_ENCRYPTED(inode))
		return -EOPNOTSUPP;
	return 0;
}

static inline int __fscrypt_prepare_link(struct inode *inode, struct inode *dir,
					 struct dentry *dentry)
{
	return -EOPNOTSUPP;
}

static inline int __fscrypt_prepare_rename(struct inode *old_dir,
					   struct dentry *old_dentry,
					   struct inode *new_dir,
					   struct dentry *new_dentry,
					   unsigned int flags)
{
	return -EOPNOTSUPP;
}

static inline int __fscrypt_prepare_lookup(struct inode *dir,
					   struct dentry *dentry,
					   struct fscrypt_name *fname)
{
	return -EOPNOTSUPP;
}

static inline int __fscrypt_prepare_symlink(struct inode *dir,
					    unsigned int len,
					    unsigned int max_len,
					    struct fscrypt_str *disk_link)
{
	return -EOPNOTSUPP;
}


static inline int __fscrypt_encrypt_symlink(struct inode *inode,
					    const char *target,
					    unsigned int len,
					    struct fscrypt_str *disk_link)
{
	return -EOPNOTSUPP;
}

static inline const char *fscrypt_get_symlink(struct inode *inode,
					      const void *caddr,
					      unsigned int max_size,
					      struct delayed_call *done)
{
	return ERR_PTR(-EOPNOTSUPP);
}

static inline void fscrypt_set_ops(struct super_block *sb,
				   const struct fscrypt_operations *s_cop)
{
}

#endif	


static inline int fscrypt_require_key(struct inode *inode)
{
	if (IS_ENCRYPTED(inode)) {
		int err = fscrypt_get_encryption_info(inode);

		if (err)
			return err;
		if (!fscrypt_has_encryption_key(inode))
			return -ENOKEY;
	}
	return 0;
}


static inline int fscrypt_prepare_link(struct dentry *old_dentry,
				       struct inode *dir,
				       struct dentry *dentry)
{
	if (IS_ENCRYPTED(dir))
		return __fscrypt_prepare_link(d_inode(old_dentry), dir, dentry);
	return 0;
}


static inline int fscrypt_prepare_rename(struct inode *old_dir,
					 struct dentry *old_dentry,
					 struct inode *new_dir,
					 struct dentry *new_dentry,
					 unsigned int flags)
{
	if (IS_ENCRYPTED(old_dir) || IS_ENCRYPTED(new_dir))
		return __fscrypt_prepare_rename(old_dir, old_dentry,
						new_dir, new_dentry, flags);
	return 0;
}


static inline int fscrypt_prepare_lookup(struct inode *dir,
					 struct dentry *dentry,
					 struct fscrypt_name *fname)
{
	if (IS_ENCRYPTED(dir))
		return __fscrypt_prepare_lookup(dir, dentry, fname);

	memset(fname, 0, sizeof(*fname));
	fname->usr_fname = &dentry->d_name;
	fname->disk_name.name = (unsigned char *)dentry->d_name.name;
	fname->disk_name.len = dentry->d_name.len;
	return 0;
}


static inline int fscrypt_prepare_setattr(struct dentry *dentry,
					  struct iattr *attr)
{
	if (attr->ia_valid & ATTR_SIZE)
		return fscrypt_require_key(d_inode(dentry));
	return 0;
}


static inline int fscrypt_prepare_symlink(struct inode *dir,
					  const char *target,
					  unsigned int len,
					  unsigned int max_len,
					  struct fscrypt_str *disk_link)
{
	if (IS_ENCRYPTED(dir) || fscrypt_dummy_context_enabled(dir))
		return __fscrypt_prepare_symlink(dir, len, max_len, disk_link);

	disk_link->name = (unsigned char *)target;
	disk_link->len = len + 1;
	if (disk_link->len > max_len)
		return -ENAMETOOLONG;
	return 0;
}


static inline int fscrypt_encrypt_symlink(struct inode *inode,
					  const char *target,
					  unsigned int len,
					  struct fscrypt_str *disk_link)
{
	if (IS_ENCRYPTED(inode))
		return __fscrypt_encrypt_symlink(inode, target, len, disk_link);
	return 0;
}


static inline void fscrypt_finalize_bounce_page(struct page **pagep)
{
	struct page *page = *pagep;

	if (fscrypt_is_bounce_page(page)) {
		*pagep = fscrypt_pagecache_page(page);
		fscrypt_free_bounce_page(page);
	}
}

#endif	
