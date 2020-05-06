/* SPDX-License-Identifier: (GPL-2.0 OR BSD-3-Clause) */


#ifndef __INCLUDE_SOUND_SOF_H
#define __INCLUDE_SOUND_SOF_H

#include <linux/pci.h>
#include <sound/soc.h>
#include <sound/soc-acpi.h>

struct snd_sof_dsp_ops;


struct snd_sof_pdata {
	const struct firmware *fw;
	const char *drv_name;
	const char *name;
	const char *platform;

	struct device *dev;

	
	void (*sof_probe_complete)(struct device *dev);

	
	const struct sof_dev_desc *desc;

	
	const char *fw_filename_prefix;
	const char *fw_filename;
	const char *tplg_filename_prefix;
	const char *tplg_filename;

	
	struct platform_device *pdev_mach;
	const struct snd_soc_acpi_mach *machine;

	void *hw_pdata;
};


struct sof_dev_desc {
	
	struct snd_soc_acpi_mach *machines;

	
	
	int resindex_lpe_base;
	int resindex_pcicfg_base;
	int resindex_imr_base;
	int irqindex_host_ipc;
	int resindex_dma_base;

	
	int dma_engine;
	int dma_size;

	
	int ipc_timeout;
	int boot_timeout;

	
	const void *chip_info;

	
	const char *nocodec_fw_filename;
	const char *nocodec_tplg_filename;

	
	const char *default_fw_path;
	const char *default_tplg_path;

	const struct snd_sof_dsp_ops *ops;
	const struct sof_arch_ops *arch_ops;
};

int sof_nocodec_setup(struct device *dev,
		      struct snd_sof_pdata *sof_pdata,
		      struct snd_soc_acpi_mach *mach,
		      const struct sof_dev_desc *desc,
		      const struct snd_sof_dsp_ops *ops);
#endif
