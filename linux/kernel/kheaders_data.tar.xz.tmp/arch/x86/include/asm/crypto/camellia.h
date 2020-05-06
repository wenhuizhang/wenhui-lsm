/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ASM_X86_CAMELLIA_H
#define ASM_X86_CAMELLIA_H

#include <crypto/b128ops.h>
#include <linux/crypto.h>
#include <linux/kernel.h>

#define CAMELLIA_MIN_KEY_SIZE	16
#define CAMELLIA_MAX_KEY_SIZE	32
#define CAMELLIA_BLOCK_SIZE	16
#define CAMELLIA_TABLE_BYTE_LEN	272
#define CAMELLIA_PARALLEL_BLOCKS 2

struct crypto_skcipher;

struct camellia_ctx {
	u64 key_table[CAMELLIA_TABLE_BYTE_LEN / sizeof(u64)];
	u32 key_length;
};

struct camellia_xts_ctx {
	struct camellia_ctx tweak_ctx;
	struct camellia_ctx crypt_ctx;
};

extern int __camellia_setkey(struct camellia_ctx *cctx,
			     const unsigned char *key,
			     unsigned int key_len, u32 *flags);

extern int xts_camellia_setkey(struct crypto_skcipher *tfm, const u8 *key,
			       unsigned int keylen);


asmlinkage void __camellia_enc_blk(struct camellia_ctx *ctx, u8 *dst,
				   const u8 *src, bool xor);
asmlinkage void camellia_dec_blk(struct camellia_ctx *ctx, u8 *dst,
				 const u8 *src);


asmlinkage void __camellia_enc_blk_2way(struct camellia_ctx *ctx, u8 *dst,
					const u8 *src, bool xor);
asmlinkage void camellia_dec_blk_2way(struct camellia_ctx *ctx, u8 *dst,
				      const u8 *src);


asmlinkage void camellia_ecb_enc_16way(struct camellia_ctx *ctx, u8 *dst,
				       const u8 *src);
asmlinkage void camellia_ecb_dec_16way(struct camellia_ctx *ctx, u8 *dst,
				       const u8 *src);

asmlinkage void camellia_cbc_dec_16way(struct camellia_ctx *ctx, u8 *dst,
				       const u8 *src);
asmlinkage void camellia_ctr_16way(struct camellia_ctx *ctx, u8 *dst,
				   const u8 *src, le128 *iv);

asmlinkage void camellia_xts_enc_16way(struct camellia_ctx *ctx, u8 *dst,
				       const u8 *src, le128 *iv);
asmlinkage void camellia_xts_dec_16way(struct camellia_ctx *ctx, u8 *dst,
				       const u8 *src, le128 *iv);

static inline void camellia_enc_blk(struct camellia_ctx *ctx, u8 *dst,
				    const u8 *src)
{
	__camellia_enc_blk(ctx, dst, src, false);
}

static inline void camellia_enc_blk_xor(struct camellia_ctx *ctx, u8 *dst,
					const u8 *src)
{
	__camellia_enc_blk(ctx, dst, src, true);
}

static inline void camellia_enc_blk_2way(struct camellia_ctx *ctx, u8 *dst,
					 const u8 *src)
{
	__camellia_enc_blk_2way(ctx, dst, src, false);
}

static inline void camellia_enc_blk_xor_2way(struct camellia_ctx *ctx, u8 *dst,
					     const u8 *src)
{
	__camellia_enc_blk_2way(ctx, dst, src, true);
}


extern void camellia_decrypt_cbc_2way(void *ctx, u128 *dst, const u128 *src);
extern void camellia_crypt_ctr(void *ctx, u128 *dst, const u128 *src,
			       le128 *iv);
extern void camellia_crypt_ctr_2way(void *ctx, u128 *dst, const u128 *src,
				    le128 *iv);

extern void camellia_xts_enc(void *ctx, u128 *dst, const u128 *src, le128 *iv);
extern void camellia_xts_dec(void *ctx, u128 *dst, const u128 *src, le128 *iv);

#endif 