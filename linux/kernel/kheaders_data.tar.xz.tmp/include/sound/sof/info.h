/* SPDX-License-Identifier: (GPL-2.0 OR BSD-3-Clause) */


#ifndef __INCLUDE_SOUND_SOF_INFO_H__
#define __INCLUDE_SOUND_SOF_INFO_H__

#include <sound/sof/header.h>
#include <sound/sof/stream.h>



#define SOF_IPC_MAX_ELEMS	16


#define SOF_IPC_INFO_BUILD		BIT(0)
#define SOF_IPC_INFO_LOCKS		BIT(1)
#define SOF_IPC_INFO_LOCKSV		BIT(2)
#define SOF_IPC_INFO_GDB		BIT(3)


enum sof_ipc_ext_data {
	SOF_IPC_EXT_DMA_BUFFER = 0,
	SOF_IPC_EXT_WINDOW,
};


struct sof_ipc_fw_version {
	struct sof_ipc_hdr hdr;
	uint16_t major;
	uint16_t minor;
	uint16_t micro;
	uint16_t build;
	uint8_t date[12];
	uint8_t time[10];
	uint8_t tag[6];
	uint32_t abi_version;

	
	uint32_t reserved[4];
} __packed;


struct sof_ipc_fw_ready {
	struct sof_ipc_cmd_hdr hdr;
	uint32_t dspbox_offset;	 
	uint32_t hostbox_offset; 
	uint32_t dspbox_size;
	uint32_t hostbox_size;
	struct sof_ipc_fw_version version;

	
	uint64_t flags;

	
	uint32_t reserved[4];
} __packed;


enum sof_ipc_region {
	SOF_IPC_REGION_DOWNBOX	= 0,
	SOF_IPC_REGION_UPBOX,
	SOF_IPC_REGION_TRACE,
	SOF_IPC_REGION_DEBUG,
	SOF_IPC_REGION_STREAM,
	SOF_IPC_REGION_REGS,
	SOF_IPC_REGION_EXCEPTION,
};

struct sof_ipc_ext_data_hdr {
	struct sof_ipc_cmd_hdr hdr;
	uint32_t type;		
} __packed;

struct sof_ipc_dma_buffer_elem {
	struct sof_ipc_hdr hdr;
	uint32_t type;		
	uint32_t id;		
	struct sof_ipc_host_buffer buffer;
} __packed;


struct sof_ipc_dma_buffer_data {
	struct sof_ipc_ext_data_hdr ext_hdr;
	uint32_t num_buffers;

	
	struct sof_ipc_dma_buffer_elem buffer[];
}  __packed;

struct sof_ipc_window_elem {
	struct sof_ipc_hdr hdr;
	uint32_t type;		
	uint32_t id;		
	uint32_t flags;		
	uint32_t size;		
	
	uint32_t offset;
} __packed;


struct sof_ipc_window {
	struct sof_ipc_ext_data_hdr ext_hdr;
	uint32_t num_windows;
	struct sof_ipc_window_elem window[];
}  __packed;

#endif
