/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __LINUX_SOC_EXYNOS_PMU_H
#define __LINUX_SOC_EXYNOS_PMU_H

struct regmap;

enum sys_powerdown {
	SYS_AFTR,
	SYS_LPA,
	SYS_SLEEP,
	NUM_SYS_POWERDOWN,
};

extern void exynos_sys_powerdown_conf(enum sys_powerdown mode);
#ifdef CONFIG_EXYNOS_PMU
extern struct regmap *exynos_get_pmu_regmap(void);
#else
static inline struct regmap *exynos_get_pmu_regmap(void)
{
	return ERR_PTR(-ENODEV);
}
#endif

#endif 
