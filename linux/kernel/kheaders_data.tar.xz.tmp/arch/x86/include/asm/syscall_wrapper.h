/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _ASM_X86_SYSCALL_WRAPPER_H
#define _ASM_X86_SYSCALL_WRAPPER_H


#define SC_X86_64_REGS_TO_ARGS(x, ...)					\
	__MAP(x,__SC_ARGS						\
		,,regs->di,,regs->si,,regs->dx				\
		,,regs->r10,,regs->r8,,regs->r9)			\


#define SC_IA32_REGS_TO_ARGS(x, ...)					\
	__MAP(x,__SC_ARGS						\
	      ,,(unsigned int)regs->bx,,(unsigned int)regs->cx		\
	      ,,(unsigned int)regs->dx,,(unsigned int)regs->si		\
	      ,,(unsigned int)regs->di,,(unsigned int)regs->bp)

#ifdef CONFIG_IA32_EMULATION

#define __IA32_COMPAT_SYS_STUBx(x, name, ...)				\
	asmlinkage long __ia32_compat_sys##name(const struct pt_regs *regs);\
	ALLOW_ERROR_INJECTION(__ia32_compat_sys##name, ERRNO);		\
	asmlinkage long __ia32_compat_sys##name(const struct pt_regs *regs)\
	{								\
		return __se_compat_sys##name(SC_IA32_REGS_TO_ARGS(x,__VA_ARGS__));\
	}								\

#define __IA32_SYS_STUBx(x, name, ...)					\
	asmlinkage long __ia32_sys##name(const struct pt_regs *regs);	\
	ALLOW_ERROR_INJECTION(__ia32_sys##name, ERRNO);			\
	asmlinkage long __ia32_sys##name(const struct pt_regs *regs)	\
	{								\
		return __se_sys##name(SC_IA32_REGS_TO_ARGS(x,__VA_ARGS__));\
	}


#define SYSCALL_DEFINE0(sname)					\
	SYSCALL_METADATA(_##sname, 0);				\
	asmlinkage long __x64_sys_##sname(void);		\
	ALLOW_ERROR_INJECTION(__x64_sys_##sname, ERRNO);	\
	SYSCALL_ALIAS(__ia32_sys_##sname, __x64_sys_##sname);	\
	asmlinkage long __x64_sys_##sname(void)

#define COND_SYSCALL(name)						\
	cond_syscall(__x64_sys_##name);					\
	cond_syscall(__ia32_sys_##name)

#define SYS_NI(name)							\
	SYSCALL_ALIAS(__x64_sys_##name, sys_ni_posix_timers);		\
	SYSCALL_ALIAS(__ia32_sys_##name, sys_ni_posix_timers)

#else 
#define __IA32_COMPAT_SYS_STUBx(x, name, ...)
#define __IA32_SYS_STUBx(x, fullname, name, ...)
#endif 


#ifdef CONFIG_X86_X32

#define __X32_COMPAT_SYS_STUBx(x, name, ...)				\
	asmlinkage long __x32_compat_sys##name(const struct pt_regs *regs);\
	ALLOW_ERROR_INJECTION(__x32_compat_sys##name, ERRNO);		\
	asmlinkage long __x32_compat_sys##name(const struct pt_regs *regs)\
	{								\
		return __se_compat_sys##name(SC_X86_64_REGS_TO_ARGS(x,__VA_ARGS__));\
	}								\

#else 
#define __X32_COMPAT_SYS_STUBx(x, name, ...)
#endif 


#ifdef CONFIG_COMPAT

#define COMPAT_SYSCALL_DEFINEx(x, name, ...)					\
	static long __se_compat_sys##name(__MAP(x,__SC_LONG,__VA_ARGS__));	\
	static inline long __do_compat_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__));\
	__IA32_COMPAT_SYS_STUBx(x, name, __VA_ARGS__)				\
	__X32_COMPAT_SYS_STUBx(x, name, __VA_ARGS__)				\
	static long __se_compat_sys##name(__MAP(x,__SC_LONG,__VA_ARGS__))	\
	{									\
		return __do_compat_sys##name(__MAP(x,__SC_DELOUSE,__VA_ARGS__));\
	}									\
	static inline long __do_compat_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__))


#define COND_SYSCALL_COMPAT(name) 					\
	cond_syscall(__ia32_compat_sys_##name);				\
	cond_syscall(__x32_compat_sys_##name)

#define COMPAT_SYS_NI(name)						\
	SYSCALL_ALIAS(__ia32_compat_sys_##name, sys_ni_posix_timers);	\
	SYSCALL_ALIAS(__x32_compat_sys_##name, sys_ni_posix_timers)

#endif 



#define __SYSCALL_DEFINEx(x, name, ...)					\
	asmlinkage long __x64_sys##name(const struct pt_regs *regs);	\
	ALLOW_ERROR_INJECTION(__x64_sys##name, ERRNO);			\
	static long __se_sys##name(__MAP(x,__SC_LONG,__VA_ARGS__));	\
	static inline long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__));\
	asmlinkage long __x64_sys##name(const struct pt_regs *regs)	\
	{								\
		return __se_sys##name(SC_X86_64_REGS_TO_ARGS(x,__VA_ARGS__));\
	}								\
	__IA32_SYS_STUBx(x, name, __VA_ARGS__)				\
	static long __se_sys##name(__MAP(x,__SC_LONG,__VA_ARGS__))	\
	{								\
		long ret = __do_sys##name(__MAP(x,__SC_CAST,__VA_ARGS__));\
		__MAP(x,__SC_TEST,__VA_ARGS__);				\
		__PROTECT(x, ret,__MAP(x,__SC_ARGS,__VA_ARGS__));	\
		return ret;						\
	}								\
	static inline long __do_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__))


#ifndef SYSCALL_DEFINE0
#define SYSCALL_DEFINE0(sname)					\
	SYSCALL_METADATA(_##sname, 0);				\
	asmlinkage long __x64_sys_##sname(void);		\
	ALLOW_ERROR_INJECTION(__x64_sys_##sname, ERRNO);	\
	asmlinkage long __x64_sys_##sname(void)
#endif

#ifndef COND_SYSCALL
#define COND_SYSCALL(name) cond_syscall(__x64_sys_##name)
#endif

#ifndef SYS_NI
#define SYS_NI(name) SYSCALL_ALIAS(__x64_sys_##name, sys_ni_posix_timers);
#endif



struct pt_regs;
asmlinkage long __x64_sys_getcpu(const struct pt_regs *regs);
asmlinkage long __x64_sys_gettimeofday(const struct pt_regs *regs);
asmlinkage long __x64_sys_time(const struct pt_regs *regs);

#endif 
