/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef _ASM_X86_UV_BIOS_H
#define _ASM_X86_UV_BIOS_H



#include <linux/rtc.h>


enum uv_bios_cmd {
	UV_BIOS_COMMON,
	UV_BIOS_GET_SN_INFO,
	UV_BIOS_FREQ_BASE,
	UV_BIOS_WATCHLIST_ALLOC,
	UV_BIOS_WATCHLIST_FREE,
	UV_BIOS_MEMPROTECT,
	UV_BIOS_GET_PARTITION_ADDR,
	UV_BIOS_SET_LEGACY_VGA_TARGET
};


enum {
	BIOS_STATUS_MORE_PASSES		=  1,
	BIOS_STATUS_SUCCESS		=  0,
	BIOS_STATUS_UNIMPLEMENTED	= -ENOSYS,
	BIOS_STATUS_EINVAL		= -EINVAL,
	BIOS_STATUS_UNAVAIL		= -EBUSY,
	BIOS_STATUS_ABORT		= -EINTR,
};


struct uv_gam_parameters {
	u64	mmr_base;
	u64	gru_base;
	u8	mmr_shift;	
	u8	gru_shift;	
	u8	gpa_shift;	
	u8	unused1;
};


#define UV_GAM_RANGE_TYPE_UNUSED	0 
#define UV_GAM_RANGE_TYPE_RAM		1 
#define UV_GAM_RANGE_TYPE_NVRAM		2 
#define UV_GAM_RANGE_TYPE_NV_WINDOW	3 
#define UV_GAM_RANGE_TYPE_NV_MAILBOX	4 
#define UV_GAM_RANGE_TYPE_HOLE		5 
#define UV_GAM_RANGE_TYPE_MAX		6


#define UV_GAM_RANGE_SHFT		26		

struct uv_gam_range_entry {
	char	type;		
	char	unused1;
	u16	nasid;		
	u16	sockid;		
	u16	pnode;		
	u32	unused2;
	u32	limit;		
};

#define	UV_SYSTAB_SIG			"UVST"
#define	UV_SYSTAB_VERSION_1		1	
#define	UV_SYSTAB_VERSION_UV4		0x400	
#define	UV_SYSTAB_VERSION_UV4_1		0x401	
#define	UV_SYSTAB_VERSION_UV4_2		0x402	
#define	UV_SYSTAB_VERSION_UV4_3		0x403	
#define	UV_SYSTAB_VERSION_UV4_LATEST	UV_SYSTAB_VERSION_UV4_3

#define	UV_SYSTAB_TYPE_UNUSED		0	
#define	UV_SYSTAB_TYPE_GAM_PARAMS	1	
#define	UV_SYSTAB_TYPE_GAM_RNG_TBL	2	
#define	UV_SYSTAB_TYPE_MAX		3


struct uv_systab {
	char signature[4];	
	u32 revision;		
	u64 function;		
	u32 size;		
	struct {
		u32 type:8;	
		u32 offset:24;	
	} entry[1];		
};
extern struct uv_systab *uv_systab;


enum {
	BIOS_FREQ_BASE_PLATFORM = 0,
	BIOS_FREQ_BASE_INTERVAL_TIMER = 1,
	BIOS_FREQ_BASE_REALTIME_CLOCK = 2
};

union partition_info_u {
	u64	val;
	struct {
		u64	hub_version	:  8,
			partition_id	: 16,
			coherence_id	: 16,
			region_size	: 24;
	};
};

enum uv_memprotect {
	UV_MEMPROT_RESTRICT_ACCESS,
	UV_MEMPROT_ALLOW_AMO,
	UV_MEMPROT_ALLOW_RW
};


extern s64 uv_bios_call(enum uv_bios_cmd, u64, u64, u64, u64, u64);
extern s64 uv_bios_call_irqsave(enum uv_bios_cmd, u64, u64, u64, u64, u64);

extern s64 uv_bios_get_sn_info(int, int *, long *, long *, long *, long *);
extern s64 uv_bios_freq_base(u64, u64 *);
extern int uv_bios_mq_watchlist_alloc(unsigned long, unsigned int,
					unsigned long *);
extern int uv_bios_mq_watchlist_free(int, int);
extern s64 uv_bios_change_memprotect(u64, u64, enum uv_memprotect);
extern s64 uv_bios_reserved_page_pa(u64, u64 *, u64 *, u64 *);
extern int uv_bios_set_legacy_vga_target(bool decode, int domain, int bus);

extern void uv_bios_init(void);

extern unsigned long sn_rtc_cycles_per_second;
extern int uv_type;
extern long sn_partition_id;
extern long sn_coherency_id;
extern long sn_region_size;
extern long system_serial_number;
#define uv_partition_coherence_id()	(sn_coherency_id)

extern struct kobject *sgi_uv_kobj;	


extern struct semaphore __efi_uv_runtime_lock;

#endif 
