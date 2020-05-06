/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_PROCESSOR_H
#define _ASM_X86_PROCESSOR_H

#include <asm/processor-flags.h>


struct task_struct;
struct mm_struct;
struct vm86;

#include <asm/math_emu.h>
#include <asm/segment.h>
#include <asm/types.h>
#include <uapi/asm/sigcontext.h>
#include <asm/current.h>
#include <asm/cpufeatures.h>
#include <asm/page.h>
#include <asm/pgtable_types.h>
#include <asm/percpu.h>
#include <asm/msr.h>
#include <asm/desc_defs.h>
#include <asm/nops.h>
#include <asm/special_insns.h>
#include <asm/fpu/types.h>
#include <asm/unwind_hints.h>

#include <linux/personality.h>
#include <linux/cache.h>
#include <linux/threads.h>
#include <linux/math64.h>
#include <linux/err.h>
#include <linux/irqflags.h>
#include <linux/mem_encrypt.h>


#define NET_IP_ALIGN	0

#define HBP_NUM 4


#ifdef CONFIG_X86_VSMP
# define ARCH_MIN_TASKALIGN		(1 << INTERNODE_CACHE_SHIFT)
# define ARCH_MIN_MMSTRUCT_ALIGN	(1 << INTERNODE_CACHE_SHIFT)
#else
# define ARCH_MIN_TASKALIGN		__alignof__(union fpregs_state)
# define ARCH_MIN_MMSTRUCT_ALIGN	0
#endif

enum tlb_infos {
	ENTRIES,
	NR_INFO
};

extern u16 __read_mostly tlb_lli_4k[NR_INFO];
extern u16 __read_mostly tlb_lli_2m[NR_INFO];
extern u16 __read_mostly tlb_lli_4m[NR_INFO];
extern u16 __read_mostly tlb_lld_4k[NR_INFO];
extern u16 __read_mostly tlb_lld_2m[NR_INFO];
extern u16 __read_mostly tlb_lld_4m[NR_INFO];
extern u16 __read_mostly tlb_lld_1g[NR_INFO];



struct cpuinfo_x86 {
	__u8			x86;		
	__u8			x86_vendor;	
	__u8			x86_model;
	__u8			x86_stepping;
#ifdef CONFIG_X86_64
	
	int			x86_tlbsize;
#endif
	__u8			x86_virt_bits;
	__u8			x86_phys_bits;
	
	__u8			x86_coreid_bits;
	__u8			cu_id;
	
	__u32			extended_cpuid_level;
	
	int			cpuid_level;
	__u32			x86_capability[NCAPINTS + NBUGINTS];
	char			x86_vendor_id[16];
	char			x86_model_id[64];
	
	unsigned int		x86_cache_size;
	int			x86_cache_alignment;	
	
	int			x86_cache_max_rmid;	
	int			x86_cache_occ_scale;	
	int			x86_power;
	unsigned long		loops_per_jiffy;
	
	u16			x86_max_cores;
	u16			apicid;
	u16			initial_apicid;
	u16			x86_clflush_size;
	
	u16			booted_cores;
	
	u16			phys_proc_id;
	
	u16			logical_proc_id;
	
	u16			cpu_core_id;
	u16			cpu_die_id;
	u16			logical_die_id;
	
	u16			cpu_index;
	u32			microcode;
	
	u8			x86_cache_bits;
	unsigned		initialized : 1;
} __randomize_layout;

struct cpuid_regs {
	u32 eax, ebx, ecx, edx;
};

enum cpuid_regs_idx {
	CPUID_EAX = 0,
	CPUID_EBX,
	CPUID_ECX,
	CPUID_EDX,
};

#define X86_VENDOR_INTEL	0
#define X86_VENDOR_CYRIX	1
#define X86_VENDOR_AMD		2
#define X86_VENDOR_UMC		3
#define X86_VENDOR_CENTAUR	5
#define X86_VENDOR_TRANSMETA	7
#define X86_VENDOR_NSC		8
#define X86_VENDOR_HYGON	9
#define X86_VENDOR_ZHAOXIN	10
#define X86_VENDOR_NUM		11

#define X86_VENDOR_UNKNOWN	0xff


extern struct cpuinfo_x86	boot_cpu_data;
extern struct cpuinfo_x86	new_cpu_data;

extern struct x86_hw_tss	doublefault_tss;
extern __u32			cpu_caps_cleared[NCAPINTS + NBUGINTS];
extern __u32			cpu_caps_set[NCAPINTS + NBUGINTS];

#ifdef CONFIG_SMP
DECLARE_PER_CPU_READ_MOSTLY(struct cpuinfo_x86, cpu_info);
#define cpu_data(cpu)		per_cpu(cpu_info, cpu)
#else
#define cpu_info		boot_cpu_data
#define cpu_data(cpu)		boot_cpu_data
#endif

extern const struct seq_operations cpuinfo_op;

#define cache_line_size()	(boot_cpu_data.x86_cache_alignment)

extern void cpu_detect(struct cpuinfo_x86 *c);

static inline unsigned long long l1tf_pfn_limit(void)
{
	return BIT_ULL(boot_cpu_data.x86_cache_bits - 1 - PAGE_SHIFT);
}

extern void early_cpu_init(void);
extern void identify_boot_cpu(void);
extern void identify_secondary_cpu(struct cpuinfo_x86 *);
extern void print_cpu_info(struct cpuinfo_x86 *);
void print_cpu_msr(struct cpuinfo_x86 *);

#ifdef CONFIG_X86_32
extern int have_cpuid_p(void);
#else
static inline int have_cpuid_p(void)
{
	return 1;
}
#endif
static inline void native_cpuid(unsigned int *eax, unsigned int *ebx,
				unsigned int *ecx, unsigned int *edx)
{
	
	asm volatile("cpuid"
	    : "=a" (*eax),
	      "=b" (*ebx),
	      "=c" (*ecx),
	      "=d" (*edx)
	    : "0" (*eax), "2" (*ecx)
	    : "memory");
}

#define native_cpuid_reg(reg)					\
static inline unsigned int native_cpuid_##reg(unsigned int op)	\
{								\
	unsigned int eax = op, ebx, ecx = 0, edx;		\
								\
	native_cpuid(&eax, &ebx, &ecx, &edx);			\
								\
	return reg;						\
}


native_cpuid_reg(eax)
native_cpuid_reg(ebx)
native_cpuid_reg(ecx)
native_cpuid_reg(edx)


static inline unsigned long read_cr3_pa(void)
{
	return __read_cr3() & CR3_ADDR_MASK;
}

static inline unsigned long native_read_cr3_pa(void)
{
	return __native_read_cr3() & CR3_ADDR_MASK;
}

static inline void load_cr3(pgd_t *pgdir)
{
	write_cr3(__sme_pa(pgdir));
}


#ifdef CONFIG_X86_32

struct x86_hw_tss {
	unsigned short		back_link, __blh;
	unsigned long		sp0;
	unsigned short		ss0, __ss0h;
	unsigned long		sp1;

	
	unsigned short		ss1;	

	unsigned short		__ss1h;
	unsigned long		sp2;
	unsigned short		ss2, __ss2h;
	unsigned long		__cr3;
	unsigned long		ip;
	unsigned long		flags;
	unsigned long		ax;
	unsigned long		cx;
	unsigned long		dx;
	unsigned long		bx;
	unsigned long		sp;
	unsigned long		bp;
	unsigned long		si;
	unsigned long		di;
	unsigned short		es, __esh;
	unsigned short		cs, __csh;
	unsigned short		ss, __ssh;
	unsigned short		ds, __dsh;
	unsigned short		fs, __fsh;
	unsigned short		gs, __gsh;
	unsigned short		ldt, __ldth;
	unsigned short		trace;
	unsigned short		io_bitmap_base;

} __attribute__((packed));
#else
struct x86_hw_tss {
	u32			reserved1;
	u64			sp0;

	
	u64			sp1;

	
	u64			sp2;

	u64			reserved2;
	u64			ist[7];
	u32			reserved3;
	u32			reserved4;
	u16			reserved5;
	u16			io_bitmap_base;

} __attribute__((packed));
#endif


#define IO_BITMAP_BITS			65536
#define IO_BITMAP_BYTES			(IO_BITMAP_BITS/8)
#define IO_BITMAP_LONGS			(IO_BITMAP_BYTES/sizeof(long))
#define IO_BITMAP_OFFSET		(offsetof(struct tss_struct, io_bitmap) - offsetof(struct tss_struct, x86_tss))
#define INVALID_IO_BITMAP_OFFSET	0x8000

struct entry_stack {
	unsigned long		words[64];
};

struct entry_stack_page {
	struct entry_stack stack;
} __aligned(PAGE_SIZE);

struct tss_struct {
	
	struct x86_hw_tss	x86_tss;

	
	unsigned long		io_bitmap[IO_BITMAP_LONGS + 1];
} __aligned(PAGE_SIZE);

DECLARE_PER_CPU_PAGE_ALIGNED(struct tss_struct, cpu_tss_rw);


#define __KERNEL_TSS_LIMIT	\
	(IO_BITMAP_OFFSET + IO_BITMAP_BYTES + sizeof(unsigned long) - 1)


struct irq_stack {
	char		stack[IRQ_STACK_SIZE];
} __aligned(IRQ_STACK_SIZE);

DECLARE_PER_CPU(struct irq_stack *, hardirq_stack_ptr);

#ifdef CONFIG_X86_32
DECLARE_PER_CPU(unsigned long, cpu_current_top_of_stack);
#else

#define cpu_current_top_of_stack cpu_tss_rw.x86_tss.sp1
#endif

#ifdef CONFIG_X86_64
struct fixed_percpu_data {
	
	char		gs_base[40];
	unsigned long	stack_canary;
};

DECLARE_PER_CPU_FIRST(struct fixed_percpu_data, fixed_percpu_data) __visible;
DECLARE_INIT_PER_CPU(fixed_percpu_data);

static inline unsigned long cpu_kernelmode_gs_base(int cpu)
{
	return (unsigned long)per_cpu(fixed_percpu_data.gs_base, cpu);
}

DECLARE_PER_CPU(unsigned int, irq_count);
extern asmlinkage void ignore_sysret(void);

#if IS_ENABLED(CONFIG_KVM)

void save_fsgs_for_kvm(void);
#endif
#else	
#ifdef CONFIG_STACKPROTECTOR

struct stack_canary {
	char __pad[20];		
	unsigned long canary;
};
DECLARE_PER_CPU_ALIGNED(struct stack_canary, stack_canary);
#endif

DECLARE_PER_CPU(struct irq_stack *, softirq_stack_ptr);
#endif	

extern unsigned int fpu_kernel_xstate_size;
extern unsigned int fpu_user_xstate_size;

struct perf_event;

typedef struct {
	unsigned long		seg;
} mm_segment_t;

struct thread_struct {
	
	struct desc_struct	tls_array[GDT_ENTRY_TLS_ENTRIES];
#ifdef CONFIG_X86_32
	unsigned long		sp0;
#endif
	unsigned long		sp;
#ifdef CONFIG_X86_32
	unsigned long		sysenter_cs;
#else
	unsigned short		es;
	unsigned short		ds;
	unsigned short		fsindex;
	unsigned short		gsindex;
#endif

#ifdef CONFIG_X86_64
	unsigned long		fsbase;
	unsigned long		gsbase;
#else
	
	unsigned long fs;
	unsigned long gs;
#endif

	
	struct perf_event	*ptrace_bps[HBP_NUM];
	
	unsigned long           debugreg6;
	
	unsigned long           ptrace_dr7;
	
	unsigned long		cr2;
	unsigned long		trap_nr;
	unsigned long		error_code;
#ifdef CONFIG_VM86
	
	struct vm86		*vm86;
#endif
	
	unsigned long		*io_bitmap_ptr;
	unsigned long		iopl;
	
	unsigned		io_bitmap_max;

	mm_segment_t		addr_limit;

	unsigned int		sig_on_uaccess_err:1;
	unsigned int		uaccess_err:1;	

	
	struct fpu		fpu;
	
};


static inline void arch_thread_struct_whitelist(unsigned long *offset,
						unsigned long *size)
{
	*offset = offsetof(struct thread_struct, fpu.state);
	*size = fpu_kernel_xstate_size;
}


#define TS_COMPAT		0x0002	


static inline void native_set_iopl_mask(unsigned mask)
{
#ifdef CONFIG_X86_32
	unsigned int reg;

	asm volatile ("pushfl;"
		      "popl %0;"
		      "andl %1, %0;"
		      "orl %2, %0;"
		      "pushl %0;"
		      "popfl"
		      : "=&r" (reg)
		      : "i" (~X86_EFLAGS_IOPL), "r" (mask));
#endif
}

static inline void
native_load_sp0(unsigned long sp0)
{
	this_cpu_write(cpu_tss_rw.x86_tss.sp0, sp0);
}

static inline void native_swapgs(void)
{
#ifdef CONFIG_X86_64
	asm volatile("swapgs" ::: "memory");
#endif
}

static inline unsigned long current_top_of_stack(void)
{
	
	return this_cpu_read_stable(cpu_current_top_of_stack);
}

static inline bool on_thread_stack(void)
{
	return (unsigned long)(current_top_of_stack() -
			       current_stack_pointer) < THREAD_SIZE;
}

#ifdef CONFIG_PARAVIRT_XXL
#include <asm/paravirt.h>
#else
#define __cpuid			native_cpuid

static inline void load_sp0(unsigned long sp0)
{
	native_load_sp0(sp0);
}

#define set_iopl_mask native_set_iopl_mask
#endif 


extern void release_thread(struct task_struct *);

unsigned long get_wchan(struct task_struct *p);


static inline void cpuid(unsigned int op,
			 unsigned int *eax, unsigned int *ebx,
			 unsigned int *ecx, unsigned int *edx)
{
	*eax = op;
	*ecx = 0;
	__cpuid(eax, ebx, ecx, edx);
}


static inline void cpuid_count(unsigned int op, int count,
			       unsigned int *eax, unsigned int *ebx,
			       unsigned int *ecx, unsigned int *edx)
{
	*eax = op;
	*ecx = count;
	__cpuid(eax, ebx, ecx, edx);
}


static inline unsigned int cpuid_eax(unsigned int op)
{
	unsigned int eax, ebx, ecx, edx;

	cpuid(op, &eax, &ebx, &ecx, &edx);

	return eax;
}

static inline unsigned int cpuid_ebx(unsigned int op)
{
	unsigned int eax, ebx, ecx, edx;

	cpuid(op, &eax, &ebx, &ecx, &edx);

	return ebx;
}

static inline unsigned int cpuid_ecx(unsigned int op)
{
	unsigned int eax, ebx, ecx, edx;

	cpuid(op, &eax, &ebx, &ecx, &edx);

	return ecx;
}

static inline unsigned int cpuid_edx(unsigned int op)
{
	unsigned int eax, ebx, ecx, edx;

	cpuid(op, &eax, &ebx, &ecx, &edx);

	return edx;
}


static __always_inline void rep_nop(void)
{
	asm volatile("rep; nop" ::: "memory");
}

static __always_inline void cpu_relax(void)
{
	rep_nop();
}


static inline void sync_core(void)
{
	
#ifdef CONFIG_X86_32
	asm volatile (
		"pushfl\n\t"
		"pushl %%cs\n\t"
		"pushl $1f\n\t"
		"iret\n\t"
		"1:"
		: ASM_CALL_CONSTRAINT : : "memory");
#else
	unsigned int tmp;

	asm volatile (
		UNWIND_HINT_SAVE
		"mov %%ss, %0\n\t"
		"pushq %q0\n\t"
		"pushq %%rsp\n\t"
		"addq $8, (%%rsp)\n\t"
		"pushfq\n\t"
		"mov %%cs, %0\n\t"
		"pushq %q0\n\t"
		"pushq $1f\n\t"
		"iretq\n\t"
		UNWIND_HINT_RESTORE
		"1:"
		: "=&r" (tmp), ASM_CALL_CONSTRAINT : : "cc", "memory");
#endif
}

extern void select_idle_routine(const struct cpuinfo_x86 *c);
extern void amd_e400_c1e_apic_setup(void);

extern unsigned long		boot_option_idle_override;

enum idle_boot_override {IDLE_NO_OVERRIDE=0, IDLE_HALT, IDLE_NOMWAIT,
			 IDLE_POLL};

extern void enable_sep_cpu(void);
extern int sysenter_setup(void);



extern struct desc_ptr		early_gdt_descr;

extern void switch_to_new_gdt(int);
extern void load_direct_gdt(int);
extern void load_fixmap_gdt(int);
extern void load_percpu_segment(int);
extern void cpu_init(void);
extern void cr4_init(void);

static inline unsigned long get_debugctlmsr(void)
{
	unsigned long debugctlmsr = 0;

#ifndef CONFIG_X86_DEBUGCTLMSR
	if (boot_cpu_data.x86 < 6)
		return 0;
#endif
	rdmsrl(MSR_IA32_DEBUGCTLMSR, debugctlmsr);

	return debugctlmsr;
}

static inline void update_debugctlmsr(unsigned long debugctlmsr)
{
#ifndef CONFIG_X86_DEBUGCTLMSR
	if (boot_cpu_data.x86 < 6)
		return;
#endif
	wrmsrl(MSR_IA32_DEBUGCTLMSR, debugctlmsr);
}

extern void set_task_blockstep(struct task_struct *task, bool on);


extern int			bootloader_type;
extern int			bootloader_version;

extern char			ignore_fpu_irq;

#define HAVE_ARCH_PICK_MMAP_LAYOUT 1
#define ARCH_HAS_PREFETCHW
#define ARCH_HAS_SPINLOCK_PREFETCH

#ifdef CONFIG_X86_32
# define BASE_PREFETCH		""
# define ARCH_HAS_PREFETCH
#else
# define BASE_PREFETCH		"prefetcht0 %P1"
#endif


static inline void prefetch(const void *x)
{
	alternative_input(BASE_PREFETCH, "prefetchnta %P1",
			  X86_FEATURE_XMM,
			  "m" (*(const char *)x));
}


static inline void prefetchw(const void *x)
{
	alternative_input(BASE_PREFETCH, "prefetchw %P1",
			  X86_FEATURE_3DNOWPREFETCH,
			  "m" (*(const char *)x));
}

static inline void spin_lock_prefetch(const void *x)
{
	prefetchw(x);
}

#define TOP_OF_INIT_STACK ((unsigned long)&init_stack + sizeof(init_stack) - \
			   TOP_OF_KERNEL_STACK_PADDING)

#define task_top_of_stack(task) ((unsigned long)(task_pt_regs(task) + 1))

#define task_pt_regs(task) \
({									\
	unsigned long __ptr = (unsigned long)task_stack_page(task);	\
	__ptr += THREAD_SIZE - TOP_OF_KERNEL_STACK_PADDING;		\
	((struct pt_regs *)__ptr) - 1;					\
})

#ifdef CONFIG_X86_32

#define IA32_PAGE_OFFSET	PAGE_OFFSET
#define TASK_SIZE		PAGE_OFFSET
#define TASK_SIZE_LOW		TASK_SIZE
#define TASK_SIZE_MAX		TASK_SIZE
#define DEFAULT_MAP_WINDOW	TASK_SIZE
#define STACK_TOP		TASK_SIZE
#define STACK_TOP_MAX		STACK_TOP

#define INIT_THREAD  {							  \
	.sp0			= TOP_OF_INIT_STACK,			  \
	.sysenter_cs		= __KERNEL_CS,				  \
	.io_bitmap_ptr		= NULL,					  \
	.addr_limit		= KERNEL_DS,				  \
}

#define KSTK_ESP(task)		(task_pt_regs(task)->sp)

#else

#define TASK_SIZE_MAX	((1UL << __VIRTUAL_MASK_SHIFT) - PAGE_SIZE)

#define DEFAULT_MAP_WINDOW	((1UL << 47) - PAGE_SIZE)


#define IA32_PAGE_OFFSET	((current->personality & ADDR_LIMIT_3GB) ? \
					0xc0000000 : 0xFFFFe000)

#define TASK_SIZE_LOW		(test_thread_flag(TIF_ADDR32) ? \
					IA32_PAGE_OFFSET : DEFAULT_MAP_WINDOW)
#define TASK_SIZE		(test_thread_flag(TIF_ADDR32) ? \
					IA32_PAGE_OFFSET : TASK_SIZE_MAX)
#define TASK_SIZE_OF(child)	((test_tsk_thread_flag(child, TIF_ADDR32)) ? \
					IA32_PAGE_OFFSET : TASK_SIZE_MAX)

#define STACK_TOP		TASK_SIZE_LOW
#define STACK_TOP_MAX		TASK_SIZE_MAX

#define INIT_THREAD  {						\
	.addr_limit		= KERNEL_DS,			\
}

extern unsigned long KSTK_ESP(struct task_struct *task);

#endif 

extern void start_thread(struct pt_regs *regs, unsigned long new_ip,
					       unsigned long new_sp);


#define __TASK_UNMAPPED_BASE(task_size)	(PAGE_ALIGN(task_size / 3))
#define TASK_UNMAPPED_BASE		__TASK_UNMAPPED_BASE(TASK_SIZE_LOW)

#define KSTK_EIP(task)		(task_pt_regs(task)->ip)


#define GET_TSC_CTL(adr)	get_tsc_mode((adr))
#define SET_TSC_CTL(val)	set_tsc_mode((val))

extern int get_tsc_mode(unsigned long adr);
extern int set_tsc_mode(unsigned int val);

DECLARE_PER_CPU(u64, msr_misc_features_shadow);


#define MPX_ENABLE_MANAGEMENT()	mpx_enable_management()
#define MPX_DISABLE_MANAGEMENT()	mpx_disable_management()

#ifdef CONFIG_X86_INTEL_MPX
extern int mpx_enable_management(void);
extern int mpx_disable_management(void);
#else
static inline int mpx_enable_management(void)
{
	return -EINVAL;
}
static inline int mpx_disable_management(void)
{
	return -EINVAL;
}
#endif 

#ifdef CONFIG_CPU_SUP_AMD
extern u16 amd_get_nb_id(int cpu);
extern u32 amd_get_nodes_per_socket(void);
#else
static inline u16 amd_get_nb_id(int cpu)		{ return 0; }
static inline u32 amd_get_nodes_per_socket(void)	{ return 0; }
#endif

static inline uint32_t hypervisor_cpuid_base(const char *sig, uint32_t leaves)
{
	uint32_t base, eax, signature[3];

	for (base = 0x40000000; base < 0x40010000; base += 0x100) {
		cpuid(base, &eax, &signature[0], &signature[1], &signature[2]);

		if (!memcmp(sig, signature, 12) &&
		    (leaves == 0 || ((eax - base) >= leaves)))
			return base;
	}

	return 0;
}

extern unsigned long arch_align_stack(unsigned long sp);
void free_init_pages(const char *what, unsigned long begin, unsigned long end);
extern void free_kernel_image_pages(void *begin, void *end);

void default_idle(void);
#ifdef	CONFIG_XEN
bool xen_set_default_idle(void);
#else
#define xen_set_default_idle 0
#endif

void stop_this_cpu(void *dummy);
void df_debug(struct pt_regs *regs, long error_code);
void microcode_check(void);

enum l1tf_mitigations {
	L1TF_MITIGATION_OFF,
	L1TF_MITIGATION_FLUSH_NOWARN,
	L1TF_MITIGATION_FLUSH,
	L1TF_MITIGATION_FLUSH_NOSMT,
	L1TF_MITIGATION_FULL,
	L1TF_MITIGATION_FULL_FORCE
};

extern enum l1tf_mitigations l1tf_mitigation;

enum mds_mitigations {
	MDS_MITIGATION_OFF,
	MDS_MITIGATION_FULL,
	MDS_MITIGATION_VMWERV,
};

#endif 
