/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __LINUX_ND_H__
#define __LINUX_ND_H__
#include <linux/fs.h>
#include <linux/ndctl.h>
#include <linux/device.h>
#include <linux/badblocks.h>

enum nvdimm_event {
	NVDIMM_REVALIDATE_POISON,
};

enum nvdimm_claim_class {
	NVDIMM_CCLASS_NONE,
	NVDIMM_CCLASS_BTT,
	NVDIMM_CCLASS_BTT2,
	NVDIMM_CCLASS_PFN,
	NVDIMM_CCLASS_DAX,
	NVDIMM_CCLASS_UNKNOWN,
};

struct nd_device_driver {
	struct device_driver drv;
	unsigned long type;
	int (*probe)(struct device *dev);
	int (*remove)(struct device *dev);
	void (*shutdown)(struct device *dev);
	void (*notify)(struct device *dev, enum nvdimm_event event);
};

static inline struct nd_device_driver *to_nd_device_driver(
		struct device_driver *drv)
{
	return container_of(drv, struct nd_device_driver, drv);
};


struct nd_namespace_common {
	int force_raw;
	struct device dev;
	struct device *claim;
	enum nvdimm_claim_class claim_class;
	int (*rw_bytes)(struct nd_namespace_common *, resource_size_t offset,
			void *buf, size_t size, int rw, unsigned long flags);
};

static inline struct nd_namespace_common *to_ndns(struct device *dev)
{
	return container_of(dev, struct nd_namespace_common, dev);
}


struct nd_namespace_io {
	struct nd_namespace_common common;
	struct resource res;
	resource_size_t size;
	void *addr;
	struct badblocks bb;
};


struct nd_namespace_pmem {
	struct nd_namespace_io nsio;
	unsigned long lbasize;
	char *alt_name;
	u8 *uuid;
	int id;
};


struct nd_namespace_blk {
	struct nd_namespace_common common;
	char *alt_name;
	u8 *uuid;
	int id;
	unsigned long lbasize;
	resource_size_t size;
	int num_resources;
	struct resource **res;
};

static inline struct nd_namespace_io *to_nd_namespace_io(const struct device *dev)
{
	return container_of(dev, struct nd_namespace_io, common.dev);
}

static inline struct nd_namespace_pmem *to_nd_namespace_pmem(const struct device *dev)
{
	struct nd_namespace_io *nsio = to_nd_namespace_io(dev);

	return container_of(nsio, struct nd_namespace_pmem, nsio);
}

static inline struct nd_namespace_blk *to_nd_namespace_blk(const struct device *dev)
{
	return container_of(dev, struct nd_namespace_blk, common.dev);
}


static inline int nvdimm_read_bytes(struct nd_namespace_common *ndns,
		resource_size_t offset, void *buf, size_t size,
		unsigned long flags)
{
	return ndns->rw_bytes(ndns, offset, buf, size, READ, flags);
}


static inline int nvdimm_write_bytes(struct nd_namespace_common *ndns,
		resource_size_t offset, void *buf, size_t size,
		unsigned long flags)
{
	return ndns->rw_bytes(ndns, offset, buf, size, WRITE, flags);
}

#define MODULE_ALIAS_ND_DEVICE(type) \
	MODULE_ALIAS("nd:t" __stringify(type) "*")
#define ND_DEVICE_MODALIAS_FMT "nd:t%d"

struct nd_region;
void nvdimm_region_notify(struct nd_region *nd_region, enum nvdimm_event event);
int __must_check __nd_driver_register(struct nd_device_driver *nd_drv,
		struct module *module, const char *mod_name);
static inline void nd_driver_unregister(struct nd_device_driver *drv)
{
	driver_unregister(&drv->drv);
}
#define nd_driver_register(driver) \
	__nd_driver_register(driver, THIS_MODULE, KBUILD_MODNAME)
#define module_nd_driver(driver) \
	module_driver(driver, nd_driver_register, nd_driver_unregister)
#endif 
