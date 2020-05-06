/* SPDX-License-Identifier: GPL-2.0 */
#ifndef LINUX_CRASH_DUMP_H
#define LINUX_CRASH_DUMP_H

#include <linux/kexec.h>
#include <linux/proc_fs.h>
#include <linux/elf.h>
#include <uapi/linux/vmcore.h>

#include <asm/pgtable.h> 

#ifdef CONFIG_CRASH_DUMP
#define ELFCORE_ADDR_MAX	(-1ULL)
#define ELFCORE_ADDR_ERR	(-2ULL)

extern unsigned long long elfcorehdr_addr;
extern unsigned long long elfcorehdr_size;

extern int elfcorehdr_alloc(unsigned long long *addr, unsigned long long *size);
extern void elfcorehdr_free(unsigned long long addr);
extern ssize_t elfcorehdr_read(char *buf, size_t count, u64 *ppos);
extern ssize_t elfcorehdr_read_notes(char *buf, size_t count, u64 *ppos);
extern int remap_oldmem_pfn_range(struct vm_area_struct *vma,
				  unsigned long from, unsigned long pfn,
				  unsigned long size, pgprot_t prot);

extern ssize_t copy_oldmem_page(unsigned long, char *, size_t,
						unsigned long, int);
extern ssize_t copy_oldmem_page_encrypted(unsigned long pfn, char *buf,
					  size_t csize, unsigned long offset,
					  int userbuf);

void vmcore_cleanup(void);


#ifndef vmcore_elf_check_arch_cross
#define vmcore_elf_check_arch_cross(x) 0
#endif


#ifndef vmcore_elf32_check_arch
#define vmcore_elf32_check_arch(x) elf_check_arch(x)
#endif

#ifndef vmcore_elf64_check_arch
#define vmcore_elf64_check_arch(x) (elf_check_arch(x) || vmcore_elf_check_arch_cross(x))
#endif



static inline bool is_kdump_kernel(void)
{
	return elfcorehdr_addr != ELFCORE_ADDR_MAX;
}



static inline int is_vmcore_usable(void)
{
	return is_kdump_kernel() && elfcorehdr_addr != ELFCORE_ADDR_ERR ? 1 : 0;
}



static inline void vmcore_unusable(void)
{
	if (is_kdump_kernel())
		elfcorehdr_addr = ELFCORE_ADDR_ERR;
}

#define HAVE_OLDMEM_PFN_IS_RAM 1
extern int register_oldmem_pfn_is_ram(int (*fn)(unsigned long pfn));
extern void unregister_oldmem_pfn_is_ram(void);

#else 
static inline bool is_kdump_kernel(void) { return 0; }
#endif 

extern unsigned long saved_max_pfn;


struct vmcoredd_data {
	char dump_name[VMCOREDD_MAX_NAME_BYTES]; 
	unsigned int size;                       
	
	int (*vmcoredd_callback)(struct vmcoredd_data *data, void *buf);
};

#ifdef CONFIG_PROC_VMCORE_DEVICE_DUMP
int vmcore_add_device_dump(struct vmcoredd_data *data);
#else
static inline int vmcore_add_device_dump(struct vmcoredd_data *data)
{
	return -EOPNOTSUPP;
}
#endif 
#endif 
