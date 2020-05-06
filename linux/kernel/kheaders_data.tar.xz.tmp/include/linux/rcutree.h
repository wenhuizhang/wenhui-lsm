/* SPDX-License-Identifier: GPL-2.0+ */


#ifndef __LINUX_RCUTREE_H
#define __LINUX_RCUTREE_H

void rcu_softirq_qs(void);
void rcu_note_context_switch(bool preempt);
int rcu_needs_cpu(u64 basem, u64 *nextevt);
void rcu_cpu_stall_reset(void);


static inline void rcu_virt_note_context_switch(int cpu)
{
	rcu_note_context_switch(false);
}

void synchronize_rcu_expedited(void);
void kfree_call_rcu(struct rcu_head *head, rcu_callback_t func);

void rcu_barrier(void);
bool rcu_eqs_special_set(int cpu);
unsigned long get_state_synchronize_rcu(void);
void cond_synchronize_rcu(unsigned long oldstate);

void rcu_idle_enter(void);
void rcu_idle_exit(void);
void rcu_irq_enter(void);
void rcu_irq_exit(void);
void rcu_irq_enter_irqson(void);
void rcu_irq_exit_irqson(void);

void exit_rcu(void);

void rcu_scheduler_starting(void);
extern int rcu_scheduler_active __read_mostly;
void rcu_end_inkernel_boot(void);
bool rcu_is_watching(void);
#ifndef CONFIG_PREEMPT
void rcu_all_qs(void);
#endif


int rcutree_prepare_cpu(unsigned int cpu);
int rcutree_online_cpu(unsigned int cpu);
int rcutree_offline_cpu(unsigned int cpu);
int rcutree_dead_cpu(unsigned int cpu);
int rcutree_dying_cpu(unsigned int cpu);
void rcu_cpu_starting(unsigned int cpu);

#endif 
