/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _LINUX_POSIX_TYPES_H
#define _LINUX_POSIX_TYPES_H

#include <linux/stddef.h>




#undef __FD_SETSIZE
#define __FD_SETSIZE	1024

typedef struct {
	unsigned long fds_bits[__FD_SETSIZE / (8 * sizeof(long))];
} __kernel_fd_set;


typedef void (*__kernel_sighandler_t)(int);


typedef int __kernel_key_t;
typedef int __kernel_mqd_t;

#include <asm/posix_types.h>

#endif 
