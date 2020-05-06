/* SPDX-License-Identifier: GPL-2.0 */


#ifndef LINUX_ASPM_H
#define LINUX_ASPM_H

#include <linux/pci.h>

#define PCIE_LINK_STATE_L0S	1
#define PCIE_LINK_STATE_L1	2
#define PCIE_LINK_STATE_CLKPM	4

#ifdef CONFIG_PCIEASPM
int pci_disable_link_state(struct pci_dev *pdev, int state);
int pci_disable_link_state_locked(struct pci_dev *pdev, int state);
void pcie_no_aspm(void);
#else
static inline int pci_disable_link_state(struct pci_dev *pdev, int state)
{ return 0; }
static inline void pcie_no_aspm(void) { }
#endif

#endif 
