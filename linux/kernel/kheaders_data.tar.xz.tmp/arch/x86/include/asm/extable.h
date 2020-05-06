/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_EXTABLE_H
#define _ASM_X86_EXTABLE_H


struct exception_table_entry {
	int insn, fixup, handler;
};
struct pt_regs;

#define ARCH_HAS_RELATIVE_EXTABLE

#define swap_ex_entry_fixup(a, b, tmp, delta)			\
	do {							\
		(a)->fixup = (b)->fixup + (delta);		\
		(b)->fixup = (tmp).fixup - (delta);		\
		(a)->handler = (b)->handler + (delta);		\
		(b)->handler = (tmp).handler - (delta);		\
	} while (0)

extern int fixup_exception(struct pt_regs *regs, int trapnr,
			   unsigned long error_code, unsigned long fault_addr);
extern int fixup_bug(struct pt_regs *regs, int trapnr);
extern bool ex_has_fault_handler(unsigned long ip);
extern void early_fixup_exception(struct pt_regs *regs, int trapnr);

#endif
