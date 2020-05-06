/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_INVPCID
#define _ASM_X86_INVPCID

static inline void __invpcid(unsigned long pcid, unsigned long addr,
			     unsigned long type)
{
	struct { u64 d[2]; } desc = { { pcid, addr } };

	
	asm volatile (".byte 0x66, 0x0f, 0x38, 0x82, 0x01"
		      : : "m" (desc), "a" (type), "c" (&desc) : "memory");
}

#define INVPCID_TYPE_INDIV_ADDR		0
#define INVPCID_TYPE_SINGLE_CTXT	1
#define INVPCID_TYPE_ALL_INCL_GLOBAL	2
#define INVPCID_TYPE_ALL_NON_GLOBAL	3


static inline void invpcid_flush_one(unsigned long pcid,
				     unsigned long addr)
{
	__invpcid(pcid, addr, INVPCID_TYPE_INDIV_ADDR);
}


static inline void invpcid_flush_single_context(unsigned long pcid)
{
	__invpcid(pcid, 0, INVPCID_TYPE_SINGLE_CTXT);
}


static inline void invpcid_flush_all(void)
{
	__invpcid(0, 0, INVPCID_TYPE_ALL_INCL_GLOBAL);
}


static inline void invpcid_flush_all_nonglobals(void)
{
	__invpcid(0, 0, INVPCID_TYPE_ALL_NON_GLOBAL);
}

#endif 
