/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __MTK_CMDQ_MAILBOX_H__
#define __MTK_CMDQ_MAILBOX_H__

#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/types.h>

#define CMDQ_INST_SIZE			8 
#define CMDQ_SUBSYS_SHIFT		16
#define CMDQ_OP_CODE_SHIFT		24
#define CMDQ_JUMP_PASS			CMDQ_INST_SIZE

#define CMDQ_WFE_UPDATE			BIT(31)
#define CMDQ_WFE_WAIT			BIT(15)
#define CMDQ_WFE_WAIT_VALUE		0x1


enum cmdq_code {
	CMDQ_CODE_MASK = 0x02,
	CMDQ_CODE_WRITE = 0x04,
	CMDQ_CODE_JUMP = 0x10,
	CMDQ_CODE_WFE = 0x20,
	CMDQ_CODE_EOC = 0x40,
};

enum cmdq_cb_status {
	CMDQ_CB_NORMAL = 0,
	CMDQ_CB_ERROR
};

struct cmdq_cb_data {
	enum cmdq_cb_status	sta;
	void			*data;
};

typedef void (*cmdq_async_flush_cb)(struct cmdq_cb_data data);

struct cmdq_task_cb {
	cmdq_async_flush_cb	cb;
	void			*data;
};

struct cmdq_pkt {
	void			*va_base;
	dma_addr_t		pa_base;
	size_t			cmd_buf_size; 
	size_t			buf_size; 
	struct cmdq_task_cb	cb;
	struct cmdq_task_cb	async_cb;
	void			*cl;
};

#endif 
