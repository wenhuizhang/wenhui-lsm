/* SPDX-License-Identifier: GPL-2.0-only */


struct timeriomem_rng_data {
	void __iomem		*address;

	
	unsigned int		period;

	
	unsigned int		quality;
};
