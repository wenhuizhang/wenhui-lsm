/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef _ORC_TYPES_H
#define _ORC_TYPES_H

#include <linux/types.h>
#include <linux/compiler.h>


#define ORC_REG_UNDEFINED		0
#define ORC_REG_PREV_SP			1
#define ORC_REG_DX			2
#define ORC_REG_DI			3
#define ORC_REG_BP			4
#define ORC_REG_SP			5
#define ORC_REG_R10			6
#define ORC_REG_R13			7
#define ORC_REG_BP_INDIRECT		8
#define ORC_REG_SP_INDIRECT		9
#define ORC_REG_MAX			15


#define ORC_TYPE_CALL			0
#define ORC_TYPE_REGS			1
#define ORC_TYPE_REGS_IRET		2
#define UNWIND_HINT_TYPE_SAVE		3
#define UNWIND_HINT_TYPE_RESTORE	4

#ifndef __ASSEMBLY__

struct orc_entry {
	s16		sp_offset;
	s16		bp_offset;
	unsigned	sp_reg:4;
	unsigned	bp_reg:4;
	unsigned	type:2;
	unsigned	end:1;
} __packed;


struct unwind_hint {
	u32		ip;
	s16		sp_offset;
	u8		sp_reg;
	u8		type;
	u8		end;
};
#endif 

#endif 
