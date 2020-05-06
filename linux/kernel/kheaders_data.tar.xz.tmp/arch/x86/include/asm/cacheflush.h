/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_CACHEFLUSH_H
#define _ASM_X86_CACHEFLUSH_H


#include <asm-generic/cacheflush.h>
#include <asm/special_insns.h>

void clflush_cache_range(void *addr, unsigned int size);

#endif 
