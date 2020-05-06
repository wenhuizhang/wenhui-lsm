/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __MTK_CMDQ_H__
#define __MTK_CMDQ_H__

#include <linux/mailbox_client.h>
#include <linux/mailbox/mtk-cmdq-mailbox.h>
#include <linux/timer.h>

#define CMDQ_NO_TIMEOUT		0xffffffffu


#define CMDQ_MAX_EVENT				0x3ff

struct cmdq_pkt;

struct cmdq_client {
	spinlock_t lock;
	u32 pkt_cnt;
	struct mbox_client client;
	struct mbox_chan *chan;
	struct timer_list timer;
	u32 timeout_ms; 
};


struct cmdq_client *cmdq_mbox_create(struct device *dev, int index,
				     u32 timeout);


void cmdq_mbox_destroy(struct cmdq_client *client);


struct cmdq_pkt *cmdq_pkt_create(struct cmdq_client *client, size_t size);


void cmdq_pkt_destroy(struct cmdq_pkt *pkt);


int cmdq_pkt_write(struct cmdq_pkt *pkt, u32 value, u32 subsys, u32 offset);


int cmdq_pkt_write_mask(struct cmdq_pkt *pkt, u32 value,
			u32 subsys, u32 offset, u32 mask);


int cmdq_pkt_wfe(struct cmdq_pkt *pkt, u32 event);


int cmdq_pkt_clear_event(struct cmdq_pkt *pkt, u32 event);


int cmdq_pkt_flush_async(struct cmdq_pkt *pkt, cmdq_async_flush_cb cb,
			 void *data);


int cmdq_pkt_flush(struct cmdq_pkt *pkt);

#endif	
