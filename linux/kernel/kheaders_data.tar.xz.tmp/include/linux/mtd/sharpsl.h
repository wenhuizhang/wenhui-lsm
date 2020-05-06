/* SPDX-License-Identifier: GPL-2.0-only */


#include <linux/mtd/rawnand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>

struct sharpsl_nand_platform_data {
	struct nand_bbt_descr	*badblock_pattern;
	const struct mtd_ooblayout_ops *ecc_layout;
	struct mtd_partition	*partitions;
	unsigned int		nr_partitions;
	const char *const	*part_parsers;
};
