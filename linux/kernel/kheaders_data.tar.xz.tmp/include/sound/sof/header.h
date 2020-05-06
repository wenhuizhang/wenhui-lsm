/* SPDX-License-Identifier: (GPL-2.0 OR BSD-3-Clause) */


#ifndef __INCLUDE_SOUND_SOF_HEADER_H__
#define __INCLUDE_SOUND_SOF_HEADER_H__

#include <uapi/sound/sof/abi.h>






#define SOF_GLB_TYPE_SHIFT			28
#define SOF_GLB_TYPE_MASK			(0xf << SOF_GLB_TYPE_SHIFT)
#define SOF_GLB_TYPE(x)				((x) << SOF_GLB_TYPE_SHIFT)


#define SOF_CMD_TYPE_SHIFT			16
#define SOF_CMD_TYPE_MASK			(0xfff << SOF_CMD_TYPE_SHIFT)
#define SOF_CMD_TYPE(x)				((x) << SOF_CMD_TYPE_SHIFT)


#define SOF_IPC_GLB_REPLY			SOF_GLB_TYPE(0x1U)
#define SOF_IPC_GLB_COMPOUND			SOF_GLB_TYPE(0x2U)
#define SOF_IPC_GLB_TPLG_MSG			SOF_GLB_TYPE(0x3U)
#define SOF_IPC_GLB_PM_MSG			SOF_GLB_TYPE(0x4U)
#define SOF_IPC_GLB_COMP_MSG			SOF_GLB_TYPE(0x5U)
#define SOF_IPC_GLB_STREAM_MSG			SOF_GLB_TYPE(0x6U)
#define SOF_IPC_FW_READY			SOF_GLB_TYPE(0x7U)
#define SOF_IPC_GLB_DAI_MSG			SOF_GLB_TYPE(0x8U)
#define SOF_IPC_GLB_TRACE_MSG			SOF_GLB_TYPE(0x9U)
#define SOF_IPC_GLB_GDB_DEBUG                   SOF_GLB_TYPE(0xAU)
#define SOF_IPC_GLB_TEST_MSG			SOF_GLB_TYPE(0xBU)




#define SOF_IPC_TPLG_COMP_NEW			SOF_CMD_TYPE(0x001)
#define SOF_IPC_TPLG_COMP_FREE			SOF_CMD_TYPE(0x002)
#define SOF_IPC_TPLG_COMP_CONNECT		SOF_CMD_TYPE(0x003)
#define SOF_IPC_TPLG_PIPE_NEW			SOF_CMD_TYPE(0x010)
#define SOF_IPC_TPLG_PIPE_FREE			SOF_CMD_TYPE(0x011)
#define SOF_IPC_TPLG_PIPE_CONNECT		SOF_CMD_TYPE(0x012)
#define SOF_IPC_TPLG_PIPE_COMPLETE		SOF_CMD_TYPE(0x013)
#define SOF_IPC_TPLG_BUFFER_NEW			SOF_CMD_TYPE(0x020)
#define SOF_IPC_TPLG_BUFFER_FREE		SOF_CMD_TYPE(0x021)


#define SOF_IPC_PM_CTX_SAVE			SOF_CMD_TYPE(0x001)
#define SOF_IPC_PM_CTX_RESTORE			SOF_CMD_TYPE(0x002)
#define SOF_IPC_PM_CTX_SIZE			SOF_CMD_TYPE(0x003)
#define SOF_IPC_PM_CLK_SET			SOF_CMD_TYPE(0x004)
#define SOF_IPC_PM_CLK_GET			SOF_CMD_TYPE(0x005)
#define SOF_IPC_PM_CLK_REQ			SOF_CMD_TYPE(0x006)
#define SOF_IPC_PM_CORE_ENABLE			SOF_CMD_TYPE(0x007)


#define SOF_IPC_COMP_SET_VALUE			SOF_CMD_TYPE(0x001)
#define SOF_IPC_COMP_GET_VALUE			SOF_CMD_TYPE(0x002)
#define SOF_IPC_COMP_SET_DATA			SOF_CMD_TYPE(0x003)
#define SOF_IPC_COMP_GET_DATA			SOF_CMD_TYPE(0x004)
#define SOF_IPC_COMP_NOTIFICATION		SOF_CMD_TYPE(0x005)


#define SOF_IPC_DAI_CONFIG			SOF_CMD_TYPE(0x001)
#define SOF_IPC_DAI_LOOPBACK			SOF_CMD_TYPE(0x002)


#define SOF_IPC_STREAM_PCM_PARAMS		SOF_CMD_TYPE(0x001)
#define SOF_IPC_STREAM_PCM_PARAMS_REPLY		SOF_CMD_TYPE(0x002)
#define SOF_IPC_STREAM_PCM_FREE			SOF_CMD_TYPE(0x003)
#define SOF_IPC_STREAM_TRIG_START		SOF_CMD_TYPE(0x004)
#define SOF_IPC_STREAM_TRIG_STOP		SOF_CMD_TYPE(0x005)
#define SOF_IPC_STREAM_TRIG_PAUSE		SOF_CMD_TYPE(0x006)
#define SOF_IPC_STREAM_TRIG_RELEASE		SOF_CMD_TYPE(0x007)
#define SOF_IPC_STREAM_TRIG_DRAIN		SOF_CMD_TYPE(0x008)
#define SOF_IPC_STREAM_TRIG_XRUN		SOF_CMD_TYPE(0x009)
#define SOF_IPC_STREAM_POSITION			SOF_CMD_TYPE(0x00a)
#define SOF_IPC_STREAM_VORBIS_PARAMS		SOF_CMD_TYPE(0x010)
#define SOF_IPC_STREAM_VORBIS_FREE		SOF_CMD_TYPE(0x011)


#define SOF_IPC_TRACE_DMA_PARAMS		SOF_CMD_TYPE(0x001)
#define SOF_IPC_TRACE_DMA_POSITION		SOF_CMD_TYPE(0x002)
#define SOF_IPC_TRACE_DMA_PARAMS_EXT		SOF_CMD_TYPE(0x003)


#define SOF_IPC_TEST_IPC_FLOOD                  SOF_CMD_TYPE(0x001)


#define SOF_IPC_MESSAGE_ID(x)			((x) & 0xffff)


#define SOF_IPC_MSG_MAX_SIZE			384



struct sof_ipc_hdr {
	uint32_t size;			
} __packed;



struct sof_ipc_cmd_hdr {
	uint32_t size;			
	uint32_t cmd;			
} __packed;


struct sof_ipc_reply {
	struct sof_ipc_cmd_hdr hdr;
	int32_t error;			
}  __packed;



struct sof_ipc_compound_hdr {
	struct sof_ipc_cmd_hdr hdr;
	uint32_t count;		
}  __packed;


struct sof_ipc_dsp_oops_arch_hdr {
	uint32_t arch;		
	uint32_t totalsize;	
}  __packed;


struct sof_ipc_dsp_oops_plat_hdr {
	uint32_t configidhi;	
	uint32_t configidlo;	
	uint32_t numaregs;	
	uint32_t stackoffset;	
	uint32_t stackptr;	
}  __packed;



#endif
