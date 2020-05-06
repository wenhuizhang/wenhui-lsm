/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef __SOUND_AK4117_H
#define __SOUND_AK4117_H



#define AK4117_REG_PWRDN	0x00	
#define AK4117_REG_CLOCK	0x01	
#define AK4117_REG_IO		0x02	
#define AK4117_REG_INT0_MASK	0x03	
#define AK4117_REG_INT1_MASK	0x04	
#define AK4117_REG_RCS0		0x05	
#define AK4117_REG_RCS1		0x06	
#define AK4117_REG_RCS2		0x07	
#define AK4117_REG_RXCSB0	0x08	
#define AK4117_REG_RXCSB1	0x09	
#define AK4117_REG_RXCSB2	0x0a	
#define AK4117_REG_RXCSB3	0x0b	
#define AK4117_REG_RXCSB4	0x0c	
#define AK4117_REG_Pc0		0x0d	
#define AK4117_REG_Pc1		0x0e	
#define AK4117_REG_Pd0		0x0f	
#define AK4117_REG_Pd1		0x10	
#define AK4117_REG_QSUB_ADDR	0x11	
#define AK4117_REG_QSUB_TRACK	0x12	
#define AK4117_REG_QSUB_INDEX	0x13	
#define AK4117_REG_QSUB_MINUTE	0x14	
#define AK4117_REG_QSUB_SECOND	0x15	
#define AK4117_REG_QSUB_FRAME	0x16	
#define AK4117_REG_QSUB_ZERO	0x17	
#define AK4117_REG_QSUB_ABSMIN	0x18	
#define AK4117_REG_QSUB_ABSSEC	0x19	
#define AK4117_REG_QSUB_ABSFRM	0x1a	


#define AK4117_REG_RXCSB_SIZE	((AK4117_REG_RXCSB4-AK4117_REG_RXCSB0)+1)
#define AK4117_REG_QSUB_SIZE	((AK4117_REG_QSUB_ABSFRM-AK4117_REG_QSUB_ADDR)+1)


#define AK4117_EXCT		(1<<4)	
#define AK4117_XTL1		(1<<3)	
#define AK4117_XTL0		(1<<2)	
#define AK4117_XTL_11_2896M	(0)
#define AK4117_XTL_12_288M	AK4117_XTL0
#define AK4117_XTL_24_576M	AK4117_XTL1
#define AK4117_XTL_EXT		(AK4117_XTL1|AK4117_XTL0)
#define AK4117_PWN		(1<<1)	
#define AK4117_RST		(1<<0)	


#define AK4117_LP		(1<<7)	
#define AK4117_PKCS1		(1<<6)	
#define AK4117_PKCS0		(1<<5)
#define AK4117_PKCS_512fs	(0)
#define AK4117_PKCS_256fs	AK4117_PKCS0
#define AK4117_PKCS_128fs	AK4117_PKCS1
#define AK4117_DIV		(1<<4)	
#define AK4117_XCKS1		(1<<3)	
#define AK4117_XCKS0		(1<<2)
#define AK4117_XCKS_128fs	(0)
#define AK4117_XCKS_256fs	AK4117_XCKS0
#define AK4117_XCKS_512fs	AK4117_XCKS1
#define AK4117_XCKS_1024fs	(AK4117_XCKS1|AK4117_XCKS0)
#define AK4117_CM1		(1<<1)	
#define AK4117_CM0		(1<<0)
#define AK4117_CM_PLL		(0)		
#define AK4117_CM_XTAL		(AK4117_CM0)	
#define AK4117_CM_PLL_XTAL	(AK4117_CM1)	
#define AK4117_CM_MONITOR	(AK4117_CM0|AK4117_CM1) 


#define AK4117_IPS		(1<<7)	
#define AK4117_UOUTE		(1<<6)	
#define AK4117_CS12		(1<<5)	
#define AK4117_EFH2		(1<<4)	
#define AK4117_EFH1		(1<<3)
#define AK4117_EFH_512LRCLK	(0)
#define AK4117_EFH_1024LRCLK	(AK4117_EFH1)
#define AK4117_EFH_2048LRCLK	(AK4117_EFH2)
#define AK4117_EFH_4096LRCLK	(AK4117_EFH1|AK4117_EFH2)
#define AK4117_DIF2		(1<<2)	
#define AK4117_DIF1		(1<<1)
#define AK4117_DIF0		(1<<0)
#define AK4117_DIF_16R		(0)				
#define AK4117_DIF_18R		(AK4117_DIF0)			
#define AK4117_DIF_20R		(AK4117_DIF1)			
#define AK4117_DIF_24R		(AK4117_DIF1|AK4117_DIF0)	
#define AK4117_DIF_24L		(AK4117_DIF2)			
#define AK4117_DIF_24I2S	(AK4117_DIF2|AK4117_DIF0)	


#define AK4117_MULK		(1<<7)	
#define AK4117_MPAR		(1<<6)	
#define AK4117_MAUTO		(1<<5)	
#define AK4117_MV		(1<<4)	
#define AK4117_MAUD		(1<<3)	
#define AK4117_MSTC		(1<<2)	
#define AK4117_MCIT		(1<<1)	
#define AK4117_MQIT		(1<<0)	


#define AK4117_UNLCK		(1<<7)	
#define AK4117_PAR		(1<<6)	
#define AK4117_AUTO		(1<<5)	
#define AK4117_V		(1<<4)	
#define AK4117_AUDION		(1<<3)	
#define AK4117_STC		(1<<2)	
#define AK4117_CINT		(1<<1)	
#define AK4117_QINT		(1<<0)	


#define AK4117_DTSCD		(1<<6)	
#define AK4117_NPCM		(1<<5)	
#define AK4117_PEM		(1<<4)	
#define AK4117_FS3		(1<<3)	
#define AK4117_FS2		(1<<2)
#define AK4117_FS1		(1<<1)
#define AK4117_FS0		(1<<0)
#define AK4117_FS_44100HZ	(0)
#define AK4117_FS_48000HZ	(AK4117_FS1)
#define AK4117_FS_32000HZ	(AK4117_FS1|AK4117_FS0)
#define AK4117_FS_88200HZ	(AK4117_FS3)
#define AK4117_FS_96000HZ	(AK4117_FS3|AK4117_FS1)
#define AK4117_FS_176400HZ	(AK4117_FS3|AK4117_FS2)
#define AK4117_FS_192000HZ	(AK4117_FS3|AK4117_FS2|AK4117_FS1)


#define AK4117_CCRC		(1<<1)	
#define AK4117_QCRC		(1<<0)	


#define AK4117_CHECK_NO_STAT	(1<<0)	
#define AK4117_CHECK_NO_RATE	(1<<1)	

#define AK4117_CONTROLS		13

typedef void (ak4117_write_t)(void *private_data, unsigned char addr, unsigned char data);
typedef unsigned char (ak4117_read_t)(void *private_data, unsigned char addr);

enum {
	AK4117_PARITY_ERRORS,
	AK4117_V_BIT_ERRORS,
	AK4117_QCRC_ERRORS,
	AK4117_CCRC_ERRORS,
	AK4117_NUM_ERRORS
};

struct ak4117 {
	struct snd_card *card;
	ak4117_write_t * write;
	ak4117_read_t * read;
	void * private_data;
	unsigned int init: 1;
	spinlock_t lock;
	unsigned char regmap[5];
	struct snd_kcontrol *kctls[AK4117_CONTROLS];
	struct snd_pcm_substream *substream;
	unsigned long errors[AK4117_NUM_ERRORS];
	unsigned char rcs0;
	unsigned char rcs1;
	unsigned char rcs2;
	struct timer_list timer;	
	void *change_callback_private;
	void (*change_callback)(struct ak4117 *ak4117, unsigned char c0, unsigned char c1);
};

int snd_ak4117_create(struct snd_card *card, ak4117_read_t *read, ak4117_write_t *write,
		      const unsigned char pgm[5], void *private_data, struct ak4117 **r_ak4117);
void snd_ak4117_reg_write(struct ak4117 *ak4117, unsigned char reg, unsigned char mask, unsigned char val);
void snd_ak4117_reinit(struct ak4117 *ak4117);
int snd_ak4117_build(struct ak4117 *ak4117, struct snd_pcm_substream *capture_substream);
int snd_ak4117_external_rate(struct ak4117 *ak4117);
int snd_ak4117_check_rate_and_errors(struct ak4117 *ak4117, unsigned int flags);

#endif 

