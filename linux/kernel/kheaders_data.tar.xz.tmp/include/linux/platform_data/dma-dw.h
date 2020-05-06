/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _PLATFORM_DATA_DMA_DW_H
#define _PLATFORM_DATA_DMA_DW_H

#include <linux/device.h>

#define DW_DMA_MAX_NR_MASTERS	4
#define DW_DMA_MAX_NR_CHANNELS	8


struct dw_dma_slave {
	struct device		*dma_dev;
	u8			src_id;
	u8			dst_id;
	u8			m_master;
	u8			p_master;
	bool			hs_polarity;
};


struct dw_dma_platform_data {
	unsigned int	nr_channels;
#define CHAN_ALLOCATION_ASCENDING	0	
#define CHAN_ALLOCATION_DESCENDING	1	
	unsigned char	chan_allocation_order;
#define CHAN_PRIORITY_ASCENDING		0	
#define CHAN_PRIORITY_DESCENDING	1	
	unsigned char	chan_priority;
	unsigned int	block_size;
	unsigned char	nr_masters;
	unsigned char	data_width[DW_DMA_MAX_NR_MASTERS];
	unsigned char	multi_block[DW_DMA_MAX_NR_CHANNELS];
#define CHAN_PROTCTL_PRIVILEGED		BIT(0)
#define CHAN_PROTCTL_BUFFERABLE		BIT(1)
#define CHAN_PROTCTL_CACHEABLE		BIT(2)
#define CHAN_PROTCTL_MASK		GENMASK(2, 0)
	unsigned char	protctl;
};

#endif 
