/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _ASM_X86_A_OUT_H
#define _ASM_X86_A_OUT_H

struct exec
{
	unsigned int a_info;	
	unsigned a_text;	
	unsigned a_data;	
	unsigned a_bss;		
	unsigned a_syms;	
	unsigned a_entry;	
	unsigned a_trsize;	
	unsigned a_drsize;	
};

#define N_TRSIZE(a)	((a).a_trsize)
#define N_DRSIZE(a)	((a).a_drsize)
#define N_SYMSIZE(a)	((a).a_syms)

#endif 
