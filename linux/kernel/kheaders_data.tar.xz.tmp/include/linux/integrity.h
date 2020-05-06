/* SPDX-License-Identifier: GPL-2.0-only */


#ifndef _LINUX_INTEGRITY_H
#define _LINUX_INTEGRITY_H

#include <linux/fs.h>

enum integrity_status {
	INTEGRITY_PASS = 0,
	INTEGRITY_PASS_IMMUTABLE,
	INTEGRITY_FAIL,
	INTEGRITY_NOLABEL,
	INTEGRITY_NOXATTRS,
	INTEGRITY_UNKNOWN,
};


#ifdef CONFIG_INTEGRITY
extern struct integrity_iint_cache *integrity_inode_get(struct inode *inode);
extern void integrity_inode_free(struct inode *inode);
extern void __init integrity_load_keys(void);

#else
static inline struct integrity_iint_cache *
				integrity_inode_get(struct inode *inode)
{
	return NULL;
}

static inline void integrity_inode_free(struct inode *inode)
{
	return;
}

static inline void integrity_load_keys(void)
{
}
#endif 

#ifdef CONFIG_INTEGRITY_ASYMMETRIC_KEYS

extern int integrity_kernel_module_request(char *kmod_name);

#else

static inline int integrity_kernel_module_request(char *kmod_name)
{
	return 0;
}

#endif 

#endif 