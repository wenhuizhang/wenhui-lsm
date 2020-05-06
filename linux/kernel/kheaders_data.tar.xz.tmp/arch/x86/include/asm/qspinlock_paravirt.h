/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_QSPINLOCK_PARAVIRT_H
#define __ASM_QSPINLOCK_PARAVIRT_H


#ifdef CONFIG_64BIT

PV_CALLEE_SAVE_REGS_THUNK(__pv_queued_spin_unlock_slowpath);
#define __pv_queued_spin_unlock	__pv_queued_spin_unlock
#define PV_UNLOCK		"__raw_callee_save___pv_queued_spin_unlock"
#define PV_UNLOCK_SLOWPATH	"__raw_callee_save___pv_queued_spin_unlock_slowpath"


asm    (".pushsection .text;"
	".globl " PV_UNLOCK ";"
	".type " PV_UNLOCK ", @function;"
	".align 4,0x90;"
	PV_UNLOCK ": "
	FRAME_BEGIN
	"push  %rdx;"
	"mov   $0x1,%eax;"
	"xor   %edx,%edx;"
	LOCK_PREFIX "cmpxchg %dl,(%rdi);"
	"cmp   $0x1,%al;"
	"jne   .slowpath;"
	"pop   %rdx;"
	FRAME_END
	"ret;"
	".slowpath: "
	"push   %rsi;"
	"movzbl %al,%esi;"
	"call " PV_UNLOCK_SLOWPATH ";"
	"pop    %rsi;"
	"pop    %rdx;"
	FRAME_END
	"ret;"
	".size " PV_UNLOCK ", .-" PV_UNLOCK ";"
	".popsection");

#else 

extern void __pv_queued_spin_unlock(struct qspinlock *lock);
PV_CALLEE_SAVE_REGS_THUNK(__pv_queued_spin_unlock);

#endif 
#endif
