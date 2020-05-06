/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PCI_H
#define _ASM_X86_PCI_H

#include <linux/mm.h> 
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/scatterlist.h>
#include <linux/numa.h>
#include <asm/io.h>
#include <asm/pat.h>
#include <asm/x86_init.h>

#ifdef __KERNEL__

struct pci_sysdata {
	int		domain;		
	int		node;		
#ifdef CONFIG_ACPI
	struct acpi_device *companion;	
#endif
#ifdef CONFIG_X86_64
	void		*iommu;		
#endif
#ifdef CONFIG_PCI_MSI_IRQ_DOMAIN
	void		*fwnode;	
#endif
#if IS_ENABLED(CONFIG_VMD)
	bool vmd_domain;		
#endif
};

extern int pci_routeirq;
extern int noioapicquirk;
extern int noioapicreroute;

#ifdef CONFIG_PCI

#ifdef CONFIG_PCI_DOMAINS
static inline int pci_domain_nr(struct pci_bus *bus)
{
	struct pci_sysdata *sd = bus->sysdata;

	return sd->domain;
}

static inline int pci_proc_domain(struct pci_bus *bus)
{
	return pci_domain_nr(bus);
}
#endif

#ifdef CONFIG_PCI_MSI_IRQ_DOMAIN
static inline void *_pci_root_bus_fwnode(struct pci_bus *bus)
{
	struct pci_sysdata *sd = bus->sysdata;

	return sd->fwnode;
}

#define pci_root_bus_fwnode	_pci_root_bus_fwnode
#endif

static inline bool is_vmd(struct pci_bus *bus)
{
#if IS_ENABLED(CONFIG_VMD)
	struct pci_sysdata *sd = bus->sysdata;

	return sd->vmd_domain;
#else
	return false;
#endif
}



extern unsigned int pcibios_assign_all_busses(void);
extern int pci_legacy_init(void);
#else
static inline int pcibios_assign_all_busses(void) { return 0; }
#endif

extern unsigned long pci_mem_start;
#define PCIBIOS_MIN_IO		0x1000
#define PCIBIOS_MIN_MEM		(pci_mem_start)

#define PCIBIOS_MIN_CARDBUS_IO	0x4000

extern int pcibios_enabled;
void pcibios_scan_root(int bus);

struct irq_routing_table *pcibios_get_irq_routing_table(void);
int pcibios_set_irq_routing(struct pci_dev *dev, int pin, int irq);


#define HAVE_PCI_MMAP
#define arch_can_pci_mmap_wc()	pat_enabled()
#define ARCH_GENERIC_PCI_MMAP_RESOURCE

#ifdef CONFIG_PCI
extern void early_quirks(void);
#else
static inline void early_quirks(void) { }
#endif

extern void pci_iommu_alloc(void);

#ifdef CONFIG_PCI_MSI

struct msi_desc;
int native_setup_msi_irqs(struct pci_dev *dev, int nvec, int type);
void native_teardown_msi_irq(unsigned int irq);
void native_restore_msi_irqs(struct pci_dev *dev);
#else
#define native_setup_msi_irqs		NULL
#define native_teardown_msi_irq		NULL
#endif
#endif  

#ifdef CONFIG_X86_64
#include <asm/pci_64.h>
#endif


#include <asm-generic/pci.h>

#ifdef CONFIG_NUMA

static inline int __pcibus_to_node(const struct pci_bus *bus)
{
	const struct pci_sysdata *sd = bus->sysdata;

	return sd->node;
}

static inline const struct cpumask *
cpumask_of_pcibus(const struct pci_bus *bus)
{
	int node;

	node = __pcibus_to_node(bus);
	return (node == NUMA_NO_NODE) ? cpu_online_mask :
			      cpumask_of_node(node);
}
#endif

struct pci_setup_rom {
	struct setup_data data;
	uint16_t vendor;
	uint16_t devid;
	uint64_t pcilen;
	unsigned long segment;
	unsigned long bus;
	unsigned long device;
	unsigned long function;
	uint8_t romdata[0];
};

#endif 
