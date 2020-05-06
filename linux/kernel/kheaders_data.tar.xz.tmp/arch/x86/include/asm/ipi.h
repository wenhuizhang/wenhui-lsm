/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef _ASM_X86_IPI_H
#define _ASM_X86_IPI_H

#ifdef CONFIG_X86_LOCAL_APIC



#include <asm/hw_irq.h>
#include <asm/apic.h>
#include <asm/smp.h>



static inline unsigned int __prepare_ICR(unsigned int shortcut, int vector,
					 unsigned int dest)
{
	unsigned int icr = shortcut | dest;

	switch (vector) {
	default:
		icr |= APIC_DM_FIXED | vector;
		break;
	case NMI_VECTOR:
		icr |= APIC_DM_NMI;
		break;
	}
	return icr;
}

static inline int __prepare_ICR2(unsigned int mask)
{
	return SET_APIC_DEST_FIELD(mask);
}

static inline void __xapic_wait_icr_idle(void)
{
	while (native_apic_mem_read(APIC_ICR) & APIC_ICR_BUSY)
		cpu_relax();
}

void __default_send_IPI_shortcut(unsigned int shortcut, int vector, unsigned int dest);


void __default_send_IPI_dest_field(unsigned int mask, int vector, unsigned int dest);

extern void default_send_IPI_single(int cpu, int vector);
extern void default_send_IPI_single_phys(int cpu, int vector);
extern void default_send_IPI_mask_sequence_phys(const struct cpumask *mask,
						 int vector);
extern void default_send_IPI_mask_allbutself_phys(const struct cpumask *mask,
							 int vector);


#define NMI_VECTOR 0x02

extern int no_broadcast;

static inline void __default_local_send_IPI_allbutself(int vector)
{
	if (no_broadcast || vector == NMI_VECTOR)
		apic->send_IPI_mask_allbutself(cpu_online_mask, vector);
	else
		__default_send_IPI_shortcut(APIC_DEST_ALLBUT, vector, apic->dest_logical);
}

static inline void __default_local_send_IPI_all(int vector)
{
	if (no_broadcast || vector == NMI_VECTOR)
		apic->send_IPI_mask(cpu_online_mask, vector);
	else
		__default_send_IPI_shortcut(APIC_DEST_ALLINC, vector, apic->dest_logical);
}

#ifdef CONFIG_X86_32
extern void default_send_IPI_mask_sequence_logical(const struct cpumask *mask,
							 int vector);
extern void default_send_IPI_mask_allbutself_logical(const struct cpumask *mask,
							 int vector);
extern void default_send_IPI_mask_logical(const struct cpumask *mask,
						 int vector);
extern void default_send_IPI_allbutself(int vector);
extern void default_send_IPI_all(int vector);
extern void default_send_IPI_self(int vector);
#endif

#endif

#endif 
