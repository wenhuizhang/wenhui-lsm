/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __MACH_SPI_H_
#define __MACH_SPI_H_


struct spi_imx_master {
	int	*chipselect;
	int	num_chipselect;
};

#endif 
