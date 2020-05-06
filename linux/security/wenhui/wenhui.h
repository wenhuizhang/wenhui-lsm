#ifndef _WENHUI_H 
#define _WENHUI_H


#include <uapi/linux/xattr.h>
#include <linux/fs.h>
#include <linux/lsm_hooks.h>

/* extended attributes */
#define XATTR_WENHUI_SUFFIX "wenhui"
#define XATTR_WENHUI_PREFIX "wenhui"
#define XATTR_NAME_WENHUI "wenhui"

#define INITCONTEXTLEN 255
#define PATHLEN 128

#define WENHUI_IGNORE 0
#define WENHUI_UNTRUSTED 1
#define WENHUI_TRUSTED 2
#define WENHUI_TARGET_SID 7

/* Mask definitions */
/* #define MAY_EXEC 1
 * #define MAY_READ 4
 * #define MAY_APPEND 8
 * #define MAY_WRITE 2
 */
#define MAY_WRITE_EXEC 3
#define NOT_EXEC -1
#define NOT_READ -4
#define NOT_APPEND -8
#define NOT_WRITE -2
#define NOT_WRITE_EXEC -3

/* cwl data to bool value */
#define	WENHUI_MODE	((cwl>>28)&(0x0000000f))


/* Flag indicating whether initialization completed */
extern int wenhui_initialized __lsm_ro_after_init;


/**
 * wenhui security label on tasks and inodes
 * @sid: the sid values specific to an object/task
 */

struct wenhui_task_security_struct {
        u32 osid;               /* SID prior to last execve */
        u32 sid;                /* current SID */
        u32 exec_sid;           /* exec SID */
        u32 create_sid;         /* fscreate SID */
        u32 keycreate_sid;      /* keycreate SID */
        u32 sockcreate_sid;     /* fscreate SID */
};


struct wenhui_file_security_struct {
        u32 sid;                /* SID of open file description */
        u32 fown_sid;           /* SID of file owner (for SIGIO) */
        u32 isid;               /* SID of inode at the time of file open */
        u32 pseqno;             /* Policy seqno at the time of file open */
};

struct wenhui_inode_security_struct {
        struct inode *inode;    /* back pointer to inode object */
        struct list_head list;  /* list of inode_security_struct */
        u32 task_sid;           /* SID of creating task */
        u32 sid;                /* SID of this object */
        u16 sclass;             /* security class of this object */
        unsigned char initialized;      /* initialization flag */
        spinlock_t lock;
};

struct wenhui_ipc_security_struct {
        u16 sclass;     /* security class of this object */
        u32 sid;        /* SID of IPC resource */
};

struct wenhui_msg_security_struct {
        u32 sid;        /* SID of message */
};


/* define security blob */
extern struct lsm_blob_sizes wenhui_blob_sizes;


#endif /* _WENHUI_H */
