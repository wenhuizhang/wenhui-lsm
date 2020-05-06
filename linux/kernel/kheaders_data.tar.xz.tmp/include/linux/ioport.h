/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _LINUX_IOPORT_H
#define _LINUX_IOPORT_H

#ifndef __ASSEMBLY__
#include <linux/compiler.h>
#include <linux/types.h>
#include <linux/bits.h>

struct resource {
	resource_size_t start;
	resource_size_t end;
	const char *name;
	unsigned long flags;
	unsigned long desc;
	struct resource *parent, *sibling, *child;
};


#define IORESOURCE_BITS		0x000000ff	

#define IORESOURCE_TYPE_BITS	0x00001f00	
#define IORESOURCE_IO		0x00000100	
#define IORESOURCE_MEM		0x00000200
#define IORESOURCE_REG		0x00000300	
#define IORESOURCE_IRQ		0x00000400
#define IORESOURCE_DMA		0x00000800
#define IORESOURCE_BUS		0x00001000

#define IORESOURCE_PREFETCH	0x00002000	
#define IORESOURCE_READONLY	0x00004000
#define IORESOURCE_CACHEABLE	0x00008000
#define IORESOURCE_RANGELENGTH	0x00010000
#define IORESOURCE_SHADOWABLE	0x00020000

#define IORESOURCE_SIZEALIGN	0x00040000	
#define IORESOURCE_STARTALIGN	0x00080000	

#define IORESOURCE_MEM_64	0x00100000
#define IORESOURCE_WINDOW	0x00200000	
#define IORESOURCE_MUXED	0x00400000	

#define IORESOURCE_EXT_TYPE_BITS 0x01000000	
#define IORESOURCE_SYSRAM	0x01000000	

#define IORESOURCE_EXCLUSIVE	0x08000000	

#define IORESOURCE_DISABLED	0x10000000
#define IORESOURCE_UNSET	0x20000000	
#define IORESOURCE_AUTO		0x40000000
#define IORESOURCE_BUSY		0x80000000	


#define IORESOURCE_SYSTEM_RAM		(IORESOURCE_MEM|IORESOURCE_SYSRAM)


#define IORESOURCE_IRQ_HIGHEDGE		(1<<0)
#define IORESOURCE_IRQ_LOWEDGE		(1<<1)
#define IORESOURCE_IRQ_HIGHLEVEL	(1<<2)
#define IORESOURCE_IRQ_LOWLEVEL		(1<<3)
#define IORESOURCE_IRQ_SHAREABLE	(1<<4)
#define IORESOURCE_IRQ_OPTIONAL 	(1<<5)


#define IORESOURCE_DMA_TYPE_MASK	(3<<0)
#define IORESOURCE_DMA_8BIT		(0<<0)
#define IORESOURCE_DMA_8AND16BIT	(1<<0)
#define IORESOURCE_DMA_16BIT		(2<<0)

#define IORESOURCE_DMA_MASTER		(1<<2)
#define IORESOURCE_DMA_BYTE		(1<<3)
#define IORESOURCE_DMA_WORD		(1<<4)

#define IORESOURCE_DMA_SPEED_MASK	(3<<6)
#define IORESOURCE_DMA_COMPATIBLE	(0<<6)
#define IORESOURCE_DMA_TYPEA		(1<<6)
#define IORESOURCE_DMA_TYPEB		(2<<6)
#define IORESOURCE_DMA_TYPEF		(3<<6)


#define IORESOURCE_MEM_WRITEABLE	(1<<0)	
#define IORESOURCE_MEM_CACHEABLE	(1<<1)	
#define IORESOURCE_MEM_RANGELENGTH	(1<<2)	
#define IORESOURCE_MEM_TYPE_MASK	(3<<3)
#define IORESOURCE_MEM_8BIT		(0<<3)
#define IORESOURCE_MEM_16BIT		(1<<3)
#define IORESOURCE_MEM_8AND16BIT	(2<<3)
#define IORESOURCE_MEM_32BIT		(3<<3)
#define IORESOURCE_MEM_SHADOWABLE	(1<<5)	
#define IORESOURCE_MEM_EXPANSIONROM	(1<<6)


#define IORESOURCE_IO_16BIT_ADDR	(1<<0)
#define IORESOURCE_IO_FIXED		(1<<1)
#define IORESOURCE_IO_SPARSE		(1<<2)


#define IORESOURCE_ROM_ENABLE		(1<<0)	
#define IORESOURCE_ROM_SHADOW		(1<<1)	


#define IORESOURCE_PCI_FIXED		(1<<4)	
#define IORESOURCE_PCI_EA_BEI		(1<<5)	


enum {
	IORES_DESC_NONE				= 0,
	IORES_DESC_CRASH_KERNEL			= 1,
	IORES_DESC_ACPI_TABLES			= 2,
	IORES_DESC_ACPI_NV_STORAGE		= 3,
	IORES_DESC_PERSISTENT_MEMORY		= 4,
	IORES_DESC_PERSISTENT_MEMORY_LEGACY	= 5,
	IORES_DESC_DEVICE_PRIVATE_MEMORY	= 6,
	IORES_DESC_RESERVED			= 7,
};


enum {
	IORES_MAP_SYSTEM_RAM		= BIT(0),
	IORES_MAP_ENCRYPTED		= BIT(1),
};


#define DEFINE_RES_NAMED(_start, _size, _name, _flags)			\
	{								\
		.start = (_start),					\
		.end = (_start) + (_size) - 1,				\
		.name = (_name),					\
		.flags = (_flags),					\
		.desc = IORES_DESC_NONE,				\
	}

#define DEFINE_RES_IO_NAMED(_start, _size, _name)			\
	DEFINE_RES_NAMED((_start), (_size), (_name), IORESOURCE_IO)
#define DEFINE_RES_IO(_start, _size)					\
	DEFINE_RES_IO_NAMED((_start), (_size), NULL)

#define DEFINE_RES_MEM_NAMED(_start, _size, _name)			\
	DEFINE_RES_NAMED((_start), (_size), (_name), IORESOURCE_MEM)
#define DEFINE_RES_MEM(_start, _size)					\
	DEFINE_RES_MEM_NAMED((_start), (_size), NULL)

#define DEFINE_RES_IRQ_NAMED(_irq, _name)				\
	DEFINE_RES_NAMED((_irq), 1, (_name), IORESOURCE_IRQ)
#define DEFINE_RES_IRQ(_irq)						\
	DEFINE_RES_IRQ_NAMED((_irq), NULL)

#define DEFINE_RES_DMA_NAMED(_dma, _name)				\
	DEFINE_RES_NAMED((_dma), 1, (_name), IORESOURCE_DMA)
#define DEFINE_RES_DMA(_dma)						\
	DEFINE_RES_DMA_NAMED((_dma), NULL)


extern struct resource ioport_resource;
extern struct resource iomem_resource;

extern struct resource *request_resource_conflict(struct resource *root, struct resource *new);
extern int request_resource(struct resource *root, struct resource *new);
extern int release_resource(struct resource *new);
void release_child_resources(struct resource *new);
extern void reserve_region_with_split(struct resource *root,
			     resource_size_t start, resource_size_t end,
			     const char *name);
extern struct resource *insert_resource_conflict(struct resource *parent, struct resource *new);
extern int insert_resource(struct resource *parent, struct resource *new);
extern void insert_resource_expand_to_fit(struct resource *root, struct resource *new);
extern int remove_resource(struct resource *old);
extern void arch_remove_reservations(struct resource *avail);
extern int allocate_resource(struct resource *root, struct resource *new,
			     resource_size_t size, resource_size_t min,
			     resource_size_t max, resource_size_t align,
			     resource_size_t (*alignf)(void *,
						       const struct resource *,
						       resource_size_t,
						       resource_size_t),
			     void *alignf_data);
struct resource *lookup_resource(struct resource *root, resource_size_t start);
int adjust_resource(struct resource *res, resource_size_t start,
		    resource_size_t size);
resource_size_t resource_alignment(struct resource *res);
static inline resource_size_t resource_size(const struct resource *res)
{
	return res->end - res->start + 1;
}
static inline unsigned long resource_type(const struct resource *res)
{
	return res->flags & IORESOURCE_TYPE_BITS;
}
static inline unsigned long resource_ext_type(const struct resource *res)
{
	return res->flags & IORESOURCE_EXT_TYPE_BITS;
}

static inline bool resource_contains(struct resource *r1, struct resource *r2)
{
	if (resource_type(r1) != resource_type(r2))
		return false;
	if (r1->flags & IORESOURCE_UNSET || r2->flags & IORESOURCE_UNSET)
		return false;
	return r1->start <= r2->start && r1->end >= r2->end;
}



#define request_region(start,n,name)		__request_region(&ioport_resource, (start), (n), (name), 0)
#define request_muxed_region(start,n,name)	__request_region(&ioport_resource, (start), (n), (name), IORESOURCE_MUXED)
#define __request_mem_region(start,n,name, excl) __request_region(&iomem_resource, (start), (n), (name), excl)
#define request_mem_region(start,n,name) __request_region(&iomem_resource, (start), (n), (name), 0)
#define request_mem_region_exclusive(start,n,name) \
	__request_region(&iomem_resource, (start), (n), (name), IORESOURCE_EXCLUSIVE)
#define rename_region(region, newname) do { (region)->name = (newname); } while (0)

extern struct resource * __request_region(struct resource *,
					resource_size_t start,
					resource_size_t n,
					const char *name, int flags);


#define release_region(start,n)	__release_region(&ioport_resource, (start), (n))
#define release_mem_region(start,n)	__release_region(&iomem_resource, (start), (n))

extern void __release_region(struct resource *, resource_size_t,
				resource_size_t);
#ifdef CONFIG_MEMORY_HOTREMOVE
extern int release_mem_region_adjustable(struct resource *, resource_size_t,
				resource_size_t);
#endif


struct device;

extern int devm_request_resource(struct device *dev, struct resource *root,
				 struct resource *new);
extern void devm_release_resource(struct device *dev, struct resource *new);

#define devm_request_region(dev,start,n,name) \
	__devm_request_region(dev, &ioport_resource, (start), (n), (name))
#define devm_request_mem_region(dev,start,n,name) \
	__devm_request_region(dev, &iomem_resource, (start), (n), (name))

extern struct resource * __devm_request_region(struct device *dev,
				struct resource *parent, resource_size_t start,
				resource_size_t n, const char *name);

#define devm_release_region(dev, start, n) \
	__devm_release_region(dev, &ioport_resource, (start), (n))
#define devm_release_mem_region(dev, start, n) \
	__devm_release_region(dev, &iomem_resource, (start), (n))

extern void __devm_release_region(struct device *dev, struct resource *parent,
				  resource_size_t start, resource_size_t n);
extern int iomem_map_sanity_check(resource_size_t addr, unsigned long size);
extern bool iomem_is_exclusive(u64 addr);

extern int
walk_system_ram_range(unsigned long start_pfn, unsigned long nr_pages,
		void *arg, int (*func)(unsigned long, unsigned long, void *));
extern int
walk_mem_res(u64 start, u64 end, void *arg,
	     int (*func)(struct resource *, void *));
extern int
walk_system_ram_res(u64 start, u64 end, void *arg,
		    int (*func)(struct resource *, void *));
extern int
walk_iomem_res_desc(unsigned long desc, unsigned long flags, u64 start, u64 end,
		    void *arg, int (*func)(struct resource *, void *));


static inline bool resource_overlaps(struct resource *r1, struct resource *r2)
{
       return (r1->start <= r2->end && r1->end >= r2->start);
}

struct resource *devm_request_free_mem_region(struct device *dev,
		struct resource *base, unsigned long size);

#endif 
#endif	
