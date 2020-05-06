/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_TEXT_PATCHING_H
#define _ASM_X86_TEXT_PATCHING_H

#include <linux/types.h>
#include <linux/stddef.h>
#include <asm/ptrace.h>

struct paravirt_patch_site;
#ifdef CONFIG_PARAVIRT
void apply_paravirt(struct paravirt_patch_site *start,
		    struct paravirt_patch_site *end);
#else
static inline void apply_paravirt(struct paravirt_patch_site *start,
				  struct paravirt_patch_site *end)
{}
#define __parainstructions	NULL
#define __parainstructions_end	NULL
#endif


#define POKE_MAX_OPCODE_SIZE	5

struct text_poke_loc {
	void *detour;
	void *addr;
	size_t len;
	const char opcode[POKE_MAX_OPCODE_SIZE];
};

extern void text_poke_early(void *addr, const void *opcode, size_t len);


extern void *text_poke(void *addr, const void *opcode, size_t len);
extern void *text_poke_kgdb(void *addr, const void *opcode, size_t len);
extern int poke_int3_handler(struct pt_regs *regs);
extern void text_poke_bp(void *addr, const void *opcode, size_t len, void *handler);
extern void text_poke_bp_batch(struct text_poke_loc *tp, unsigned int nr_entries);
extern int after_bootmem;
extern __ro_after_init struct mm_struct *poking_mm;
extern __ro_after_init unsigned long poking_addr;

#ifndef CONFIG_UML_X86
static inline void int3_emulate_jmp(struct pt_regs *regs, unsigned long ip)
{
	regs->ip = ip;
}

#define INT3_INSN_SIZE 1
#define CALL_INSN_SIZE 5

static inline void int3_emulate_push(struct pt_regs *regs, unsigned long val)
{
	
	regs->sp -= sizeof(unsigned long);
	*(unsigned long *)regs->sp = val;
}

static inline void int3_emulate_call(struct pt_regs *regs, unsigned long func)
{
	int3_emulate_push(regs, regs->ip - INT3_INSN_SIZE + CALL_INSN_SIZE);
	int3_emulate_jmp(regs, func);
}
#endif 

#endif 
