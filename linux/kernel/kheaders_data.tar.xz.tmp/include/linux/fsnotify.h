/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_FS_NOTIFY_H
#define _LINUX_FS_NOTIFY_H



#include <linux/fsnotify_backend.h>
#include <linux/audit.h>
#include <linux/slab.h>
#include <linux/bug.h>


static inline int fsnotify_dirent(struct inode *dir, struct dentry *dentry,
				  __u32 mask)
{
	return fsnotify(dir, mask, d_inode(dentry), FSNOTIFY_EVENT_INODE,
			&dentry->d_name, 0);
}


static inline int fsnotify_parent(const struct path *path,
				  struct dentry *dentry, __u32 mask)
{
	if (!dentry)
		dentry = path->dentry;

	return __fsnotify_parent(path, dentry, mask);
}


static inline int fsnotify_path(struct inode *inode, const struct path *path,
				__u32 mask)
{
	int ret = fsnotify_parent(path, NULL, mask);

	if (ret)
		return ret;
	return fsnotify(inode, mask, path, FSNOTIFY_EVENT_PATH, NULL, 0);
}


static inline int fsnotify_perm(struct file *file, int mask)
{
	int ret;
	const struct path *path = &file->f_path;
	struct inode *inode = file_inode(file);
	__u32 fsnotify_mask = 0;

	if (file->f_mode & FMODE_NONOTIFY)
		return 0;
	if (!(mask & (MAY_READ | MAY_OPEN)))
		return 0;
	if (mask & MAY_OPEN) {
		fsnotify_mask = FS_OPEN_PERM;

		if (file->f_flags & __FMODE_EXEC) {
			ret = fsnotify_path(inode, path, FS_OPEN_EXEC_PERM);

			if (ret)
				return ret;
		}
	} else if (mask & MAY_READ) {
		fsnotify_mask = FS_ACCESS_PERM;
	}

	if (S_ISDIR(inode->i_mode))
		fsnotify_mask |= FS_ISDIR;

	return fsnotify_path(inode, path, fsnotify_mask);
}


static inline void fsnotify_link_count(struct inode *inode)
{
	__u32 mask = FS_ATTRIB;

	if (S_ISDIR(inode->i_mode))
		mask |= FS_ISDIR;

	fsnotify(inode, mask, inode, FSNOTIFY_EVENT_INODE, NULL, 0);
}


static inline void fsnotify_move(struct inode *old_dir, struct inode *new_dir,
				 const struct qstr *old_name,
				 int isdir, struct inode *target,
				 struct dentry *moved)
{
	struct inode *source = moved->d_inode;
	u32 fs_cookie = fsnotify_get_cookie();
	__u32 old_dir_mask = FS_MOVED_FROM;
	__u32 new_dir_mask = FS_MOVED_TO;
	__u32 mask = FS_MOVE_SELF;
	const struct qstr *new_name = &moved->d_name;

	if (old_dir == new_dir)
		old_dir_mask |= FS_DN_RENAME;

	if (isdir) {
		old_dir_mask |= FS_ISDIR;
		new_dir_mask |= FS_ISDIR;
		mask |= FS_ISDIR;
	}

	fsnotify(old_dir, old_dir_mask, source, FSNOTIFY_EVENT_INODE, old_name,
		 fs_cookie);
	fsnotify(new_dir, new_dir_mask, source, FSNOTIFY_EVENT_INODE, new_name,
		 fs_cookie);

	if (target)
		fsnotify_link_count(target);

	if (source)
		fsnotify(source, mask, source, FSNOTIFY_EVENT_INODE, NULL, 0);
	audit_inode_child(new_dir, moved, AUDIT_TYPE_CHILD_CREATE);
}


static inline void fsnotify_inode_delete(struct inode *inode)
{
	__fsnotify_inode_delete(inode);
}


static inline void fsnotify_vfsmount_delete(struct vfsmount *mnt)
{
	__fsnotify_vfsmount_delete(mnt);
}


static inline void fsnotify_inoderemove(struct inode *inode)
{
	__u32 mask = FS_DELETE_SELF;

	if (S_ISDIR(inode->i_mode))
		mask |= FS_ISDIR;

	fsnotify(inode, mask, inode, FSNOTIFY_EVENT_INODE, NULL, 0);
	__fsnotify_inode_delete(inode);
}


static inline void fsnotify_create(struct inode *inode, struct dentry *dentry)
{
	audit_inode_child(inode, dentry, AUDIT_TYPE_CHILD_CREATE);

	fsnotify_dirent(inode, dentry, FS_CREATE);
}


static inline void fsnotify_link(struct inode *dir, struct inode *inode, struct dentry *new_dentry)
{
	fsnotify_link_count(inode);
	audit_inode_child(dir, new_dentry, AUDIT_TYPE_CHILD_CREATE);

	fsnotify(dir, FS_CREATE, inode, FSNOTIFY_EVENT_INODE, &new_dentry->d_name, 0);
}


static inline void fsnotify_unlink(struct inode *dir, struct dentry *dentry)
{
	
	WARN_ON_ONCE(d_is_negative(dentry));

	fsnotify_dirent(dir, dentry, FS_DELETE);
}


static inline void fsnotify_mkdir(struct inode *inode, struct dentry *dentry)
{
	audit_inode_child(inode, dentry, AUDIT_TYPE_CHILD_CREATE);

	fsnotify_dirent(inode, dentry, FS_CREATE | FS_ISDIR);
}


static inline void fsnotify_rmdir(struct inode *dir, struct dentry *dentry)
{
	
	WARN_ON_ONCE(d_is_negative(dentry));

	fsnotify_dirent(dir, dentry, FS_DELETE | FS_ISDIR);
}


static inline void fsnotify_access(struct file *file)
{
	const struct path *path = &file->f_path;
	struct inode *inode = file_inode(file);
	__u32 mask = FS_ACCESS;

	if (S_ISDIR(inode->i_mode))
		mask |= FS_ISDIR;

	if (!(file->f_mode & FMODE_NONOTIFY))
		fsnotify_path(inode, path, mask);
}


static inline void fsnotify_modify(struct file *file)
{
	const struct path *path = &file->f_path;
	struct inode *inode = file_inode(file);
	__u32 mask = FS_MODIFY;

	if (S_ISDIR(inode->i_mode))
		mask |= FS_ISDIR;

	if (!(file->f_mode & FMODE_NONOTIFY))
		fsnotify_path(inode, path, mask);
}


static inline void fsnotify_open(struct file *file)
{
	const struct path *path = &file->f_path;
	struct inode *inode = file_inode(file);
	__u32 mask = FS_OPEN;

	if (S_ISDIR(inode->i_mode))
		mask |= FS_ISDIR;
	if (file->f_flags & __FMODE_EXEC)
		mask |= FS_OPEN_EXEC;

	fsnotify_path(inode, path, mask);
}


static inline void fsnotify_close(struct file *file)
{
	const struct path *path = &file->f_path;
	struct inode *inode = file_inode(file);
	fmode_t mode = file->f_mode;
	__u32 mask = (mode & FMODE_WRITE) ? FS_CLOSE_WRITE : FS_CLOSE_NOWRITE;

	if (S_ISDIR(inode->i_mode))
		mask |= FS_ISDIR;

	if (!(file->f_mode & FMODE_NONOTIFY))
		fsnotify_path(inode, path, mask);
}


static inline void fsnotify_xattr(struct dentry *dentry)
{
	struct inode *inode = dentry->d_inode;
	__u32 mask = FS_ATTRIB;

	if (S_ISDIR(inode->i_mode))
		mask |= FS_ISDIR;

	fsnotify_parent(NULL, dentry, mask);
	fsnotify(inode, mask, inode, FSNOTIFY_EVENT_INODE, NULL, 0);
}


static inline void fsnotify_change(struct dentry *dentry, unsigned int ia_valid)
{
	struct inode *inode = dentry->d_inode;
	__u32 mask = 0;

	if (ia_valid & ATTR_UID)
		mask |= FS_ATTRIB;
	if (ia_valid & ATTR_GID)
		mask |= FS_ATTRIB;
	if (ia_valid & ATTR_SIZE)
		mask |= FS_MODIFY;

	
	if ((ia_valid & (ATTR_ATIME | ATTR_MTIME)) == (ATTR_ATIME | ATTR_MTIME))
		mask |= FS_ATTRIB;
	else if (ia_valid & ATTR_ATIME)
		mask |= FS_ACCESS;
	else if (ia_valid & ATTR_MTIME)
		mask |= FS_MODIFY;

	if (ia_valid & ATTR_MODE)
		mask |= FS_ATTRIB;

	if (mask) {
		if (S_ISDIR(inode->i_mode))
			mask |= FS_ISDIR;

		fsnotify_parent(NULL, dentry, mask);
		fsnotify(inode, mask, inode, FSNOTIFY_EVENT_INODE, NULL, 0);
	}
}

#endif	
