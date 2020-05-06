/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SPECIAL_INSNS_H
#define _ASM_X86_SPECIAL_INSNS_H


#ifdef __KERNEL__

#include <asm/nops.h>
#include <asm/processor-flags.h>
#include <linux/jump_label.h>


extern unsigned long __force_order;

void native_write_cr0(unsigned long val);

static inline unsigned long native_read_cr0(void)
{
	unsigned long val;
	asm volatile("mov %%cr0,%0\n\t" : "=r" (val), "=m" (__force_order));
	return val;
}

static inline unsigned long native_read_cr2(void)
{
	unsigned long val;
	asm volatile("mov %%cr2,%0\n\t" : "=r" (val), "=m" (__force_order));
	return val;
}

static inline void native_write_cr2(unsigned long val)
{
	asm volatile("mov %0,%%cr2": : "r" (val), "m" (__force_order));
}

static inline unsigned long __native_read_cr3(void)
{
	unsigned long val;
	asm volatile("mov %%cr3,%0\n\t" : "=r" (val), "=m" (__force_order));
	return val;
}

static inline void native_write_cr3(unsigned long val)
{
	asm volatile("mov %0,%%cr3": : "r" (val), "m" (__force_order));
}

static inline unsigned long native_read_cr4(void)
{
	unsigned long val;
#ifdef CONFIG_X86_32
	
	asm volatile("1: mov %%cr4, %0\n"
		     "2:\n"
		     _ASM_EXTABLE(1b, 2b)
		     : "=r" (val), "=m" (__force_order) : "0" (0));
#else
	
	asm volatile("mov %%cr4,%0\n\t" : "=r" (val), "=m" (__force_order));
#endif
	return val;
}

void native_write_cr4(unsigned long val);

#ifdef CONFIG_X86_64
static inline unsigned long native_read_cr8(void)
{
	unsigned long cr8;
	asm volatile("movq %%cr8,%0" : "=r" (cr8));
	return cr8;
}

static inline void native_write_cr8(unsigned long val)
{
	asm volatile("movq %0,%%cr8" :: "r" (val) : "memory");
}
#endif

#ifdef CONFIG_X86_INTEL_MEMORY_PROTECTION_KEYS
static inline u32 rdpkru(void)
{
	u32 ecx = 0;
	u32 edx, pkru;

	
	asm volatile(".byte 0x0f,0x01,0xee\n\t"
		     : "=a" (pkru), "=d" (edx)
		     : "c" (ecx));
	return pkru;
}

static inline void wrpkru(u32 pkru)
{
	u32 ecx = 0, edx = 0;

	
	asm volatile(".byte 0x0f,0x01,0xef\n\t"
		     : : "a" (pkru), "c"(ecx), "d"(edx));
}

static inline void __write_pkru(u32 pkru)
{
	
	if (pkru == rdpkru())
		return;

	wrpkru(pkru);
}

#else
static inline u32 rdpkru(void)
{
	return 0;
}

static inline void __write_pkru(u32 pkru)
{
}
#endif

static inline void native_wbinvd(void)
{
	asm volatile("wbinvd": : :"memory");
}

extern asmlinkage void native_load_gs_index(unsigned);

static inline unsigned long __read_cr4(void)
{
	return native_read_cr4();
}

#ifdef CONFIG_PARAVIRT_XXL
#include <asm/paravirt.h>
#else

static inline unsigned long read_cr0(void)
{
	return native_read_cr0();
}

static inline void write_cr0(unsigned long x)
{
	native_write_cr0(x);
}

static inline unsigned long read_cr2(void)
{
	return native_read_cr2();
}

static inline void write_cr2(unsigned long x)
{
	native_write_cr2(x);
}


static inline unsigned long __read_cr3(void)
{
	return __native_read_cr3();
}

static inline void write_cr3(unsigned long x)
{
	native_write_cr3(x);
}

static inline void __write_cr4(unsigned long x)
{
	native_write_cr4(x);
}

static inline void wbinvd(void)
{
	native_wbinvd();
}

#ifdef CONFIG_X86_64

static inline unsigned long read_cr8(void)
{
	return native_read_cr8();
}

static inline void write_cr8(unsigned long x)
{
	native_write_cr8(x);
}

static inline void load_gs_index(unsigned selector)
{
	native_load_gs_index(selector);
}

#endif

#endif 

static inline void clflush(volatile void *__p)
{
	asm volatile("clflush %0" : "+m" (*(volatile char __force *)__p));
}

static inline void clflushopt(volatile void *__p)
{
	alternative_io(".byte " __stringify(NOP_DS_PREFIX) "; clflush %P0",
		       ".byte 0x66; clflush %P0",
		       X86_FEATURE_CLFLUSHOPT,
		       "+m" (*(volatile char __force *)__p));
}

static inline void clwb(volatile void *__p)
{
	volatile struct { char x[64]; } *p = __p;

	asm volatile(ALTERNATIVE_2(
		".byte " __stringify(NOP_DS_PREFIX) "; clflush (%[pax])",
		".byte 0x66; clflush (%[pax])", 
		X86_FEATURE_CLFLUSHOPT,
		".byte 0x66, 0x0f, 0xae, 0x30",  
		X86_FEATURE_CLWB)
		: [p] "+m" (*p)
		: [pax] "a" (p));
}

#define nop() asm volatile ("nop")


#endif 

#endif 
