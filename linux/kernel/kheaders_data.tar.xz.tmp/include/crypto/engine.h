/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef _CRYPTO_ENGINE_H
#define _CRYPTO_ENGINE_H

#include <linux/crypto.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <crypto/algapi.h>
#include <crypto/aead.h>
#include <crypto/akcipher.h>
#include <crypto/hash.h>
#include <crypto/skcipher.h>

#define ENGINE_NAME_LEN	30

struct crypto_engine {
	char			name[ENGINE_NAME_LEN];
	bool			idling;
	bool			busy;
	bool			running;
	bool			cur_req_prepared;

	struct list_head	list;
	spinlock_t		queue_lock;
	struct crypto_queue	queue;
	struct device		*dev;

	bool			rt;

	int (*prepare_crypt_hardware)(struct crypto_engine *engine);
	int (*unprepare_crypt_hardware)(struct crypto_engine *engine);

	struct kthread_worker           *kworker;
	struct kthread_work             pump_requests;

	void				*priv_data;
	struct crypto_async_request	*cur_req;
};


struct crypto_engine_op {
	int (*prepare_request)(struct crypto_engine *engine,
			       void *areq);
	int (*unprepare_request)(struct crypto_engine *engine,
				 void *areq);
	int (*do_one_request)(struct crypto_engine *engine,
			      void *areq);
};

struct crypto_engine_ctx {
	struct crypto_engine_op op;
};

int crypto_transfer_ablkcipher_request_to_engine(struct crypto_engine *engine,
						 struct ablkcipher_request *req);
int crypto_transfer_aead_request_to_engine(struct crypto_engine *engine,
					   struct aead_request *req);
int crypto_transfer_akcipher_request_to_engine(struct crypto_engine *engine,
					       struct akcipher_request *req);
int crypto_transfer_hash_request_to_engine(struct crypto_engine *engine,
					       struct ahash_request *req);
int crypto_transfer_skcipher_request_to_engine(struct crypto_engine *engine,
					       struct skcipher_request *req);
void crypto_finalize_ablkcipher_request(struct crypto_engine *engine,
					struct ablkcipher_request *req, int err);
void crypto_finalize_aead_request(struct crypto_engine *engine,
				  struct aead_request *req, int err);
void crypto_finalize_akcipher_request(struct crypto_engine *engine,
				      struct akcipher_request *req, int err);
void crypto_finalize_hash_request(struct crypto_engine *engine,
				  struct ahash_request *req, int err);
void crypto_finalize_skcipher_request(struct crypto_engine *engine,
				      struct skcipher_request *req, int err);
int crypto_engine_start(struct crypto_engine *engine);
int crypto_engine_stop(struct crypto_engine *engine);
struct crypto_engine *crypto_engine_alloc_init(struct device *dev, bool rt);
int crypto_engine_exit(struct crypto_engine *engine);

#endif 
