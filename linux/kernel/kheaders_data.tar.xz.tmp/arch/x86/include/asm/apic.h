/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef _ASM_X86_APIC_H
#define _ASM_X86_APIC_H

#include <linux/cpumask.h>

#include <asm/alternative.h>
#include <asm/cpufeature.h>
#include <asm/apicdef.h>
#include <linux/atomic.h>
#include <asm/fixmap.h>
#include <asm/mpspec.h>
#include <asm/msr.h>
#include <asm/hardirq.h>

#define ARCH_APICTIMER_STOPS_ON_C3	1


#define APIC_QUIET   0
#define APIC_VERBOSE 1
#define APIC_DEBUG   2


#define APIC_EXTNMI_BSP		0 
#define APIC_EXTNMI_ALL		1
#define APIC_EXTNMI_NONE	2


#define apic_printk(v, s, a...) do {       \
		if ((v) <= apic_verbosity) \
			printk(s, ##a);    \
	} while (0)


#if defined(CONFIG_X86_LOCAL_APIC) && defined(CONFIG_X86_32)
extern void generic_apic_probe(void);
#else
static inline void generic_apic_probe(void)
{
}
#endif

#ifdef CONFIG_X86_LOCAL_APIC

extern int apic_verbosity;
extern int local_apic_timer_c2_ok;

extern int disable_apic;
extern unsigned int lapic_timer_period;

extern enum apic_intr_mode_id apic_intr_mode;
enum apic_intr_mode_id {
	APIC_PIC,
	APIC_VIRTUAL_WIRE,
	APIC_VIRTUAL_WIRE_NO_CONFIG,
	APIC_SYMMETRIC_IO,
	APIC_SYMMETRIC_IO_NO_ROUTING
};

#ifdef CONFIG_SMP
extern void __inquire_remote_apic(int apicid);
#else 
static inline void __inquire_remote_apic(int apicid)
{
}
#endif 

static inline void default_inquire_remote_apic(int apicid)
{
	if (apic_verbosity >= APIC_DEBUG)
		__inquire_remote_apic(apicid);
}


static inline bool apic_from_smp_config(void)
{
	return smp_found_config && !disable_apic;
}


#ifdef CONFIG_PARAVIRT
#include <asm/paravirt.h>
#endif

extern int setup_profiling_timer(unsigned int);

static inline void native_apic_mem_write(u32 reg, u32 v)
{
	volatile u32 *addr = (volatile u32 *)(APIC_BASE + reg);

	alternative_io("movl %0, %P1", "xchgl %0, %P1", X86_BUG_11AP,
		       ASM_OUTPUT2("=r" (v), "=m" (*addr)),
		       ASM_OUTPUT2("0" (v), "m" (*addr)));
}

static inline u32 native_apic_mem_read(u32 reg)
{
	return *((volatile u32 *)(APIC_BASE + reg));
}

extern void native_apic_wait_icr_idle(void);
extern u32 native_safe_apic_wait_icr_idle(void);
extern void native_apic_icr_write(u32 low, u32 id);
extern u64 native_apic_icr_read(void);

static inline bool apic_is_x2apic_enabled(void)
{
	u64 msr;

	if (rdmsrl_safe(MSR_IA32_APICBASE, &msr))
		return false;
	return msr & X2APIC_ENABLE;
}

extern void enable_IR_x2apic(void);

extern int get_physical_broadcast(void);

extern int lapic_get_maxlvt(void);
extern void clear_local_APIC(void);
extern void disconnect_bsp_APIC(int virt_wire_setup);
extern void disable_local_APIC(void);
extern void lapic_shutdown(void);
extern void sync_Arb_IDs(void);
extern void init_bsp_APIC(void);
extern void apic_intr_mode_init(void);
extern void init_apic_mappings(void);
void register_lapic_address(unsigned long address);
extern void setup_boot_APIC_clock(void);
extern void setup_secondary_APIC_clock(void);
extern void lapic_update_tsc_freq(void);

#ifdef CONFIG_X86_64
static inline int apic_force_enable(unsigned long addr)
{
	return -1;
}
#else
extern int apic_force_enable(unsigned long addr);
#endif

extern void apic_ap_setup(void);


#ifdef CONFIG_X86_64
extern int apic_is_clustered_box(void);
#else
static inline int apic_is_clustered_box(void)
{
	return 0;
}
#endif

extern int setup_APIC_eilvt(u8 lvt_off, u8 vector, u8 msg_type, u8 mask);
extern void lapic_assign_system_vectors(void);
extern void lapic_assign_legacy_vector(unsigned int isairq, bool replace);
extern void lapic_online(void);
extern void lapic_offline(void);
extern bool apic_needs_pit(void);

#else 
static inline void lapic_shutdown(void) { }
#define local_apic_timer_c2_ok		1
static inline void init_apic_mappings(void) { }
static inline void disable_local_APIC(void) { }
# define setup_boot_APIC_clock x86_init_noop
# define setup_secondary_APIC_clock x86_init_noop
static inline void lapic_update_tsc_freq(void) { }
static inline void init_bsp_APIC(void) { }
static inline void apic_intr_mode_init(void) { }
static inline void lapic_assign_system_vectors(void) { }
static inline void lapic_assign_legacy_vector(unsigned int i, bool r) { }
static inline bool apic_needs_pit(void) { return true; }
#endif 

#ifdef CONFIG_X86_X2APIC

static inline void x2apic_wrmsr_fence(void)
{
	asm volatile("mfence" : : : "memory");
}

static inline void native_apic_msr_write(u32 reg, u32 v)
{
	if (reg == APIC_DFR || reg == APIC_ID || reg == APIC_LDR ||
	    reg == APIC_LVR)
		return;

	wrmsr(APIC_BASE_MSR + (reg >> 4), v, 0);
}

static inline void native_apic_msr_eoi_write(u32 reg, u32 v)
{
	__wrmsr(APIC_BASE_MSR + (APIC_EOI >> 4), APIC_EOI_ACK, 0);
}

static inline u32 native_apic_msr_read(u32 reg)
{
	u64 msr;

	if (reg == APIC_DFR)
		return -1;

	rdmsrl(APIC_BASE_MSR + (reg >> 4), msr);
	return (u32)msr;
}

static inline void native_x2apic_wait_icr_idle(void)
{
	
	return;
}

static inline u32 native_safe_x2apic_wait_icr_idle(void)
{
	
	return 0;
}

static inline void native_x2apic_icr_write(u32 low, u32 id)
{
	wrmsrl(APIC_BASE_MSR + (APIC_ICR >> 4), ((__u64) id) << 32 | low);
}

static inline u64 native_x2apic_icr_read(void)
{
	unsigned long val;

	rdmsrl(APIC_BASE_MSR + (APIC_ICR >> 4), val);
	return val;
}

extern int x2apic_mode;
extern int x2apic_phys;
extern void __init check_x2apic(void);
extern void x2apic_setup(void);
static inline int x2apic_enabled(void)
{
	return boot_cpu_has(X86_FEATURE_X2APIC) && apic_is_x2apic_enabled();
}

#define x2apic_supported()	(boot_cpu_has(X86_FEATURE_X2APIC))
#else 
static inline void check_x2apic(void) { }
static inline void x2apic_setup(void) { }
static inline int x2apic_enabled(void) { return 0; }

#define x2apic_mode		(0)
#define	x2apic_supported()	(0)
#endif 

struct irq_data;


struct apic {
	
	void	(*eoi_write)(u32 reg, u32 v);
	void	(*native_eoi_write)(u32 reg, u32 v);
	void	(*write)(u32 reg, u32 v);
	u32	(*read)(u32 reg);

	
	void	(*wait_icr_idle)(void);
	u32	(*safe_wait_icr_idle)(void);

	void	(*send_IPI)(int cpu, int vector);
	void	(*send_IPI_mask)(const struct cpumask *mask, int vector);
	void	(*send_IPI_mask_allbutself)(const struct cpumask *msk, int vec);
	void	(*send_IPI_allbutself)(int vector);
	void	(*send_IPI_all)(int vector);
	void	(*send_IPI_self)(int vector);

	
	u32	dest_logical;
	u32	disable_esr;
	u32	irq_delivery_mode;
	u32	irq_dest_mode;

	u32	(*calc_dest_apicid)(unsigned int cpu);

	
	u64	(*icr_read)(void);
	void	(*icr_write)(u32 low, u32 high);

	
	int	(*probe)(void);
	int	(*acpi_madt_oem_check)(char *oem_id, char *oem_table_id);
	int	(*apic_id_valid)(u32 apicid);
	int	(*apic_id_registered)(void);

	bool	(*check_apicid_used)(physid_mask_t *map, int apicid);
	void	(*init_apic_ldr)(void);
	void	(*ioapic_phys_id_map)(physid_mask_t *phys_map, physid_mask_t *retmap);
	void	(*setup_apic_routing)(void);
	int	(*cpu_present_to_apicid)(int mps_cpu);
	void	(*apicid_to_cpu_present)(int phys_apicid, physid_mask_t *retmap);
	int	(*check_phys_apicid_present)(int phys_apicid);
	int	(*phys_pkg_id)(int cpuid_apic, int index_msb);

	u32	(*get_apic_id)(unsigned long x);
	u32	(*set_apic_id)(unsigned int id);

	
	int	(*wakeup_secondary_cpu)(int apicid, unsigned long start_eip);

	void	(*inquire_remote_apic)(int apicid);

#ifdef CONFIG_X86_32
	
	int (*x86_32_early_logical_apicid)(int cpu);
#endif
	char	*name;
};


extern struct apic *apic;


#define apic_driver(sym)					\
	static const struct apic *__apicdrivers_##sym __used		\
	__aligned(sizeof(struct apic *))			\
	__section(.apicdrivers) = { &sym }

#define apic_drivers(sym1, sym2)					\
	static struct apic *__apicdrivers_##sym1##sym2[2] __used	\
	__aligned(sizeof(struct apic *))				\
	__section(.apicdrivers) = { &sym1, &sym2 }

extern struct apic *__apicdrivers[], *__apicdrivers_end[];


#ifdef CONFIG_SMP
extern int wakeup_secondary_cpu_via_nmi(int apicid, unsigned long start_eip);
extern int lapic_can_unplug_cpu(void);
#endif

#ifdef CONFIG_X86_LOCAL_APIC

static inline u32 apic_read(u32 reg)
{
	return apic->read(reg);
}

static inline void apic_write(u32 reg, u32 val)
{
	apic->write(reg, val);
}

static inline void apic_eoi(void)
{
	apic->eoi_write(APIC_EOI, APIC_EOI_ACK);
}

static inline u64 apic_icr_read(void)
{
	return apic->icr_read();
}

static inline void apic_icr_write(u32 low, u32 high)
{
	apic->icr_write(low, high);
}

static inline void apic_wait_icr_idle(void)
{
	apic->wait_icr_idle();
}

static inline u32 safe_apic_wait_icr_idle(void)
{
	return apic->safe_wait_icr_idle();
}

extern void __init apic_set_eoi_write(void (*eoi_write)(u32 reg, u32 v));

#else 

static inline u32 apic_read(u32 reg) { return 0; }
static inline void apic_write(u32 reg, u32 val) { }
static inline void apic_eoi(void) { }
static inline u64 apic_icr_read(void) { return 0; }
static inline void apic_icr_write(u32 low, u32 high) { }
static inline void apic_wait_icr_idle(void) { }
static inline u32 safe_apic_wait_icr_idle(void) { return 0; }
static inline void apic_set_eoi_write(void (*eoi_write)(u32 reg, u32 v)) {}

#endif 

extern void apic_ack_irq(struct irq_data *data);

static inline void ack_APIC_irq(void)
{
	
	apic_eoi();
}

static inline unsigned default_get_apic_id(unsigned long x)
{
	unsigned int ver = GET_APIC_VERSION(apic_read(APIC_LVR));

	if (APIC_XAPIC(ver) || boot_cpu_has(X86_FEATURE_EXTD_APICID))
		return (x >> 24) & 0xFF;
	else
		return (x >> 24) & 0x0F;
}


#define TRAMPOLINE_PHYS_LOW		0x467
#define TRAMPOLINE_PHYS_HIGH		0x469

#ifdef CONFIG_X86_64
extern void apic_send_IPI_self(int vector);

DECLARE_PER_CPU(int, x2apic_extra_bits);
#endif

extern void generic_bigsmp_probe(void);

#ifdef CONFIG_X86_LOCAL_APIC

#include <asm/smp.h>

#define APIC_DFR_VALUE	(APIC_DFR_FLAT)

DECLARE_EARLY_PER_CPU_READ_MOSTLY(u16, x86_bios_cpu_apicid);

extern struct apic apic_noop;

static inline unsigned int read_apic_id(void)
{
	unsigned int reg = apic_read(APIC_ID);

	return apic->get_apic_id(reg);
}

extern int default_apic_id_valid(u32 apicid);
extern int default_acpi_madt_oem_check(char *, char *);
extern void default_setup_apic_routing(void);

extern u32 apic_default_calc_apicid(unsigned int cpu);
extern u32 apic_flat_calc_apicid(unsigned int cpu);

extern bool default_check_apicid_used(physid_mask_t *map, int apicid);
extern void default_ioapic_phys_id_map(physid_mask_t *phys_map, physid_mask_t *retmap);
extern int default_cpu_present_to_apicid(int mps_cpu);
extern int default_check_phys_apicid_present(int phys_apicid);

#endif 

#ifdef CONFIG_SMP
bool apic_id_is_primary_thread(unsigned int id);
#else
static inline bool apic_id_is_primary_thread(unsigned int id) { return false; }
#endif

extern void irq_enter(void);
extern void irq_exit(void);

static inline void entering_irq(void)
{
	irq_enter();
	kvm_set_cpu_l1tf_flush_l1d();
}

static inline void entering_ack_irq(void)
{
	entering_irq();
	ack_APIC_irq();
}

static inline void ipi_entering_ack_irq(void)
{
	irq_enter();
	ack_APIC_irq();
	kvm_set_cpu_l1tf_flush_l1d();
}

static inline void exiting_irq(void)
{
	irq_exit();
}

static inline void exiting_ack_irq(void)
{
	ack_APIC_irq();
	irq_exit();
}

extern void ioapic_zap_locks(void);

#endif 
