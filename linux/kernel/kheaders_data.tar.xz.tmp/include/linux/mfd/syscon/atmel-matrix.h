/* SPDX-License-Identifier: GPL-2.0+ */


#ifndef _LINUX_MFD_SYSCON_ATMEL_MATRIX_H
#define _LINUX_MFD_SYSCON_ATMEL_MATRIX_H

#define AT91SAM9260_MATRIX_MCFG			0x00
#define AT91SAM9260_MATRIX_SCFG			0x40
#define AT91SAM9260_MATRIX_PRS			0x80
#define AT91SAM9260_MATRIX_MRCR			0x100
#define AT91SAM9260_MATRIX_EBICSA		0x11c

#define AT91SAM9261_MATRIX_MRCR			0x0
#define AT91SAM9261_MATRIX_SCFG			0x4
#define AT91SAM9261_MATRIX_TCR			0x24
#define AT91SAM9261_MATRIX_EBICSA		0x30
#define AT91SAM9261_MATRIX_USBPUCR		0x34

#define AT91SAM9263_MATRIX_MCFG			0x00
#define AT91SAM9263_MATRIX_SCFG			0x40
#define AT91SAM9263_MATRIX_PRS			0x80
#define AT91SAM9263_MATRIX_MRCR			0x100
#define AT91SAM9263_MATRIX_TCR			0x114
#define AT91SAM9263_MATRIX_EBI0CSA		0x120
#define AT91SAM9263_MATRIX_EBI1CSA		0x124

#define AT91SAM9RL_MATRIX_MCFG			0x00
#define AT91SAM9RL_MATRIX_SCFG			0x40
#define AT91SAM9RL_MATRIX_PRS			0x80
#define AT91SAM9RL_MATRIX_MRCR			0x100
#define AT91SAM9RL_MATRIX_TCR			0x114
#define AT91SAM9RL_MATRIX_EBICSA		0x120

#define AT91SAM9G45_MATRIX_MCFG			0x00
#define AT91SAM9G45_MATRIX_SCFG			0x40
#define AT91SAM9G45_MATRIX_PRS			0x80
#define AT91SAM9G45_MATRIX_MRCR			0x100
#define AT91SAM9G45_MATRIX_TCR			0x110
#define AT91SAM9G45_MATRIX_DDRMPR		0x118
#define AT91SAM9G45_MATRIX_EBICSA		0x128

#define AT91SAM9N12_MATRIX_MCFG			0x00
#define AT91SAM9N12_MATRIX_SCFG			0x40
#define AT91SAM9N12_MATRIX_PRS			0x80
#define AT91SAM9N12_MATRIX_MRCR			0x100
#define AT91SAM9N12_MATRIX_EBICSA		0x118

#define AT91SAM9X5_MATRIX_MCFG			0x00
#define AT91SAM9X5_MATRIX_SCFG			0x40
#define AT91SAM9X5_MATRIX_PRS			0x80
#define AT91SAM9X5_MATRIX_MRCR			0x100
#define AT91SAM9X5_MATRIX_EBICSA		0x120

#define SAMA5D3_MATRIX_MCFG			0x00
#define SAMA5D3_MATRIX_SCFG			0x40
#define SAMA5D3_MATRIX_PRS			0x80
#define SAMA5D3_MATRIX_MRCR			0x100

#define AT91_MATRIX_MCFG(o, x)			((o) + ((x) * 0x4))
#define AT91_MATRIX_ULBT			GENMASK(2, 0)
#define AT91_MATRIX_ULBT_INFINITE		(0 << 0)
#define AT91_MATRIX_ULBT_SINGLE			(1 << 0)
#define AT91_MATRIX_ULBT_FOUR			(2 << 0)
#define AT91_MATRIX_ULBT_EIGHT			(3 << 0)
#define AT91_MATRIX_ULBT_SIXTEEN		(4 << 0)

#define AT91_MATRIX_SCFG(o, x)			((o) + ((x) * 0x4))
#define AT91_MATRIX_SLOT_CYCLE			GENMASK(7,  0)
#define AT91_MATRIX_DEFMSTR_TYPE		GENMASK(17, 16)
#define AT91_MATRIX_DEFMSTR_TYPE_NONE		(0 << 16)
#define AT91_MATRIX_DEFMSTR_TYPE_LAST		(1 << 16)
#define AT91_MATRIX_DEFMSTR_TYPE_FIXED		(2 << 16)
#define AT91_MATRIX_FIXED_DEFMSTR		GENMASK(20, 18)
#define AT91_MATRIX_ARBT			GENMASK(25, 24)
#define AT91_MATRIX_ARBT_ROUND_ROBIN		(0 << 24)
#define AT91_MATRIX_ARBT_FIXED_PRIORITY		(1 << 24)

#define AT91_MATRIX_ITCM_SIZE			GENMASK(3, 0)
#define AT91_MATRIX_ITCM_0			(0 << 0)
#define AT91_MATRIX_ITCM_16			(5 << 0)
#define AT91_MATRIX_ITCM_32			(6 << 0)
#define AT91_MATRIX_ITCM_64			(7 << 0)
#define	AT91_MATRIX_DTCM_SIZE			GENMASK(7, 4)
#define	AT91_MATRIX_DTCM_0			(0 << 4)
#define	AT91_MATRIX_DTCM_16			(5 << 4)
#define AT91_MATRIX_DTCM_32			(6 << 4)
#define AT91_MATRIX_DTCM_64			(7 << 4)

#define AT91_MATRIX_PRAS(o, x)			((o) + ((x) * 0x8))
#define AT91_MATRIX_PRBS(o, x)			((o) + ((x) * 0x8) + 0x4)
#define AT91_MATRIX_MPR(x)			GENMASK(((x) * 0x4) + 1, ((x) * 0x4))

#define AT91_MATRIX_RCB(x)			BIT(x)

#define AT91_MATRIX_CSA(cs, val)		(val << (cs))
#define AT91_MATRIX_DBPUC			BIT(8)
#define AT91_MATRIX_DBPDC			BIT(9)
#define AT91_MATRIX_VDDIOMSEL			BIT(16)
#define AT91_MATRIX_VDDIOMSEL_1_8V		(0 << 16)
#define AT91_MATRIX_VDDIOMSEL_3_3V		(1 << 16)
#define AT91_MATRIX_EBI_IOSR			BIT(17)
#define AT91_MATRIX_DDR_IOSR			BIT(18)
#define AT91_MATRIX_NFD0_SELECT			BIT(24)
#define AT91_MATRIX_DDR_MP_EN			BIT(25)
#define AT91_MATRIX_EBI_NUM_CS			8

#define AT91_MATRIX_USBPUCR_PUON		BIT(30)

#endif 
