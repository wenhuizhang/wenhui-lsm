/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _SYSFS_H_
#define _SYSFS_H_

#include <linux/kernfs.h>
#include <linux/compiler.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/lockdep.h>
#include <linux/kobject_ns.h>
#include <linux/stat.h>
#include <linux/atomic.h>

struct kobject;
struct module;
struct bin_attribute;
enum kobj_ns_type;

struct attribute {
	const char		*name;
	umode_t			mode;
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	bool			ignore_lockdep:1;
	struct lock_class_key	*key;
	struct lock_class_key	skey;
#endif
};


#ifdef CONFIG_DEBUG_LOCK_ALLOC
#define sysfs_attr_init(attr)				\
do {							\
	static struct lock_class_key __key;		\
							\
	(attr)->key = &__key;				\
} while (0)
#else
#define sysfs_attr_init(attr) do {} while (0)
#endif


struct attribute_group {
	const char		*name;
	umode_t			(*is_visible)(struct kobject *,
					      struct attribute *, int);
	umode_t			(*is_bin_visible)(struct kobject *,
						  struct bin_attribute *, int);
	struct attribute	**attrs;
	struct bin_attribute	**bin_attrs;
};



#define SYSFS_PREALLOC 010000

#define __ATTR(_name, _mode, _show, _store) {				\
	.attr = {.name = __stringify(_name),				\
		 .mode = VERIFY_OCTAL_PERMISSIONS(_mode) },		\
	.show	= _show,						\
	.store	= _store,						\
}

#define __ATTR_PREALLOC(_name, _mode, _show, _store) {			\
	.attr = {.name = __stringify(_name),				\
		 .mode = SYSFS_PREALLOC | VERIFY_OCTAL_PERMISSIONS(_mode) },\
	.show	= _show,						\
	.store	= _store,						\
}

#define __ATTR_RO(_name) {						\
	.attr	= { .name = __stringify(_name), .mode = 0444 },		\
	.show	= _name##_show,						\
}

#define __ATTR_RO_MODE(_name, _mode) {					\
	.attr	= { .name = __stringify(_name),				\
		    .mode = VERIFY_OCTAL_PERMISSIONS(_mode) },		\
	.show	= _name##_show,						\
}

#define __ATTR_WO(_name) {						\
	.attr	= { .name = __stringify(_name), .mode = 0200 },		\
	.store	= _name##_store,					\
}

#define __ATTR_RW(_name) __ATTR(_name, 0644, _name##_show, _name##_store)

#define __ATTR_NULL { .attr = { .name = NULL } }

#ifdef CONFIG_DEBUG_LOCK_ALLOC
#define __ATTR_IGNORE_LOCKDEP(_name, _mode, _show, _store) {	\
	.attr = {.name = __stringify(_name), .mode = _mode,	\
			.ignore_lockdep = true },		\
	.show		= _show,				\
	.store		= _store,				\
}
#else
#define __ATTR_IGNORE_LOCKDEP	__ATTR
#endif

#define __ATTRIBUTE_GROUPS(_name)				\
static const struct attribute_group *_name##_groups[] = {	\
	&_name##_group,						\
	NULL,							\
}

#define ATTRIBUTE_GROUPS(_name)					\
static const struct attribute_group _name##_group = {		\
	.attrs = _name##_attrs,					\
};								\
__ATTRIBUTE_GROUPS(_name)

struct file;
struct vm_area_struct;

struct bin_attribute {
	struct attribute	attr;
	size_t			size;
	void			*private;
	ssize_t (*read)(struct file *, struct kobject *, struct bin_attribute *,
			char *, loff_t, size_t);
	ssize_t (*write)(struct file *, struct kobject *, struct bin_attribute *,
			 char *, loff_t, size_t);
	int (*mmap)(struct file *, struct kobject *, struct bin_attribute *attr,
		    struct vm_area_struct *vma);
};


#define sysfs_bin_attr_init(bin_attr) sysfs_attr_init(&(bin_attr)->attr)


#define __BIN_ATTR(_name, _mode, _read, _write, _size) {		\
	.attr = { .name = __stringify(_name), .mode = _mode },		\
	.read	= _read,						\
	.write	= _write,						\
	.size	= _size,						\
}

#define __BIN_ATTR_RO(_name, _size) {					\
	.attr	= { .name = __stringify(_name), .mode = 0444 },		\
	.read	= _name##_read,						\
	.size	= _size,						\
}

#define __BIN_ATTR_RW(_name, _size)					\
	__BIN_ATTR(_name, 0644, _name##_read, _name##_write, _size)

#define __BIN_ATTR_NULL __ATTR_NULL

#define BIN_ATTR(_name, _mode, _read, _write, _size)			\
struct bin_attribute bin_attr_##_name = __BIN_ATTR(_name, _mode, _read,	\
					_write, _size)

#define BIN_ATTR_RO(_name, _size)					\
struct bin_attribute bin_attr_##_name = __BIN_ATTR_RO(_name, _size)

#define BIN_ATTR_RW(_name, _size)					\
struct bin_attribute bin_attr_##_name = __BIN_ATTR_RW(_name, _size)

struct sysfs_ops {
	ssize_t	(*show)(struct kobject *, struct attribute *, char *);
	ssize_t	(*store)(struct kobject *, struct attribute *, const char *, size_t);
};

#ifdef CONFIG_SYSFS

int __must_check sysfs_create_dir_ns(struct kobject *kobj, const void *ns);
void sysfs_remove_dir(struct kobject *kobj);
int __must_check sysfs_rename_dir_ns(struct kobject *kobj, const char *new_name,
				     const void *new_ns);
int __must_check sysfs_move_dir_ns(struct kobject *kobj,
				   struct kobject *new_parent_kobj,
				   const void *new_ns);
int __must_check sysfs_create_mount_point(struct kobject *parent_kobj,
					  const char *name);
void sysfs_remove_mount_point(struct kobject *parent_kobj,
			      const char *name);

int __must_check sysfs_create_file_ns(struct kobject *kobj,
				      const struct attribute *attr,
				      const void *ns);
int __must_check sysfs_create_files(struct kobject *kobj,
				   const struct attribute * const *attr);
int __must_check sysfs_chmod_file(struct kobject *kobj,
				  const struct attribute *attr, umode_t mode);
struct kernfs_node *sysfs_break_active_protection(struct kobject *kobj,
						  const struct attribute *attr);
void sysfs_unbreak_active_protection(struct kernfs_node *kn);
void sysfs_remove_file_ns(struct kobject *kobj, const struct attribute *attr,
			  const void *ns);
bool sysfs_remove_file_self(struct kobject *kobj, const struct attribute *attr);
void sysfs_remove_files(struct kobject *kobj, const struct attribute * const *attr);

int __must_check sysfs_create_bin_file(struct kobject *kobj,
				       const struct bin_attribute *attr);
void sysfs_remove_bin_file(struct kobject *kobj,
			   const struct bin_attribute *attr);

int __must_check sysfs_create_link(struct kobject *kobj, struct kobject *target,
				   const char *name);
int __must_check sysfs_create_link_nowarn(struct kobject *kobj,
					  struct kobject *target,
					  const char *name);
void sysfs_remove_link(struct kobject *kobj, const char *name);

int sysfs_rename_link_ns(struct kobject *kobj, struct kobject *target,
			 const char *old_name, const char *new_name,
			 const void *new_ns);

void sysfs_delete_link(struct kobject *dir, struct kobject *targ,
			const char *name);

int __must_check sysfs_create_group(struct kobject *kobj,
				    const struct attribute_group *grp);
int __must_check sysfs_create_groups(struct kobject *kobj,
				     const struct attribute_group **groups);
int __must_check sysfs_update_groups(struct kobject *kobj,
				     const struct attribute_group **groups);
int sysfs_update_group(struct kobject *kobj,
		       const struct attribute_group *grp);
void sysfs_remove_group(struct kobject *kobj,
			const struct attribute_group *grp);
void sysfs_remove_groups(struct kobject *kobj,
			 const struct attribute_group **groups);
int sysfs_add_file_to_group(struct kobject *kobj,
			const struct attribute *attr, const char *group);
void sysfs_remove_file_from_group(struct kobject *kobj,
			const struct attribute *attr, const char *group);
int sysfs_merge_group(struct kobject *kobj,
		       const struct attribute_group *grp);
void sysfs_unmerge_group(struct kobject *kobj,
		       const struct attribute_group *grp);
int sysfs_add_link_to_group(struct kobject *kobj, const char *group_name,
			    struct kobject *target, const char *link_name);
void sysfs_remove_link_from_group(struct kobject *kobj, const char *group_name,
				  const char *link_name);
int __compat_only_sysfs_link_entry_to_kobj(struct kobject *kobj,
				      struct kobject *target_kobj,
				      const char *target_name);

void sysfs_notify(struct kobject *kobj, const char *dir, const char *attr);

int __must_check sysfs_init(void);

static inline void sysfs_enable_ns(struct kernfs_node *kn)
{
	return kernfs_enable_ns(kn);
}

#else 

static inline int sysfs_create_dir_ns(struct kobject *kobj, const void *ns)
{
	return 0;
}

static inline void sysfs_remove_dir(struct kobject *kobj)
{
}

static inline int sysfs_rename_dir_ns(struct kobject *kobj,
				      const char *new_name, const void *new_ns)
{
	return 0;
}

static inline int sysfs_move_dir_ns(struct kobject *kobj,
				    struct kobject *new_parent_kobj,
				    const void *new_ns)
{
	return 0;
}

static inline int sysfs_create_mount_point(struct kobject *parent_kobj,
					   const char *name)
{
	return 0;
}

static inline void sysfs_remove_mount_point(struct kobject *parent_kobj,
					    const char *name)
{
}

static inline int sysfs_create_file_ns(struct kobject *kobj,
				       const struct attribute *attr,
				       const void *ns)
{
	return 0;
}

static inline int sysfs_create_files(struct kobject *kobj,
				    const struct attribute * const *attr)
{
	return 0;
}

static inline int sysfs_chmod_file(struct kobject *kobj,
				   const struct attribute *attr, umode_t mode)
{
	return 0;
}

static inline struct kernfs_node *
sysfs_break_active_protection(struct kobject *kobj,
			      const struct attribute *attr)
{
	return NULL;
}

static inline void sysfs_unbreak_active_protection(struct kernfs_node *kn)
{
}

static inline void sysfs_remove_file_ns(struct kobject *kobj,
					const struct attribute *attr,
					const void *ns)
{
}

static inline bool sysfs_remove_file_self(struct kobject *kobj,
					  const struct attribute *attr)
{
	return false;
}

static inline void sysfs_remove_files(struct kobject *kobj,
				     const struct attribute * const *attr)
{
}

static inline int sysfs_create_bin_file(struct kobject *kobj,
					const struct bin_attribute *attr)
{
	return 0;
}

static inline void sysfs_remove_bin_file(struct kobject *kobj,
					 const struct bin_attribute *attr)
{
}

static inline int sysfs_create_link(struct kobject *kobj,
				    struct kobject *target, const char *name)
{
	return 0;
}

static inline int sysfs_create_link_nowarn(struct kobject *kobj,
					   struct kobject *target,
					   const char *name)
{
	return 0;
}

static inline void sysfs_remove_link(struct kobject *kobj, const char *name)
{
}

static inline int sysfs_rename_link_ns(struct kobject *k, struct kobject *t,
				       const char *old_name,
				       const char *new_name, const void *ns)
{
	return 0;
}

static inline void sysfs_delete_link(struct kobject *k, struct kobject *t,
				     const char *name)
{
}

static inline int sysfs_create_group(struct kobject *kobj,
				     const struct attribute_group *grp)
{
	return 0;
}

static inline int sysfs_create_groups(struct kobject *kobj,
				      const struct attribute_group **groups)
{
	return 0;
}

static inline int sysfs_update_groups(struct kobject *kobj,
				      const struct attribute_group **groups)
{
	return 0;
}

static inline int sysfs_update_group(struct kobject *kobj,
				const struct attribute_group *grp)
{
	return 0;
}

static inline void sysfs_remove_group(struct kobject *kobj,
				      const struct attribute_group *grp)
{
}

static inline void sysfs_remove_groups(struct kobject *kobj,
				       const struct attribute_group **groups)
{
}

static inline int sysfs_add_file_to_group(struct kobject *kobj,
		const struct attribute *attr, const char *group)
{
	return 0;
}

static inline void sysfs_remove_file_from_group(struct kobject *kobj,
		const struct attribute *attr, const char *group)
{
}

static inline int sysfs_merge_group(struct kobject *kobj,
		       const struct attribute_group *grp)
{
	return 0;
}

static inline void sysfs_unmerge_group(struct kobject *kobj,
		       const struct attribute_group *grp)
{
}

static inline int sysfs_add_link_to_group(struct kobject *kobj,
		const char *group_name, struct kobject *target,
		const char *link_name)
{
	return 0;
}

static inline void sysfs_remove_link_from_group(struct kobject *kobj,
		const char *group_name, const char *link_name)
{
}

static inline int __compat_only_sysfs_link_entry_to_kobj(
	struct kobject *kobj,
	struct kobject *target_kobj,
	const char *target_name)
{
	return 0;
}

static inline void sysfs_notify(struct kobject *kobj, const char *dir,
				const char *attr)
{
}

static inline int __must_check sysfs_init(void)
{
	return 0;
}

static inline void sysfs_enable_ns(struct kernfs_node *kn)
{
}

#endif 

static inline int __must_check sysfs_create_file(struct kobject *kobj,
						 const struct attribute *attr)
{
	return sysfs_create_file_ns(kobj, attr, NULL);
}

static inline void sysfs_remove_file(struct kobject *kobj,
				     const struct attribute *attr)
{
	sysfs_remove_file_ns(kobj, attr, NULL);
}

static inline int sysfs_rename_link(struct kobject *kobj, struct kobject *target,
				    const char *old_name, const char *new_name)
{
	return sysfs_rename_link_ns(kobj, target, old_name, new_name, NULL);
}

static inline void sysfs_notify_dirent(struct kernfs_node *kn)
{
	kernfs_notify(kn);
}

static inline struct kernfs_node *sysfs_get_dirent(struct kernfs_node *parent,
						   const char *name)
{
	return kernfs_find_and_get(parent, name);
}

static inline struct kernfs_node *sysfs_get(struct kernfs_node *kn)
{
	kernfs_get(kn);
	return kn;
}

static inline void sysfs_put(struct kernfs_node *kn)
{
	kernfs_put(kn);
}

#endif 
