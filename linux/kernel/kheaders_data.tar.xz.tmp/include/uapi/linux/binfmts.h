/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LINUX_BINFMTS_H
#define _UAPI_LINUX_BINFMTS_H

#include <linux/capability.h>

struct pt_regs;


#define MAX_ARG_STRLEN (PAGE_SIZE * 32)
#define MAX_ARG_STRINGS 0x7FFFFFFF


#define BINPRM_BUF_SIZE 256

#endif 
