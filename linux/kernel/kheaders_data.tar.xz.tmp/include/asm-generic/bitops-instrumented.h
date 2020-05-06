/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _ASM_GENERIC_BITOPS_INSTRUMENTED_H
#define _ASM_GENERIC_BITOPS_INSTRUMENTED_H

#include <linux/kasan-checks.h>


static inline void set_bit(long nr, volatile unsigned long *addr)
{
	kasan_check_write(addr + BIT_WORD(nr), sizeof(long));
	arch_set_bit(nr, addr);
}


static inline void __set_bit(long nr, volatile unsigned long *addr)
{
	kasan_check_write(addr + BIT_WORD(nr), sizeof(long));
	arch___set_bit(nr, addr);
}


static inline void clear_bit(long nr, volatile unsigned long *addr)
{
	kasan_check_write(addr + BIT_WORD(nr), sizeof(long));
	arch_clear_bit(nr, addr);
}


static inline void __clear_bit(long nr, volatile unsigned long *addr)
{
	kasan_check_write(addr + BIT_WORD(nr), sizeof(long));
	arch___clear_bit(nr, addr);
}


static inline void clear_bit_unlock(long nr, volatile unsigned long *addr)
{
	kasan_check_write(addr + BIT_WORD(nr), sizeof(long));
	arch_clear_bit_unlock(nr, addr);
}


static inline void __clear_bit_unlock(long nr, volatile unsigned long *addr)
{
	kasan_check_write(addr + BIT_WORD(nr), sizeof(long));
	arch___clear_bit_unlock(nr, addr);
}


static inline void change_bit(long nr, volatile unsigned long *addr)
{
	kasan_check_write(addr + BIT_WORD(nr), sizeof(long));
	arch_change_bit(nr, addr);
}


static inline void __change_bit(long nr, volatile unsigned long *addr)
{
	kasan_check_write(addr + BIT_WORD(nr), sizeof(long));
	arch___change_bit(nr, addr);
}


static inline bool test_and_set_bit(long nr, volatile unsigned long *addr)
{
	kasan_check_write(addr + BIT_WORD(nr), sizeof(long));
	return arch_test_and_set_bit(nr, addr);
}


static inline bool __test_and_set_bit(long nr, volatile unsigned long *addr)
{
	kasan_check_write(addr + BIT_WORD(nr), sizeof(long));
	return arch___test_and_set_bit(nr, addr);
}


static inline bool test_and_set_bit_lock(long nr, volatile unsigned long *addr)
{
	kasan_check_write(addr + BIT_WORD(nr), sizeof(long));
	return arch_test_and_set_bit_lock(nr, addr);
}


static inline bool test_and_clear_bit(long nr, volatile unsigned long *addr)
{
	kasan_check_write(addr + BIT_WORD(nr), sizeof(long));
	return arch_test_and_clear_bit(nr, addr);
}


static inline bool __test_and_clear_bit(long nr, volatile unsigned long *addr)
{
	kasan_check_write(addr + BIT_WORD(nr), sizeof(long));
	return arch___test_and_clear_bit(nr, addr);
}


static inline bool test_and_change_bit(long nr, volatile unsigned long *addr)
{
	kasan_check_write(addr + BIT_WORD(nr), sizeof(long));
	return arch_test_and_change_bit(nr, addr);
}


static inline bool __test_and_change_bit(long nr, volatile unsigned long *addr)
{
	kasan_check_write(addr + BIT_WORD(nr), sizeof(long));
	return arch___test_and_change_bit(nr, addr);
}


static inline bool test_bit(long nr, const volatile unsigned long *addr)
{
	kasan_check_read(addr + BIT_WORD(nr), sizeof(long));
	return arch_test_bit(nr, addr);
}

#if defined(arch_clear_bit_unlock_is_negative_byte)

static inline bool
clear_bit_unlock_is_negative_byte(long nr, volatile unsigned long *addr)
{
	kasan_check_write(addr + BIT_WORD(nr), sizeof(long));
	return arch_clear_bit_unlock_is_negative_byte(nr, addr);
}

#define clear_bit_unlock_is_negative_byte clear_bit_unlock_is_negative_byte
#endif

#endif 
