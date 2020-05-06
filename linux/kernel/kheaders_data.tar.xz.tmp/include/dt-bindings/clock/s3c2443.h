/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _DT_BINDINGS_CLOCK_SAMSUNG_S3C2443_CLOCK_H
#define _DT_BINDINGS_CLOCK_SAMSUNG_S3C2443_CLOCK_H




#define MSYSCLK			1
#define ESYSCLK			2
#define ARMDIV			3
#define ARMCLK			4
#define HCLK			5
#define PCLK			6
#define MPLL			7
#define EPLL			8


#define SCLK_HSSPI0		16
#define SCLK_FIMD		17
#define SCLK_I2S0		18
#define SCLK_I2S1		19
#define SCLK_HSMMC1		20
#define SCLK_HSMMC_EXT		21
#define SCLK_CAM		22
#define SCLK_UART		23
#define SCLK_USBH		24


#define MUX_HSSPI0		32
#define MUX_HSSPI1		33
#define MUX_HSMMC0		34
#define MUX_HSMMC1		35


#define HCLK_DMA0		48
#define HCLK_DMA1		49
#define HCLK_DMA2		50
#define HCLK_DMA3		51
#define HCLK_DMA4		52
#define HCLK_DMA5		53
#define HCLK_DMA6		54
#define HCLK_DMA7		55
#define HCLK_CAM		56
#define HCLK_LCD		57
#define HCLK_USBH		58
#define HCLK_USBD		59
#define HCLK_IROM		60
#define HCLK_HSMMC0		61
#define HCLK_HSMMC1		62
#define HCLK_CFC		63
#define HCLK_SSMC		64
#define HCLK_DRAM		65
#define HCLK_2D			66


#define PCLK_UART0		72
#define PCLK_UART1		73
#define PCLK_UART2		74
#define PCLK_UART3		75
#define PCLK_I2C0		76
#define PCLK_SDI		77
#define PCLK_SPI0		78
#define PCLK_ADC		79
#define PCLK_AC97		80
#define PCLK_I2S0		81
#define PCLK_PWM		82
#define PCLK_WDT		83
#define PCLK_RTC		84
#define PCLK_GPIO		85
#define PCLK_SPI1		86
#define PCLK_CHIPID		87
#define PCLK_I2C1		88
#define PCLK_I2S1		89
#define PCLK_PCM		90


#define NR_CLKS			(PCLK_PCM + 1)

#endif 
