/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_COMPAT_H
#define _LINUX_COMPAT_H


#include <linux/types.h>
#include <linux/time.h>

#include <linux/stat.h>
#include <linux/param.h>	
#include <linux/sem.h>
#include <linux/socket.h>
#include <linux/if.h>
#include <linux/fs.h>
#include <linux/aio_abi.h>	
#include <linux/uaccess.h>
#include <linux/unistd.h>

#include <asm/compat.h>

#ifdef CONFIG_COMPAT
#include <asm/siginfo.h>
#include <asm/signal.h>
#endif

#ifdef CONFIG_ARCH_HAS_SYSCALL_WRAPPER

#include <asm/syscall_wrapper.h>
#endif 

#ifndef COMPAT_USE_64BIT_TIME
#define COMPAT_USE_64BIT_TIME 0
#endif

#ifndef __SC_DELOUSE
#define __SC_DELOUSE(t,v) ((__force t)(unsigned long)(v))
#endif

#ifndef COMPAT_SYSCALL_DEFINE0
#define COMPAT_SYSCALL_DEFINE0(name) \
	asmlinkage long compat_sys_##name(void); \
	ALLOW_ERROR_INJECTION(compat_sys_##name, ERRNO); \
	asmlinkage long compat_sys_##name(void)
#endif 

#define COMPAT_SYSCALL_DEFINE1(name, ...) \
        COMPAT_SYSCALL_DEFINEx(1, _##name, __VA_ARGS__)
#define COMPAT_SYSCALL_DEFINE2(name, ...) \
	COMPAT_SYSCALL_DEFINEx(2, _##name, __VA_ARGS__)
#define COMPAT_SYSCALL_DEFINE3(name, ...) \
	COMPAT_SYSCALL_DEFINEx(3, _##name, __VA_ARGS__)
#define COMPAT_SYSCALL_DEFINE4(name, ...) \
	COMPAT_SYSCALL_DEFINEx(4, _##name, __VA_ARGS__)
#define COMPAT_SYSCALL_DEFINE5(name, ...) \
	COMPAT_SYSCALL_DEFINEx(5, _##name, __VA_ARGS__)
#define COMPAT_SYSCALL_DEFINE6(name, ...) \
	COMPAT_SYSCALL_DEFINEx(6, _##name, __VA_ARGS__)


#ifndef COMPAT_SYSCALL_DEFINEx
#define COMPAT_SYSCALL_DEFINEx(x, name, ...)					\
	__diag_push();								\
	__diag_ignore(GCC, 8, "-Wattribute-alias",				\
		      "Type aliasing is used to sanitize syscall arguments");\
	asmlinkage long compat_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__));	\
	asmlinkage long compat_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__))	\
		__attribute__((alias(__stringify(__se_compat_sys##name))));	\
	ALLOW_ERROR_INJECTION(compat_sys##name, ERRNO);				\
	static inline long __do_compat_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__));\
	asmlinkage long __se_compat_sys##name(__MAP(x,__SC_LONG,__VA_ARGS__));	\
	asmlinkage long __se_compat_sys##name(__MAP(x,__SC_LONG,__VA_ARGS__))	\
	{									\
		long ret = __do_compat_sys##name(__MAP(x,__SC_DELOUSE,__VA_ARGS__));\
		__MAP(x,__SC_TEST,__VA_ARGS__);					\
		return ret;							\
	}									\
	__diag_pop();								\
	static inline long __do_compat_sys##name(__MAP(x,__SC_DECL,__VA_ARGS__))
#endif 

#ifdef CONFIG_COMPAT

#ifndef compat_user_stack_pointer
#define compat_user_stack_pointer() current_user_stack_pointer()
#endif
#ifndef compat_sigaltstack	
typedef struct compat_sigaltstack {
	compat_uptr_t			ss_sp;
	int				ss_flags;
	compat_size_t			ss_size;
} compat_stack_t;
#endif
#ifndef COMPAT_MINSIGSTKSZ
#define COMPAT_MINSIGSTKSZ	MINSIGSTKSZ
#endif

#define compat_jiffies_to_clock_t(x)	\
		(((unsigned long)(x) * COMPAT_USER_HZ) / HZ)

typedef __compat_uid32_t	compat_uid_t;
typedef __compat_gid32_t	compat_gid_t;

struct compat_sel_arg_struct;
struct rusage;

struct compat_itimerval {
	struct old_timeval32	it_interval;
	struct old_timeval32	it_value;
};

struct itimerval;
int get_compat_itimerval(struct itimerval *, const struct compat_itimerval __user *);
int put_compat_itimerval(struct compat_itimerval __user *, const struct itimerval *);

struct compat_tms {
	compat_clock_t		tms_utime;
	compat_clock_t		tms_stime;
	compat_clock_t		tms_cutime;
	compat_clock_t		tms_cstime;
};

#define _COMPAT_NSIG_WORDS	(_COMPAT_NSIG / _COMPAT_NSIG_BPW)

typedef struct {
	compat_sigset_word	sig[_COMPAT_NSIG_WORDS];
} compat_sigset_t;

int set_compat_user_sigmask(const compat_sigset_t __user *umask,
			    size_t sigsetsize);

struct compat_sigaction {
#ifndef __ARCH_HAS_IRIX_SIGACTION
	compat_uptr_t			sa_handler;
	compat_ulong_t			sa_flags;
#else
	compat_uint_t			sa_flags;
	compat_uptr_t			sa_handler;
#endif
#ifdef __ARCH_HAS_SA_RESTORER
	compat_uptr_t			sa_restorer;
#endif
	compat_sigset_t			sa_mask __packed;
};

typedef union compat_sigval {
	compat_int_t	sival_int;
	compat_uptr_t	sival_ptr;
} compat_sigval_t;

typedef struct compat_siginfo {
	int si_signo;
#ifndef __ARCH_HAS_SWAPPED_SIGINFO
	int si_errno;
	int si_code;
#else
	int si_code;
	int si_errno;
#endif

	union {
		int _pad[128/sizeof(int) - 3];

		
		struct {
			compat_pid_t _pid;	
			__compat_uid32_t _uid;	
		} _kill;

		
		struct {
			compat_timer_t _tid;	
			int _overrun;		
			compat_sigval_t _sigval;	
		} _timer;

		
		struct {
			compat_pid_t _pid;	
			__compat_uid32_t _uid;	
			compat_sigval_t _sigval;
		} _rt;

		
		struct {
			compat_pid_t _pid;	
			__compat_uid32_t _uid;	
			int _status;		
			compat_clock_t _utime;
			compat_clock_t _stime;
		} _sigchld;

#ifdef CONFIG_X86_X32_ABI
		
		struct {
			compat_pid_t _pid;	
			__compat_uid32_t _uid;	
			int _status;		
			compat_s64 _utime;
			compat_s64 _stime;
		} _sigchld_x32;
#endif

		
		struct {
			compat_uptr_t _addr;	
#ifdef __ARCH_SI_TRAPNO
			int _trapno;	
#endif
#define __COMPAT_ADDR_BND_PKEY_PAD  (__alignof__(compat_uptr_t) < sizeof(short) ? \
				     sizeof(short) : __alignof__(compat_uptr_t))
			union {
				
				short int _addr_lsb;	
				
				struct {
					char _dummy_bnd[__COMPAT_ADDR_BND_PKEY_PAD];
					compat_uptr_t _lower;
					compat_uptr_t _upper;
				} _addr_bnd;
				
				struct {
					char _dummy_pkey[__COMPAT_ADDR_BND_PKEY_PAD];
					u32 _pkey;
				} _addr_pkey;
			};
		} _sigfault;

		
		struct {
			compat_long_t _band;	
			int _fd;
		} _sigpoll;

		struct {
			compat_uptr_t _call_addr; 
			int _syscall;	
			unsigned int _arch;	
		} _sigsys;
	} _sifields;
} compat_siginfo_t;


extern int compat_get_timespec(struct timespec *, const void __user *);
extern int compat_put_timespec(const struct timespec *, void __user *);
extern int compat_get_timeval(struct timeval *, const void __user *);
extern int compat_put_timeval(const struct timeval *, void __user *);

struct compat_iovec {
	compat_uptr_t	iov_base;
	compat_size_t	iov_len;
};

struct compat_rlimit {
	compat_ulong_t	rlim_cur;
	compat_ulong_t	rlim_max;
};

struct compat_rusage {
	struct old_timeval32 ru_utime;
	struct old_timeval32 ru_stime;
	compat_long_t	ru_maxrss;
	compat_long_t	ru_ixrss;
	compat_long_t	ru_idrss;
	compat_long_t	ru_isrss;
	compat_long_t	ru_minflt;
	compat_long_t	ru_majflt;
	compat_long_t	ru_nswap;
	compat_long_t	ru_inblock;
	compat_long_t	ru_oublock;
	compat_long_t	ru_msgsnd;
	compat_long_t	ru_msgrcv;
	compat_long_t	ru_nsignals;
	compat_long_t	ru_nvcsw;
	compat_long_t	ru_nivcsw;
};

extern int put_compat_rusage(const struct rusage *,
			     struct compat_rusage __user *);

struct compat_siginfo;
struct __compat_aio_sigset;

struct compat_dirent {
	u32		d_ino;
	compat_off_t	d_off;
	u16		d_reclen;
	char		d_name[256];
};

struct compat_ustat {
	compat_daddr_t		f_tfree;
	compat_ino_t		f_tinode;
	char			f_fname[6];
	char			f_fpack[6];
};

#define COMPAT_SIGEV_PAD_SIZE	((SIGEV_MAX_SIZE/sizeof(int)) - 3)

typedef struct compat_sigevent {
	compat_sigval_t sigev_value;
	compat_int_t sigev_signo;
	compat_int_t sigev_notify;
	union {
		compat_int_t _pad[COMPAT_SIGEV_PAD_SIZE];
		compat_int_t _tid;

		struct {
			compat_uptr_t _function;
			compat_uptr_t _attribute;
		} _sigev_thread;
	} _sigev_un;
} compat_sigevent_t;

struct compat_ifmap {
	compat_ulong_t mem_start;
	compat_ulong_t mem_end;
	unsigned short base_addr;
	unsigned char irq;
	unsigned char dma;
	unsigned char port;
};

struct compat_if_settings {
	unsigned int type;	
	unsigned int size;	
	compat_uptr_t ifs_ifsu;	
};

struct compat_ifreq {
	union {
		char	ifrn_name[IFNAMSIZ];    
	} ifr_ifrn;
	union {
		struct	sockaddr ifru_addr;
		struct	sockaddr ifru_dstaddr;
		struct	sockaddr ifru_broadaddr;
		struct	sockaddr ifru_netmask;
		struct	sockaddr ifru_hwaddr;
		short	ifru_flags;
		compat_int_t	ifru_ivalue;
		compat_int_t	ifru_mtu;
		struct	compat_ifmap ifru_map;
		char	ifru_slave[IFNAMSIZ];   
		char	ifru_newname[IFNAMSIZ];
		compat_caddr_t	ifru_data;
		struct	compat_if_settings ifru_settings;
	} ifr_ifru;
};

struct compat_ifconf {
	compat_int_t	ifc_len;                
	compat_caddr_t  ifcbuf;
};

struct compat_robust_list {
	compat_uptr_t			next;
};

struct compat_robust_list_head {
	struct compat_robust_list	list;
	compat_long_t			futex_offset;
	compat_uptr_t			list_op_pending;
};

#ifdef CONFIG_COMPAT_OLD_SIGACTION
struct compat_old_sigaction {
	compat_uptr_t			sa_handler;
	compat_old_sigset_t		sa_mask;
	compat_ulong_t			sa_flags;
	compat_uptr_t			sa_restorer;
};
#endif

struct compat_keyctl_kdf_params {
	compat_uptr_t hashname;
	compat_uptr_t otherinfo;
	__u32 otherinfolen;
	__u32 __spare[8];
};

struct compat_statfs;
struct compat_statfs64;
struct compat_old_linux_dirent;
struct compat_linux_dirent;
struct linux_dirent64;
struct compat_msghdr;
struct compat_mmsghdr;
struct compat_sysinfo;
struct compat_sysctl_args;
struct compat_kexec_segment;
struct compat_mq_attr;
struct compat_msgbuf;

extern void compat_exit_robust_list(struct task_struct *curr);

#define BITS_PER_COMPAT_LONG    (8*sizeof(compat_long_t))

#define BITS_TO_COMPAT_LONGS(bits) DIV_ROUND_UP(bits, BITS_PER_COMPAT_LONG)

long compat_get_bitmap(unsigned long *mask, const compat_ulong_t __user *umask,
		       unsigned long bitmap_size);
long compat_put_bitmap(compat_ulong_t __user *umask, unsigned long *mask,
		       unsigned long bitmap_size);
int copy_siginfo_from_user32(kernel_siginfo_t *to, const struct compat_siginfo __user *from);
int copy_siginfo_to_user32(struct compat_siginfo __user *to, const kernel_siginfo_t *from);
int get_compat_sigevent(struct sigevent *event,
		const struct compat_sigevent __user *u_event);

static inline int old_timeval32_compare(struct old_timeval32 *lhs,
					struct old_timeval32 *rhs)
{
	if (lhs->tv_sec < rhs->tv_sec)
		return -1;
	if (lhs->tv_sec > rhs->tv_sec)
		return 1;
	return lhs->tv_usec - rhs->tv_usec;
}

static inline int old_timespec32_compare(struct old_timespec32 *lhs,
					struct old_timespec32 *rhs)
{
	if (lhs->tv_sec < rhs->tv_sec)
		return -1;
	if (lhs->tv_sec > rhs->tv_sec)
		return 1;
	return lhs->tv_nsec - rhs->tv_nsec;
}

extern int get_compat_sigset(sigset_t *set, const compat_sigset_t __user *compat);


static inline int
put_compat_sigset(compat_sigset_t __user *compat, const sigset_t *set,
		  unsigned int size)
{
	
#ifdef __BIG_ENDIAN
	compat_sigset_t v;
	switch (_NSIG_WORDS) {
	case 4: v.sig[7] = (set->sig[3] >> 32); v.sig[6] = set->sig[3];
		
	case 3: v.sig[5] = (set->sig[2] >> 32); v.sig[4] = set->sig[2];
		
	case 2: v.sig[3] = (set->sig[1] >> 32); v.sig[2] = set->sig[1];
		
	case 1: v.sig[1] = (set->sig[0] >> 32); v.sig[0] = set->sig[0];
	}
	return copy_to_user(compat, &v, size) ? -EFAULT : 0;
#else
	return copy_to_user(compat, set, size) ? -EFAULT : 0;
#endif
}

extern int compat_ptrace_request(struct task_struct *child,
				 compat_long_t request,
				 compat_ulong_t addr, compat_ulong_t data);

extern long compat_arch_ptrace(struct task_struct *child, compat_long_t request,
			       compat_ulong_t addr, compat_ulong_t data);

struct epoll_event;	

extern ssize_t compat_rw_copy_check_uvector(int type,
		const struct compat_iovec __user *uvector,
		unsigned long nr_segs,
		unsigned long fast_segs, struct iovec *fast_pointer,
		struct iovec **ret_pointer);

extern void __user *compat_alloc_user_space(unsigned long len);

int compat_restore_altstack(const compat_stack_t __user *uss);
int __compat_save_altstack(compat_stack_t __user *, unsigned long);
#define compat_save_altstack_ex(uss, sp) do { \
	compat_stack_t __user *__uss = uss; \
	struct task_struct *t = current; \
	put_user_ex(ptr_to_compat((void __user *)t->sas_ss_sp), &__uss->ss_sp); \
	put_user_ex(t->sas_ss_flags, &__uss->ss_flags); \
	put_user_ex(t->sas_ss_size, &__uss->ss_size); \
	if (t->sas_ss_flags & SS_AUTODISARM) \
		sas_ss_reset(t); \
} while (0);


#ifndef CONFIG_ARCH_HAS_SYSCALL_WRAPPER
asmlinkage long compat_sys_io_setup(unsigned nr_reqs, u32 __user *ctx32p);
asmlinkage long compat_sys_io_submit(compat_aio_context_t ctx_id, int nr,
				     u32 __user *iocb);
asmlinkage long compat_sys_io_pgetevents(compat_aio_context_t ctx_id,
					compat_long_t min_nr,
					compat_long_t nr,
					struct io_event __user *events,
					struct old_timespec32 __user *timeout,
					const struct __compat_aio_sigset __user *usig);
asmlinkage long compat_sys_io_pgetevents_time64(compat_aio_context_t ctx_id,
					compat_long_t min_nr,
					compat_long_t nr,
					struct io_event __user *events,
					struct __kernel_timespec __user *timeout,
					const struct __compat_aio_sigset __user *usig);


asmlinkage long compat_sys_lookup_dcookie(u32, u32, char __user *, compat_size_t);


asmlinkage long compat_sys_epoll_pwait(int epfd,
			struct epoll_event __user *events,
			int maxevents, int timeout,
			const compat_sigset_t __user *sigmask,
			compat_size_t sigsetsize);


asmlinkage long compat_sys_fcntl(unsigned int fd, unsigned int cmd,
				 compat_ulong_t arg);
asmlinkage long compat_sys_fcntl64(unsigned int fd, unsigned int cmd,
				   compat_ulong_t arg);


asmlinkage long compat_sys_ioctl(unsigned int fd, unsigned int cmd,
				 compat_ulong_t arg);


asmlinkage long compat_sys_mount(const char __user *dev_name,
				 const char __user *dir_name,
				 const char __user *type, compat_ulong_t flags,
				 const void __user *data);


asmlinkage long compat_sys_statfs(const char __user *pathname,
				  struct compat_statfs __user *buf);
asmlinkage long compat_sys_statfs64(const char __user *pathname,
				    compat_size_t sz,
				    struct compat_statfs64 __user *buf);
asmlinkage long compat_sys_fstatfs(unsigned int fd,
				   struct compat_statfs __user *buf);
asmlinkage long compat_sys_fstatfs64(unsigned int fd, compat_size_t sz,
				     struct compat_statfs64 __user *buf);
asmlinkage long compat_sys_truncate(const char __user *, compat_off_t);
asmlinkage long compat_sys_ftruncate(unsigned int, compat_ulong_t);

asmlinkage long compat_sys_openat(int dfd, const char __user *filename,
				  int flags, umode_t mode);


asmlinkage long compat_sys_getdents(unsigned int fd,
				    struct compat_linux_dirent __user *dirent,
				    unsigned int count);


asmlinkage long compat_sys_lseek(unsigned int, compat_off_t, unsigned int);
asmlinkage ssize_t compat_sys_readv(compat_ulong_t fd,
		const struct compat_iovec __user *vec, compat_ulong_t vlen);
asmlinkage ssize_t compat_sys_writev(compat_ulong_t fd,
		const struct compat_iovec __user *vec, compat_ulong_t vlen);

asmlinkage ssize_t compat_sys_preadv(compat_ulong_t fd,
		const struct compat_iovec __user *vec,
		compat_ulong_t vlen, u32 pos_low, u32 pos_high);
asmlinkage ssize_t compat_sys_pwritev(compat_ulong_t fd,
		const struct compat_iovec __user *vec,
		compat_ulong_t vlen, u32 pos_low, u32 pos_high);
#ifdef __ARCH_WANT_COMPAT_SYS_PREADV64
asmlinkage long compat_sys_preadv64(unsigned long fd,
		const struct compat_iovec __user *vec,
		unsigned long vlen, loff_t pos);
#endif

#ifdef __ARCH_WANT_COMPAT_SYS_PWRITEV64
asmlinkage long compat_sys_pwritev64(unsigned long fd,
		const struct compat_iovec __user *vec,
		unsigned long vlen, loff_t pos);
#endif


asmlinkage long compat_sys_sendfile(int out_fd, int in_fd,
				    compat_off_t __user *offset, compat_size_t count);
asmlinkage long compat_sys_sendfile64(int out_fd, int in_fd,
				    compat_loff_t __user *offset, compat_size_t count);


asmlinkage long compat_sys_pselect6_time32(int n, compat_ulong_t __user *inp,
				    compat_ulong_t __user *outp,
				    compat_ulong_t __user *exp,
				    struct old_timespec32 __user *tsp,
				    void __user *sig);
asmlinkage long compat_sys_pselect6_time64(int n, compat_ulong_t __user *inp,
				    compat_ulong_t __user *outp,
				    compat_ulong_t __user *exp,
				    struct __kernel_timespec __user *tsp,
				    void __user *sig);
asmlinkage long compat_sys_ppoll_time32(struct pollfd __user *ufds,
				 unsigned int nfds,
				 struct old_timespec32 __user *tsp,
				 const compat_sigset_t __user *sigmask,
				 compat_size_t sigsetsize);
asmlinkage long compat_sys_ppoll_time64(struct pollfd __user *ufds,
				 unsigned int nfds,
				 struct __kernel_timespec __user *tsp,
				 const compat_sigset_t __user *sigmask,
				 compat_size_t sigsetsize);


asmlinkage long compat_sys_signalfd4(int ufd,
				     const compat_sigset_t __user *sigmask,
				     compat_size_t sigsetsize, int flags);


asmlinkage long compat_sys_vmsplice(int fd, const struct compat_iovec __user *,
				    unsigned int nr_segs, unsigned int flags);


asmlinkage long compat_sys_newfstatat(unsigned int dfd,
				      const char __user *filename,
				      struct compat_stat __user *statbuf,
				      int flag);
asmlinkage long compat_sys_newfstat(unsigned int fd,
				    struct compat_stat __user *statbuf);




asmlinkage long compat_sys_waitid(int, compat_pid_t,
		struct compat_siginfo __user *, int,
		struct compat_rusage __user *);




asmlinkage long
compat_sys_set_robust_list(struct compat_robust_list_head __user *head,
			   compat_size_t len);
asmlinkage long
compat_sys_get_robust_list(int pid, compat_uptr_t __user *head_ptr,
			   compat_size_t __user *len_ptr);


asmlinkage long compat_sys_getitimer(int which,
				     struct compat_itimerval __user *it);
asmlinkage long compat_sys_setitimer(int which,
				     struct compat_itimerval __user *in,
				     struct compat_itimerval __user *out);


asmlinkage long compat_sys_kexec_load(compat_ulong_t entry,
				      compat_ulong_t nr_segments,
				      struct compat_kexec_segment __user *,
				      compat_ulong_t flags);


asmlinkage long compat_sys_timer_create(clockid_t which_clock,
			struct compat_sigevent __user *timer_event_spec,
			timer_t __user *created_timer_id);


asmlinkage long compat_sys_ptrace(compat_long_t request, compat_long_t pid,
				  compat_long_t addr, compat_long_t data);


asmlinkage long compat_sys_sched_setaffinity(compat_pid_t pid,
				     unsigned int len,
				     compat_ulong_t __user *user_mask_ptr);
asmlinkage long compat_sys_sched_getaffinity(compat_pid_t pid,
				     unsigned int len,
				     compat_ulong_t __user *user_mask_ptr);


asmlinkage long compat_sys_sigaltstack(const compat_stack_t __user *uss_ptr,
				       compat_stack_t __user *uoss_ptr);
asmlinkage long compat_sys_rt_sigsuspend(compat_sigset_t __user *unewset,
					 compat_size_t sigsetsize);
#ifndef CONFIG_ODD_RT_SIGACTION
asmlinkage long compat_sys_rt_sigaction(int,
				 const struct compat_sigaction __user *,
				 struct compat_sigaction __user *,
				 compat_size_t);
#endif
asmlinkage long compat_sys_rt_sigprocmask(int how, compat_sigset_t __user *set,
					  compat_sigset_t __user *oset,
					  compat_size_t sigsetsize);
asmlinkage long compat_sys_rt_sigpending(compat_sigset_t __user *uset,
					 compat_size_t sigsetsize);
asmlinkage long compat_sys_rt_sigtimedwait_time32(compat_sigset_t __user *uthese,
		struct compat_siginfo __user *uinfo,
		struct old_timespec32 __user *uts, compat_size_t sigsetsize);
asmlinkage long compat_sys_rt_sigtimedwait_time64(compat_sigset_t __user *uthese,
		struct compat_siginfo __user *uinfo,
		struct __kernel_timespec __user *uts, compat_size_t sigsetsize);
asmlinkage long compat_sys_rt_sigqueueinfo(compat_pid_t pid, int sig,
				struct compat_siginfo __user *uinfo);



asmlinkage long compat_sys_times(struct compat_tms __user *tbuf);
asmlinkage long compat_sys_getrlimit(unsigned int resource,
				     struct compat_rlimit __user *rlim);
asmlinkage long compat_sys_setrlimit(unsigned int resource,
				     struct compat_rlimit __user *rlim);
asmlinkage long compat_sys_getrusage(int who, struct compat_rusage __user *ru);


asmlinkage long compat_sys_gettimeofday(struct old_timeval32 __user *tv,
		struct timezone __user *tz);
asmlinkage long compat_sys_settimeofday(struct old_timeval32 __user *tv,
		struct timezone __user *tz);


asmlinkage long compat_sys_sysinfo(struct compat_sysinfo __user *info);


asmlinkage long compat_sys_mq_open(const char __user *u_name,
			int oflag, compat_mode_t mode,
			struct compat_mq_attr __user *u_attr);
asmlinkage long compat_sys_mq_notify(mqd_t mqdes,
			const struct compat_sigevent __user *u_notification);
asmlinkage long compat_sys_mq_getsetattr(mqd_t mqdes,
			const struct compat_mq_attr __user *u_mqstat,
			struct compat_mq_attr __user *u_omqstat);


asmlinkage long compat_sys_msgctl(int first, int second, void __user *uptr);
asmlinkage long compat_sys_msgrcv(int msqid, compat_uptr_t msgp,
		compat_ssize_t msgsz, compat_long_t msgtyp, int msgflg);
asmlinkage long compat_sys_msgsnd(int msqid, compat_uptr_t msgp,
		compat_ssize_t msgsz, int msgflg);


asmlinkage long compat_sys_semctl(int semid, int semnum, int cmd, int arg);


asmlinkage long compat_sys_shmctl(int first, int second, void __user *uptr);
asmlinkage long compat_sys_shmat(int shmid, compat_uptr_t shmaddr, int shmflg);


asmlinkage long compat_sys_recvfrom(int fd, void __user *buf, compat_size_t len,
			    unsigned flags, struct sockaddr __user *addr,
			    int __user *addrlen);
asmlinkage long compat_sys_setsockopt(int fd, int level, int optname,
				      char __user *optval, unsigned int optlen);
asmlinkage long compat_sys_getsockopt(int fd, int level, int optname,
				      char __user *optval, int __user *optlen);
asmlinkage long compat_sys_sendmsg(int fd, struct compat_msghdr __user *msg,
				   unsigned flags);
asmlinkage long compat_sys_recvmsg(int fd, struct compat_msghdr __user *msg,
				   unsigned int flags);




asmlinkage long compat_sys_keyctl(u32 option,
			      u32 arg2, u32 arg3, u32 arg4, u32 arg5);


asmlinkage long compat_sys_execve(const char __user *filename, const compat_uptr_t __user *argv,
		     const compat_uptr_t __user *envp);




asmlinkage long compat_sys_mbind(compat_ulong_t start, compat_ulong_t len,
				 compat_ulong_t mode,
				 compat_ulong_t __user *nmask,
				 compat_ulong_t maxnode, compat_ulong_t flags);
asmlinkage long compat_sys_get_mempolicy(int __user *policy,
					 compat_ulong_t __user *nmask,
					 compat_ulong_t maxnode,
					 compat_ulong_t addr,
					 compat_ulong_t flags);
asmlinkage long compat_sys_set_mempolicy(int mode, compat_ulong_t __user *nmask,
					 compat_ulong_t maxnode);
asmlinkage long compat_sys_migrate_pages(compat_pid_t pid,
		compat_ulong_t maxnode, const compat_ulong_t __user *old_nodes,
		const compat_ulong_t __user *new_nodes);
asmlinkage long compat_sys_move_pages(pid_t pid, compat_ulong_t nr_pages,
				      __u32 __user *pages,
				      const int __user *nodes,
				      int __user *status,
				      int flags);

asmlinkage long compat_sys_rt_tgsigqueueinfo(compat_pid_t tgid,
					compat_pid_t pid, int sig,
					struct compat_siginfo __user *uinfo);
asmlinkage long compat_sys_recvmmsg_time64(int fd, struct compat_mmsghdr __user *mmsg,
				    unsigned vlen, unsigned int flags,
				    struct __kernel_timespec __user *timeout);
asmlinkage long compat_sys_recvmmsg_time32(int fd, struct compat_mmsghdr __user *mmsg,
				    unsigned vlen, unsigned int flags,
				    struct old_timespec32 __user *timeout);
asmlinkage long compat_sys_wait4(compat_pid_t pid,
				 compat_uint_t __user *stat_addr, int options,
				 struct compat_rusage __user *ru);
asmlinkage long compat_sys_fanotify_mark(int, unsigned int, __u32, __u32,
					    int, const char __user *);
asmlinkage long compat_sys_open_by_handle_at(int mountdirfd,
					     struct file_handle __user *handle,
					     int flags);
asmlinkage long compat_sys_sendmmsg(int fd, struct compat_mmsghdr __user *mmsg,
				    unsigned vlen, unsigned int flags);
asmlinkage ssize_t compat_sys_process_vm_readv(compat_pid_t pid,
		const struct compat_iovec __user *lvec,
		compat_ulong_t liovcnt, const struct compat_iovec __user *rvec,
		compat_ulong_t riovcnt, compat_ulong_t flags);
asmlinkage ssize_t compat_sys_process_vm_writev(compat_pid_t pid,
		const struct compat_iovec __user *lvec,
		compat_ulong_t liovcnt, const struct compat_iovec __user *rvec,
		compat_ulong_t riovcnt, compat_ulong_t flags);
asmlinkage long compat_sys_execveat(int dfd, const char __user *filename,
		     const compat_uptr_t __user *argv,
		     const compat_uptr_t __user *envp, int flags);
asmlinkage ssize_t compat_sys_preadv2(compat_ulong_t fd,
		const struct compat_iovec __user *vec,
		compat_ulong_t vlen, u32 pos_low, u32 pos_high, rwf_t flags);
asmlinkage ssize_t compat_sys_pwritev2(compat_ulong_t fd,
		const struct compat_iovec __user *vec,
		compat_ulong_t vlen, u32 pos_low, u32 pos_high, rwf_t flags);
#ifdef __ARCH_WANT_COMPAT_SYS_PREADV64V2
asmlinkage long  compat_sys_readv64v2(unsigned long fd,
		const struct compat_iovec __user *vec,
		unsigned long vlen, loff_t pos, rwf_t flags);
#endif

#ifdef __ARCH_WANT_COMPAT_SYS_PWRITEV64V2
asmlinkage long compat_sys_pwritev64v2(unsigned long fd,
		const struct compat_iovec __user *vec,
		unsigned long vlen, loff_t pos, rwf_t flags);
#endif





asmlinkage long compat_sys_open(const char __user *filename, int flags,
				umode_t mode);


asmlinkage long compat_sys_signalfd(int ufd,
				    const compat_sigset_t __user *sigmask,
				    compat_size_t sigsetsize);


asmlinkage long compat_sys_newstat(const char __user *filename,
				   struct compat_stat __user *statbuf);
asmlinkage long compat_sys_newlstat(const char __user *filename,
				    struct compat_stat __user *statbuf);


asmlinkage long compat_sys_select(int n, compat_ulong_t __user *inp,
		compat_ulong_t __user *outp, compat_ulong_t __user *exp,
		struct old_timeval32 __user *tvp);
asmlinkage long compat_sys_ustat(unsigned dev, struct compat_ustat __user *u32);
asmlinkage long compat_sys_recv(int fd, void __user *buf, compat_size_t len,
				unsigned flags);
asmlinkage long compat_sys_sysctl(struct compat_sysctl_args __user *args);


asmlinkage long compat_sys_old_readdir(unsigned int fd,
				       struct compat_old_linux_dirent __user *,
				       unsigned int count);


asmlinkage long compat_sys_old_select(struct compat_sel_arg_struct __user *arg);


asmlinkage long compat_sys_ipc(u32, int, int, u32, compat_uptr_t, u32);


#ifdef __ARCH_WANT_SYS_SIGPENDING
asmlinkage long compat_sys_sigpending(compat_old_sigset_t __user *set);
#endif

#ifdef __ARCH_WANT_SYS_SIGPROCMASK
asmlinkage long compat_sys_sigprocmask(int how, compat_old_sigset_t __user *nset,
				       compat_old_sigset_t __user *oset);
#endif
#ifdef CONFIG_COMPAT_OLD_SIGACTION
asmlinkage long compat_sys_sigaction(int sig,
                                   const struct compat_old_sigaction __user *act,
                                   struct compat_old_sigaction __user *oact);
#endif


asmlinkage long compat_sys_socketcall(int call, u32 __user *args);

#endif 




#ifndef in_compat_syscall
static inline bool in_compat_syscall(void) { return is_compat_task(); }
#endif


static inline struct old_timeval32 ns_to_old_timeval32(s64 nsec)
{
	struct timeval tv;
	struct old_timeval32 ctv;

	tv = ns_to_timeval(nsec);
	ctv.tv_sec = tv.tv_sec;
	ctv.tv_usec = tv.tv_usec;

	return ctv;
}



int kcompat_sys_statfs64(const char __user * pathname, compat_size_t sz,
		     struct compat_statfs64 __user * buf);
int kcompat_sys_fstatfs64(unsigned int fd, compat_size_t sz,
			  struct compat_statfs64 __user * buf);

#else 

#define is_compat_task() (0)

#define in_compat_syscall in_compat_syscall
static inline bool in_compat_syscall(void) { return false; }

#endif 

#endif 
