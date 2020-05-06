/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _ASM_X86_CLOCKSOURCE_H
#define _ASM_X86_CLOCKSOURCE_H

#define VCLOCK_NONE	0	
#define VCLOCK_TSC	1	
#define VCLOCK_PVCLOCK	2	
#define VCLOCK_HVCLOCK	3	
#define VCLOCK_MAX	3

struct arch_clocksource_data {
	int vclock_mode;
};

#endif 
