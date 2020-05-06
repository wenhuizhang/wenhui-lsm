/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _LINUX_KALLSYMS_H
#define _LINUX_KALLSYMS_H

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/mm.h>
#include <linux/module.h>

#include <asm/sections.h>

#define KSYM_NAME_LEN 128
#define KSYM_SYMBOL_LEN (sizeof("%s+%#lx/%#lx [%s]") + (KSYM_NAME_LEN - 1) + \
			 2*(BITS_PER_LONG*3/10) + (MODULE_NAME_LEN - 1) + 1)

struct module;

static inline int is_kernel_inittext(unsigned long addr)
{
	if (addr >= (unsigned long)_sinittext
	    && addr <= (unsigned long)_einittext)
		return 1;
	return 0;
}

static inline int is_kernel_text(unsigned long addr)
{
	if ((addr >= (unsigned long)_stext && addr <= (unsigned long)_etext) ||
	    arch_is_kernel_text(addr))
		return 1;
	return in_gate_area_no_mm(addr);
}

static inline int is_kernel(unsigned long addr)
{
	if (addr >= (unsigned long)_stext && addr <= (unsigned long)_end)
		return 1;
	return in_gate_area_no_mm(addr);
}

static inline int is_ksym_addr(unsigned long addr)
{
	if (IS_ENABLED(CONFIG_KALLSYMS_ALL))
		return is_kernel(addr);

	return is_kernel_text(addr) || is_kernel_inittext(addr);
}

static inline void *dereference_symbol_descriptor(void *ptr)
{
#ifdef HAVE_DEREFERENCE_FUNCTION_DESCRIPTOR
	struct module *mod;

	ptr = dereference_kernel_function_descriptor(ptr);
	if (is_ksym_addr((unsigned long)ptr))
		return ptr;

	preempt_disable();
	mod = __module_address((unsigned long)ptr);
	preempt_enable();

	if (mod)
		ptr = dereference_module_function_descriptor(mod, ptr);
#endif
	return ptr;
}

#ifdef CONFIG_KALLSYMS

unsigned long kallsyms_lookup_name(const char *name);


int kallsyms_on_each_symbol(int (*fn)(void *, const char *, struct module *,
				      unsigned long),
			    void *data);

extern int kallsyms_lookup_size_offset(unsigned long addr,
				  unsigned long *symbolsize,
				  unsigned long *offset);


const char *kallsyms_lookup(unsigned long addr,
			    unsigned long *symbolsize,
			    unsigned long *offset,
			    char **modname, char *namebuf);


extern int sprint_symbol(char *buffer, unsigned long address);
extern int sprint_symbol_no_offset(char *buffer, unsigned long address);
extern int sprint_backtrace(char *buffer, unsigned long address);

int lookup_symbol_name(unsigned long addr, char *symname);
int lookup_symbol_attrs(unsigned long addr, unsigned long *size, unsigned long *offset, char *modname, char *name);


extern int kallsyms_show_value(void);

#else 

static inline unsigned long kallsyms_lookup_name(const char *name)
{
	return 0;
}

static inline int kallsyms_on_each_symbol(int (*fn)(void *, const char *,
						    struct module *,
						    unsigned long),
					  void *data)
{
	return 0;
}

static inline int kallsyms_lookup_size_offset(unsigned long addr,
					      unsigned long *symbolsize,
					      unsigned long *offset)
{
	return 0;
}

static inline const char *kallsyms_lookup(unsigned long addr,
					  unsigned long *symbolsize,
					  unsigned long *offset,
					  char **modname, char *namebuf)
{
	return NULL;
}

static inline int sprint_symbol(char *buffer, unsigned long addr)
{
	*buffer = '\0';
	return 0;
}

static inline int sprint_symbol_no_offset(char *buffer, unsigned long addr)
{
	*buffer = '\0';
	return 0;
}

static inline int sprint_backtrace(char *buffer, unsigned long addr)
{
	*buffer = '\0';
	return 0;
}

static inline int lookup_symbol_name(unsigned long addr, char *symname)
{
	return -ERANGE;
}

static inline int lookup_symbol_attrs(unsigned long addr, unsigned long *size, unsigned long *offset, char *modname, char *name)
{
	return -ERANGE;
}

static inline int kallsyms_show_value(void)
{
	return false;
}

#endif 

static inline void print_ip_sym(unsigned long ip)
{
	printk("[<%px>] %pS\n", (void *) ip, (void *) ip);
}

#endif 
