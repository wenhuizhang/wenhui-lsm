/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_BINFMTS_H
#define _LINUX_BINFMTS_H

#include <linux/sched.h>
#include <linux/unistd.h>
#include <asm/exec.h>
#include <uapi/linux/binfmts.h>

struct filename;

#define CORENAME_MAX_SIZE 128


struct linux_binprm {
#ifdef CONFIG_MMU
	struct vm_area_struct *vma;
	unsigned long vma_pages;
#else
# define MAX_ARG_PAGES	32
	struct page *page[MAX_ARG_PAGES];
#endif
	struct mm_struct *mm;
	unsigned long p; 
	unsigned long argmin; 
	unsigned int
		
		called_set_creds:1,
		
		cap_elevated:1,
		
		secureexec:1;
#ifdef __alpha__
	unsigned int taso:1;
#endif
	unsigned int recursion_depth; 
	struct file * file;
	struct cred *cred;	
	int unsafe;		
	unsigned int per_clear;	
	int argc, envc;
	const char * filename;	
	const char * interp;	
	unsigned interp_flags;
	unsigned interp_data;
	unsigned long loader, exec;

	struct rlimit rlim_stack; 

	char buf[BINPRM_BUF_SIZE];
} __randomize_layout;

#define BINPRM_FLAGS_ENFORCE_NONDUMP_BIT 0
#define BINPRM_FLAGS_ENFORCE_NONDUMP (1 << BINPRM_FLAGS_ENFORCE_NONDUMP_BIT)


#define BINPRM_FLAGS_EXECFD_BIT 1
#define BINPRM_FLAGS_EXECFD (1 << BINPRM_FLAGS_EXECFD_BIT)


#define BINPRM_FLAGS_PATH_INACCESSIBLE_BIT 2
#define BINPRM_FLAGS_PATH_INACCESSIBLE (1 << BINPRM_FLAGS_PATH_INACCESSIBLE_BIT)


struct coredump_params {
	const kernel_siginfo_t *siginfo;
	struct pt_regs *regs;
	struct file *file;
	unsigned long limit;
	unsigned long mm_flags;
	loff_t written;
	loff_t pos;
};


struct linux_binfmt {
	struct list_head lh;
	struct module *module;
	int (*load_binary)(struct linux_binprm *);
	int (*load_shlib)(struct file *);
	int (*core_dump)(struct coredump_params *cprm);
	unsigned long min_coredump;	
} __randomize_layout;

extern void __register_binfmt(struct linux_binfmt *fmt, int insert);


static inline void register_binfmt(struct linux_binfmt *fmt)
{
	__register_binfmt(fmt, 0);
}

static inline void insert_binfmt(struct linux_binfmt *fmt)
{
	__register_binfmt(fmt, 1);
}

extern void unregister_binfmt(struct linux_binfmt *);

extern int prepare_binprm(struct linux_binprm *);
extern int __must_check remove_arg_zero(struct linux_binprm *);
extern int search_binary_handler(struct linux_binprm *);
extern int flush_old_exec(struct linux_binprm * bprm);
extern void setup_new_exec(struct linux_binprm * bprm);
extern void finalize_exec(struct linux_binprm *bprm);
extern void would_dump(struct linux_binprm *, struct file *);

extern int suid_dumpable;


#define EXSTACK_DEFAULT   0	
#define EXSTACK_DISABLE_X 1	
#define EXSTACK_ENABLE_X  2	

extern int setup_arg_pages(struct linux_binprm * bprm,
			   unsigned long stack_top,
			   int executable_stack);
extern int transfer_args_to_stack(struct linux_binprm *bprm,
				  unsigned long *sp_location);
extern int bprm_change_interp(const char *interp, struct linux_binprm *bprm);
extern int copy_strings_kernel(int argc, const char *const *argv,
			       struct linux_binprm *bprm);
extern void install_exec_creds(struct linux_binprm *bprm);
extern void set_binfmt(struct linux_binfmt *new);
extern ssize_t read_code(struct file *, unsigned long, loff_t, size_t);

extern int do_execve(struct filename *,
		     const char __user * const __user *,
		     const char __user * const __user *);
extern int do_execveat(int, struct filename *,
		       const char __user * const __user *,
		       const char __user * const __user *,
		       int);
int do_execve_file(struct file *file, void *__argv, void *__envp);

#endif 
