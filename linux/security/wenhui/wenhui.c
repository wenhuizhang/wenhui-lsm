#include <linux/string.h>
#include <linux/printk.h>
#include <linux/spinlock.h>
#include <linux/lsm_hooks.h>
#include <linux/security.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/cred.h>
#include <linux/dcache.h>
#include <linux/binfmts.h>
#include <linux/msg.h>
#include <linux/xattr.h>
#include <linux/slab.h>


#include <linux/delay.h> /* usleep_range */
#include <linux/module.h>



#include "wenhui.h"


extern int wenhui_mode;

struct lsm_blob_sizes wenhui_blob_sizes  __lsm_ro_after_init = { 
        .lbs_cred = sizeof(struct wenhui_task_security_struct),
        .lbs_file = sizeof(struct wenhui_file_security_struct),
        .lbs_inode = sizeof(struct wenhui_inode_security_struct),
        .lbs_ipc = sizeof(struct wenhui_ipc_security_struct),
        .lbs_msg_msg = sizeof(struct wenhui_msg_security_struct),
};


enum label_initialized {
        WENHUI_LABEL_INVALID,          /* invalid or not initialized */
        WENHUI_LABEL_INITIALIZED,      /* initialized */
        WENHUI_LABEL_PENDING
};



/* Flag indicating whether initialization completed */
int wenhui_initialized __lsm_ro_after_init = 1;


static inline struct wenhui_task_security_struct *wenhui_cred(const struct cred *cred)
{
        return cred->security + wenhui_blob_sizes.lbs_cred;
}

static inline struct wenhui_file_security_struct *wenhui_file(const struct file *file)
{
        return file->f_security + wenhui_blob_sizes.lbs_file;
}

/* get osid of an inode*/ 
static inline struct wenhui_inode_security_struct *wenhui_inode(const struct inode *inode)
{
        if (unlikely(!inode->i_security))
                return NULL;
        return inode->i_security + wenhui_blob_sizes.lbs_inode;
}

static inline struct wenhui_msg_security_struct *wenhui_msg_msg(const struct msg_msg *msg_msg)
{
        return msg_msg->security + wenhui_blob_sizes.lbs_msg_msg;
}

static inline struct wenhui_ipc_security_struct *wenhui_ipc(
                                                const struct kern_ipc_perm *ipc)
{
        return ipc->security + wenhui_blob_sizes.lbs_ipc;
}




/*
* Minimal support for a secondary security module,
* just to allow the use of the capability module.
*/
//static struct security_operations *secondry_ops;


/* Convert context string to sid value (WENHUI_*) */

static u32 security_context_to_sid(char *context)
{
	// printk(KERN_INFO "%s: have context: %s\n", __func__, context);
	u32 sid;

	if (!context) {
		sid = WENHUI_IGNORE;
		goto out;
	}

	if (strcmp(context, "untrusted") == 0) {
		sid = WENHUI_UNTRUSTED;
		// printk(KERN_INFO "%s: have UN-Trusted context: %s\n", __func__, context);
		goto out;
	}
	
	if (strcmp(context, "trusted") == 0) {
		sid = WENHUI_TRUSTED;
		// printk(KERN_INFO "%s: have Trusted context: %s\n", __func__, context);
		goto out;
	}

	if (strcmp(context, "target") == 0){
		sid = WENHUI_TARGET_SID;
		goto out;
	}
		
	sid = WENHUI_TRUSTED;
	// sid = WENHUI_IGNORE;

out:
	//printk(KERN_INFO "%s: have sid: 0x%d\n", __func__, sid);
	return sid;
}



/*
 * get the security ID of a set of credentials
 */
static inline u32 wenhui_cred_sid(const struct cred *cred)
{
        const struct wenhui_task_security_struct *tsec;

        tsec = wenhui_cred(cred);
        return tsec->sid;
}



/*
 * get the security ID of task
 */
static inline u32 get_task_sid(struct task_struct *task)
{

	u32 sid;

	rcu_read_lock();
	sid = wenhui_cred_sid(__task_cred(task));
	rcu_read_unlock();
	// in the test, make everything trusted
	sid = WENHUI_TRUSTED;
	return sid;
	//return (u32) task->security;
}



/*
 * get the security ID of inode
 */
u32 wenhui_get_inode_sid(struct inode *inode)
{
	struct wenhui_inode_security_struct *isec;
	u32 sid;

	//printk(KERN_INFO "inode=%p\n", inode);
	isec = inode->i_security;

	//printk(KERN_INFO "isec=%p\n", isec);
	//printk(KERN_INFO "WENHUI LSM: wenhui_initialized is %d, sid is 0x%x\n", wenhui_initialized, isec->sid);	
	if (wenhui_initialized) {
		sid = security_context_to_sid((char*)isec);
	} else {
		sid = WENHUI_IGNORE;
	}

	// in the test, make everything trusted
	sid = WENHUI_TRUSTED;
	return sid;
}





/*
 * get the security ID of inode
 */
// wenhui_inode
static struct wenhui_inode_security_struct *wenhui_inode_security_novalidate(struct inode *inode)
{
	return wenhui_inode(inode);
}

static void wenhui_inode_getsecid(struct inode *inode, u32 *secid)
{
	struct wenhui_inode_security_struct *isec = wenhui_inode_security_novalidate(inode);
	*secid = isec->sid;
}



int has_perm(u32 ssid_full, u32 osid, u32 ops)
{
	u32 ssid = 0xfffffff & ssid_full;
	// printk(KERN_INFO "%s: wenhui has perm ssid 0x%d; osid 0x%d; ops 0x%x; mode %d\n", __func__, ssid, osid, ops, wenhui_mode);

	if(wenhui_mode == 0){
		return 0;
	}	
	// policy checking module, time changes based on wenhui_mode value	
	if( ssid && osid ){
		// printk(KERN_INFO  "WENHUI LSM : wenhui sleep! \t osid is 0x%x \t ssid is 0x%x\n", osid, ssid);
		// int i;
		// for (i = 0; i < wenhui_mode; i++ ){
			// usleep_range(1, 2);
			udelay(wenhui_mode);
		// }
		return 0;
	}
        /* Other processes - allow */
        else {
		// printk(KERN_INFO  "WENHUI LSM : wenhui sleep! \t osid is 0x%x \t ssid is 0x%x\n", osid, ssid);
		// int i;
		// for (i = 0; i < wenhui_mode; i++ ){
		//	 usleep_range(1, 2);
			udelay(wenhui_mode);
		// }
		return 0;
	}

	return -9;  /* should not get here */
}




int inode_has_perm(struct task_struct *task, struct inode *inode, int ops)
{
	int rtn = 0;
	u32 ssid;
	u32 osid;

	if (inode == NULL) 						return 0;

	ssid = get_task_sid(task);
	osid = wenhui_get_inode_sid(inode);
	//printk(KERN_INFO  "WENHUI LSM : \n\t osid is 0x%x\n\t ssid is 0x%x\n", osid, ssid);
	
	/* exceptions */
	// inode->i_sb->s_type->name
	//if(!(strcmp(inode->i_sb->s_type->name, "ext4") == 0))		return 0;

	/* authorize if proc, debugfs etc. files*/
	/*	
	if(strcmp(inode->i_sb->s_type->name, "proc") == 0)		return 0;
	if(strcmp(inode->i_sb->s_type->name, "debugfs") == 0)		return 0;
	if(strcmp(inode->i_sb->s_type->name, "tracefs") == 0)		return 0;
	if(strcmp(inode->i_sb->s_type->name, "pstore") == 0)		return 0;
	if(strcmp(inode->i_sb->s_type->name, "sysfs") == 0)		return 0;
	if(strcmp(inode->i_sb->s_type->name, "cgroup") == 0)		return 0;
	if(strcmp(inode->i_sb->s_type->name, "cgroup2") == 0)		return 0;
	*/

	/*check if has ssid and osid*/
	/* if(!(ssid && osid)){
		//printk(KERN_INFO  "WENHUI LSM: sid missing ssid is 0x%d, osid is 0x%d\n", ssid, osid);
		return 0; // permissive
	}*/
	
	// printk(KERN_INFO  "WENHUI LSM: checking valid ssid is 0x%d, osid is 0x%d\n", ssid, osid);
	/* Do Authorize */
	//int has_perm(u32 ssid_full, u32 osid, u32 ops)
	rtn = has_perm(ssid, osid, ops);

	/* Then, use this code to print relevant denials: for our processes or on our objects */
	/*
	if (rtn) {
		printk(KERN_INFO "%s: task pid=%d of ssid 0x%x "
		       "NOT authorized (%d) for inode osid 0x%x for ops 0x%x\n",
		       __func__, current->pid, ssid, rtn, osid, ops);
	}else{
		// Then, use this code to print relevant authorizations: for our processes 
		printk(KERN_INFO "%s: target task pid=%d of ssid 0x%x " 
			"authorized for inode osid:ops 0x%x:0x%x\n",
		       __func__, current->pid, ssid, osid, ops);
	}
	*/

	return rtn;
}





static int wenhui_inode_permission(struct inode *inode, int mask)
{
	int rtn;

	/* no need to check if no mask (ops) */
	if (!mask) {
		/* No permission to check.  Existence test. */
		return 0;
	}

	/* get the dentry for inode_has_perm */
	if ( !inode ) {
		return 0; /*permissive*/
	}

	if ( current->cred ) {  // ssid
		u32 ssid;
		memcpy(&ssid, current->cred->security, sizeof(u32));
		//printk(KERN_INFO "%s: ssid is %d\n", __func__, ssid);
	}

	rtn = inode_has_perm(current, inode, mask);

	// return rtn; 
	return rtn; /* permissive */	
}


/* Label process based on xattr of executable file */
static int wenhui_bprm_set_creds(struct linux_binprm *bprm)
{
	struct inode *inode = bprm->file->f_inode;
	/* Determine the label for the new process */
  	u32 osid = wenhui_get_inode_sid(inode);

	/* if the inode's sid indicates trusted or untrusted, then set task->security */
	if (osid && current->cred) {
		memcpy(current->cred->security, &osid, sizeof(u32));
		// printk(KERN_INFO "%s: set task pid=%d of ssid 0x%x\n", __func__, current->pid, osid);
	}

	return 0;
}


static int wenhui_inode_init_security(struct inode *inode, struct inode *dir,
                                       const struct qstr *qstr,
                                       const char **name,
                                       void **value, size_t *len)
{
	u32 ssid = get_task_sid(current);
	u32 actual_ssid = 0xfffffff & ssid;
	// struct qstr *qstr = NULL;
	char *namep = NULL;
	char *valuep = NULL;

	if (!inode || !dir)
		return -EOPNOTSUPP;

	
	if (actual_ssid == WENHUI_IGNORE)
		return -EOPNOTSUPP;

	//printk(KERN_INFO "%s: pid %d:0x%x creating a new file\n", __func__, current->pid, ssid);

	/* get attribute name */
	namep = kstrdup(XATTR_WENHUI_SUFFIX, GFP_KERNEL);
	if (!namep)
		return -ENOMEM;
	*name = namep;

	/* set xattr value and length */
	if (actual_ssid == WENHUI_TRUSTED) {
		valuep = kstrdup("trusted", GFP_KERNEL);
		*len = 8;
		//printk(KERN_INFO "%s: task pid=%d of ssid 0x%x creates Trusted object\n",
		//       __func__, current->pid, actual_ssid);
	}
	else if (actual_ssid == WENHUI_UNTRUSTED) {
		valuep = kstrdup("untrusted", GFP_KERNEL);
		*len = 10;
		//printk(KERN_INFO "%s: task pid=%d of ssid 0x%x creates UN-Trusted object\n",
		//       __func__, current->pid, actual_ssid);
	}

	if (!valuep)
		return -ENOMEM;
	*value = valuep;

	return 0;		
}

/**
 * wenhui_inode_alloc_security - allocate an inode blob
 * @inode: the inode in need of a blob
 *
 * Returns 0 if it gets a blob, -ENOMEM otherwise
 */
static int wenhui_inode_alloc_security(struct inode *inode)
{
	struct wenhui_inode_security_struct *isec = wenhui_inode(inode);
	
	if (isec) {
		isec->inode = inode;
		spin_lock_init(&isec->lock);
	}
	return 0;
}

static int wenhui_inode_setxattr(struct dentry *dentry, const char *name,
                                  const void *value, size_t size, int flags)
{
	struct inode *inode;
	u32 mask = MAY_WRITE;
	u32 ssid, osid;
	int rtn;

	if (!strncmp(name, XATTR_NAME_WENHUI, sizeof(XATTR_NAME_WENHUI) - 1)) {
		// sample ignores these
		rtn = 0;
		goto out;
	}

	if (!dentry || !dentry->d_inode) {
		rtn = -EPERM;
		goto out;
	}

	/* Gather inputs for inode_has_perm */
        // current is a global current pointer
	inode = dentry->d_inode;
	ssid = get_task_sid(current);
	osid = wenhui_get_inode_sid(inode);


	/* record attribute setting request before authorization */
	if (ssid && osid) {
		//printk(KERN_INFO "%s: task pid=%d of label 0x%x setting attribute %s"
		//       "for object of label 0x%x\n",
		//       __func__, current->pid, ssid, (name ? name : "unk?"), osid);
	}

	rtn = inode_has_perm(current, inode, mask);		
	goto out;

out:
	return rtn;
}


static void wenhui_task_to_inode(struct task_struct *p,
                                  struct inode *inode)
{
        struct wenhui_inode_security_struct *isec = wenhui_inode(inode);
 	// printk("!!!!!!!%s is called\n", __func__);
        u32 sid = get_task_sid(p);

	if (isec) {
       		spin_lock(&isec->lock);
	        isec->sid = sid; 
        	isec->initialized = WENHUI_LABEL_INITIALIZED;
	        spin_unlock(&isec->lock);
	}
 	// printk("!!!!!!!%s is ended\n", __func__);
}

static int wenhui_inode_create(struct inode *dir, struct dentry *dentry, umode_t mode)
{
	u32 ssid = get_task_sid(current);
	u32 osid;

	if (!dir) {
		//printk(KERN_INFO "%s: no inode created by task of ssid 0x%x\n",
		//       __func__, ssid);
		return 0;
	}

	osid = wenhui_get_inode_sid(dir);

	if (ssid == WENHUI_UNTRUSTED) {
		//printk(KERN_INFO "%s: untrusted task pid=%d with sid 0x%x"
		//	" creating file with sid 0x%x\n",
		//       __func__, current->pid, ssid, osid);
	}

	return 0;
}


static int wenhui_file_permission(struct file *file, int mask)
{
        struct inode *inode;
	int rtn;

	/* no need to check if no mask (ops) */
        if (!mask) {
                /* No permission to check.  Existence test. */
		return 0;
        }

	/* NULL file */
	if (!file || !file->f_path.dentry) {
		//printk(KERN_INFO "%s: no file by task of pid 0x%x\n",
		//       __func__, current->pid);
		return 0;
	}

	/* Collect arguments for call to inode_has_perm */
        // current is a global current pointer
	//inode = file->f_dentry->d_inode;
	inode = file->f_path.dentry->d_inode;

	if ( current->cred ) {  // ssid
		//printk(KERN_INFO "%s: file path info\n", __func__);
	}

	rtn = inode_has_perm(current, inode, mask);

	return 0; /* permissive */	
}






/*
 * This is the list of hooks that we will using for our security module.
 */
static struct security_hook_list wenhui_hooks[] __lsm_ro_after_init = {
	/*
	 * inode function to assign a label and to check permission
	 */
	LSM_HOOK_INIT(inode_permission, wenhui_inode_permission),
	LSM_HOOK_INIT(bprm_set_creds, wenhui_bprm_set_creds),
	LSM_HOOK_INIT(inode_alloc_security, wenhui_inode_alloc_security),
	LSM_HOOK_INIT(inode_init_security, wenhui_inode_init_security),
	LSM_HOOK_INIT(inode_setxattr, wenhui_inode_setxattr),
	LSM_HOOK_INIT(inode_getsecid, wenhui_inode_getsecid),
	LSM_HOOK_INIT(inode_create, wenhui_inode_create),
	LSM_HOOK_INIT(file_permission, wenhui_file_permission),
};

static int __init wenhui_security_init(void)
{
	// pr_info("WENHUI LSM: wenhui LSM initializing ...\n");

	/* register the wenhui hooks with lsm security framework*/
	security_add_hooks(wenhui_hooks, ARRAY_SIZE(wenhui_hooks), "wenhui");
	// pr_info("WENHUI LSM: wenhui LSM hook added ...\n");

        /* Report that WENHUI LSM successfully initialized */
        wenhui_initialized = 1;

	return 0;
}

/*
 * early registration with the kernel
 */


DEFINE_LSM(wenhui_security_init) = {
        .init = wenhui_security_init,
        .name = "wenhui",
	.blobs = &wenhui_blob_sizes,
};


