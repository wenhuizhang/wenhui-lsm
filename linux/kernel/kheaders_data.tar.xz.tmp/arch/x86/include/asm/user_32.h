/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_USER_32_H
#define _ASM_X86_USER_32_H

#include <asm/page.h>




struct user_i387_struct {
	long	cwd;
	long	swd;
	long	twd;
	long	fip;
	long	fcs;
	long	foo;
	long	fos;
	long	st_space[20];	
};

struct user_fxsr_struct {
	unsigned short	cwd;
	unsigned short	swd;
	unsigned short	twd;
	unsigned short	fop;
	long	fip;
	long	fcs;
	long	foo;
	long	fos;
	long	mxcsr;
	long	reserved;
	long	st_space[32];	
	long	xmm_space[32];	
	long	padding[56];
};


struct user_regs_struct {
	unsigned long	bx;
	unsigned long	cx;
	unsigned long	dx;
	unsigned long	si;
	unsigned long	di;
	unsigned long	bp;
	unsigned long	ax;
	unsigned long	ds;
	unsigned long	es;
	unsigned long	fs;
	unsigned long	gs;
	unsigned long	orig_ax;
	unsigned long	ip;
	unsigned long	cs;
	unsigned long	flags;
	unsigned long	sp;
	unsigned long	ss;
};


struct user{

  struct user_regs_struct regs;	

  int u_fpvalid;		
				
  struct user_i387_struct i387;	

  unsigned long int u_tsize;	
  unsigned long int u_dsize;	
  unsigned long int u_ssize;	
  unsigned long start_code;     
  unsigned long start_stack;	
  long int signal;     		
  int reserved;			
  unsigned long u_ar0;		
				
  struct user_i387_struct *u_fpstate;	
  unsigned long magic;		
  char u_comm[32];		
  int u_debugreg[8];
};
#define NBPG PAGE_SIZE
#define UPAGES 1
#define HOST_TEXT_START_ADDR (u.start_code)
#define HOST_STACK_END_ADDR (u.start_stack + u.u_ssize * NBPG)

#endif 
