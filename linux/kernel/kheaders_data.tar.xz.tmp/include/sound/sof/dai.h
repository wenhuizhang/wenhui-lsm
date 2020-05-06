/* SPDX-License-Identifier: (GPL-2.0 OR BSD-3-Clause) */


#ifndef __INCLUDE_SOUND_SOF_DAI_H__
#define __INCLUDE_SOUND_SOF_DAI_H__

#include <sound/sof/header.h>
#include <sound/sof/dai-intel.h>



#define SOF_DAI_FMT_I2S		1 
#define SOF_DAI_FMT_RIGHT_J	2 
#define SOF_DAI_FMT_LEFT_J	3 
#define SOF_DAI_FMT_DSP_A	4 
#define SOF_DAI_FMT_DSP_B	5 
#define SOF_DAI_FMT_PDM		6 

#define SOF_DAI_FMT_CONT	(1 << 4) 
#define SOF_DAI_FMT_GATED	(0 << 4) 

#define SOF_DAI_FMT_NB_NF	(0 << 8) 
#define SOF_DAI_FMT_NB_IF	(2 << 8) 
#define SOF_DAI_FMT_IB_NF	(3 << 8) 
#define SOF_DAI_FMT_IB_IF	(4 << 8) 

#define SOF_DAI_FMT_CBM_CFM	(0 << 12) 
#define SOF_DAI_FMT_CBS_CFM	(2 << 12) 
#define SOF_DAI_FMT_CBM_CFS	(3 << 12) 
#define SOF_DAI_FMT_CBS_CFS	(4 << 12) 

#define SOF_DAI_FMT_FORMAT_MASK		0x000f
#define SOF_DAI_FMT_CLOCK_MASK		0x00f0
#define SOF_DAI_FMT_INV_MASK		0x0f00
#define SOF_DAI_FMT_MASTER_MASK		0xf000


enum sof_ipc_dai_type {
	SOF_DAI_INTEL_NONE = 0,		
	SOF_DAI_INTEL_SSP,		
	SOF_DAI_INTEL_DMIC,		
	SOF_DAI_INTEL_HDA,		
	SOF_DAI_INTEL_SOUNDWIRE,	
};


struct sof_ipc_dai_config {
	struct sof_ipc_cmd_hdr hdr;
	uint32_t type;		
	uint32_t dai_index;	

	
	uint16_t format;	
	uint16_t reserved16;	

	
	uint32_t reserved[8];

	
	union {
		struct sof_ipc_dai_ssp_params ssp;
		struct sof_ipc_dai_dmic_params dmic;
		struct sof_ipc_dai_hda_params hda;
	};
} __packed;

#endif
