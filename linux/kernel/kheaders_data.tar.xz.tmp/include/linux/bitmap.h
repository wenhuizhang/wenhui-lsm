/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_BITMAP_H
#define __LINUX_BITMAP_H

#ifndef __ASSEMBLY__

#include <linux/types.h>
#include <linux/bitops.h>
#include <linux/string.h>
#include <linux/kernel.h>










extern unsigned long *bitmap_alloc(unsigned int nbits, gfp_t flags);
extern unsigned long *bitmap_zalloc(unsigned int nbits, gfp_t flags);
extern void bitmap_free(const unsigned long *bitmap);



extern int __bitmap_empty(const unsigned long *bitmap, unsigned int nbits);
extern int __bitmap_full(const unsigned long *bitmap, unsigned int nbits);
extern int __bitmap_equal(const unsigned long *bitmap1,
			  const unsigned long *bitmap2, unsigned int nbits);
extern void __bitmap_complement(unsigned long *dst, const unsigned long *src,
			unsigned int nbits);
extern void __bitmap_shift_right(unsigned long *dst, const unsigned long *src,
				unsigned int shift, unsigned int nbits);
extern void __bitmap_shift_left(unsigned long *dst, const unsigned long *src,
				unsigned int shift, unsigned int nbits);
extern int __bitmap_and(unsigned long *dst, const unsigned long *bitmap1,
			const unsigned long *bitmap2, unsigned int nbits);
extern void __bitmap_or(unsigned long *dst, const unsigned long *bitmap1,
			const unsigned long *bitmap2, unsigned int nbits);
extern void __bitmap_xor(unsigned long *dst, const unsigned long *bitmap1,
			const unsigned long *bitmap2, unsigned int nbits);
extern int __bitmap_andnot(unsigned long *dst, const unsigned long *bitmap1,
			const unsigned long *bitmap2, unsigned int nbits);
extern int __bitmap_intersects(const unsigned long *bitmap1,
			const unsigned long *bitmap2, unsigned int nbits);
extern int __bitmap_subset(const unsigned long *bitmap1,
			const unsigned long *bitmap2, unsigned int nbits);
extern int __bitmap_weight(const unsigned long *bitmap, unsigned int nbits);
extern void __bitmap_set(unsigned long *map, unsigned int start, int len);
extern void __bitmap_clear(unsigned long *map, unsigned int start, int len);

extern unsigned long bitmap_find_next_zero_area_off(unsigned long *map,
						    unsigned long size,
						    unsigned long start,
						    unsigned int nr,
						    unsigned long align_mask,
						    unsigned long align_offset);


static inline unsigned long
bitmap_find_next_zero_area(unsigned long *map,
			   unsigned long size,
			   unsigned long start,
			   unsigned int nr,
			   unsigned long align_mask)
{
	return bitmap_find_next_zero_area_off(map, size, start, nr,
					      align_mask, 0);
}

extern int __bitmap_parse(const char *buf, unsigned int buflen, int is_user,
			unsigned long *dst, int nbits);
extern int bitmap_parse_user(const char __user *ubuf, unsigned int ulen,
			unsigned long *dst, int nbits);
extern int bitmap_parselist(const char *buf, unsigned long *maskp,
			int nmaskbits);
extern int bitmap_parselist_user(const char __user *ubuf, unsigned int ulen,
			unsigned long *dst, int nbits);
extern void bitmap_remap(unsigned long *dst, const unsigned long *src,
		const unsigned long *old, const unsigned long *new, unsigned int nbits);
extern int bitmap_bitremap(int oldbit,
		const unsigned long *old, const unsigned long *new, int bits);
extern void bitmap_onto(unsigned long *dst, const unsigned long *orig,
		const unsigned long *relmap, unsigned int bits);
extern void bitmap_fold(unsigned long *dst, const unsigned long *orig,
		unsigned int sz, unsigned int nbits);
extern int bitmap_find_free_region(unsigned long *bitmap, unsigned int bits, int order);
extern void bitmap_release_region(unsigned long *bitmap, unsigned int pos, int order);
extern int bitmap_allocate_region(unsigned long *bitmap, unsigned int pos, int order);

#ifdef __BIG_ENDIAN
extern void bitmap_copy_le(unsigned long *dst, const unsigned long *src, unsigned int nbits);
#else
#define bitmap_copy_le bitmap_copy
#endif
extern unsigned int bitmap_ord_to_pos(const unsigned long *bitmap, unsigned int ord, unsigned int nbits);
extern int bitmap_print_to_pagebuf(bool list, char *buf,
				   const unsigned long *maskp, int nmaskbits);

#define BITMAP_FIRST_WORD_MASK(start) (~0UL << ((start) & (BITS_PER_LONG - 1)))
#define BITMAP_LAST_WORD_MASK(nbits) (~0UL >> (-(nbits) & (BITS_PER_LONG - 1)))


#define small_const_nbits(nbits) \
	(__builtin_constant_p(nbits) && (nbits) <= BITS_PER_LONG && (nbits) > 0)

static inline void bitmap_zero(unsigned long *dst, unsigned int nbits)
{
	unsigned int len = BITS_TO_LONGS(nbits) * sizeof(unsigned long);
	memset(dst, 0, len);
}

static inline void bitmap_fill(unsigned long *dst, unsigned int nbits)
{
	unsigned int len = BITS_TO_LONGS(nbits) * sizeof(unsigned long);
	memset(dst, 0xff, len);
}

static inline void bitmap_copy(unsigned long *dst, const unsigned long *src,
			unsigned int nbits)
{
	unsigned int len = BITS_TO_LONGS(nbits) * sizeof(unsigned long);
	memcpy(dst, src, len);
}


static inline void bitmap_copy_clear_tail(unsigned long *dst,
		const unsigned long *src, unsigned int nbits)
{
	bitmap_copy(dst, src, nbits);
	if (nbits % BITS_PER_LONG)
		dst[nbits / BITS_PER_LONG] &= BITMAP_LAST_WORD_MASK(nbits);
}


#if BITS_PER_LONG == 64
extern void bitmap_from_arr32(unsigned long *bitmap, const u32 *buf,
							unsigned int nbits);
extern void bitmap_to_arr32(u32 *buf, const unsigned long *bitmap,
							unsigned int nbits);
#else
#define bitmap_from_arr32(bitmap, buf, nbits)			\
	bitmap_copy_clear_tail((unsigned long *) (bitmap),	\
			(const unsigned long *) (buf), (nbits))
#define bitmap_to_arr32(buf, bitmap, nbits)			\
	bitmap_copy_clear_tail((unsigned long *) (buf),		\
			(const unsigned long *) (bitmap), (nbits))
#endif

static inline int bitmap_and(unsigned long *dst, const unsigned long *src1,
			const unsigned long *src2, unsigned int nbits)
{
	if (small_const_nbits(nbits))
		return (*dst = *src1 & *src2 & BITMAP_LAST_WORD_MASK(nbits)) != 0;
	return __bitmap_and(dst, src1, src2, nbits);
}

static inline void bitmap_or(unsigned long *dst, const unsigned long *src1,
			const unsigned long *src2, unsigned int nbits)
{
	if (small_const_nbits(nbits))
		*dst = *src1 | *src2;
	else
		__bitmap_or(dst, src1, src2, nbits);
}

static inline void bitmap_xor(unsigned long *dst, const unsigned long *src1,
			const unsigned long *src2, unsigned int nbits)
{
	if (small_const_nbits(nbits))
		*dst = *src1 ^ *src2;
	else
		__bitmap_xor(dst, src1, src2, nbits);
}

static inline int bitmap_andnot(unsigned long *dst, const unsigned long *src1,
			const unsigned long *src2, unsigned int nbits)
{
	if (small_const_nbits(nbits))
		return (*dst = *src1 & ~(*src2) & BITMAP_LAST_WORD_MASK(nbits)) != 0;
	return __bitmap_andnot(dst, src1, src2, nbits);
}

static inline void bitmap_complement(unsigned long *dst, const unsigned long *src,
			unsigned int nbits)
{
	if (small_const_nbits(nbits))
		*dst = ~(*src);
	else
		__bitmap_complement(dst, src, nbits);
}

#ifdef __LITTLE_ENDIAN
#define BITMAP_MEM_ALIGNMENT 8
#else
#define BITMAP_MEM_ALIGNMENT (8 * sizeof(unsigned long))
#endif
#define BITMAP_MEM_MASK (BITMAP_MEM_ALIGNMENT - 1)

static inline int bitmap_equal(const unsigned long *src1,
			const unsigned long *src2, unsigned int nbits)
{
	if (small_const_nbits(nbits))
		return !((*src1 ^ *src2) & BITMAP_LAST_WORD_MASK(nbits));
	if (__builtin_constant_p(nbits & BITMAP_MEM_MASK) &&
	    IS_ALIGNED(nbits, BITMAP_MEM_ALIGNMENT))
		return !memcmp(src1, src2, nbits / 8);
	return __bitmap_equal(src1, src2, nbits);
}

static inline int bitmap_intersects(const unsigned long *src1,
			const unsigned long *src2, unsigned int nbits)
{
	if (small_const_nbits(nbits))
		return ((*src1 & *src2) & BITMAP_LAST_WORD_MASK(nbits)) != 0;
	else
		return __bitmap_intersects(src1, src2, nbits);
}

static inline int bitmap_subset(const unsigned long *src1,
			const unsigned long *src2, unsigned int nbits)
{
	if (small_const_nbits(nbits))
		return ! ((*src1 & ~(*src2)) & BITMAP_LAST_WORD_MASK(nbits));
	else
		return __bitmap_subset(src1, src2, nbits);
}

static inline int bitmap_empty(const unsigned long *src, unsigned nbits)
{
	if (small_const_nbits(nbits))
		return ! (*src & BITMAP_LAST_WORD_MASK(nbits));

	return find_first_bit(src, nbits) == nbits;
}

static inline int bitmap_full(const unsigned long *src, unsigned int nbits)
{
	if (small_const_nbits(nbits))
		return ! (~(*src) & BITMAP_LAST_WORD_MASK(nbits));

	return find_first_zero_bit(src, nbits) == nbits;
}

static __always_inline int bitmap_weight(const unsigned long *src, unsigned int nbits)
{
	if (small_const_nbits(nbits))
		return hweight_long(*src & BITMAP_LAST_WORD_MASK(nbits));
	return __bitmap_weight(src, nbits);
}

static __always_inline void bitmap_set(unsigned long *map, unsigned int start,
		unsigned int nbits)
{
	if (__builtin_constant_p(nbits) && nbits == 1)
		__set_bit(start, map);
	else if (__builtin_constant_p(start & BITMAP_MEM_MASK) &&
		 IS_ALIGNED(start, BITMAP_MEM_ALIGNMENT) &&
		 __builtin_constant_p(nbits & BITMAP_MEM_MASK) &&
		 IS_ALIGNED(nbits, BITMAP_MEM_ALIGNMENT))
		memset((char *)map + start / 8, 0xff, nbits / 8);
	else
		__bitmap_set(map, start, nbits);
}

static __always_inline void bitmap_clear(unsigned long *map, unsigned int start,
		unsigned int nbits)
{
	if (__builtin_constant_p(nbits) && nbits == 1)
		__clear_bit(start, map);
	else if (__builtin_constant_p(start & BITMAP_MEM_MASK) &&
		 IS_ALIGNED(start, BITMAP_MEM_ALIGNMENT) &&
		 __builtin_constant_p(nbits & BITMAP_MEM_MASK) &&
		 IS_ALIGNED(nbits, BITMAP_MEM_ALIGNMENT))
		memset((char *)map + start / 8, 0, nbits / 8);
	else
		__bitmap_clear(map, start, nbits);
}

static inline void bitmap_shift_right(unsigned long *dst, const unsigned long *src,
				unsigned int shift, unsigned int nbits)
{
	if (small_const_nbits(nbits))
		*dst = (*src & BITMAP_LAST_WORD_MASK(nbits)) >> shift;
	else
		__bitmap_shift_right(dst, src, shift, nbits);
}

static inline void bitmap_shift_left(unsigned long *dst, const unsigned long *src,
				unsigned int shift, unsigned int nbits)
{
	if (small_const_nbits(nbits))
		*dst = (*src << shift) & BITMAP_LAST_WORD_MASK(nbits);
	else
		__bitmap_shift_left(dst, src, shift, nbits);
}

static inline int bitmap_parse(const char *buf, unsigned int buflen,
			unsigned long *maskp, int nmaskbits)
{
	return __bitmap_parse(buf, buflen, 0, maskp, nmaskbits);
}


#if __BITS_PER_LONG == 64
#define BITMAP_FROM_U64(n) (n)
#else
#define BITMAP_FROM_U64(n) ((unsigned long) ((u64)(n) & ULONG_MAX)), \
				((unsigned long) ((u64)(n) >> 32))
#endif


static inline void bitmap_from_u64(unsigned long *dst, u64 mask)
{
	dst[0] = mask & ULONG_MAX;

	if (sizeof(mask) > sizeof(unsigned long))
		dst[1] = mask >> 32;
}

#endif 

#endif 
