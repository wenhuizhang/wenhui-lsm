/* SPDX-License-Identifier: GPL-2.0+ */


#ifndef _LINUX_SRCU_TREE_H
#define _LINUX_SRCU_TREE_H

#include <linux/rcu_node_tree.h>
#include <linux/completion.h>

struct srcu_node;
struct srcu_struct;


struct srcu_data {
	
	unsigned long srcu_lock_count[2];	
	unsigned long srcu_unlock_count[2];	

	
	spinlock_t __private lock ____cacheline_internodealigned_in_smp;
	struct rcu_segcblist srcu_cblist;	
	unsigned long srcu_gp_seq_needed;	
	unsigned long srcu_gp_seq_needed_exp;	
	bool srcu_cblist_invoking;		
	struct timer_list delay_work;		
	struct work_struct work;		
	struct rcu_head srcu_barrier_head;	
	struct srcu_node *mynode;		
	unsigned long grpmask;			
						
	int cpu;
	struct srcu_struct *ssp;
};


struct srcu_node {
	spinlock_t __private lock;
	unsigned long srcu_have_cbs[4];		
						
						
	unsigned long srcu_data_have_cbs[4];	
						
	unsigned long srcu_gp_seq_needed_exp;	
	struct srcu_node *srcu_parent;		
	int grplo;				
	int grphi;				
};


struct srcu_struct {
	struct srcu_node node[NUM_RCU_NODES];	
	struct srcu_node *level[RCU_NUM_LVLS + 1];
						
	struct mutex srcu_cb_mutex;		
	spinlock_t __private lock;		
	struct mutex srcu_gp_mutex;		
	unsigned int srcu_idx;			
	unsigned long srcu_gp_seq;		
	unsigned long srcu_gp_seq_needed;	
	unsigned long srcu_gp_seq_needed_exp;	
	unsigned long srcu_last_gp_end;		
	struct srcu_data __percpu *sda;		
	unsigned long srcu_barrier_seq;		
	struct mutex srcu_barrier_mutex;	
	struct completion srcu_barrier_completion;
						
	atomic_t srcu_barrier_cpu_cnt;		
						
						
	struct delayed_work work;
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map dep_map;
#endif 
};


#define SRCU_STATE_IDLE		0
#define SRCU_STATE_SCAN1	1
#define SRCU_STATE_SCAN2	2

#define __SRCU_STRUCT_INIT(name, pcpu_name)				\
{									\
	.sda = &pcpu_name,						\
	.lock = __SPIN_LOCK_UNLOCKED(name.lock),			\
	.srcu_gp_seq_needed = -1UL,					\
	.work = __DELAYED_WORK_INITIALIZER(name.work, NULL, 0),		\
	__SRCU_DEP_MAP_INIT(name)					\
}


#ifdef MODULE
# define __DEFINE_SRCU(name, is_static)					\
	is_static struct srcu_struct name;				\
	struct srcu_struct * const __srcu_struct_##name			\
		__section("___srcu_struct_ptrs") = &name
#else
# define __DEFINE_SRCU(name, is_static)					\
	static DEFINE_PER_CPU(struct srcu_data, name##_srcu_data);	\
	is_static struct srcu_struct name =				\
		__SRCU_STRUCT_INIT(name, name##_srcu_data)
#endif
#define DEFINE_SRCU(name)		__DEFINE_SRCU(name, )
#define DEFINE_STATIC_SRCU(name)	__DEFINE_SRCU(name, static)

void synchronize_srcu_expedited(struct srcu_struct *ssp);
void srcu_barrier(struct srcu_struct *ssp);
void srcu_torture_stats_print(struct srcu_struct *ssp, char *tt, char *tf);

#endif
