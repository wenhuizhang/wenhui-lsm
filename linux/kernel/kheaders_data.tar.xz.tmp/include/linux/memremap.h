/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_MEMREMAP_H_
#define _LINUX_MEMREMAP_H_
#include <linux/ioport.h>
#include <linux/percpu-refcount.h>

struct resource;
struct device;


struct vmem_altmap {
	const unsigned long base_pfn;
	const unsigned long reserve;
	unsigned long free;
	unsigned long align;
	unsigned long alloc;
};


enum memory_type {
	
	MEMORY_DEVICE_PRIVATE = 1,
	MEMORY_DEVICE_FS_DAX,
	MEMORY_DEVICE_DEVDAX,
	MEMORY_DEVICE_PCI_P2PDMA,
};

struct dev_pagemap_ops {
	
	void (*page_free)(struct page *page);

	
	void (*kill)(struct dev_pagemap *pgmap);

	
	void (*cleanup)(struct dev_pagemap *pgmap);

	
	vm_fault_t (*migrate_to_ram)(struct vm_fault *vmf);
};

#define PGMAP_ALTMAP_VALID	(1 << 0)


struct dev_pagemap {
	struct vmem_altmap altmap;
	struct resource res;
	struct percpu_ref *ref;
	struct percpu_ref internal_ref;
	struct completion done;
	struct device *dev;
	enum memory_type type;
	unsigned int flags;
	u64 pci_p2pdma_bus_offset;
	const struct dev_pagemap_ops *ops;
};

static inline struct vmem_altmap *pgmap_altmap(struct dev_pagemap *pgmap)
{
	if (pgmap->flags & PGMAP_ALTMAP_VALID)
		return &pgmap->altmap;
	return NULL;
}

#ifdef CONFIG_ZONE_DEVICE
void *devm_memremap_pages(struct device *dev, struct dev_pagemap *pgmap);
void devm_memunmap_pages(struct device *dev, struct dev_pagemap *pgmap);
struct dev_pagemap *get_dev_pagemap(unsigned long pfn,
		struct dev_pagemap *pgmap);

unsigned long vmem_altmap_offset(struct vmem_altmap *altmap);
void vmem_altmap_free(struct vmem_altmap *altmap, unsigned long nr_pfns);
#else
static inline void *devm_memremap_pages(struct device *dev,
		struct dev_pagemap *pgmap)
{
	
	WARN_ON_ONCE(1);
	return ERR_PTR(-ENXIO);
}

static inline void devm_memunmap_pages(struct device *dev,
		struct dev_pagemap *pgmap)
{
}

static inline struct dev_pagemap *get_dev_pagemap(unsigned long pfn,
		struct dev_pagemap *pgmap)
{
	return NULL;
}

static inline unsigned long vmem_altmap_offset(struct vmem_altmap *altmap)
{
	return 0;
}

static inline void vmem_altmap_free(struct vmem_altmap *altmap,
		unsigned long nr_pfns)
{
}
#endif 

static inline void put_dev_pagemap(struct dev_pagemap *pgmap)
{
	if (pgmap)
		percpu_ref_put(pgmap->ref);
}
#endif 
