/* SPDX-License-Identifier: GPL-2.0-only */


#ifndef __DT_BINDINGS_AT91_DMA_H__
#define __DT_BINDINGS_AT91_DMA_H__




#define AT91_DMA_CFG_PER_ID_MASK	(0xff)
#define AT91_DMA_CFG_PER_ID(id)		(id & AT91_DMA_CFG_PER_ID_MASK)


#define AT91_DMA_CFG_FIFOCFG_OFFSET	(8)
#define AT91_DMA_CFG_FIFOCFG_MASK	(0xf << AT91_DMA_CFG_FIFOCFG_OFFSET)
#define AT91_DMA_CFG_FIFOCFG_HALF	(0x0 << AT91_DMA_CFG_FIFOCFG_OFFSET)	
#define AT91_DMA_CFG_FIFOCFG_ALAP	(0x1 << AT91_DMA_CFG_FIFOCFG_OFFSET)	
#define AT91_DMA_CFG_FIFOCFG_ASAP	(0x2 << AT91_DMA_CFG_FIFOCFG_OFFSET)	



#define AT91_XDMAC_DT_MEM_IF_MASK	(0x1)
#define AT91_XDMAC_DT_MEM_IF_OFFSET	(13)
#define AT91_XDMAC_DT_MEM_IF(mem_if)	(((mem_if) & AT91_XDMAC_DT_MEM_IF_MASK) \
					<< AT91_XDMAC_DT_MEM_IF_OFFSET)
#define AT91_XDMAC_DT_GET_MEM_IF(cfg)	(((cfg) >> AT91_XDMAC_DT_MEM_IF_OFFSET) \
					& AT91_XDMAC_DT_MEM_IF_MASK)

#define AT91_XDMAC_DT_PER_IF_MASK	(0x1)
#define AT91_XDMAC_DT_PER_IF_OFFSET	(14)
#define AT91_XDMAC_DT_PER_IF(per_if)	(((per_if) & AT91_XDMAC_DT_PER_IF_MASK) \
					<< AT91_XDMAC_DT_PER_IF_OFFSET)
#define AT91_XDMAC_DT_GET_PER_IF(cfg)	(((cfg) >> AT91_XDMAC_DT_PER_IF_OFFSET) \
					& AT91_XDMAC_DT_PER_IF_MASK)

#define AT91_XDMAC_DT_PERID_MASK	(0x7f)
#define AT91_XDMAC_DT_PERID_OFFSET	(24)
#define AT91_XDMAC_DT_PERID(perid)	(((perid) & AT91_XDMAC_DT_PERID_MASK) \
					<< AT91_XDMAC_DT_PERID_OFFSET)
#define AT91_XDMAC_DT_GET_PERID(cfg)	(((cfg) >> AT91_XDMAC_DT_PERID_OFFSET) \
					& AT91_XDMAC_DT_PERID_MASK)

#endif 