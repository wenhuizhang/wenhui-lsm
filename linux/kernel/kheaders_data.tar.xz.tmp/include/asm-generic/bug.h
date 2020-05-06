/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_GENERIC_BUG_H
#define _ASM_GENERIC_BUG_H

#include <linux/compiler.h>

#define CUT_HERE		"------------[ cut here ]------------\n"

#ifdef CONFIG_GENERIC_BUG
#define BUGFLAG_WARNING		(1 << 0)
#define BUGFLAG_ONCE		(1 << 1)
#define BUGFLAG_DONE		(1 << 2)
#define BUGFLAG_TAINT(taint)	((taint) << 8)
#define BUG_GET_TAINT(bug)	((bug)->flags >> 8)
#endif

#ifndef __ASSEMBLY__
#include <linux/kernel.h>

#ifdef CONFIG_BUG

#ifdef CONFIG_GENERIC_BUG
struct bug_entry {
#ifndef CONFIG_GENERIC_BUG_RELATIVE_POINTERS
	unsigned long	bug_addr;
#else
	signed int	bug_addr_disp;
#endif
#ifdef CONFIG_DEBUG_BUGVERBOSE
#ifndef CONFIG_GENERIC_BUG_RELATIVE_POINTERS
	const char	*file;
#else
	signed int	file_disp;
#endif
	unsigned short	line;
#endif
	unsigned short	flags;
};
#endif	


#ifndef HAVE_ARCH_BUG
#define BUG() do { \
	printk("BUG: failure at %s:%d/%s()!\n", __FILE__, __LINE__, __func__); \
	barrier_before_unreachable(); \
	panic("BUG!"); \
} while (0)
#endif

#ifndef HAVE_ARCH_BUG_ON
#define BUG_ON(condition) do { if (unlikely(condition)) BUG(); } while (0)
#endif

#ifdef __WARN_FLAGS
#define __WARN_TAINT(taint)		__WARN_FLAGS(BUGFLAG_TAINT(taint))
#define __WARN_ONCE_TAINT(taint)	__WARN_FLAGS(BUGFLAG_ONCE|BUGFLAG_TAINT(taint))

#define WARN_ON_ONCE(condition) ({				\
	int __ret_warn_on = !!(condition);			\
	if (unlikely(__ret_warn_on))				\
		__WARN_ONCE_TAINT(TAINT_WARN);			\
	unlikely(__ret_warn_on);				\
})
#endif


#ifndef __WARN_TAINT
extern __printf(3, 4)
void warn_slowpath_fmt(const char *file, const int line,
		       const char *fmt, ...);
extern __printf(4, 5)
void warn_slowpath_fmt_taint(const char *file, const int line, unsigned taint,
			     const char *fmt, ...);
extern void warn_slowpath_null(const char *file, const int line);
#define WANT_WARN_ON_SLOWPATH
#define __WARN()		warn_slowpath_null(__FILE__, __LINE__)
#define __WARN_printf(arg...)	warn_slowpath_fmt(__FILE__, __LINE__, arg)
#define __WARN_printf_taint(taint, arg...)				\
	warn_slowpath_fmt_taint(__FILE__, __LINE__, taint, arg)
#else
extern __printf(1, 2) void __warn_printk(const char *fmt, ...);
#define __WARN() do { \
	printk(KERN_WARNING CUT_HERE); __WARN_TAINT(TAINT_WARN); \
} while (0)
#define __WARN_printf(arg...)	__WARN_printf_taint(TAINT_WARN, arg)
#define __WARN_printf_taint(taint, arg...)				\
	do { __warn_printk(arg); __WARN_TAINT(taint); } while (0)
#endif


struct warn_args;
struct pt_regs;

void __warn(const char *file, int line, void *caller, unsigned taint,
	    struct pt_regs *regs, struct warn_args *args);

#ifndef WARN_ON
#define WARN_ON(condition) ({						\
	int __ret_warn_on = !!(condition);				\
	if (unlikely(__ret_warn_on))					\
		__WARN();						\
	unlikely(__ret_warn_on);					\
})
#endif

#ifndef WARN
#define WARN(condition, format...) ({					\
	int __ret_warn_on = !!(condition);				\
	if (unlikely(__ret_warn_on))					\
		__WARN_printf(format);					\
	unlikely(__ret_warn_on);					\
})
#endif

#define WARN_TAINT(condition, taint, format...) ({			\
	int __ret_warn_on = !!(condition);				\
	if (unlikely(__ret_warn_on))					\
		__WARN_printf_taint(taint, format);			\
	unlikely(__ret_warn_on);					\
})

#ifndef WARN_ON_ONCE
#define WARN_ON_ONCE(condition)	({				\
	static bool __section(.data.once) __warned;		\
	int __ret_warn_once = !!(condition);			\
								\
	if (unlikely(__ret_warn_once && !__warned)) {		\
		__warned = true;				\
		WARN_ON(1);					\
	}							\
	unlikely(__ret_warn_once);				\
})
#endif

#define WARN_ONCE(condition, format...)	({			\
	static bool __section(.data.once) __warned;		\
	int __ret_warn_once = !!(condition);			\
								\
	if (unlikely(__ret_warn_once && !__warned)) {		\
		__warned = true;				\
		WARN(1, format);				\
	}							\
	unlikely(__ret_warn_once);				\
})

#define WARN_TAINT_ONCE(condition, taint, format...)	({	\
	static bool __section(.data.once) __warned;		\
	int __ret_warn_once = !!(condition);			\
								\
	if (unlikely(__ret_warn_once && !__warned)) {		\
		__warned = true;				\
		WARN_TAINT(1, taint, format);			\
	}							\
	unlikely(__ret_warn_once);				\
})

#else 
#ifndef HAVE_ARCH_BUG
#define BUG() do {} while (1)
#endif

#ifndef HAVE_ARCH_BUG_ON
#define BUG_ON(condition) do { if (condition) BUG(); } while (0)
#endif

#ifndef HAVE_ARCH_WARN_ON
#define WARN_ON(condition) ({						\
	int __ret_warn_on = !!(condition);				\
	unlikely(__ret_warn_on);					\
})
#endif

#ifndef WARN
#define WARN(condition, format...) ({					\
	int __ret_warn_on = !!(condition);				\
	no_printk(format);						\
	unlikely(__ret_warn_on);					\
})
#endif

#define WARN_ON_ONCE(condition) WARN_ON(condition)
#define WARN_ONCE(condition, format...) WARN(condition, format)
#define WARN_TAINT(condition, taint, format...) WARN(condition, format)
#define WARN_TAINT_ONCE(condition, taint, format...) WARN(condition, format)

#endif


#ifdef CONFIG_SMP
# define WARN_ON_SMP(x)			WARN_ON(x)
#else

# define WARN_ON_SMP(x)			({0;})
#endif

#endif 

#endif
