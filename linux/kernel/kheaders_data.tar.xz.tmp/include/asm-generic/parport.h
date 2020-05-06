/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_GENERIC_PARPORT_H
#define __ASM_GENERIC_PARPORT_H



static int parport_pc_find_isa_ports(int autoirq, int autodma);
static int parport_pc_find_nonpci_ports(int autoirq, int autodma)
{
#ifdef CONFIG_ISA
	return parport_pc_find_isa_ports(autoirq, autodma);
#else
	return 0;
#endif
}

#endif 
