/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SIGHANDLING_H
#define _ASM_X86_SIGHANDLING_H

#include <linux/compiler.h>
#include <linux/ptrace.h>
#include <linux/signal.h>

#include <asm/processor-flags.h>

#define FIX_EFLAGS	(X86_EFLAGS_AC | X86_EFLAGS_OF | \
			 X86_EFLAGS_DF | X86_EFLAGS_TF | X86_EFLAGS_SF | \
			 X86_EFLAGS_ZF | X86_EFLAGS_AF | X86_EFLAGS_PF | \
			 X86_EFLAGS_CF | X86_EFLAGS_RF)

void signal_fault(struct pt_regs *regs, void __user *frame, char *where);
int setup_sigcontext(struct sigcontext __user *sc, void __user *fpstate,
		     struct pt_regs *regs, unsigned long mask);


#ifdef CONFIG_X86_X32_ABI
asmlinkage long sys32_x32_rt_sigreturn(void);
#endif

#endif 
