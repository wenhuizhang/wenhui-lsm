/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef __ASM_GENERIC_SEMBUF_H
#define __ASM_GENERIC_SEMBUF_H

#include <asm/bitsperlong.h>


struct semid64_ds {
	struct ipc64_perm sem_perm;	
#if __BITS_PER_LONG == 64
	__kernel_time_t	sem_otime;	
	__kernel_time_t	sem_ctime;	
#else
	unsigned long	sem_otime;	
	unsigned long	sem_otime_high;
	unsigned long	sem_ctime;	
	unsigned long	sem_ctime_high;
#endif
	unsigned long	sem_nsems;	
	unsigned long	__unused3;
	unsigned long	__unused4;
};

#endif 
