/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_CRYPTO_ATMEL_H
#define __LINUX_CRYPTO_ATMEL_H

#include <linux/platform_data/dma-atmel.h>


struct crypto_dma_data {
	struct at_dma_slave	txdata;
	struct at_dma_slave	rxdata;
};


struct crypto_platform_data {
	struct crypto_dma_data	*dma_slave;
};

#endif 
