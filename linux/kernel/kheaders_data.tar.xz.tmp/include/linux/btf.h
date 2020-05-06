/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _LINUX_BTF_H
#define _LINUX_BTF_H 1

#include <linux/types.h>

struct btf;
struct btf_member;
struct btf_type;
union bpf_attr;

extern const struct file_operations btf_fops;

void btf_put(struct btf *btf);
int btf_new_fd(const union bpf_attr *attr);
struct btf *btf_get_by_fd(int fd);
int btf_get_info_by_fd(const struct btf *btf,
		       const union bpf_attr *attr,
		       union bpf_attr __user *uattr);

const struct btf_type *btf_type_id_size(const struct btf *btf,
					u32 *type_id,
					u32 *ret_size);
void btf_type_seq_show(const struct btf *btf, u32 type_id, void *obj,
		       struct seq_file *m);
int btf_get_fd_by_id(u32 id);
u32 btf_id(const struct btf *btf);
bool btf_member_is_reg_int(const struct btf *btf, const struct btf_type *s,
			   const struct btf_member *m,
			   u32 expected_offset, u32 expected_size);
int btf_find_spin_lock(const struct btf *btf, const struct btf_type *t);
bool btf_type_is_void(const struct btf_type *t);

#ifdef CONFIG_BPF_SYSCALL
const struct btf_type *btf_type_by_id(const struct btf *btf, u32 type_id);
const char *btf_name_by_offset(const struct btf *btf, u32 offset);
#else
static inline const struct btf_type *btf_type_by_id(const struct btf *btf,
						    u32 type_id)
{
	return NULL;
}
static inline const char *btf_name_by_offset(const struct btf *btf,
					     u32 offset)
{
	return NULL;
}
#endif

#endif
