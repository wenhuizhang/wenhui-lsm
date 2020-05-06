#ifndef __ASM_OFFSETS_H__
#define __ASM_OFFSETS_H__


#define PV_CPU_usergs_sysret64 256 
#define PV_CPU_swapgs 272 

#define KVM_STEAL_TIME_preempted 16 

#define pt_regs_bx 40 
#define pt_regs_cx 88 
#define pt_regs_dx 96 
#define pt_regs_sp 152 
#define pt_regs_bp 32 
#define pt_regs_si 104 
#define pt_regs_di 112 
#define pt_regs_r8 72 
#define pt_regs_r9 64 
#define pt_regs_r10 56 
#define pt_regs_r11 48 
#define pt_regs_r12 24 
#define pt_regs_r13 16 
#define pt_regs_r14 8 
#define pt_regs_r15 0 
#define pt_regs_flags 144 

#define saved_context_cr0 200 
#define saved_context_cr2 208 
#define saved_context_cr3 216 
#define saved_context_cr4 224 
#define saved_context_cr8 232 
#define saved_context_gdt_desc 275 

#define TSS_ist 36 
#define DB_STACK_OFFSET 8192 

#define stack_canary_offset 40 

#define __NR_syscall_max 547 
#define NR_syscalls 548 
#define __NR_syscall_compat_max 435 
#define IA32_NR_syscalls 436 

#define TASK_threadsp 4824 
#define TASK_stack_canary 2248 

#define TASK_addr_limit 4952 

#define crypto_tfm_ctx_offset 64 

#define pbe_address 0 
#define pbe_orig_address 8 
#define pbe_next 16 

#define IA32_SIGCONTEXT_ax 44 
#define IA32_SIGCONTEXT_bx 32 
#define IA32_SIGCONTEXT_cx 40 
#define IA32_SIGCONTEXT_dx 36 
#define IA32_SIGCONTEXT_si 20 
#define IA32_SIGCONTEXT_di 16 
#define IA32_SIGCONTEXT_bp 24 
#define IA32_SIGCONTEXT_sp 28 
#define IA32_SIGCONTEXT_ip 56 

#define IA32_RT_SIGFRAME_sigcontext 164 

#define PV_IRQ_irq_disable 312 
#define PV_IRQ_irq_enable 320 
#define PV_CPU_iret 264 
#define PV_MMU_read_cr2 392 

#define XEN_vcpu_info_mask 1 
#define XEN_vcpu_info_pending 0 
#define XEN_vcpu_info_arch_cr2 16 

#define BP_scratch 484 
#define BP_secure_boot 492 
#define BP_loadflags 529 
#define BP_hardware_subarch 572 
#define BP_version 518 
#define BP_kernel_alignment 560 
#define BP_init_size 608 
#define BP_pref_address 600 
#define BP_code32_start 532 

#define PTREGS_SIZE 168 
#define TLB_STATE_user_pcid_flush_mask 22 
#define CPU_ENTRY_AREA_entry_stack 4096 
#define SIZEOF_entry_stack 512 
#define MASK_entry_stack -512 
#define TSS_sp0 4 
#define TSS_sp1 12 
#define TSS_sp2 20 

#endif
