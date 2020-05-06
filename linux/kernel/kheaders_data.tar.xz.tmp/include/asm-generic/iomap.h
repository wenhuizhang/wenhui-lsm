/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __GENERIC_IO_H
#define __GENERIC_IO_H

#include <linux/linkage.h>
#include <asm/byteorder.h>




extern unsigned int ioread8(void __iomem *);
extern unsigned int ioread16(void __iomem *);
extern unsigned int ioread16be(void __iomem *);
extern unsigned int ioread32(void __iomem *);
extern unsigned int ioread32be(void __iomem *);
#ifdef CONFIG_64BIT
extern u64 ioread64(void __iomem *);
extern u64 ioread64be(void __iomem *);
#endif

#ifdef readq
#define ioread64_lo_hi ioread64_lo_hi
#define ioread64_hi_lo ioread64_hi_lo
#define ioread64be_lo_hi ioread64be_lo_hi
#define ioread64be_hi_lo ioread64be_hi_lo
extern u64 ioread64_lo_hi(void __iomem *addr);
extern u64 ioread64_hi_lo(void __iomem *addr);
extern u64 ioread64be_lo_hi(void __iomem *addr);
extern u64 ioread64be_hi_lo(void __iomem *addr);
#endif

extern void iowrite8(u8, void __iomem *);
extern void iowrite16(u16, void __iomem *);
extern void iowrite16be(u16, void __iomem *);
extern void iowrite32(u32, void __iomem *);
extern void iowrite32be(u32, void __iomem *);
#ifdef CONFIG_64BIT
extern void iowrite64(u64, void __iomem *);
extern void iowrite64be(u64, void __iomem *);
#endif

#ifdef writeq
#define iowrite64_lo_hi iowrite64_lo_hi
#define iowrite64_hi_lo iowrite64_hi_lo
#define iowrite64be_lo_hi iowrite64be_lo_hi
#define iowrite64be_hi_lo iowrite64be_hi_lo
extern void iowrite64_lo_hi(u64 val, void __iomem *addr);
extern void iowrite64_hi_lo(u64 val, void __iomem *addr);
extern void iowrite64be_lo_hi(u64 val, void __iomem *addr);
extern void iowrite64be_hi_lo(u64 val, void __iomem *addr);
#endif


extern void ioread8_rep(void __iomem *port, void *buf, unsigned long count);
extern void ioread16_rep(void __iomem *port, void *buf, unsigned long count);
extern void ioread32_rep(void __iomem *port, void *buf, unsigned long count);

extern void iowrite8_rep(void __iomem *port, const void *buf, unsigned long count);
extern void iowrite16_rep(void __iomem *port, const void *buf, unsigned long count);
extern void iowrite32_rep(void __iomem *port, const void *buf, unsigned long count);

#ifdef CONFIG_HAS_IOPORT_MAP

extern void __iomem *ioport_map(unsigned long port, unsigned int nr);
extern void ioport_unmap(void __iomem *);
#endif

#ifndef ARCH_HAS_IOREMAP_WC
#define ioremap_wc ioremap_nocache
#endif

#ifndef ARCH_HAS_IOREMAP_WT
#define ioremap_wt ioremap_nocache
#endif

#ifdef CONFIG_PCI

struct pci_dev;
extern void pci_iounmap(struct pci_dev *dev, void __iomem *);
#elif defined(CONFIG_GENERIC_IOMAP)
struct pci_dev;
static inline void pci_iounmap(struct pci_dev *dev, void __iomem *addr)
{ }
#endif

#include <asm-generic/pci_iomap.h>

#endif
