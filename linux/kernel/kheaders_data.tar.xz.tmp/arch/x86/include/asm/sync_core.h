/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SYNC_CORE_H
#define _ASM_X86_SYNC_CORE_H

#include <linux/preempt.h>
#include <asm/processor.h>
#include <asm/cpufeature.h>


static inline void sync_core_before_usermode(void)
{
	
	if (static_cpu_has(X86_FEATURE_PTI))
		return;
	
	if (in_irq() || in_nmi())
		return;
	sync_core();
}

#endif 
