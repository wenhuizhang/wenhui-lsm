/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _ASM_GENERIC_PCI_H
#define _ASM_GENERIC_PCI_H

#ifndef HAVE_ARCH_PCI_GET_LEGACY_IDE_IRQ
static inline int pci_get_legacy_ide_irq(struct pci_dev *dev, int channel)
{
	return channel ? 15 : 14;
}
#endif 

#endif 
