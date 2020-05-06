/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_RESCTRL_SCHED_H
#define _ASM_X86_RESCTRL_SCHED_H

#ifdef CONFIG_X86_CPU_RESCTRL

#include <linux/sched.h>
#include <linux/jump_label.h>

#define IA32_PQR_ASSOC	0x0c8f


struct resctrl_pqr_state {
	u32			cur_rmid;
	u32			cur_closid;
	u32			default_rmid;
	u32			default_closid;
};

DECLARE_PER_CPU(struct resctrl_pqr_state, pqr_state);

DECLARE_STATIC_KEY_FALSE(rdt_enable_key);
DECLARE_STATIC_KEY_FALSE(rdt_alloc_enable_key);
DECLARE_STATIC_KEY_FALSE(rdt_mon_enable_key);


static void __resctrl_sched_in(void)
{
	struct resctrl_pqr_state *state = this_cpu_ptr(&pqr_state);
	u32 closid = state->default_closid;
	u32 rmid = state->default_rmid;

	
	if (static_branch_likely(&rdt_alloc_enable_key)) {
		if (current->closid)
			closid = current->closid;
	}

	if (static_branch_likely(&rdt_mon_enable_key)) {
		if (current->rmid)
			rmid = current->rmid;
	}

	if (closid != state->cur_closid || rmid != state->cur_rmid) {
		state->cur_closid = closid;
		state->cur_rmid = rmid;
		wrmsr(IA32_PQR_ASSOC, rmid, closid);
	}
}

static inline void resctrl_sched_in(void)
{
	if (static_branch_likely(&rdt_enable_key))
		__resctrl_sched_in();
}

#else

static inline void resctrl_sched_in(void) {}

#endif 

#endif 
