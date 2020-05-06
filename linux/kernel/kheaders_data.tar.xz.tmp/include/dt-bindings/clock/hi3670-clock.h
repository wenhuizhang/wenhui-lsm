/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __DT_BINDINGS_CLOCK_HI3670_H
#define __DT_BINDINGS_CLOCK_HI3670_H


#define HI3670_CLK_STUB_CLUSTER0		0
#define HI3670_CLK_STUB_CLUSTER1		1
#define HI3670_CLK_STUB_GPU			2
#define HI3670_CLK_STUB_DDR			3
#define HI3670_CLK_STUB_DDR_VOTE		4
#define HI3670_CLK_STUB_DDR_LIMIT		5
#define HI3670_CLK_STUB_NUM			6


#define HI3670_CLKIN_SYS			0
#define HI3670_CLKIN_REF			1
#define HI3670_CLK_FLL_SRC			2
#define HI3670_CLK_PPLL0			3
#define HI3670_CLK_PPLL1			4
#define HI3670_CLK_PPLL2			5
#define HI3670_CLK_PPLL3			6
#define HI3670_CLK_PPLL4			7
#define HI3670_CLK_PPLL6			8
#define HI3670_CLK_PPLL7			9
#define HI3670_CLK_PPLL_PCIE			10
#define HI3670_CLK_PCIEPLL_REV			11
#define HI3670_CLK_SCPLL			12
#define HI3670_PCLK				13
#define HI3670_CLK_UART0_DBG			14
#define HI3670_CLK_UART6			15
#define HI3670_OSC32K				16
#define HI3670_OSC19M				17
#define HI3670_CLK_480M				18
#define HI3670_CLK_INVALID			19
#define HI3670_CLK_DIV_SYSBUS			20
#define HI3670_CLK_FACTOR_MMC			21
#define HI3670_CLK_SD_SYS			22
#define HI3670_CLK_SDIO_SYS			23
#define HI3670_CLK_DIV_A53HPM			24
#define HI3670_CLK_DIV_320M			25
#define HI3670_PCLK_GATE_UART0			26
#define HI3670_CLK_FACTOR_UART0			27
#define HI3670_CLK_FACTOR_USB3PHY_PLL		28
#define HI3670_CLK_GATE_ABB_USB			29
#define HI3670_CLK_GATE_UFSPHY_REF		30
#define HI3670_ICS_VOLT_HIGH			31
#define HI3670_ICS_VOLT_MIDDLE			32
#define HI3670_VENC_VOLT_HOLD			33
#define HI3670_VDEC_VOLT_HOLD			34
#define HI3670_EDC_VOLT_HOLD			35
#define HI3670_CLK_ISP_SNCLK_FAC		36
#define HI3670_CLK_FACTOR_RXDPHY		37
#define HI3670_AUTODIV_SYSBUS			38
#define HI3670_AUTODIV_EMMC0BUS			39
#define HI3670_PCLK_ANDGT_MMC1_PCIE		40
#define HI3670_CLK_GATE_VCODECBUS_GT		41
#define HI3670_CLK_ANDGT_SD			42
#define HI3670_CLK_SD_SYS_GT			43
#define HI3670_CLK_ANDGT_SDIO			44
#define HI3670_CLK_SDIO_SYS_GT			45
#define HI3670_CLK_A53HPM_ANDGT			46
#define HI3670_CLK_320M_PLL_GT			47
#define HI3670_CLK_ANDGT_UARTH			48
#define HI3670_CLK_ANDGT_UARTL			49
#define HI3670_CLK_ANDGT_UART0			50
#define HI3670_CLK_ANDGT_SPI			51
#define HI3670_CLK_ANDGT_PCIEAXI		52
#define HI3670_CLK_DIV_AO_ASP_GT		53
#define HI3670_CLK_GATE_CSI_TRANS		54
#define HI3670_CLK_GATE_DSI_TRANS		55
#define HI3670_CLK_ANDGT_PTP			56
#define HI3670_CLK_ANDGT_OUT0			57
#define HI3670_CLK_ANDGT_OUT1			58
#define HI3670_CLKGT_DP_AUDIO_PLL_AO		59
#define HI3670_CLK_ANDGT_VDEC			60
#define HI3670_CLK_ANDGT_VENC			61
#define HI3670_CLK_ISP_SNCLK_ANGT		62
#define HI3670_CLK_ANDGT_RXDPHY			63
#define HI3670_CLK_ANDGT_ICS			64
#define HI3670_AUTODIV_DMABUS			65
#define HI3670_CLK_MUX_SYSBUS			66
#define HI3670_CLK_MUX_VCODECBUS		67
#define HI3670_CLK_MUX_SD_SYS			68
#define HI3670_CLK_MUX_SD_PLL			69
#define HI3670_CLK_MUX_SDIO_SYS			70
#define HI3670_CLK_MUX_SDIO_PLL			71
#define HI3670_CLK_MUX_A53HPM			72
#define HI3670_CLK_MUX_320M			73
#define HI3670_CLK_MUX_UARTH			74
#define HI3670_CLK_MUX_UARTL			75
#define HI3670_CLK_MUX_UART0			76
#define HI3670_CLK_MUX_I2C			77
#define HI3670_CLK_MUX_SPI			78
#define HI3670_CLK_MUX_PCIEAXI			79
#define HI3670_CLK_MUX_AO_ASP			80
#define HI3670_CLK_MUX_VDEC			81
#define HI3670_CLK_MUX_VENC			82
#define HI3670_CLK_ISP_SNCLK_MUX0		83
#define HI3670_CLK_ISP_SNCLK_MUX1		84
#define HI3670_CLK_ISP_SNCLK_MUX2		85
#define HI3670_CLK_MUX_RXDPHY_CFG		86
#define HI3670_CLK_MUX_ICS			87
#define HI3670_CLK_DIV_CFGBUS			88
#define HI3670_CLK_DIV_MMC0BUS			89
#define HI3670_CLK_DIV_MMC1BUS			90
#define HI3670_PCLK_DIV_MMC1_PCIE		91
#define HI3670_CLK_DIV_VCODECBUS		92
#define HI3670_CLK_DIV_SD			93
#define HI3670_CLK_DIV_SDIO			94
#define HI3670_CLK_DIV_UARTH			95
#define HI3670_CLK_DIV_UARTL			96
#define HI3670_CLK_DIV_UART0			97
#define HI3670_CLK_DIV_I2C			98
#define HI3670_CLK_DIV_SPI			99
#define HI3670_CLK_DIV_PCIEAXI			100
#define HI3670_CLK_DIV_AO_ASP			101
#define HI3670_CLK_DIV_CSI_TRANS		102
#define HI3670_CLK_DIV_DSI_TRANS		103
#define HI3670_CLK_DIV_PTP			104
#define HI3670_CLK_DIV_CLKOUT0_PLL		105
#define HI3670_CLK_DIV_CLKOUT1_PLL		106
#define HI3670_CLKDIV_DP_AUDIO_PLL_AO		107
#define HI3670_CLK_DIV_VDEC			108
#define HI3670_CLK_DIV_VENC			109
#define HI3670_CLK_ISP_SNCLK_DIV0		110
#define HI3670_CLK_ISP_SNCLK_DIV1		111
#define HI3670_CLK_ISP_SNCLK_DIV2		112
#define HI3670_CLK_DIV_ICS			113
#define HI3670_PPLL1_EN_ACPU			114
#define HI3670_PPLL2_EN_ACPU			115
#define HI3670_PPLL3_EN_ACPU			116
#define HI3670_PPLL1_GT_CPU			117
#define HI3670_PPLL2_GT_CPU			118
#define HI3670_PPLL3_GT_CPU			119
#define HI3670_CLK_GATE_PPLL2_MEDIA		120
#define HI3670_CLK_GATE_PPLL3_MEDIA		121
#define HI3670_CLK_GATE_PPLL4_MEDIA		122
#define HI3670_CLK_GATE_PPLL6_MEDIA		123
#define HI3670_CLK_GATE_PPLL7_MEDIA		124
#define HI3670_PCLK_GPIO0			125
#define HI3670_PCLK_GPIO1			126
#define HI3670_PCLK_GPIO2			127
#define HI3670_PCLK_GPIO3			128
#define HI3670_PCLK_GPIO4			129
#define HI3670_PCLK_GPIO5			130
#define HI3670_PCLK_GPIO6			131
#define HI3670_PCLK_GPIO7			132
#define HI3670_PCLK_GPIO8			133
#define HI3670_PCLK_GPIO9			134
#define HI3670_PCLK_GPIO10			135
#define HI3670_PCLK_GPIO11			136
#define HI3670_PCLK_GPIO12			137
#define HI3670_PCLK_GPIO13			138
#define HI3670_PCLK_GPIO14			139
#define HI3670_PCLK_GPIO15			140
#define HI3670_PCLK_GPIO16			141
#define HI3670_PCLK_GPIO17			142
#define HI3670_PCLK_GPIO20			143
#define HI3670_PCLK_GPIO21			144
#define HI3670_PCLK_GATE_DSI0			145
#define HI3670_PCLK_GATE_DSI1			146
#define HI3670_HCLK_GATE_USB3OTG		147
#define HI3670_ACLK_GATE_USB3DVFS		148
#define HI3670_HCLK_GATE_SDIO			149
#define HI3670_PCLK_GATE_PCIE_SYS		150
#define HI3670_PCLK_GATE_PCIE_PHY		151
#define HI3670_PCLK_GATE_MMC1_PCIE		152
#define HI3670_PCLK_GATE_MMC0_IOC		153
#define HI3670_PCLK_GATE_MMC1_IOC		154
#define HI3670_CLK_GATE_DMAC			155
#define HI3670_CLK_GATE_VCODECBUS2DDR		156
#define HI3670_CLK_CCI400_BYPASS		157
#define HI3670_CLK_GATE_CCI400			158
#define HI3670_CLK_GATE_SD			159
#define HI3670_HCLK_GATE_SD			160
#define HI3670_CLK_GATE_SDIO			161
#define HI3670_CLK_GATE_A57HPM			162
#define HI3670_CLK_GATE_A53HPM			163
#define HI3670_CLK_GATE_PA_A53			164
#define HI3670_CLK_GATE_PA_A57			165
#define HI3670_CLK_GATE_PA_G3D			166
#define HI3670_CLK_GATE_GPUHPM			167
#define HI3670_CLK_GATE_PERIHPM			168
#define HI3670_CLK_GATE_AOHPM			169
#define HI3670_CLK_GATE_UART1			170
#define HI3670_CLK_GATE_UART4			171
#define HI3670_PCLK_GATE_UART1			172
#define HI3670_PCLK_GATE_UART4			173
#define HI3670_CLK_GATE_UART2			174
#define HI3670_CLK_GATE_UART5			175
#define HI3670_PCLK_GATE_UART2			176
#define HI3670_PCLK_GATE_UART5			177
#define HI3670_CLK_GATE_UART0			178
#define HI3670_CLK_GATE_I2C3			179
#define HI3670_CLK_GATE_I2C4			180
#define HI3670_CLK_GATE_I2C7			181
#define HI3670_PCLK_GATE_I2C3			182
#define HI3670_PCLK_GATE_I2C4			183
#define HI3670_PCLK_GATE_I2C7			184
#define HI3670_CLK_GATE_SPI1			185
#define HI3670_CLK_GATE_SPI4			186
#define HI3670_PCLK_GATE_SPI1			187
#define HI3670_PCLK_GATE_SPI4			188
#define HI3670_CLK_GATE_USB3OTG_REF		189
#define HI3670_CLK_GATE_USB2PHY_REF		190
#define HI3670_CLK_GATE_PCIEAUX			191
#define HI3670_ACLK_GATE_PCIE			192
#define HI3670_CLK_GATE_MMC1_PCIEAXI		193
#define HI3670_CLK_GATE_PCIEPHY_REF		194
#define HI3670_CLK_GATE_PCIE_DEBOUNCE		195
#define HI3670_CLK_GATE_PCIEIO			196
#define HI3670_CLK_GATE_PCIE_HP			197
#define HI3670_CLK_GATE_AO_ASP			198
#define HI3670_PCLK_GATE_PCTRL			199
#define HI3670_CLK_CSI_TRANS_GT			200
#define HI3670_CLK_DSI_TRANS_GT			201
#define HI3670_CLK_GATE_PWM			202
#define HI3670_ABB_AUDIO_EN0			203
#define HI3670_ABB_AUDIO_EN1			204
#define HI3670_ABB_AUDIO_GT_EN0			205
#define HI3670_ABB_AUDIO_GT_EN1			206
#define HI3670_CLK_GATE_DP_AUDIO_PLL_AO		207
#define HI3670_PERI_VOLT_HOLD			208
#define HI3670_PERI_VOLT_MIDDLE			209
#define HI3670_CLK_GATE_ISP_SNCLK0		210
#define HI3670_CLK_GATE_ISP_SNCLK1		211
#define HI3670_CLK_GATE_ISP_SNCLK2		212
#define HI3670_CLK_GATE_RXDPHY0_CFG		213
#define HI3670_CLK_GATE_RXDPHY1_CFG		214
#define HI3670_CLK_GATE_RXDPHY2_CFG		215
#define HI3670_CLK_GATE_TXDPHY0_CFG		216
#define HI3670_CLK_GATE_TXDPHY0_REF		217
#define HI3670_CLK_GATE_TXDPHY1_CFG		218
#define HI3670_CLK_GATE_TXDPHY1_REF		219
#define HI3670_CLK_GATE_MEDIA_TCXO		220


#define HI3670_CLK_ANDGT_IOPERI			0
#define HI3670_CLKANDGT_ASP_SUBSYS_PERI		1
#define HI3670_CLK_ANGT_ASP_SUBSYS		2
#define HI3670_CLK_MUX_UFS_SUBSYS		3
#define HI3670_CLK_MUX_CLKOUT0			4
#define HI3670_CLK_MUX_CLKOUT1			5
#define HI3670_CLK_MUX_ASP_SUBSYS_PERI		6
#define HI3670_CLK_MUX_ASP_PLL			7
#define HI3670_CLK_DIV_AOBUS			8
#define HI3670_CLK_DIV_UFS_SUBSYS		9
#define HI3670_CLK_DIV_IOPERI			10
#define HI3670_CLK_DIV_CLKOUT0_TCXO		11
#define HI3670_CLK_DIV_CLKOUT1_TCXO		12
#define HI3670_CLK_ASP_SUBSYS_PERI_DIV		13
#define HI3670_CLK_DIV_ASP_SUBSYS		14
#define HI3670_PPLL0_EN_ACPU			15
#define HI3670_PPLL0_GT_CPU			16
#define HI3670_CLK_GATE_PPLL0_MEDIA		17
#define HI3670_PCLK_GPIO18			18
#define HI3670_PCLK_GPIO19			19
#define HI3670_CLK_GATE_SPI			20
#define HI3670_PCLK_GATE_SPI			21
#define HI3670_CLK_GATE_UFS_SUBSYS		22
#define HI3670_CLK_GATE_UFSIO_REF		23
#define HI3670_PCLK_AO_GPIO0			24
#define HI3670_PCLK_AO_GPIO1			25
#define HI3670_PCLK_AO_GPIO2			26
#define HI3670_PCLK_AO_GPIO3			27
#define HI3670_PCLK_AO_GPIO4			28
#define HI3670_PCLK_AO_GPIO5			29
#define HI3670_PCLK_AO_GPIO6			30
#define HI3670_CLK_GATE_OUT0			31
#define HI3670_CLK_GATE_OUT1			32
#define HI3670_PCLK_GATE_SYSCNT			33
#define HI3670_CLK_GATE_SYSCNT			34
#define HI3670_CLK_GATE_ASP_SUBSYS_PERI		35
#define HI3670_CLK_GATE_ASP_SUBSYS		36
#define HI3670_CLK_GATE_ASP_TCXO		37
#define HI3670_CLK_GATE_DP_AUDIO_PLL		38


#define HI3670_GATE_ABB_192			0


#define HI3670_GATE_UFS_TCXO_EN			0
#define HI3670_GATE_USB_TCXO_EN			1


#define HI3670_CLK_GATE_I2C0			0
#define HI3670_CLK_GATE_I2C1			1
#define HI3670_CLK_GATE_I2C2			2
#define HI3670_CLK_GATE_SPI0			3
#define HI3670_CLK_GATE_SPI2			4
#define HI3670_CLK_GATE_UART3			5
#define HI3670_CLK_I2C0_GATE_IOMCU		6
#define HI3670_CLK_I2C1_GATE_IOMCU		7
#define HI3670_CLK_I2C2_GATE_IOMCU		8
#define HI3670_CLK_SPI0_GATE_IOMCU		9
#define HI3670_CLK_SPI2_GATE_IOMCU		10
#define HI3670_CLK_UART3_GATE_IOMCU		11
#define HI3670_CLK_GATE_PERI0_IOMCU		12


#define HI3670_CLK_GATE_VIVOBUS_ANDGT		0
#define HI3670_CLK_ANDGT_EDC0			1
#define HI3670_CLK_ANDGT_LDI0			2
#define HI3670_CLK_ANDGT_LDI1			3
#define HI3670_CLK_MMBUF_PLL_ANDGT		4
#define HI3670_PCLK_MMBUF_ANDGT			5
#define HI3670_CLK_MUX_VIVOBUS			6
#define HI3670_CLK_MUX_EDC0			7
#define HI3670_CLK_MUX_LDI0			8
#define HI3670_CLK_MUX_LDI1			9
#define HI3670_CLK_SW_MMBUF			10
#define HI3670_CLK_DIV_VIVOBUS			11
#define HI3670_CLK_DIV_EDC0			12
#define HI3670_CLK_DIV_LDI0			13
#define HI3670_CLK_DIV_LDI1			14
#define HI3670_ACLK_DIV_MMBUF			15
#define HI3670_PCLK_DIV_MMBUF			16
#define HI3670_ACLK_GATE_NOC_DSS		17
#define HI3670_PCLK_GATE_NOC_DSS_CFG		18
#define HI3670_PCLK_GATE_MMBUF_CFG		19
#define HI3670_PCLK_GATE_DISP_NOC_SUBSYS	20
#define HI3670_ACLK_GATE_DISP_NOC_SUBSYS	21
#define HI3670_PCLK_GATE_DSS			22
#define HI3670_ACLK_GATE_DSS			23
#define HI3670_CLK_GATE_VIVOBUSFREQ		24
#define HI3670_CLK_GATE_EDC0			25
#define HI3670_CLK_GATE_LDI0			26
#define HI3670_CLK_GATE_LDI1FREQ		27
#define HI3670_CLK_GATE_BRG			28
#define HI3670_ACLK_GATE_ASC			29
#define HI3670_CLK_GATE_DSS_AXI_MM		30
#define HI3670_CLK_GATE_MMBUF			31
#define HI3670_PCLK_GATE_MMBUF			32
#define HI3670_CLK_GATE_ATDIV_VIVO		33


#define HI3670_CLK_GATE_VDECFREQ		0
#define HI3670_CLK_GATE_VENCFREQ		1
#define HI3670_CLK_GATE_ICSFREQ			2

#endif 
