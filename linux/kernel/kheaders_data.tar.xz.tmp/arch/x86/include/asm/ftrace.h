/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_FTRACE_H
#define _ASM_X86_FTRACE_H

#ifdef CONFIG_FUNCTION_TRACER
#ifndef CC_USING_FENTRY
# error Compiler does not support fentry?
#endif
# define MCOUNT_ADDR		((unsigned long)(__fentry__))
#define MCOUNT_INSN_SIZE	5 

#ifdef CONFIG_DYNAMIC_FTRACE
#define ARCH_SUPPORTS_FTRACE_OPS 1
#endif

#define HAVE_FUNCTION_GRAPH_RET_ADDR_PTR

#ifndef __ASSEMBLY__
extern atomic_t modifying_ftrace_code;
extern void __fentry__(void);

static inline unsigned long ftrace_call_adjust(unsigned long addr)
{
	
	return addr;
}

#ifdef CONFIG_DYNAMIC_FTRACE

struct dyn_arch_ftrace {
	
};

int ftrace_int3_handler(struct pt_regs *regs);

#define FTRACE_GRAPH_TRAMP_ADDR FTRACE_GRAPH_ADDR

#endif 
#endif 
#endif 


#ifndef __ASSEMBLY__

#define ARCH_HAS_SYSCALL_MATCH_SYM_NAME
static inline bool arch_syscall_match_sym_name(const char *sym, const char *name)
{
	
	return !strcmp(sym + 3, name + 3) ||
		(!strncmp(sym, "__x64_", 6) && !strcmp(sym + 9, name + 3)) ||
		(!strncmp(sym, "__ia32_", 7) && !strcmp(sym + 10, name + 3));
}

#ifndef COMPILE_OFFSETS

#if defined(CONFIG_FTRACE_SYSCALLS) && defined(CONFIG_IA32_EMULATION)
#include <linux/compat.h>


#define ARCH_TRACE_IGNORE_COMPAT_SYSCALLS 1
static inline bool arch_trace_is_compat_syscall(struct pt_regs *regs)
{
	return in_32bit_syscall();
}
#endif 
#endif 
#endif 

#endif 
