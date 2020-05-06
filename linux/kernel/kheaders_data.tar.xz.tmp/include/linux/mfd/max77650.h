/* SPDX-License-Identifier: GPL-2.0 */


#ifndef MAX77650_H
#define MAX77650_H

#include <linux/bits.h>

#define MAX77650_REG_INT_GLBL		0x00
#define MAX77650_REG_INT_CHG		0x01
#define MAX77650_REG_STAT_CHG_A		0x02
#define MAX77650_REG_STAT_CHG_B		0x03
#define MAX77650_REG_ERCFLAG		0x04
#define MAX77650_REG_STAT_GLBL		0x05
#define MAX77650_REG_INTM_GLBL		0x06
#define MAX77650_REG_INTM_CHG		0x07
#define MAX77650_REG_CNFG_GLBL		0x10
#define MAX77650_REG_CID		0x11
#define MAX77650_REG_CNFG_GPIO		0x12
#define MAX77650_REG_CNFG_CHG_A		0x18
#define MAX77650_REG_CNFG_CHG_B		0x19
#define MAX77650_REG_CNFG_CHG_C		0x1a
#define MAX77650_REG_CNFG_CHG_D		0x1b
#define MAX77650_REG_CNFG_CHG_E		0x1c
#define MAX77650_REG_CNFG_CHG_F		0x1d
#define MAX77650_REG_CNFG_CHG_G		0x1e
#define MAX77650_REG_CNFG_CHG_H		0x1f
#define MAX77650_REG_CNFG_CHG_I		0x20
#define MAX77650_REG_CNFG_SBB_TOP	0x28
#define MAX77650_REG_CNFG_SBB0_A	0x29
#define MAX77650_REG_CNFG_SBB0_B	0x2a
#define MAX77650_REG_CNFG_SBB1_A	0x2b
#define MAX77650_REG_CNFG_SBB1_B	0x2c
#define MAX77650_REG_CNFG_SBB2_A	0x2d
#define MAX77650_REG_CNFG_SBB2_B	0x2e
#define MAX77650_REG_CNFG_LDO_A		0x38
#define MAX77650_REG_CNFG_LDO_B		0x39
#define MAX77650_REG_CNFG_LED0_A	0x40
#define MAX77650_REG_CNFG_LED1_A	0x41
#define MAX77650_REG_CNFG_LED2_A	0x42
#define MAX77650_REG_CNFG_LED0_B	0x43
#define MAX77650_REG_CNFG_LED1_B	0x44
#define MAX77650_REG_CNFG_LED2_B	0x45
#define MAX77650_REG_CNFG_LED_TOP	0x46

#define MAX77650_CID_MASK		GENMASK(3, 0)
#define MAX77650_CID_BITS(_reg)		(_reg & MAX77650_CID_MASK)

#define MAX77650_CID_77650A		0x03
#define MAX77650_CID_77650C		0x0a
#define MAX77650_CID_77651A		0x06
#define MAX77650_CID_77651B		0x08

#endif 
