/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef _CRYPTO_DH_
#define _CRYPTO_DH_




struct dh {
	void *key;
	void *p;
	void *q;
	void *g;
	unsigned int key_size;
	unsigned int p_size;
	unsigned int q_size;
	unsigned int g_size;
};


unsigned int crypto_dh_key_len(const struct dh *params);


int crypto_dh_encode_key(char *buf, unsigned int len, const struct dh *params);


int crypto_dh_decode_key(const char *buf, unsigned int len, struct dh *params);

#endif
