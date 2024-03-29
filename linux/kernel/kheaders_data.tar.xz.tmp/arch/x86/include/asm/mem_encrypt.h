/* SPDX-License-Identifier: GPL-2.0-only */


#ifndef __X86_MEM_ENCRYPT_H__
#define __X86_MEM_ENCRYPT_H__

#ifndef __ASSEMBLY__

#include <linux/init.h>

#include <asm/bootparam.h>

#ifdef CONFIG_AMD_MEM_ENCRYPT

extern u64 sme_me_mask;
extern bool sev_enabled;

void sme_encrypt_execute(unsigned long encrypted_kernel_vaddr,
			 unsigned long decrypted_kernel_vaddr,
			 unsigned long kernel_len,
			 unsigned long encryption_wa,
			 unsigned long encryption_pgd);

void __init sme_early_encrypt(resource_size_t paddr,
			      unsigned long size);
void __init sme_early_decrypt(resource_size_t paddr,
			      unsigned long size);

void __init sme_map_bootdata(char *real_mode_data);
void __init sme_unmap_bootdata(char *real_mode_data);

void __init sme_early_init(void);

void __init sme_encrypt_kernel(struct boot_params *bp);
void __init sme_enable(struct boot_params *bp);

int __init early_set_memory_decrypted(unsigned long vaddr, unsigned long size);
int __init early_set_memory_encrypted(unsigned long vaddr, unsigned long size);


void __init mem_encrypt_init(void);
void __init mem_encrypt_free_decrypted_mem(void);

bool sme_active(void);
bool sev_active(void);

#define __bss_decrypted __attribute__((__section__(".bss..decrypted")))

#else	

#define sme_me_mask	0ULL

static inline void __init sme_early_encrypt(resource_size_t paddr,
					    unsigned long size) { }
static inline void __init sme_early_decrypt(resource_size_t paddr,
					    unsigned long size) { }

static inline void __init sme_map_bootdata(char *real_mode_data) { }
static inline void __init sme_unmap_bootdata(char *real_mode_data) { }

static inline void __init sme_early_init(void) { }

static inline void __init sme_encrypt_kernel(struct boot_params *bp) { }
static inline void __init sme_enable(struct boot_params *bp) { }

static inline bool sme_active(void) { return false; }
static inline bool sev_active(void) { return false; }

static inline int __init
early_set_memory_decrypted(unsigned long vaddr, unsigned long size) { return 0; }
static inline int __init
early_set_memory_encrypted(unsigned long vaddr, unsigned long size) { return 0; }

#define __bss_decrypted

#endif	


#define __sme_pa(x)		(__pa(x) | sme_me_mask)
#define __sme_pa_nodebug(x)	(__pa_nodebug(x) | sme_me_mask)

extern char __start_bss_decrypted[], __end_bss_decrypted[], __start_bss_decrypted_unused[];

#endif	

#endif	
