/* SPDX-License-Identifier: GPL-2.0 */
#ifndef	__SPI_BITBANG_H
#define	__SPI_BITBANG_H

#include <linux/workqueue.h>

struct spi_bitbang {
	struct mutex		lock;
	u8			busy;
	u8			use_dma;
	u16			flags;		

	struct spi_master	*master;

	
	int	(*setup_transfer)(struct spi_device *spi,
			struct spi_transfer *t);

	void	(*chipselect)(struct spi_device *spi, int is_on);
#define	BITBANG_CS_ACTIVE	1	
#define	BITBANG_CS_INACTIVE	0

	
	int	(*txrx_bufs)(struct spi_device *spi, struct spi_transfer *t);

	
	u32	(*txrx_word[4])(struct spi_device *spi,
			unsigned nsecs,
			u32 word, u8 bits, unsigned flags);
	int	(*set_line_direction)(struct spi_device *spi, bool output);
};


extern int spi_bitbang_setup(struct spi_device *spi);
extern void spi_bitbang_cleanup(struct spi_device *spi);
extern int spi_bitbang_setup_transfer(struct spi_device *spi,
				      struct spi_transfer *t);


extern int spi_bitbang_start(struct spi_bitbang *spi);
extern int spi_bitbang_init(struct spi_bitbang *spi);
extern void spi_bitbang_stop(struct spi_bitbang *spi);

#endif	
