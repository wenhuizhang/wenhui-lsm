/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_GENERIC_VSYSCALL_H
#define __ASM_GENERIC_VSYSCALL_H

#ifndef __ASSEMBLY__

#ifndef __arch_get_k_vdso_data
static __always_inline struct vdso_data *__arch_get_k_vdso_data(void)
{
	return NULL;
}
#endif 

#ifndef __arch_update_vdso_data
static __always_inline int __arch_update_vdso_data(void)
{
	return 0;
}
#endif 

#ifndef __arch_get_clock_mode
static __always_inline int __arch_get_clock_mode(struct timekeeper *tk)
{
	return 0;
}
#endif 

#ifndef __arch_use_vsyscall
static __always_inline int __arch_use_vsyscall(struct vdso_data *vdata)
{
	return 1;
}
#endif 

#ifndef __arch_update_vsyscall
static __always_inline void __arch_update_vsyscall(struct vdso_data *vdata,
						   struct timekeeper *tk)
{
}
#endif 

#ifndef __arch_sync_vdso_data
static __always_inline void __arch_sync_vdso_data(struct vdso_data *vdata)
{
}
#endif 

#endif 

#endif 
