/* SPDX-License-Identifier: GPL-2.0 */



#ifdef CONFIG_SMP
BUILD_INTERRUPT(reschedule_interrupt,RESCHEDULE_VECTOR)
BUILD_INTERRUPT(call_function_interrupt,CALL_FUNCTION_VECTOR)
BUILD_INTERRUPT(call_function_single_interrupt,CALL_FUNCTION_SINGLE_VECTOR)
BUILD_INTERRUPT(irq_move_cleanup_interrupt, IRQ_MOVE_CLEANUP_VECTOR)
BUILD_INTERRUPT(reboot_interrupt, REBOOT_VECTOR)
#endif

#ifdef CONFIG_HAVE_KVM
BUILD_INTERRUPT(kvm_posted_intr_ipi, POSTED_INTR_VECTOR)
BUILD_INTERRUPT(kvm_posted_intr_wakeup_ipi, POSTED_INTR_WAKEUP_VECTOR)
BUILD_INTERRUPT(kvm_posted_intr_nested_ipi, POSTED_INTR_NESTED_VECTOR)
#endif


#ifdef CONFIG_X86_LOCAL_APIC

BUILD_INTERRUPT(apic_timer_interrupt,LOCAL_TIMER_VECTOR)
BUILD_INTERRUPT(error_interrupt,ERROR_APIC_VECTOR)
BUILD_INTERRUPT(spurious_interrupt,SPURIOUS_APIC_VECTOR)
BUILD_INTERRUPT(x86_platform_ipi, X86_PLATFORM_IPI_VECTOR)

#ifdef CONFIG_IRQ_WORK
BUILD_INTERRUPT(irq_work_interrupt, IRQ_WORK_VECTOR)
#endif

#ifdef CONFIG_X86_THERMAL_VECTOR
BUILD_INTERRUPT(thermal_interrupt,THERMAL_APIC_VECTOR)
#endif

#ifdef CONFIG_X86_MCE_THRESHOLD
BUILD_INTERRUPT(threshold_interrupt,THRESHOLD_APIC_VECTOR)
#endif

#ifdef CONFIG_X86_MCE_AMD
BUILD_INTERRUPT(deferred_error_interrupt, DEFERRED_ERROR_VECTOR)
#endif
#endif