/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef __ASM_GENERIC_IPCBUF_H
#define __ASM_GENERIC_IPCBUF_H



struct ipc64_perm {
	__kernel_key_t		key;
	__kernel_uid32_t	uid;
	__kernel_gid32_t	gid;
	__kernel_uid32_t	cuid;
	__kernel_gid32_t	cgid;
	__kernel_mode_t		mode;
				
	unsigned char		__pad1[4 - sizeof(__kernel_mode_t)];
	unsigned short		seq;
	unsigned short		__pad2;
	__kernel_ulong_t	__unused1;
	__kernel_ulong_t	__unused2;
};

#endif 
