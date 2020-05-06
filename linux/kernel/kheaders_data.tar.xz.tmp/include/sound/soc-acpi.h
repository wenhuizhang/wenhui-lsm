/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (C) 2013-15, Intel Corporation. All rights reserved.
 */

#ifndef __LINUX_SND_SOC_ACPI_H
#define __LINUX_SND_SOC_ACPI_H

#include <linux/stddef.h>
#include <linux/acpi.h>
#include <linux/mod_devicetable.h>

struct snd_soc_acpi_package_context {
	char *name;           
	int length;           
	struct acpi_buffer *format;
	struct acpi_buffer *state;
	bool data_valid;
};


#define SND_ACPI_I2C_ID_LEN (4 + ACPI_ID_LEN + 3 + 1)

#if IS_ENABLED(CONFIG_ACPI)

struct snd_soc_acpi_mach *
snd_soc_acpi_find_machine(struct snd_soc_acpi_mach *machines);

bool snd_soc_acpi_find_package_from_hid(const u8 hid[ACPI_ID_LEN],
				    struct snd_soc_acpi_package_context *ctx);


struct snd_soc_acpi_mach *snd_soc_acpi_codec_list(void *arg);

#else

static inline struct snd_soc_acpi_mach *
snd_soc_acpi_find_machine(struct snd_soc_acpi_mach *machines)
{
	return NULL;
}

static inline bool
snd_soc_acpi_find_package_from_hid(const u8 hid[ACPI_ID_LEN],
				   struct snd_soc_acpi_package_context *ctx)
{
	return false;
}


static inline struct snd_soc_acpi_mach *snd_soc_acpi_codec_list(void *arg)
{
	return NULL;
}
#endif


struct snd_soc_acpi_mach_params {
	u32 acpi_ipc_irq_index;
	const char *platform;
	u32 codec_mask;
	u32 dmic_num;
};



struct snd_soc_acpi_mach {
	const u8 id[ACPI_ID_LEN];
	const char *drv_name;
	const char *fw_filename;
	const char *board;
	struct snd_soc_acpi_mach * (*machine_quirk)(void *arg);
	const void *quirk_data;
	void *pdata;
	struct snd_soc_acpi_mach_params mach_params;
	const char *sof_fw_filename;
	const char *sof_tplg_filename;
};

#define SND_SOC_ACPI_MAX_CODECS 3


struct snd_soc_acpi_codecs {
	int num_codecs;
	u8 codecs[SND_SOC_ACPI_MAX_CODECS][ACPI_ID_LEN];
};

#endif
