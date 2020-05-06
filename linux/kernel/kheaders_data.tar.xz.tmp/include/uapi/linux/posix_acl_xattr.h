/* SPDX-License-Identifier: LGPL-2.1+ WITH Linux-syscall-note */


#ifndef __UAPI_POSIX_ACL_XATTR_H
#define __UAPI_POSIX_ACL_XATTR_H

#include <linux/types.h>


#define POSIX_ACL_XATTR_VERSION	0x0002


#define ACL_UNDEFINED_ID	(-1)

struct posix_acl_xattr_entry {
	__le16			e_tag;
	__le16			e_perm;
	__le32			e_id;
};

struct posix_acl_xattr_header {
	__le32			a_version;
};

#endif	
