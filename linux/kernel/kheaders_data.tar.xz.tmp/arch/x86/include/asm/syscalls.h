/* SPDX-License-Identifier: GPL-2.0-only */


#ifndef _ASM_X86_SYSCALLS_H
#define _ASM_X86_SYSCALLS_H

#include <linux/compiler.h>
#include <linux/linkage.h>
#include <linux/signal.h>
#include <linux/types.h>



long ksys_ioperm(unsigned long from, unsigned long num, int turn_on);

#ifdef CONFIG_X86_32

asmlinkage long sys_ioperm(unsigned long, unsigned long, int);
asmlinkage long sys_iopl(unsigned int);


asmlinkage long sys_modify_ldt(int, void __user *, unsigned long);


asmlinkage long sys_rt_sigreturn(void);


asmlinkage long sys_set_thread_area(struct user_desc __user *);
asmlinkage long sys_get_thread_area(struct user_desc __user *);




asmlinkage long sys_sigreturn(void);


struct vm86_struct;
asmlinkage long sys_vm86old(struct vm86_struct __user *);
asmlinkage long sys_vm86(unsigned long, unsigned long);

#endif 
#endif 
