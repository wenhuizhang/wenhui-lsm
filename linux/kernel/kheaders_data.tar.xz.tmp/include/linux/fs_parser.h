/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef _LINUX_FS_PARSER_H
#define _LINUX_FS_PARSER_H

#include <linux/fs_context.h>

struct path;

struct constant_table {
	const char	*name;
	int		value;
};


enum fs_parameter_type {
	__fs_param_wasnt_defined,
	fs_param_is_flag,
	fs_param_is_bool,
	fs_param_is_u32,
	fs_param_is_u32_octal,
	fs_param_is_u32_hex,
	fs_param_is_s32,
	fs_param_is_u64,
	fs_param_is_enum,
	fs_param_is_string,
	fs_param_is_blob,
	fs_param_is_blockdev,
	fs_param_is_path,
	fs_param_is_fd,
	nr__fs_parameter_type,
};


struct fs_parameter_spec {
	const char		*name;
	u8			opt;	
	enum fs_parameter_type	type:8;	
	unsigned short		flags;
#define fs_param_v_optional	0x0001	
#define fs_param_neg_with_no	0x0002	
#define fs_param_neg_with_empty	0x0004	
#define fs_param_deprecated	0x0008	
};

struct fs_parameter_enum {
	u8		opt;		
	char		name[14];
	u8		value;
};

struct fs_parameter_description {
	const char	name[16];		
	const struct fs_parameter_spec *specs;	
	const struct fs_parameter_enum *enums;	
};


struct fs_parse_result {
	bool			negated;	
	bool			has_value;	
	union {
		bool		boolean;	
		int		int_32;		
		unsigned int	uint_32;	
		u64		uint_64;	
	};
};

extern int fs_parse(struct fs_context *fc,
		    const struct fs_parameter_description *desc,
		    struct fs_parameter *value,
		    struct fs_parse_result *result);
extern int fs_lookup_param(struct fs_context *fc,
			   struct fs_parameter *param,
			   bool want_bdev,
			   struct path *_path);

extern int __lookup_constant(const struct constant_table tbl[], size_t tbl_size,
			     const char *name, int not_found);
#define lookup_constant(t, n, nf) __lookup_constant(t, ARRAY_SIZE(t), (n), (nf))

#ifdef CONFIG_VALIDATE_FS_PARSER
extern bool validate_constant_table(const struct constant_table *tbl, size_t tbl_size,
				    int low, int high, int special);
extern bool fs_validate_description(const struct fs_parameter_description *desc);
#else
static inline bool validate_constant_table(const struct constant_table *tbl, size_t tbl_size,
					   int low, int high, int special)
{ return true; }
static inline bool fs_validate_description(const struct fs_parameter_description *desc)
{ return true; }
#endif


#define __fsparam(TYPE, NAME, OPT, FLAGS) \
	{ \
		.name = NAME, \
		.opt = OPT, \
		.type = TYPE, \
		.flags = FLAGS \
	}

#define fsparam_flag(NAME, OPT)	__fsparam(fs_param_is_flag, NAME, OPT, 0)
#define fsparam_flag_no(NAME, OPT) \
				__fsparam(fs_param_is_flag, NAME, OPT, \
					    fs_param_neg_with_no)
#define fsparam_bool(NAME, OPT)	__fsparam(fs_param_is_bool, NAME, OPT, 0)
#define fsparam_u32(NAME, OPT)	__fsparam(fs_param_is_u32, NAME, OPT, 0)
#define fsparam_u32oct(NAME, OPT) \
				__fsparam(fs_param_is_u32_octal, NAME, OPT, 0)
#define fsparam_u32hex(NAME, OPT) \
				__fsparam(fs_param_is_u32_hex, NAME, OPT, 0)
#define fsparam_s32(NAME, OPT)	__fsparam(fs_param_is_s32, NAME, OPT, 0)
#define fsparam_u64(NAME, OPT)	__fsparam(fs_param_is_u64, NAME, OPT, 0)
#define fsparam_enum(NAME, OPT)	__fsparam(fs_param_is_enum, NAME, OPT, 0)
#define fsparam_string(NAME, OPT) \
				__fsparam(fs_param_is_string, NAME, OPT, 0)
#define fsparam_blob(NAME, OPT)	__fsparam(fs_param_is_blob, NAME, OPT, 0)
#define fsparam_bdev(NAME, OPT)	__fsparam(fs_param_is_blockdev, NAME, OPT, 0)
#define fsparam_path(NAME, OPT)	__fsparam(fs_param_is_path, NAME, OPT, 0)
#define fsparam_fd(NAME, OPT)	__fsparam(fs_param_is_fd, NAME, OPT, 0)


#endif 
