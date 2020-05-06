/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _DT_BINDINGS_CLOCK_TEGRA210_CAR_H
#define _DT_BINDINGS_CLOCK_TEGRA210_CAR_H




#define TEGRA210_CLK_ISPB 3
#define TEGRA210_CLK_RTC 4
#define TEGRA210_CLK_TIMER 5
#define TEGRA210_CLK_UARTA 6

#define TEGRA210_CLK_GPIO 8
#define TEGRA210_CLK_SDMMC2 9

#define TEGRA210_CLK_I2S1 11
#define TEGRA210_CLK_I2C1 12

#define TEGRA210_CLK_SDMMC1 14
#define TEGRA210_CLK_SDMMC4 15

#define TEGRA210_CLK_PWM 17
#define TEGRA210_CLK_I2S2 18



#define TEGRA210_CLK_USBD 22
#define TEGRA210_CLK_ISPA 23


#define TEGRA210_CLK_DISP2 26
#define TEGRA210_CLK_DISP1 27
#define TEGRA210_CLK_HOST1X 28

#define TEGRA210_CLK_I2S0 30


#define TEGRA210_CLK_MC 32
#define TEGRA210_CLK_AHBDMA 33
#define TEGRA210_CLK_APBDMA 34



#define TEGRA210_CLK_PMC 38

#define TEGRA210_CLK_KFUSE 40
#define TEGRA210_CLK_SBC1 41


#define TEGRA210_CLK_SBC2 44

#define TEGRA210_CLK_SBC3 46
#define TEGRA210_CLK_I2C5 47
#define TEGRA210_CLK_DSIA 48



#define TEGRA210_CLK_CSI 52

#define TEGRA210_CLK_I2C2 54
#define TEGRA210_CLK_UARTC 55
#define TEGRA210_CLK_MIPI_CAL 56
#define TEGRA210_CLK_EMC 57
#define TEGRA210_CLK_USB2 58




#define TEGRA210_CLK_BSEV 63


#define TEGRA210_CLK_UARTD 65

#define TEGRA210_CLK_I2C3 67
#define TEGRA210_CLK_SBC4 68
#define TEGRA210_CLK_SDMMC3 69
#define TEGRA210_CLK_PCIE 70
#define TEGRA210_CLK_OWR 71
#define TEGRA210_CLK_AFI 72
#define TEGRA210_CLK_CSITE 73


#define TEGRA210_CLK_LA 76

#define TEGRA210_CLK_SOC_THERM 78
#define TEGRA210_CLK_DTV 79

#define TEGRA210_CLK_I2CSLOW 81
#define TEGRA210_CLK_DSIB 82
#define TEGRA210_CLK_TSEC 83





#define TEGRA210_CLK_XUSB_HOST 89


#define TEGRA210_CLK_CSUS 92







#define TEGRA210_CLK_MSELECT 99
#define TEGRA210_CLK_TSENSOR 100
#define TEGRA210_CLK_I2S3 101
#define TEGRA210_CLK_I2S4 102
#define TEGRA210_CLK_I2C4 103


#define TEGRA210_CLK_D_AUDIO 106
#define TEGRA210_CLK_APB2APE 107



#define TEGRA210_CLK_HDA2CODEC_2X 111






#define TEGRA210_CLK_SPDIF_2X 118
#define TEGRA210_CLK_ACTMON 119
#define TEGRA210_CLK_EXTERN1 120
#define TEGRA210_CLK_EXTERN2 121
#define TEGRA210_CLK_EXTERN3 122
#define TEGRA210_CLK_SATA_OOB 123
#define TEGRA210_CLK_SATA 124
#define TEGRA210_CLK_HDA 125



#define TEGRA210_CLK_HDA2HDMI 128







#define TEGRA210_CLK_CEC 136







#define TEGRA210_CLK_XUSB_GATE 143
#define TEGRA210_CLK_CILAB 144
#define TEGRA210_CLK_CILCD 145
#define TEGRA210_CLK_CILE 146
#define TEGRA210_CLK_DSIALP 147
#define TEGRA210_CLK_DSIBLP 148
#define TEGRA210_CLK_ENTROPY 149


#define TEGRA210_CLK_DP2 152



#define TEGRA210_CLK_XUSB_SS 156





#define TEGRA210_CLK_DMIC1 161
#define TEGRA210_CLK_DMIC2 162



#define TEGRA210_CLK_I2C6 166




#define TEGRA210_CLK_VIM2_CLK 171

#define TEGRA210_CLK_MIPIBIF 173



#define TEGRA210_CLK_CLK72MHZ 177
#define TEGRA210_CLK_VIC03 178


#define TEGRA210_CLK_DPAUX 181
#define TEGRA210_CLK_SOR0 182
#define TEGRA210_CLK_SOR1 183
#define TEGRA210_CLK_GPU 184
#define TEGRA210_CLK_DBGAPB 185

#define TEGRA210_CLK_PLL_P_OUT_ADSP 187

#define TEGRA210_CLK_PLL_G_REF 189




#define TEGRA210_CLK_SDMMC_LEGACY 193
#define TEGRA210_CLK_NVDEC 194
#define TEGRA210_CLK_NVJPG 195

#define TEGRA210_CLK_DMIC3 197
#define TEGRA210_CLK_APE 198
#define TEGRA210_CLK_ADSP 199


#define TEGRA210_CLK_MAUD 202



#define TEGRA210_CLK_TSECB 206
#define TEGRA210_CLK_DPAUX1 207
#define TEGRA210_CLK_VI_I2C 208
#define TEGRA210_CLK_HSIC_TRK 209
#define TEGRA210_CLK_USB2_TRK 210
#define TEGRA210_CLK_QSPI 211
#define TEGRA210_CLK_UARTAPE 212





#define TEGRA210_CLK_ADSP_NEON 218
#define TEGRA210_CLK_NVENC 219
#define TEGRA210_CLK_IQC2 220
#define TEGRA210_CLK_IQC1 221
#define TEGRA210_CLK_SOR_SAFE 222
#define TEGRA210_CLK_PLL_P_OUT_CPU 223


#define TEGRA210_CLK_UARTB 224
#define TEGRA210_CLK_VFIR 225
#define TEGRA210_CLK_SPDIF_IN 226
#define TEGRA210_CLK_SPDIF_OUT 227
#define TEGRA210_CLK_VI 228
#define TEGRA210_CLK_VI_SENSOR 229
#define TEGRA210_CLK_FUSE 230
#define TEGRA210_CLK_FUSE_BURN 231
#define TEGRA210_CLK_CLK_32K 232
#define TEGRA210_CLK_CLK_M 233
#define TEGRA210_CLK_CLK_M_DIV2 234
#define TEGRA210_CLK_CLK_M_DIV4 235
#define TEGRA210_CLK_PLL_REF 236
#define TEGRA210_CLK_PLL_C 237
#define TEGRA210_CLK_PLL_C_OUT1 238
#define TEGRA210_CLK_PLL_C2 239
#define TEGRA210_CLK_PLL_C3 240
#define TEGRA210_CLK_PLL_M 241
#define TEGRA210_CLK_PLL_M_OUT1 242
#define TEGRA210_CLK_PLL_P 243
#define TEGRA210_CLK_PLL_P_OUT1 244
#define TEGRA210_CLK_PLL_P_OUT2 245
#define TEGRA210_CLK_PLL_P_OUT3 246
#define TEGRA210_CLK_PLL_P_OUT4 247
#define TEGRA210_CLK_PLL_A 248
#define TEGRA210_CLK_PLL_A_OUT0 249
#define TEGRA210_CLK_PLL_D 250
#define TEGRA210_CLK_PLL_D_OUT0 251
#define TEGRA210_CLK_PLL_D2 252
#define TEGRA210_CLK_PLL_D2_OUT0 253
#define TEGRA210_CLK_PLL_U 254
#define TEGRA210_CLK_PLL_U_480M 255

#define TEGRA210_CLK_PLL_U_60M 256
#define TEGRA210_CLK_PLL_U_48M 257

#define TEGRA210_CLK_PLL_X 259
#define TEGRA210_CLK_PLL_X_OUT0 260
#define TEGRA210_CLK_PLL_RE_VCO 261
#define TEGRA210_CLK_PLL_RE_OUT 262
#define TEGRA210_CLK_PLL_E 263
#define TEGRA210_CLK_SPDIF_IN_SYNC 264
#define TEGRA210_CLK_I2S0_SYNC 265
#define TEGRA210_CLK_I2S1_SYNC 266
#define TEGRA210_CLK_I2S2_SYNC 267
#define TEGRA210_CLK_I2S3_SYNC 268
#define TEGRA210_CLK_I2S4_SYNC 269
#define TEGRA210_CLK_VIMCLK_SYNC 270
#define TEGRA210_CLK_AUDIO0 271
#define TEGRA210_CLK_AUDIO1 272
#define TEGRA210_CLK_AUDIO2 273
#define TEGRA210_CLK_AUDIO3 274
#define TEGRA210_CLK_AUDIO4 275
#define TEGRA210_CLK_SPDIF 276
#define TEGRA210_CLK_CLK_OUT_1 277
#define TEGRA210_CLK_CLK_OUT_2 278
#define TEGRA210_CLK_CLK_OUT_3 279
#define TEGRA210_CLK_BLINK 280

#define TEGRA210_CLK_SOR1_SRC 282
#define TEGRA210_CLK_SOR1_OUT 282

#define TEGRA210_CLK_XUSB_HOST_SRC 284
#define TEGRA210_CLK_XUSB_FALCON_SRC 285
#define TEGRA210_CLK_XUSB_FS_SRC 286
#define TEGRA210_CLK_XUSB_SS_SRC 287

#define TEGRA210_CLK_XUSB_DEV_SRC 288
#define TEGRA210_CLK_XUSB_DEV 289
#define TEGRA210_CLK_XUSB_HS_SRC 290
#define TEGRA210_CLK_SCLK 291
#define TEGRA210_CLK_HCLK 292
#define TEGRA210_CLK_PCLK 293
#define TEGRA210_CLK_CCLK_G 294
#define TEGRA210_CLK_CCLK_LP 295
#define TEGRA210_CLK_DFLL_REF 296
#define TEGRA210_CLK_DFLL_SOC 297
#define TEGRA210_CLK_VI_SENSOR2 298
#define TEGRA210_CLK_PLL_P_OUT5 299
#define TEGRA210_CLK_CML0 300
#define TEGRA210_CLK_CML1 301
#define TEGRA210_CLK_PLL_C4 302
#define TEGRA210_CLK_PLL_DP 303
#define TEGRA210_CLK_PLL_E_MUX 304
#define TEGRA210_CLK_PLL_MB 305
#define TEGRA210_CLK_PLL_A1 306
#define TEGRA210_CLK_PLL_D_DSI_OUT 307
#define TEGRA210_CLK_PLL_C4_OUT0 308
#define TEGRA210_CLK_PLL_C4_OUT1 309
#define TEGRA210_CLK_PLL_C4_OUT2 310
#define TEGRA210_CLK_PLL_C4_OUT3 311
#define TEGRA210_CLK_PLL_U_OUT 312
#define TEGRA210_CLK_PLL_U_OUT1 313
#define TEGRA210_CLK_PLL_U_OUT2 314
#define TEGRA210_CLK_USB2_HSIC_TRK 315
#define TEGRA210_CLK_PLL_P_OUT_HSIO 316
#define TEGRA210_CLK_PLL_P_OUT_XUSB 317
#define TEGRA210_CLK_XUSB_SSP_SRC 318
#define TEGRA210_CLK_PLL_RE_OUT1 319


#define TEGRA210_CLK_ISP 322
#define TEGRA210_CLK_PLL_A_OUT_ADSP 323
#define TEGRA210_CLK_PLL_A_OUT0_OUT_ADSP 324


























#define TEGRA210_CLK_AUDIO0_MUX 350
#define TEGRA210_CLK_AUDIO1_MUX 351
#define TEGRA210_CLK_AUDIO2_MUX 352
#define TEGRA210_CLK_AUDIO3_MUX 353
#define TEGRA210_CLK_AUDIO4_MUX 354
#define TEGRA210_CLK_SPDIF_MUX 355
#define TEGRA210_CLK_CLK_OUT_1_MUX 356
#define TEGRA210_CLK_CLK_OUT_2_MUX 357
#define TEGRA210_CLK_CLK_OUT_3_MUX 358
#define TEGRA210_CLK_DSIA_MUX 359
#define TEGRA210_CLK_DSIB_MUX 360
#define TEGRA210_CLK_SOR0_LVDS 361
#define TEGRA210_CLK_XUSB_SS_DIV2 362

#define TEGRA210_CLK_PLL_M_UD 363
#define TEGRA210_CLK_PLL_C_UD 364
#define TEGRA210_CLK_SCLK_MUX 365

#define TEGRA210_CLK_ACLK 370

#define TEGRA210_CLK_DMIC1_SYNC_CLK 388
#define TEGRA210_CLK_DMIC1_SYNC_CLK_MUX 389
#define TEGRA210_CLK_DMIC2_SYNC_CLK 390
#define TEGRA210_CLK_DMIC2_SYNC_CLK_MUX 391
#define TEGRA210_CLK_DMIC3_SYNC_CLK 392
#define TEGRA210_CLK_DMIC3_SYNC_CLK_MUX 393

#define TEGRA210_CLK_CLK_MAX 394

#endif	
