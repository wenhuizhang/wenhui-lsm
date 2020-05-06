/* SPDX-License-Identifier: ((GPL-2.0 WITH Linux-syscall-note) OR Linux-OpenIB) */


#ifndef _UAPI_LINUX_TLS_H
#define _UAPI_LINUX_TLS_H

#include <linux/types.h>


#define TLS_TX			1	
#define TLS_RX			2	


#define TLS_VERSION_MINOR(ver)	((ver) & 0xFF)
#define TLS_VERSION_MAJOR(ver)	(((ver) >> 8) & 0xFF)

#define TLS_VERSION_NUMBER(id)	((((id##_VERSION_MAJOR) & 0xFF) << 8) |	\
				 ((id##_VERSION_MINOR) & 0xFF))

#define TLS_1_2_VERSION_MAJOR	0x3
#define TLS_1_2_VERSION_MINOR	0x3
#define TLS_1_2_VERSION		TLS_VERSION_NUMBER(TLS_1_2)

#define TLS_1_3_VERSION_MAJOR	0x3
#define TLS_1_3_VERSION_MINOR	0x4
#define TLS_1_3_VERSION		TLS_VERSION_NUMBER(TLS_1_3)


#define TLS_CIPHER_AES_GCM_128				51
#define TLS_CIPHER_AES_GCM_128_IV_SIZE			8
#define TLS_CIPHER_AES_GCM_128_KEY_SIZE		16
#define TLS_CIPHER_AES_GCM_128_SALT_SIZE		4
#define TLS_CIPHER_AES_GCM_128_TAG_SIZE		16
#define TLS_CIPHER_AES_GCM_128_REC_SEQ_SIZE		8

#define TLS_CIPHER_AES_GCM_256				52
#define TLS_CIPHER_AES_GCM_256_IV_SIZE			8
#define TLS_CIPHER_AES_GCM_256_KEY_SIZE		32
#define TLS_CIPHER_AES_GCM_256_SALT_SIZE		4
#define TLS_CIPHER_AES_GCM_256_TAG_SIZE		16
#define TLS_CIPHER_AES_GCM_256_REC_SEQ_SIZE		8

#define TLS_CIPHER_AES_CCM_128				53
#define TLS_CIPHER_AES_CCM_128_IV_SIZE			8
#define TLS_CIPHER_AES_CCM_128_KEY_SIZE		16
#define TLS_CIPHER_AES_CCM_128_SALT_SIZE		4
#define TLS_CIPHER_AES_CCM_128_TAG_SIZE		16
#define TLS_CIPHER_AES_CCM_128_REC_SEQ_SIZE		8

#define TLS_SET_RECORD_TYPE	1
#define TLS_GET_RECORD_TYPE	2

struct tls_crypto_info {
	__u16 version;
	__u16 cipher_type;
};

struct tls12_crypto_info_aes_gcm_128 {
	struct tls_crypto_info info;
	unsigned char iv[TLS_CIPHER_AES_GCM_128_IV_SIZE];
	unsigned char key[TLS_CIPHER_AES_GCM_128_KEY_SIZE];
	unsigned char salt[TLS_CIPHER_AES_GCM_128_SALT_SIZE];
	unsigned char rec_seq[TLS_CIPHER_AES_GCM_128_REC_SEQ_SIZE];
};

struct tls12_crypto_info_aes_gcm_256 {
	struct tls_crypto_info info;
	unsigned char iv[TLS_CIPHER_AES_GCM_256_IV_SIZE];
	unsigned char key[TLS_CIPHER_AES_GCM_256_KEY_SIZE];
	unsigned char salt[TLS_CIPHER_AES_GCM_256_SALT_SIZE];
	unsigned char rec_seq[TLS_CIPHER_AES_GCM_256_REC_SEQ_SIZE];
};

struct tls12_crypto_info_aes_ccm_128 {
	struct tls_crypto_info info;
	unsigned char iv[TLS_CIPHER_AES_CCM_128_IV_SIZE];
	unsigned char key[TLS_CIPHER_AES_CCM_128_KEY_SIZE];
	unsigned char salt[TLS_CIPHER_AES_CCM_128_SALT_SIZE];
	unsigned char rec_seq[TLS_CIPHER_AES_CCM_128_REC_SEQ_SIZE];
};

#endif 
