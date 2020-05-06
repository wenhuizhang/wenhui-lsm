/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __LINUX_MTD_NAND_H
#define __LINUX_MTD_NAND_H

#include <linux/mtd/mtd.h>


struct nand_memory_organization {
	unsigned int bits_per_cell;
	unsigned int pagesize;
	unsigned int oobsize;
	unsigned int pages_per_eraseblock;
	unsigned int eraseblocks_per_lun;
	unsigned int max_bad_eraseblocks_per_lun;
	unsigned int planes_per_lun;
	unsigned int luns_per_target;
	unsigned int ntargets;
};

#define NAND_MEMORG(bpc, ps, os, ppe, epl, mbb, ppl, lpt, nt)	\
	{							\
		.bits_per_cell = (bpc),				\
		.pagesize = (ps),				\
		.oobsize = (os),				\
		.pages_per_eraseblock = (ppe),			\
		.eraseblocks_per_lun = (epl),			\
		.max_bad_eraseblocks_per_lun = (mbb),		\
		.planes_per_lun = (ppl),			\
		.luns_per_target = (lpt),			\
		.ntargets = (nt),				\
	}


struct nand_row_converter {
	unsigned int lun_addr_shift;
	unsigned int eraseblock_addr_shift;
};


struct nand_pos {
	unsigned int target;
	unsigned int lun;
	unsigned int plane;
	unsigned int eraseblock;
	unsigned int page;
};


struct nand_page_io_req {
	struct nand_pos pos;
	unsigned int dataoffs;
	unsigned int datalen;
	union {
		const void *out;
		void *in;
	} databuf;
	unsigned int ooboffs;
	unsigned int ooblen;
	union {
		const void *out;
		void *in;
	} oobbuf;
	int mode;
};


struct nand_ecc_req {
	unsigned int strength;
	unsigned int step_size;
};

#define NAND_ECCREQ(str, stp) { .strength = (str), .step_size = (stp) }


struct nand_bbt {
	unsigned long *cache;
};

struct nand_device;


struct nand_ops {
	int (*erase)(struct nand_device *nand, const struct nand_pos *pos);
	int (*markbad)(struct nand_device *nand, const struct nand_pos *pos);
	bool (*isbad)(struct nand_device *nand, const struct nand_pos *pos);
};


struct nand_device {
	struct mtd_info mtd;
	struct nand_memory_organization memorg;
	struct nand_ecc_req eccreq;
	struct nand_row_converter rowconv;
	struct nand_bbt bbt;
	const struct nand_ops *ops;
};


struct nand_io_iter {
	struct nand_page_io_req req;
	unsigned int oobbytes_per_page;
	unsigned int dataleft;
	unsigned int oobleft;
};


static inline struct nand_device *mtd_to_nanddev(struct mtd_info *mtd)
{
	return container_of(mtd, struct nand_device, mtd);
}


static inline struct mtd_info *nanddev_to_mtd(struct nand_device *nand)
{
	return &nand->mtd;
}


static inline unsigned int nanddev_bits_per_cell(const struct nand_device *nand)
{
	return nand->memorg.bits_per_cell;
}


static inline size_t nanddev_page_size(const struct nand_device *nand)
{
	return nand->memorg.pagesize;
}


static inline unsigned int
nanddev_per_page_oobsize(const struct nand_device *nand)
{
	return nand->memorg.oobsize;
}


static inline unsigned int
nanddev_pages_per_eraseblock(const struct nand_device *nand)
{
	return nand->memorg.pages_per_eraseblock;
}


static inline unsigned int
nanddev_pages_per_target(const struct nand_device *nand)
{
	return nand->memorg.pages_per_eraseblock *
	       nand->memorg.eraseblocks_per_lun *
	       nand->memorg.luns_per_target;
}


static inline size_t nanddev_eraseblock_size(const struct nand_device *nand)
{
	return nand->memorg.pagesize * nand->memorg.pages_per_eraseblock;
}


static inline unsigned int
nanddev_eraseblocks_per_lun(const struct nand_device *nand)
{
	return nand->memorg.eraseblocks_per_lun;
}


static inline unsigned int
nanddev_eraseblocks_per_target(const struct nand_device *nand)
{
	return nand->memorg.eraseblocks_per_lun * nand->memorg.luns_per_target;
}


static inline u64 nanddev_target_size(const struct nand_device *nand)
{
	return (u64)nand->memorg.luns_per_target *
	       nand->memorg.eraseblocks_per_lun *
	       nand->memorg.pages_per_eraseblock *
	       nand->memorg.pagesize;
}


static inline unsigned int nanddev_ntargets(const struct nand_device *nand)
{
	return nand->memorg.ntargets;
}


static inline unsigned int nanddev_neraseblocks(const struct nand_device *nand)
{
	return nand->memorg.ntargets * nand->memorg.luns_per_target *
	       nand->memorg.eraseblocks_per_lun;
}


static inline u64 nanddev_size(const struct nand_device *nand)
{
	return nanddev_target_size(nand) * nanddev_ntargets(nand);
}


static inline struct nand_memory_organization *
nanddev_get_memorg(struct nand_device *nand)
{
	return &nand->memorg;
}

int nanddev_init(struct nand_device *nand, const struct nand_ops *ops,
		 struct module *owner);
void nanddev_cleanup(struct nand_device *nand);


static inline int nanddev_register(struct nand_device *nand)
{
	return mtd_device_register(&nand->mtd, NULL, 0);
}


static inline int nanddev_unregister(struct nand_device *nand)
{
	return mtd_device_unregister(&nand->mtd);
}


static inline void nanddev_set_of_node(struct nand_device *nand,
				       struct device_node *np)
{
	mtd_set_of_node(&nand->mtd, np);
}


static inline struct device_node *nanddev_get_of_node(struct nand_device *nand)
{
	return mtd_get_of_node(&nand->mtd);
}


static inline unsigned int nanddev_offs_to_pos(struct nand_device *nand,
					       loff_t offs,
					       struct nand_pos *pos)
{
	unsigned int pageoffs;
	u64 tmp = offs;

	pageoffs = do_div(tmp, nand->memorg.pagesize);
	pos->page = do_div(tmp, nand->memorg.pages_per_eraseblock);
	pos->eraseblock = do_div(tmp, nand->memorg.eraseblocks_per_lun);
	pos->plane = pos->eraseblock % nand->memorg.planes_per_lun;
	pos->lun = do_div(tmp, nand->memorg.luns_per_target);
	pos->target = tmp;

	return pageoffs;
}


static inline int nanddev_pos_cmp(const struct nand_pos *a,
				  const struct nand_pos *b)
{
	if (a->target != b->target)
		return a->target < b->target ? -1 : 1;

	if (a->lun != b->lun)
		return a->lun < b->lun ? -1 : 1;

	if (a->eraseblock != b->eraseblock)
		return a->eraseblock < b->eraseblock ? -1 : 1;

	if (a->page != b->page)
		return a->page < b->page ? -1 : 1;

	return 0;
}


static inline loff_t nanddev_pos_to_offs(struct nand_device *nand,
					 const struct nand_pos *pos)
{
	unsigned int npages;

	npages = pos->page +
		 ((pos->eraseblock +
		   (pos->lun +
		    (pos->target * nand->memorg.luns_per_target)) *
		   nand->memorg.eraseblocks_per_lun) *
		  nand->memorg.pages_per_eraseblock);

	return (loff_t)npages * nand->memorg.pagesize;
}


static inline unsigned int nanddev_pos_to_row(struct nand_device *nand,
					      const struct nand_pos *pos)
{
	return (pos->lun << nand->rowconv.lun_addr_shift) |
	       (pos->eraseblock << nand->rowconv.eraseblock_addr_shift) |
	       pos->page;
}


static inline void nanddev_pos_next_target(struct nand_device *nand,
					   struct nand_pos *pos)
{
	pos->page = 0;
	pos->plane = 0;
	pos->eraseblock = 0;
	pos->lun = 0;
	pos->target++;
}


static inline void nanddev_pos_next_lun(struct nand_device *nand,
					struct nand_pos *pos)
{
	if (pos->lun >= nand->memorg.luns_per_target - 1)
		return nanddev_pos_next_target(nand, pos);

	pos->lun++;
	pos->page = 0;
	pos->plane = 0;
	pos->eraseblock = 0;
}


static inline void nanddev_pos_next_eraseblock(struct nand_device *nand,
					       struct nand_pos *pos)
{
	if (pos->eraseblock >= nand->memorg.eraseblocks_per_lun - 1)
		return nanddev_pos_next_lun(nand, pos);

	pos->eraseblock++;
	pos->page = 0;
	pos->plane = pos->eraseblock % nand->memorg.planes_per_lun;
}


static inline void nanddev_pos_next_page(struct nand_device *nand,
					 struct nand_pos *pos)
{
	if (pos->page >= nand->memorg.pages_per_eraseblock - 1)
		return nanddev_pos_next_eraseblock(nand, pos);

	pos->page++;
}


static inline void nanddev_io_iter_init(struct nand_device *nand,
					loff_t offs, struct mtd_oob_ops *req,
					struct nand_io_iter *iter)
{
	struct mtd_info *mtd = nanddev_to_mtd(nand);

	iter->req.mode = req->mode;
	iter->req.dataoffs = nanddev_offs_to_pos(nand, offs, &iter->req.pos);
	iter->req.ooboffs = req->ooboffs;
	iter->oobbytes_per_page = mtd_oobavail(mtd, req);
	iter->dataleft = req->len;
	iter->oobleft = req->ooblen;
	iter->req.databuf.in = req->datbuf;
	iter->req.datalen = min_t(unsigned int,
				  nand->memorg.pagesize - iter->req.dataoffs,
				  iter->dataleft);
	iter->req.oobbuf.in = req->oobbuf;
	iter->req.ooblen = min_t(unsigned int,
				 iter->oobbytes_per_page - iter->req.ooboffs,
				 iter->oobleft);
}


static inline void nanddev_io_iter_next_page(struct nand_device *nand,
					     struct nand_io_iter *iter)
{
	nanddev_pos_next_page(nand, &iter->req.pos);
	iter->dataleft -= iter->req.datalen;
	iter->req.databuf.in += iter->req.datalen;
	iter->oobleft -= iter->req.ooblen;
	iter->req.oobbuf.in += iter->req.ooblen;
	iter->req.dataoffs = 0;
	iter->req.ooboffs = 0;
	iter->req.datalen = min_t(unsigned int, nand->memorg.pagesize,
				  iter->dataleft);
	iter->req.ooblen = min_t(unsigned int, iter->oobbytes_per_page,
				 iter->oobleft);
}


static inline bool nanddev_io_iter_end(struct nand_device *nand,
				       const struct nand_io_iter *iter)
{
	if (iter->dataleft || iter->oobleft)
		return false;

	return true;
}


#define nanddev_io_for_each_page(nand, start, req, iter)		\
	for (nanddev_io_iter_init(nand, start, req, iter);		\
	     !nanddev_io_iter_end(nand, iter);				\
	     nanddev_io_iter_next_page(nand, iter))

bool nanddev_isbad(struct nand_device *nand, const struct nand_pos *pos);
bool nanddev_isreserved(struct nand_device *nand, const struct nand_pos *pos);
int nanddev_erase(struct nand_device *nand, const struct nand_pos *pos);
int nanddev_markbad(struct nand_device *nand, const struct nand_pos *pos);


enum nand_bbt_block_status {
	NAND_BBT_BLOCK_STATUS_UNKNOWN,
	NAND_BBT_BLOCK_GOOD,
	NAND_BBT_BLOCK_WORN,
	NAND_BBT_BLOCK_RESERVED,
	NAND_BBT_BLOCK_FACTORY_BAD,
	NAND_BBT_BLOCK_NUM_STATUS,
};

int nanddev_bbt_init(struct nand_device *nand);
void nanddev_bbt_cleanup(struct nand_device *nand);
int nanddev_bbt_update(struct nand_device *nand);
int nanddev_bbt_get_block_status(const struct nand_device *nand,
				 unsigned int entry);
int nanddev_bbt_set_block_status(struct nand_device *nand, unsigned int entry,
				 enum nand_bbt_block_status status);
int nanddev_bbt_markbad(struct nand_device *nand, unsigned int block);


static inline unsigned int nanddev_bbt_pos_to_entry(struct nand_device *nand,
						    const struct nand_pos *pos)
{
	return pos->eraseblock +
	       ((pos->lun + (pos->target * nand->memorg.luns_per_target)) *
		nand->memorg.eraseblocks_per_lun);
}


static inline bool nanddev_bbt_is_initialized(struct nand_device *nand)
{
	return !!nand->bbt.cache;
}


int nanddev_mtd_erase(struct mtd_info *mtd, struct erase_info *einfo);
int nanddev_mtd_max_bad_blocks(struct mtd_info *mtd, loff_t offs, size_t len);

#endif 
