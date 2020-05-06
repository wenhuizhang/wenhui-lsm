/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _HWBM_H
#define _HWBM_H

struct hwbm_pool {
	
	int size;
	
	int frag_size;
	
	int buf_num;
	
	int (*construct)(struct hwbm_pool *bm_pool, void *buf);
	
	struct mutex buf_lock;
	
	void *priv;
};
#ifdef CONFIG_HWBM
void hwbm_buf_free(struct hwbm_pool *bm_pool, void *buf);
int hwbm_pool_refill(struct hwbm_pool *bm_pool, gfp_t gfp);
int hwbm_pool_add(struct hwbm_pool *bm_pool, unsigned int buf_num);
#else
void hwbm_buf_free(struct hwbm_pool *bm_pool, void *buf) {}
int hwbm_pool_refill(struct hwbm_pool *bm_pool, gfp_t gfp) { return 0; }
int hwbm_pool_add(struct hwbm_pool *bm_pool, unsigned int buf_num)
{ return 0; }
#endif 
#endif 
