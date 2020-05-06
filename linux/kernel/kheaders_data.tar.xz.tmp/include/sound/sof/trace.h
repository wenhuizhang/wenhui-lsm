/* SPDX-License-Identifier: (GPL-2.0 OR BSD-3-Clause) */


#ifndef __INCLUDE_SOUND_SOF_TRACE_H__
#define __INCLUDE_SOUND_SOF_TRACE_H__

#include <sound/sof/header.h>
#include <sound/sof/stream.h>



#define SOF_TRACE_FILENAME_SIZE		32



struct sof_ipc_dma_trace_params {
	struct sof_ipc_cmd_hdr hdr;
	struct sof_ipc_host_buffer buffer;
	uint32_t stream_tag;
}  __packed;


struct sof_ipc_dma_trace_params_ext {
	struct sof_ipc_cmd_hdr hdr;
	struct sof_ipc_host_buffer buffer;
	uint32_t stream_tag;
	uint64_t timestamp_ns; 
	uint32_t reserved[8];
}  __packed;


struct sof_ipc_dma_trace_posn {
	struct sof_ipc_reply rhdr;
	uint32_t host_offset;	
	uint32_t overflow;	
	uint32_t messages;	
}  __packed;




#define SOF_IPC_PANIC_MAGIC			0x0dead000
#define SOF_IPC_PANIC_MAGIC_MASK		0x0ffff000
#define SOF_IPC_PANIC_CODE_MASK			0x00000fff
#define SOF_IPC_PANIC_MEM			(SOF_IPC_PANIC_MAGIC | 0x0)
#define SOF_IPC_PANIC_WORK			(SOF_IPC_PANIC_MAGIC | 0x1)
#define SOF_IPC_PANIC_IPC			(SOF_IPC_PANIC_MAGIC | 0x2)
#define SOF_IPC_PANIC_ARCH			(SOF_IPC_PANIC_MAGIC | 0x3)
#define SOF_IPC_PANIC_PLATFORM			(SOF_IPC_PANIC_MAGIC | 0x4)
#define SOF_IPC_PANIC_TASK			(SOF_IPC_PANIC_MAGIC | 0x5)
#define SOF_IPC_PANIC_EXCEPTION			(SOF_IPC_PANIC_MAGIC | 0x6)
#define SOF_IPC_PANIC_DEADLOCK			(SOF_IPC_PANIC_MAGIC | 0x7)
#define SOF_IPC_PANIC_STACK			(SOF_IPC_PANIC_MAGIC | 0x8)
#define SOF_IPC_PANIC_IDLE			(SOF_IPC_PANIC_MAGIC | 0x9)
#define SOF_IPC_PANIC_WFI			(SOF_IPC_PANIC_MAGIC | 0xa)
#define SOF_IPC_PANIC_ASSERT			(SOF_IPC_PANIC_MAGIC | 0xb)


struct sof_ipc_panic_info {
	struct sof_ipc_hdr hdr;
	uint32_t code;			
	char filename[SOF_TRACE_FILENAME_SIZE];
	uint32_t linenum;
}  __packed;

#endif
