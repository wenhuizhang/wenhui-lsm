/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PURGATORY_H
#define _ASM_X86_PURGATORY_H

#ifndef __ASSEMBLY__
#include <linux/purgatory.h>

extern void purgatory(void);

extern unsigned long purgatory_backup_dest;
extern unsigned long purgatory_backup_src;
extern unsigned long purgatory_backup_sz;
#endif	

#endif 
