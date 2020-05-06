/* SPDX-License-Identifier: GPL-2.0-only */


#ifndef __ASM_ARM_KVM_ARCH_TIMER_H
#define __ASM_ARM_KVM_ARCH_TIMER_H

#include <linux/clocksource.h>
#include <linux/hrtimer.h>

enum kvm_arch_timers {
	TIMER_PTIMER,
	TIMER_VTIMER,
	NR_KVM_TIMERS
};

enum kvm_arch_timer_regs {
	TIMER_REG_CNT,
	TIMER_REG_CVAL,
	TIMER_REG_TVAL,
	TIMER_REG_CTL,
};

struct arch_timer_context {
	struct kvm_vcpu			*vcpu;

	
	u32				cnt_ctl;
	u64				cnt_cval;

	
	struct kvm_irq_level		irq;

	
	u64				cntvoff;

	
	struct hrtimer			hrtimer;

	
	bool				loaded;

	
	u32				host_timer_irq;
	u32				host_timer_irq_flags;
};

struct timer_map {
	struct arch_timer_context *direct_vtimer;
	struct arch_timer_context *direct_ptimer;
	struct arch_timer_context *emul_ptimer;
};

struct arch_timer_cpu {
	struct arch_timer_context timers[NR_KVM_TIMERS];

	
	struct hrtimer			bg_timer;

	
	bool			enabled;
};

int kvm_timer_hyp_init(bool);
int kvm_timer_enable(struct kvm_vcpu *vcpu);
int kvm_timer_vcpu_reset(struct kvm_vcpu *vcpu);
void kvm_timer_vcpu_init(struct kvm_vcpu *vcpu);
void kvm_timer_sync_hwstate(struct kvm_vcpu *vcpu);
bool kvm_timer_should_notify_user(struct kvm_vcpu *vcpu);
void kvm_timer_update_run(struct kvm_vcpu *vcpu);
void kvm_timer_vcpu_terminate(struct kvm_vcpu *vcpu);

u64 kvm_arm_timer_get_reg(struct kvm_vcpu *, u64 regid);
int kvm_arm_timer_set_reg(struct kvm_vcpu *, u64 regid, u64 value);

int kvm_arm_timer_set_attr(struct kvm_vcpu *vcpu, struct kvm_device_attr *attr);
int kvm_arm_timer_get_attr(struct kvm_vcpu *vcpu, struct kvm_device_attr *attr);
int kvm_arm_timer_has_attr(struct kvm_vcpu *vcpu, struct kvm_device_attr *attr);

bool kvm_timer_is_pending(struct kvm_vcpu *vcpu);

u64 kvm_phys_timer_read(void);

void kvm_timer_vcpu_load(struct kvm_vcpu *vcpu);
void kvm_timer_vcpu_put(struct kvm_vcpu *vcpu);

void kvm_timer_init_vhe(void);

bool kvm_arch_timer_get_input_level(int vintid);

#define vcpu_timer(v)	(&(v)->arch.timer_cpu)
#define vcpu_get_timer(v,t)	(&vcpu_timer(v)->timers[(t)])
#define vcpu_vtimer(v)	(&(v)->arch.timer_cpu.timers[TIMER_VTIMER])
#define vcpu_ptimer(v)	(&(v)->arch.timer_cpu.timers[TIMER_PTIMER])

#define arch_timer_ctx_index(ctx)	((ctx) - vcpu_timer((ctx)->vcpu)->timers)

u64 kvm_arm_timer_read_sysreg(struct kvm_vcpu *vcpu,
			      enum kvm_arch_timers tmr,
			      enum kvm_arch_timer_regs treg);
void kvm_arm_timer_write_sysreg(struct kvm_vcpu *vcpu,
				enum kvm_arch_timers tmr,
				enum kvm_arch_timer_regs treg,
				u64 val);

#endif
