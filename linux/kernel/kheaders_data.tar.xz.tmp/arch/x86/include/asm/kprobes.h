/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _ASM_X86_KPROBES_H
#define _ASM_X86_KPROBES_H


#include <asm-generic/kprobes.h>

#define BREAKPOINT_INSTRUCTION	0xcc

#ifdef CONFIG_KPROBES
#include <linux/types.h>
#include <linux/ptrace.h>
#include <linux/percpu.h>
#include <asm/insn.h>

#define  __ARCH_WANT_KPROBES_INSN_SLOT

struct pt_regs;
struct kprobe;

typedef u8 kprobe_opcode_t;
#define RELATIVEJUMP_OPCODE 0xe9
#define RELATIVEJUMP_SIZE 5
#define RELATIVECALL_OPCODE 0xe8
#define RELATIVE_ADDR_SIZE 4
#define MAX_STACK_SIZE 64
#define CUR_STACK_SIZE(ADDR) \
	(current_top_of_stack() - (unsigned long)(ADDR))
#define MIN_STACK_SIZE(ADDR)				\
	(MAX_STACK_SIZE < CUR_STACK_SIZE(ADDR) ?	\
	 MAX_STACK_SIZE : CUR_STACK_SIZE(ADDR))

#define flush_insn_slot(p)	do { } while (0)


extern __visible kprobe_opcode_t optprobe_template_entry[];
extern __visible kprobe_opcode_t optprobe_template_val[];
extern __visible kprobe_opcode_t optprobe_template_call[];
extern __visible kprobe_opcode_t optprobe_template_end[];
#define MAX_OPTIMIZED_LENGTH (MAX_INSN_SIZE + RELATIVE_ADDR_SIZE)
#define MAX_OPTINSN_SIZE 				\
	(((unsigned long)optprobe_template_end -	\
	  (unsigned long)optprobe_template_entry) +	\
	 MAX_OPTIMIZED_LENGTH + RELATIVEJUMP_SIZE)

extern const int kretprobe_blacklist_size;

void arch_remove_kprobe(struct kprobe *p);
asmlinkage void kretprobe_trampoline(void);

extern void arch_kprobe_override_function(struct pt_regs *regs);


struct arch_specific_insn {
	
	kprobe_opcode_t *insn;
	
	bool boostable;
	bool if_modifier;
};

struct arch_optimized_insn {
	
	kprobe_opcode_t copied_insn[RELATIVE_ADDR_SIZE];
	
	kprobe_opcode_t *insn;
	
	size_t size;
};


static inline int arch_prepared_optinsn(struct arch_optimized_insn *optinsn)
{
	return optinsn->size;
}

struct prev_kprobe {
	struct kprobe *kp;
	unsigned long status;
	unsigned long old_flags;
	unsigned long saved_flags;
};


struct kprobe_ctlblk {
	unsigned long kprobe_status;
	unsigned long kprobe_old_flags;
	unsigned long kprobe_saved_flags;
	struct prev_kprobe prev_kprobe;
};

extern int kprobe_fault_handler(struct pt_regs *regs, int trapnr);
extern int kprobe_exceptions_notify(struct notifier_block *self,
				    unsigned long val, void *data);
extern int kprobe_int3_handler(struct pt_regs *regs);
extern int kprobe_debug_handler(struct pt_regs *regs);

#endif 
#endif 
