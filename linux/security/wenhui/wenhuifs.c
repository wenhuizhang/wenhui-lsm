#include <linux/security.h>
#include <linux/cred.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/binfmts.h>
#include <linux/in.h>
#include <linux/spinlock.h>
#include <linux/msg.h>


#include "wenhui.h"


#define BUF_SIZE 16
int wenhui_mode = 0;


/*
 * get the subjective security ID of the current task
 */
static inline u32 get_current_sid(void)
{
        const struct wenhui_task_security_struct *tsec = current_security();

        return tsec->sid;
}


static ssize_t wenhui_file_read(struct file *filp, char __user *buffer,
				size_t count, loff_t *ppos)
{
	/*reading mode, (e.g. CW-Lite value) value from the kernel*/
	char tmpbuf[BUF_SIZE];
	memset(&tmpbuf, 0, BUF_SIZE);
	// printk(KERN_INFO "WENHUI LSM: %s , page value is %p, count is %ld\n", __func__, &page, count);

	//u32 ssid = get_current_sid();
	//int wenhui_mode = ( 0xf0000000 & ssid ) > 28;

	sprintf(tmpbuf, "%d", wenhui_mode);
	goto to_user;

to_user:
	if( *ppos >  0)
		return 0;
	if (count > BUF_SIZE)
		count = BUF_SIZE;
		
	if(copy_to_user(buffer, tmpbuf + *ppos, count))
		return -EFAULT;

	*ppos += count;

	return count;
}


static ssize_t wenhui_file_write(struct file *filp, const char __user *buffer,
		size_t count, loff_t *ppos)
{

	/* collect value to write from user space*/
	char page[BUF_SIZE];

	memset(&page, 0, BUF_SIZE);

	if( *ppos > 0)
		return 0;
	if(copy_from_user(page + *ppos, buffer, count))
		return -EFAULT;

	*ppos += count;

	// printk(KERN_INFO "WENHUI LSM: %s , page value is %p, count is %ld\n", __func__, &page, count);

	
	sscanf(page, "%d", &wenhui_mode);
	printk(KERN_INFO "WENHUI LSM: wenhui mode wenhui_mode %d\n", wenhui_mode);

	return count;

}



static const struct file_operations wenhui_file_fops = {
	.read = wenhui_file_read,
	.write = wenhui_file_write,
};


static int __init wenhui_init_securityfs(void)
{
	int ret;
	struct dentry *policy_dir;
	struct dentry *policy_file;
	wenhui_mode = 0;

	if(!wenhui_initialized)
		return 0;

	policy_dir = securityfs_create_dir("wenhui", NULL);
	if(IS_ERR(policy_dir)){
		ret = PTR_ERR(policy_dir);
		goto Fail;
	}


	policy_file = securityfs_create_file("wenhui_mode", 0644, policy_dir, NULL, &wenhui_file_fops);
	if(IS_ERR(policy_file)){
		printk(KERN_INFO "WENHUI LSM: create debugfs 'wenhui_mode' file failed\n");
		ret = PTR_ERR(policy_file);
		goto Fail;
	}

	printk(KERN_INFO "WENHUI LSM: debugfs created :\n \t policy dir: 0x%pd, \n\t policy file: 0x%pd.\n", policy_dir, policy_file);
	return 0;
Fail:
	securityfs_remove(policy_dir);
	policy_dir = NULL;
	return ret;


}


fs_initcall(wenhui_init_securityfs);
