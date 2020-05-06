/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _DT_BINDINGS_MAILBOX_TEGRA186_HSP_H
#define _DT_BINDINGS_MAILBOX_TEGRA186_HSP_H


#define TEGRA_HSP_MBOX_TYPE_DB 0x0
#define TEGRA_HSP_MBOX_TYPE_SM 0x1
#define TEGRA_HSP_MBOX_TYPE_SS 0x2
#define TEGRA_HSP_MBOX_TYPE_AS 0x3


#define TEGRA_HSP_DB_MASTER_CCPLEX 17
#define TEGRA_HSP_DB_MASTER_BPMP 19


#define TEGRA_HSP_SM_MASK 0x00ffffff
#define TEGRA_HSP_SM_FLAG_RX (0 << 31)
#define TEGRA_HSP_SM_FLAG_TX (1 << 31)

#define TEGRA_HSP_SM_RX(x) (TEGRA_HSP_SM_FLAG_RX | ((x) & TEGRA_HSP_SM_MASK))
#define TEGRA_HSP_SM_TX(x) (TEGRA_HSP_SM_FLAG_TX | ((x) & TEGRA_HSP_SM_MASK))

#endif