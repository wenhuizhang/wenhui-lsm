/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_CPUFEATURES_H
#define _ASM_X86_CPUFEATURES_H

#ifndef _ASM_X86_REQUIRED_FEATURES_H
#include <asm/required-features.h>
#endif

#ifndef _ASM_X86_DISABLED_FEATURES_H
#include <asm/disabled-features.h>
#endif


#define NCAPINTS			19	   
#define NBUGINTS			1	   




#define X86_FEATURE_FPU			( 0*32+ 0) 
#define X86_FEATURE_VME			( 0*32+ 1) 
#define X86_FEATURE_DE			( 0*32+ 2) 
#define X86_FEATURE_PSE			( 0*32+ 3) 
#define X86_FEATURE_TSC			( 0*32+ 4) 
#define X86_FEATURE_MSR			( 0*32+ 5) 
#define X86_FEATURE_PAE			( 0*32+ 6) 
#define X86_FEATURE_MCE			( 0*32+ 7) 
#define X86_FEATURE_CX8			( 0*32+ 8) 
#define X86_FEATURE_APIC		( 0*32+ 9) 
#define X86_FEATURE_SEP			( 0*32+11) 
#define X86_FEATURE_MTRR		( 0*32+12) 
#define X86_FEATURE_PGE			( 0*32+13) 
#define X86_FEATURE_MCA			( 0*32+14) 
#define X86_FEATURE_CMOV		( 0*32+15) 
#define X86_FEATURE_PAT			( 0*32+16) 
#define X86_FEATURE_PSE36		( 0*32+17) 
#define X86_FEATURE_PN			( 0*32+18) 
#define X86_FEATURE_CLFLUSH		( 0*32+19) 
#define X86_FEATURE_DS			( 0*32+21) 
#define X86_FEATURE_ACPI		( 0*32+22) 
#define X86_FEATURE_MMX			( 0*32+23) 
#define X86_FEATURE_FXSR		( 0*32+24) 
#define X86_FEATURE_XMM			( 0*32+25) 
#define X86_FEATURE_XMM2		( 0*32+26) 
#define X86_FEATURE_SELFSNOOP		( 0*32+27) 
#define X86_FEATURE_HT			( 0*32+28) 
#define X86_FEATURE_ACC			( 0*32+29) 
#define X86_FEATURE_IA64		( 0*32+30) 
#define X86_FEATURE_PBE			( 0*32+31) 



#define X86_FEATURE_SYSCALL		( 1*32+11) 
#define X86_FEATURE_MP			( 1*32+19) 
#define X86_FEATURE_NX			( 1*32+20) 
#define X86_FEATURE_MMXEXT		( 1*32+22) 
#define X86_FEATURE_FXSR_OPT		( 1*32+25) 
#define X86_FEATURE_GBPAGES		( 1*32+26) 
#define X86_FEATURE_RDTSCP		( 1*32+27) 
#define X86_FEATURE_LM			( 1*32+29) 
#define X86_FEATURE_3DNOWEXT		( 1*32+30) 
#define X86_FEATURE_3DNOW		( 1*32+31) 


#define X86_FEATURE_RECOVERY		( 2*32+ 0) 
#define X86_FEATURE_LONGRUN		( 2*32+ 1) 
#define X86_FEATURE_LRTI		( 2*32+ 3) 



#define X86_FEATURE_CXMMX		( 3*32+ 0) 
#define X86_FEATURE_K6_MTRR		( 3*32+ 1) 
#define X86_FEATURE_CYRIX_ARR		( 3*32+ 2) 
#define X86_FEATURE_CENTAUR_MCR		( 3*32+ 3) 


#define X86_FEATURE_K8			( 3*32+ 4) 
#define X86_FEATURE_K7			( 3*32+ 5) 
#define X86_FEATURE_P3			( 3*32+ 6) 
#define X86_FEATURE_P4			( 3*32+ 7) 
#define X86_FEATURE_CONSTANT_TSC	( 3*32+ 8) 
#define X86_FEATURE_UP			( 3*32+ 9) 
#define X86_FEATURE_ART			( 3*32+10) 
#define X86_FEATURE_ARCH_PERFMON	( 3*32+11) 
#define X86_FEATURE_PEBS		( 3*32+12) 
#define X86_FEATURE_BTS			( 3*32+13) 
#define X86_FEATURE_SYSCALL32		( 3*32+14) 
#define X86_FEATURE_SYSENTER32		( 3*32+15) 
#define X86_FEATURE_REP_GOOD		( 3*32+16) 
#define X86_FEATURE_MFENCE_RDTSC	( 3*32+17) 
#define X86_FEATURE_LFENCE_RDTSC	( 3*32+18) 
#define X86_FEATURE_ACC_POWER		( 3*32+19) 
#define X86_FEATURE_NOPL		( 3*32+20) 
#define X86_FEATURE_ALWAYS		( 3*32+21) 
#define X86_FEATURE_XTOPOLOGY		( 3*32+22) 
#define X86_FEATURE_TSC_RELIABLE	( 3*32+23) 
#define X86_FEATURE_NONSTOP_TSC		( 3*32+24) 
#define X86_FEATURE_CPUID		( 3*32+25) 
#define X86_FEATURE_EXTD_APICID		( 3*32+26) 
#define X86_FEATURE_AMD_DCM		( 3*32+27) 
#define X86_FEATURE_APERFMPERF		( 3*32+28) 
#define X86_FEATURE_NONSTOP_TSC_S3	( 3*32+30) 
#define X86_FEATURE_TSC_KNOWN_FREQ	( 3*32+31) 


#define X86_FEATURE_XMM3		( 4*32+ 0) 
#define X86_FEATURE_PCLMULQDQ		( 4*32+ 1) 
#define X86_FEATURE_DTES64		( 4*32+ 2) 
#define X86_FEATURE_MWAIT		( 4*32+ 3) 
#define X86_FEATURE_DSCPL		( 4*32+ 4) 
#define X86_FEATURE_VMX			( 4*32+ 5) 
#define X86_FEATURE_SMX			( 4*32+ 6) 
#define X86_FEATURE_EST			( 4*32+ 7) 
#define X86_FEATURE_TM2			( 4*32+ 8) 
#define X86_FEATURE_SSSE3		( 4*32+ 9) 
#define X86_FEATURE_CID			( 4*32+10) 
#define X86_FEATURE_SDBG		( 4*32+11) 
#define X86_FEATURE_FMA			( 4*32+12) 
#define X86_FEATURE_CX16		( 4*32+13) 
#define X86_FEATURE_XTPR		( 4*32+14) 
#define X86_FEATURE_PDCM		( 4*32+15) 
#define X86_FEATURE_PCID		( 4*32+17) 
#define X86_FEATURE_DCA			( 4*32+18) 
#define X86_FEATURE_XMM4_1		( 4*32+19) 
#define X86_FEATURE_XMM4_2		( 4*32+20) 
#define X86_FEATURE_X2APIC		( 4*32+21) 
#define X86_FEATURE_MOVBE		( 4*32+22) 
#define X86_FEATURE_POPCNT		( 4*32+23) 
#define X86_FEATURE_TSC_DEADLINE_TIMER	( 4*32+24) 
#define X86_FEATURE_AES			( 4*32+25) 
#define X86_FEATURE_XSAVE		( 4*32+26) 
#define X86_FEATURE_OSXSAVE		( 4*32+27) 
#define X86_FEATURE_AVX			( 4*32+28) 
#define X86_FEATURE_F16C		( 4*32+29) 
#define X86_FEATURE_RDRAND		( 4*32+30) 
#define X86_FEATURE_HYPERVISOR		( 4*32+31) 


#define X86_FEATURE_XSTORE		( 5*32+ 2) 
#define X86_FEATURE_XSTORE_EN		( 5*32+ 3) 
#define X86_FEATURE_XCRYPT		( 5*32+ 6) 
#define X86_FEATURE_XCRYPT_EN		( 5*32+ 7) 
#define X86_FEATURE_ACE2		( 5*32+ 8) 
#define X86_FEATURE_ACE2_EN		( 5*32+ 9) 
#define X86_FEATURE_PHE			( 5*32+10) 
#define X86_FEATURE_PHE_EN		( 5*32+11) 
#define X86_FEATURE_PMM			( 5*32+12) 
#define X86_FEATURE_PMM_EN		( 5*32+13) 


#define X86_FEATURE_LAHF_LM		( 6*32+ 0) 
#define X86_FEATURE_CMP_LEGACY		( 6*32+ 1) 
#define X86_FEATURE_SVM			( 6*32+ 2) 
#define X86_FEATURE_EXTAPIC		( 6*32+ 3) 
#define X86_FEATURE_CR8_LEGACY		( 6*32+ 4) 
#define X86_FEATURE_ABM			( 6*32+ 5) 
#define X86_FEATURE_SSE4A		( 6*32+ 6) 
#define X86_FEATURE_MISALIGNSSE		( 6*32+ 7) 
#define X86_FEATURE_3DNOWPREFETCH	( 6*32+ 8) 
#define X86_FEATURE_OSVW		( 6*32+ 9) 
#define X86_FEATURE_IBS			( 6*32+10) 
#define X86_FEATURE_XOP			( 6*32+11) 
#define X86_FEATURE_SKINIT		( 6*32+12) 
#define X86_FEATURE_WDT			( 6*32+13) 
#define X86_FEATURE_LWP			( 6*32+15) 
#define X86_FEATURE_FMA4		( 6*32+16) 
#define X86_FEATURE_TCE			( 6*32+17) 
#define X86_FEATURE_NODEID_MSR		( 6*32+19) 
#define X86_FEATURE_TBM			( 6*32+21) 
#define X86_FEATURE_TOPOEXT		( 6*32+22) 
#define X86_FEATURE_PERFCTR_CORE	( 6*32+23) 
#define X86_FEATURE_PERFCTR_NB		( 6*32+24) 
#define X86_FEATURE_BPEXT		( 6*32+26) 
#define X86_FEATURE_PTSC		( 6*32+27) 
#define X86_FEATURE_PERFCTR_LLC		( 6*32+28) 
#define X86_FEATURE_MWAITX		( 6*32+29) 


#define X86_FEATURE_RING3MWAIT		( 7*32+ 0) 
#define X86_FEATURE_CPUID_FAULT		( 7*32+ 1) 
#define X86_FEATURE_CPB			( 7*32+ 2) 
#define X86_FEATURE_EPB			( 7*32+ 3) 
#define X86_FEATURE_CAT_L3		( 7*32+ 4) 
#define X86_FEATURE_CAT_L2		( 7*32+ 5) 
#define X86_FEATURE_CDP_L3		( 7*32+ 6) 
#define X86_FEATURE_INVPCID_SINGLE	( 7*32+ 7) 
#define X86_FEATURE_HW_PSTATE		( 7*32+ 8) 
#define X86_FEATURE_PROC_FEEDBACK	( 7*32+ 9) 
#define X86_FEATURE_SME			( 7*32+10) 
#define X86_FEATURE_PTI			( 7*32+11) 
#define X86_FEATURE_RETPOLINE		( 7*32+12) 
#define X86_FEATURE_RETPOLINE_AMD	( 7*32+13) 
#define X86_FEATURE_INTEL_PPIN		( 7*32+14) 
#define X86_FEATURE_CDP_L2		( 7*32+15) 
#define X86_FEATURE_MSR_SPEC_CTRL	( 7*32+16) 
#define X86_FEATURE_SSBD		( 7*32+17) 
#define X86_FEATURE_MBA			( 7*32+18) 
#define X86_FEATURE_RSB_CTXSW		( 7*32+19) 
#define X86_FEATURE_SEV			( 7*32+20) 
#define X86_FEATURE_USE_IBPB		( 7*32+21) 
#define X86_FEATURE_USE_IBRS_FW		( 7*32+22) 
#define X86_FEATURE_SPEC_STORE_BYPASS_DISABLE	( 7*32+23) 
#define X86_FEATURE_LS_CFG_SSBD		( 7*32+24)  
#define X86_FEATURE_IBRS		( 7*32+25) 
#define X86_FEATURE_IBPB		( 7*32+26) 
#define X86_FEATURE_STIBP		( 7*32+27) 
#define X86_FEATURE_ZEN			( 7*32+28) 
#define X86_FEATURE_L1TF_PTEINV		( 7*32+29) 
#define X86_FEATURE_IBRS_ENHANCED	( 7*32+30) 


#define X86_FEATURE_TPR_SHADOW		( 8*32+ 0) 
#define X86_FEATURE_VNMI		( 8*32+ 1) 
#define X86_FEATURE_FLEXPRIORITY	( 8*32+ 2) 
#define X86_FEATURE_EPT			( 8*32+ 3) 
#define X86_FEATURE_VPID		( 8*32+ 4) 

#define X86_FEATURE_VMMCALL		( 8*32+15) 
#define X86_FEATURE_XENPV		( 8*32+16) 
#define X86_FEATURE_EPT_AD		( 8*32+17) 


#define X86_FEATURE_FSGSBASE		( 9*32+ 0) 
#define X86_FEATURE_TSC_ADJUST		( 9*32+ 1) 
#define X86_FEATURE_BMI1		( 9*32+ 3) 
#define X86_FEATURE_HLE			( 9*32+ 4) 
#define X86_FEATURE_AVX2		( 9*32+ 5) 
#define X86_FEATURE_FDP_EXCPTN_ONLY	( 9*32+ 6) 
#define X86_FEATURE_SMEP		( 9*32+ 7) 
#define X86_FEATURE_BMI2		( 9*32+ 8) 
#define X86_FEATURE_ERMS		( 9*32+ 9) 
#define X86_FEATURE_INVPCID		( 9*32+10) 
#define X86_FEATURE_RTM			( 9*32+11) 
#define X86_FEATURE_CQM			( 9*32+12) 
#define X86_FEATURE_ZERO_FCS_FDS	( 9*32+13) 
#define X86_FEATURE_MPX			( 9*32+14) 
#define X86_FEATURE_RDT_A		( 9*32+15) 
#define X86_FEATURE_AVX512F		( 9*32+16) 
#define X86_FEATURE_AVX512DQ		( 9*32+17) 
#define X86_FEATURE_RDSEED		( 9*32+18) 
#define X86_FEATURE_ADX			( 9*32+19) 
#define X86_FEATURE_SMAP		( 9*32+20) 
#define X86_FEATURE_AVX512IFMA		( 9*32+21) 
#define X86_FEATURE_CLFLUSHOPT		( 9*32+23) 
#define X86_FEATURE_CLWB		( 9*32+24) 
#define X86_FEATURE_INTEL_PT		( 9*32+25) 
#define X86_FEATURE_AVX512PF		( 9*32+26) 
#define X86_FEATURE_AVX512ER		( 9*32+27) 
#define X86_FEATURE_AVX512CD		( 9*32+28) 
#define X86_FEATURE_SHA_NI		( 9*32+29) 
#define X86_FEATURE_AVX512BW		( 9*32+30) 
#define X86_FEATURE_AVX512VL		( 9*32+31) 


#define X86_FEATURE_XSAVEOPT		(10*32+ 0) 
#define X86_FEATURE_XSAVEC		(10*32+ 1) 
#define X86_FEATURE_XGETBV1		(10*32+ 2) 
#define X86_FEATURE_XSAVES		(10*32+ 3) 


#define X86_FEATURE_CQM_LLC		(11*32+ 0) 
#define X86_FEATURE_CQM_OCCUP_LLC	(11*32+ 1) 
#define X86_FEATURE_CQM_MBM_TOTAL	(11*32+ 2) 
#define X86_FEATURE_CQM_MBM_LOCAL	(11*32+ 3) 
#define X86_FEATURE_FENCE_SWAPGS_USER	(11*32+ 4) 
#define X86_FEATURE_FENCE_SWAPGS_KERNEL	(11*32+ 5) 


#define X86_FEATURE_AVX512_BF16		(12*32+ 5) 


#define X86_FEATURE_CLZERO		(13*32+ 0) 
#define X86_FEATURE_IRPERF		(13*32+ 1) 
#define X86_FEATURE_XSAVEERPTR		(13*32+ 2) 
#define X86_FEATURE_WBNOINVD		(13*32+ 9) 
#define X86_FEATURE_AMD_IBPB		(13*32+12) 
#define X86_FEATURE_AMD_IBRS		(13*32+14) 
#define X86_FEATURE_AMD_STIBP		(13*32+15) 
#define X86_FEATURE_AMD_STIBP_ALWAYS_ON	(13*32+17) 
#define X86_FEATURE_AMD_SSBD		(13*32+24) 
#define X86_FEATURE_VIRT_SSBD		(13*32+25) 
#define X86_FEATURE_AMD_SSB_NO		(13*32+26) 


#define X86_FEATURE_DTHERM		(14*32+ 0) 
#define X86_FEATURE_IDA			(14*32+ 1) 
#define X86_FEATURE_ARAT		(14*32+ 2) 
#define X86_FEATURE_PLN			(14*32+ 4) 
#define X86_FEATURE_PTS			(14*32+ 6) 
#define X86_FEATURE_HWP			(14*32+ 7) 
#define X86_FEATURE_HWP_NOTIFY		(14*32+ 8) 
#define X86_FEATURE_HWP_ACT_WINDOW	(14*32+ 9) 
#define X86_FEATURE_HWP_EPP		(14*32+10) 
#define X86_FEATURE_HWP_PKG_REQ		(14*32+11) 


#define X86_FEATURE_NPT			(15*32+ 0) 
#define X86_FEATURE_LBRV		(15*32+ 1) 
#define X86_FEATURE_SVML		(15*32+ 2) 
#define X86_FEATURE_NRIPS		(15*32+ 3) 
#define X86_FEATURE_TSCRATEMSR		(15*32+ 4) 
#define X86_FEATURE_VMCBCLEAN		(15*32+ 5) 
#define X86_FEATURE_FLUSHBYASID		(15*32+ 6) 
#define X86_FEATURE_DECODEASSISTS	(15*32+ 7) 
#define X86_FEATURE_PAUSEFILTER		(15*32+10) 
#define X86_FEATURE_PFTHRESHOLD		(15*32+12) 
#define X86_FEATURE_AVIC		(15*32+13) 
#define X86_FEATURE_V_VMSAVE_VMLOAD	(15*32+15) 
#define X86_FEATURE_VGIF		(15*32+16) 


#define X86_FEATURE_AVX512VBMI		(16*32+ 1) 
#define X86_FEATURE_UMIP		(16*32+ 2) 
#define X86_FEATURE_PKU			(16*32+ 3) 
#define X86_FEATURE_OSPKE		(16*32+ 4) 
#define X86_FEATURE_WAITPKG		(16*32+ 5) 
#define X86_FEATURE_AVX512_VBMI2	(16*32+ 6) 
#define X86_FEATURE_GFNI		(16*32+ 8) 
#define X86_FEATURE_VAES		(16*32+ 9) 
#define X86_FEATURE_VPCLMULQDQ		(16*32+10) 
#define X86_FEATURE_AVX512_VNNI		(16*32+11) 
#define X86_FEATURE_AVX512_BITALG	(16*32+12) 
#define X86_FEATURE_TME			(16*32+13) 
#define X86_FEATURE_AVX512_VPOPCNTDQ	(16*32+14) 
#define X86_FEATURE_LA57		(16*32+16) 
#define X86_FEATURE_RDPID		(16*32+22) 
#define X86_FEATURE_CLDEMOTE		(16*32+25) 
#define X86_FEATURE_MOVDIRI		(16*32+27) 
#define X86_FEATURE_MOVDIR64B		(16*32+28) 


#define X86_FEATURE_OVERFLOW_RECOV	(17*32+ 0) 
#define X86_FEATURE_SUCCOR		(17*32+ 1) 
#define X86_FEATURE_SMCA		(17*32+ 3) 


#define X86_FEATURE_AVX512_4VNNIW	(18*32+ 2) 
#define X86_FEATURE_AVX512_4FMAPS	(18*32+ 3) 
#define X86_FEATURE_MD_CLEAR		(18*32+10) 
#define X86_FEATURE_TSX_FORCE_ABORT	(18*32+13) 
#define X86_FEATURE_PCONFIG		(18*32+18) 
#define X86_FEATURE_SPEC_CTRL		(18*32+26) 
#define X86_FEATURE_INTEL_STIBP		(18*32+27) 
#define X86_FEATURE_FLUSH_L1D		(18*32+28) 
#define X86_FEATURE_ARCH_CAPABILITIES	(18*32+29) 
#define X86_FEATURE_SPEC_CTRL_SSBD	(18*32+31) 


#define X86_BUG(x)			(NCAPINTS*32 + (x))

#define X86_BUG_F00F			X86_BUG(0) 
#define X86_BUG_FDIV			X86_BUG(1) 
#define X86_BUG_COMA			X86_BUG(2) 
#define X86_BUG_AMD_TLB_MMATCH		X86_BUG(3) 
#define X86_BUG_AMD_APIC_C1E		X86_BUG(4) 
#define X86_BUG_11AP			X86_BUG(5) 
#define X86_BUG_FXSAVE_LEAK		X86_BUG(6) 
#define X86_BUG_CLFLUSH_MONITOR		X86_BUG(7) 
#define X86_BUG_SYSRET_SS_ATTRS		X86_BUG(8) 
#ifdef CONFIG_X86_32

#define X86_BUG_ESPFIX			X86_BUG(9) 
#endif
#define X86_BUG_NULL_SEG		X86_BUG(10) 
#define X86_BUG_SWAPGS_FENCE		X86_BUG(11) 
#define X86_BUG_MONITOR			X86_BUG(12) 
#define X86_BUG_AMD_E400		X86_BUG(13) 
#define X86_BUG_CPU_MELTDOWN		X86_BUG(14) 
#define X86_BUG_SPECTRE_V1		X86_BUG(15) 
#define X86_BUG_SPECTRE_V2		X86_BUG(16) 
#define X86_BUG_SPEC_STORE_BYPASS	X86_BUG(17) 
#define X86_BUG_L1TF			X86_BUG(18) 
#define X86_BUG_MDS			X86_BUG(19) 
#define X86_BUG_MSBDS_ONLY		X86_BUG(20) 
#define X86_BUG_SWAPGS			X86_BUG(21) 

#endif 