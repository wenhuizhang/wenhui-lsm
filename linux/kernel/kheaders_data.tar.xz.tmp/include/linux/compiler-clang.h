/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_COMPILER_TYPES_H
#error "Please don't include <linux/compiler-clang.h> directly, include <linux/compiler.h> instead."
#endif



#define uninitialized_var(x) x = *(&(x))


#define __UNIQUE_ID(prefix) __PASTE(__PASTE(__UNIQUE_ID_, prefix), __COUNTER__)


#define KASAN_ABI_VERSION 5

#if __has_feature(address_sanitizer) || __has_feature(hwaddress_sanitizer)

#define __SANITIZE_ADDRESS__
#define __no_sanitize_address \
		__attribute__((no_sanitize("address", "hwaddress")))
#else
#define __no_sanitize_address
#endif


#if __has_builtin(__builtin_mul_overflow) && \
    __has_builtin(__builtin_add_overflow) && \
    __has_builtin(__builtin_sub_overflow)
#define COMPILER_HAS_GENERIC_BUILTIN_OVERFLOW 1
#endif


#define barrier() __asm__ __volatile__("" : : : "memory")
