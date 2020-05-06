/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef __SOUND_PCM_H
#define __SOUND_PCM_H



#include <sound/asound.h>
#include <sound/memalloc.h>
#include <sound/minors.h>
#include <linux/poll.h>
#include <linux/mm.h>
#include <linux/bitops.h>
#include <linux/pm_qos.h>
#include <linux/refcount.h>

#define snd_pcm_substream_chip(substream) ((substream)->private_data)
#define snd_pcm_chip(pcm) ((pcm)->private_data)

#if IS_ENABLED(CONFIG_SND_PCM_OSS)
#include <sound/pcm_oss.h>
#endif



struct snd_pcm_hardware {
	unsigned int info;		
	u64 formats;			
	unsigned int rates;		
	unsigned int rate_min;		
	unsigned int rate_max;		
	unsigned int channels_min;	
	unsigned int channels_max;	
	size_t buffer_bytes_max;	
	size_t period_bytes_min;	
	size_t period_bytes_max;	
	unsigned int periods_min;	
	unsigned int periods_max;	
	size_t fifo_size;		
};

struct snd_pcm_substream;

struct snd_pcm_audio_tstamp_config; 
struct snd_pcm_audio_tstamp_report;

struct snd_pcm_ops {
	int (*open)(struct snd_pcm_substream *substream);
	int (*close)(struct snd_pcm_substream *substream);
	int (*ioctl)(struct snd_pcm_substream * substream,
		     unsigned int cmd, void *arg);
	int (*hw_params)(struct snd_pcm_substream *substream,
			 struct snd_pcm_hw_params *params);
	int (*hw_free)(struct snd_pcm_substream *substream);
	int (*prepare)(struct snd_pcm_substream *substream);
	int (*trigger)(struct snd_pcm_substream *substream, int cmd);
	snd_pcm_uframes_t (*pointer)(struct snd_pcm_substream *substream);
	int (*get_time_info)(struct snd_pcm_substream *substream,
			struct timespec *system_ts, struct timespec *audio_ts,
			struct snd_pcm_audio_tstamp_config *audio_tstamp_config,
			struct snd_pcm_audio_tstamp_report *audio_tstamp_report);
	int (*fill_silence)(struct snd_pcm_substream *substream, int channel,
			    unsigned long pos, unsigned long bytes);
	int (*copy_user)(struct snd_pcm_substream *substream, int channel,
			 unsigned long pos, void __user *buf,
			 unsigned long bytes);
	int (*copy_kernel)(struct snd_pcm_substream *substream, int channel,
			   unsigned long pos, void *buf, unsigned long bytes);
	struct page *(*page)(struct snd_pcm_substream *substream,
			     unsigned long offset);
	int (*mmap)(struct snd_pcm_substream *substream, struct vm_area_struct *vma);
	int (*ack)(struct snd_pcm_substream *substream);
};



#if defined(CONFIG_SND_DYNAMIC_MINORS)
#define SNDRV_PCM_DEVICES	(SNDRV_OS_MINORS-2)
#else
#define SNDRV_PCM_DEVICES	8
#endif

#define SNDRV_PCM_IOCTL1_RESET		0

#define SNDRV_PCM_IOCTL1_CHANNEL_INFO	2

#define SNDRV_PCM_IOCTL1_FIFO_SIZE	4

#define SNDRV_PCM_TRIGGER_STOP		0
#define SNDRV_PCM_TRIGGER_START		1
#define SNDRV_PCM_TRIGGER_PAUSE_PUSH	3
#define SNDRV_PCM_TRIGGER_PAUSE_RELEASE	4
#define SNDRV_PCM_TRIGGER_SUSPEND	5
#define SNDRV_PCM_TRIGGER_RESUME	6
#define SNDRV_PCM_TRIGGER_DRAIN		7

#define SNDRV_PCM_POS_XRUN		((snd_pcm_uframes_t)-1)


#define SNDRV_PCM_RATE_5512		(1<<0)		
#define SNDRV_PCM_RATE_8000		(1<<1)		
#define SNDRV_PCM_RATE_11025		(1<<2)		
#define SNDRV_PCM_RATE_16000		(1<<3)		
#define SNDRV_PCM_RATE_22050		(1<<4)		
#define SNDRV_PCM_RATE_32000		(1<<5)		
#define SNDRV_PCM_RATE_44100		(1<<6)		
#define SNDRV_PCM_RATE_48000		(1<<7)		
#define SNDRV_PCM_RATE_64000		(1<<8)		
#define SNDRV_PCM_RATE_88200		(1<<9)		
#define SNDRV_PCM_RATE_96000		(1<<10)		
#define SNDRV_PCM_RATE_176400		(1<<11)		
#define SNDRV_PCM_RATE_192000		(1<<12)		

#define SNDRV_PCM_RATE_CONTINUOUS	(1<<30)		
#define SNDRV_PCM_RATE_KNOT		(1<<31)		

#define SNDRV_PCM_RATE_8000_44100	(SNDRV_PCM_RATE_8000|SNDRV_PCM_RATE_11025|\
					 SNDRV_PCM_RATE_16000|SNDRV_PCM_RATE_22050|\
					 SNDRV_PCM_RATE_32000|SNDRV_PCM_RATE_44100)
#define SNDRV_PCM_RATE_8000_48000	(SNDRV_PCM_RATE_8000_44100|SNDRV_PCM_RATE_48000)
#define SNDRV_PCM_RATE_8000_96000	(SNDRV_PCM_RATE_8000_48000|SNDRV_PCM_RATE_64000|\
					 SNDRV_PCM_RATE_88200|SNDRV_PCM_RATE_96000)
#define SNDRV_PCM_RATE_8000_192000	(SNDRV_PCM_RATE_8000_96000|SNDRV_PCM_RATE_176400|\
					 SNDRV_PCM_RATE_192000)
#define _SNDRV_PCM_FMTBIT(fmt)		(1ULL << (__force int)SNDRV_PCM_FORMAT_##fmt)
#define SNDRV_PCM_FMTBIT_S8		_SNDRV_PCM_FMTBIT(S8)
#define SNDRV_PCM_FMTBIT_U8		_SNDRV_PCM_FMTBIT(U8)
#define SNDRV_PCM_FMTBIT_S16_LE		_SNDRV_PCM_FMTBIT(S16_LE)
#define SNDRV_PCM_FMTBIT_S16_BE		_SNDRV_PCM_FMTBIT(S16_BE)
#define SNDRV_PCM_FMTBIT_U16_LE		_SNDRV_PCM_FMTBIT(U16_LE)
#define SNDRV_PCM_FMTBIT_U16_BE		_SNDRV_PCM_FMTBIT(U16_BE)
#define SNDRV_PCM_FMTBIT_S24_LE		_SNDRV_PCM_FMTBIT(S24_LE)
#define SNDRV_PCM_FMTBIT_S24_BE		_SNDRV_PCM_FMTBIT(S24_BE)
#define SNDRV_PCM_FMTBIT_U24_LE		_SNDRV_PCM_FMTBIT(U24_LE)
#define SNDRV_PCM_FMTBIT_U24_BE		_SNDRV_PCM_FMTBIT(U24_BE)
#define SNDRV_PCM_FMTBIT_S32_LE		_SNDRV_PCM_FMTBIT(S32_LE)
#define SNDRV_PCM_FMTBIT_S32_BE		_SNDRV_PCM_FMTBIT(S32_BE)
#define SNDRV_PCM_FMTBIT_U32_LE		_SNDRV_PCM_FMTBIT(U32_LE)
#define SNDRV_PCM_FMTBIT_U32_BE		_SNDRV_PCM_FMTBIT(U32_BE)
#define SNDRV_PCM_FMTBIT_FLOAT_LE	_SNDRV_PCM_FMTBIT(FLOAT_LE)
#define SNDRV_PCM_FMTBIT_FLOAT_BE	_SNDRV_PCM_FMTBIT(FLOAT_BE)
#define SNDRV_PCM_FMTBIT_FLOAT64_LE	_SNDRV_PCM_FMTBIT(FLOAT64_LE)
#define SNDRV_PCM_FMTBIT_FLOAT64_BE	_SNDRV_PCM_FMTBIT(FLOAT64_BE)
#define SNDRV_PCM_FMTBIT_IEC958_SUBFRAME_LE _SNDRV_PCM_FMTBIT(IEC958_SUBFRAME_LE)
#define SNDRV_PCM_FMTBIT_IEC958_SUBFRAME_BE _SNDRV_PCM_FMTBIT(IEC958_SUBFRAME_BE)
#define SNDRV_PCM_FMTBIT_MU_LAW		_SNDRV_PCM_FMTBIT(MU_LAW)
#define SNDRV_PCM_FMTBIT_A_LAW		_SNDRV_PCM_FMTBIT(A_LAW)
#define SNDRV_PCM_FMTBIT_IMA_ADPCM	_SNDRV_PCM_FMTBIT(IMA_ADPCM)
#define SNDRV_PCM_FMTBIT_MPEG		_SNDRV_PCM_FMTBIT(MPEG)
#define SNDRV_PCM_FMTBIT_GSM		_SNDRV_PCM_FMTBIT(GSM)
#define SNDRV_PCM_FMTBIT_S20_LE	_SNDRV_PCM_FMTBIT(S20_LE)
#define SNDRV_PCM_FMTBIT_U20_LE	_SNDRV_PCM_FMTBIT(U20_LE)
#define SNDRV_PCM_FMTBIT_S20_BE	_SNDRV_PCM_FMTBIT(S20_BE)
#define SNDRV_PCM_FMTBIT_U20_BE	_SNDRV_PCM_FMTBIT(U20_BE)
#define SNDRV_PCM_FMTBIT_SPECIAL	_SNDRV_PCM_FMTBIT(SPECIAL)
#define SNDRV_PCM_FMTBIT_S24_3LE	_SNDRV_PCM_FMTBIT(S24_3LE)
#define SNDRV_PCM_FMTBIT_U24_3LE	_SNDRV_PCM_FMTBIT(U24_3LE)
#define SNDRV_PCM_FMTBIT_S24_3BE	_SNDRV_PCM_FMTBIT(S24_3BE)
#define SNDRV_PCM_FMTBIT_U24_3BE	_SNDRV_PCM_FMTBIT(U24_3BE)
#define SNDRV_PCM_FMTBIT_S20_3LE	_SNDRV_PCM_FMTBIT(S20_3LE)
#define SNDRV_PCM_FMTBIT_U20_3LE	_SNDRV_PCM_FMTBIT(U20_3LE)
#define SNDRV_PCM_FMTBIT_S20_3BE	_SNDRV_PCM_FMTBIT(S20_3BE)
#define SNDRV_PCM_FMTBIT_U20_3BE	_SNDRV_PCM_FMTBIT(U20_3BE)
#define SNDRV_PCM_FMTBIT_S18_3LE	_SNDRV_PCM_FMTBIT(S18_3LE)
#define SNDRV_PCM_FMTBIT_U18_3LE	_SNDRV_PCM_FMTBIT(U18_3LE)
#define SNDRV_PCM_FMTBIT_S18_3BE	_SNDRV_PCM_FMTBIT(S18_3BE)
#define SNDRV_PCM_FMTBIT_U18_3BE	_SNDRV_PCM_FMTBIT(U18_3BE)
#define SNDRV_PCM_FMTBIT_G723_24	_SNDRV_PCM_FMTBIT(G723_24)
#define SNDRV_PCM_FMTBIT_G723_24_1B	_SNDRV_PCM_FMTBIT(G723_24_1B)
#define SNDRV_PCM_FMTBIT_G723_40	_SNDRV_PCM_FMTBIT(G723_40)
#define SNDRV_PCM_FMTBIT_G723_40_1B	_SNDRV_PCM_FMTBIT(G723_40_1B)
#define SNDRV_PCM_FMTBIT_DSD_U8		_SNDRV_PCM_FMTBIT(DSD_U8)
#define SNDRV_PCM_FMTBIT_DSD_U16_LE	_SNDRV_PCM_FMTBIT(DSD_U16_LE)
#define SNDRV_PCM_FMTBIT_DSD_U32_LE	_SNDRV_PCM_FMTBIT(DSD_U32_LE)
#define SNDRV_PCM_FMTBIT_DSD_U16_BE	_SNDRV_PCM_FMTBIT(DSD_U16_BE)
#define SNDRV_PCM_FMTBIT_DSD_U32_BE	_SNDRV_PCM_FMTBIT(DSD_U32_BE)

#ifdef SNDRV_LITTLE_ENDIAN
#define SNDRV_PCM_FMTBIT_S16		SNDRV_PCM_FMTBIT_S16_LE
#define SNDRV_PCM_FMTBIT_U16		SNDRV_PCM_FMTBIT_U16_LE
#define SNDRV_PCM_FMTBIT_S24		SNDRV_PCM_FMTBIT_S24_LE
#define SNDRV_PCM_FMTBIT_U24		SNDRV_PCM_FMTBIT_U24_LE
#define SNDRV_PCM_FMTBIT_S32		SNDRV_PCM_FMTBIT_S32_LE
#define SNDRV_PCM_FMTBIT_U32		SNDRV_PCM_FMTBIT_U32_LE
#define SNDRV_PCM_FMTBIT_FLOAT		SNDRV_PCM_FMTBIT_FLOAT_LE
#define SNDRV_PCM_FMTBIT_FLOAT64	SNDRV_PCM_FMTBIT_FLOAT64_LE
#define SNDRV_PCM_FMTBIT_IEC958_SUBFRAME SNDRV_PCM_FMTBIT_IEC958_SUBFRAME_LE
#define SNDRV_PCM_FMTBIT_S20		SNDRV_PCM_FMTBIT_S20_LE
#define SNDRV_PCM_FMTBIT_U20		SNDRV_PCM_FMTBIT_U20_LE
#endif
#ifdef SNDRV_BIG_ENDIAN
#define SNDRV_PCM_FMTBIT_S16		SNDRV_PCM_FMTBIT_S16_BE
#define SNDRV_PCM_FMTBIT_U16		SNDRV_PCM_FMTBIT_U16_BE
#define SNDRV_PCM_FMTBIT_S24		SNDRV_PCM_FMTBIT_S24_BE
#define SNDRV_PCM_FMTBIT_U24		SNDRV_PCM_FMTBIT_U24_BE
#define SNDRV_PCM_FMTBIT_S32		SNDRV_PCM_FMTBIT_S32_BE
#define SNDRV_PCM_FMTBIT_U32		SNDRV_PCM_FMTBIT_U32_BE
#define SNDRV_PCM_FMTBIT_FLOAT		SNDRV_PCM_FMTBIT_FLOAT_BE
#define SNDRV_PCM_FMTBIT_FLOAT64	SNDRV_PCM_FMTBIT_FLOAT64_BE
#define SNDRV_PCM_FMTBIT_IEC958_SUBFRAME SNDRV_PCM_FMTBIT_IEC958_SUBFRAME_BE
#define SNDRV_PCM_FMTBIT_S20		SNDRV_PCM_FMTBIT_S20_BE
#define SNDRV_PCM_FMTBIT_U20		SNDRV_PCM_FMTBIT_U20_BE
#endif

struct snd_pcm_file {
	struct snd_pcm_substream *substream;
	int no_compat_mmap;
	unsigned int user_pversion;	
};

struct snd_pcm_hw_rule;
typedef int (*snd_pcm_hw_rule_func_t)(struct snd_pcm_hw_params *params,
				      struct snd_pcm_hw_rule *rule);

struct snd_pcm_hw_rule {
	unsigned int cond;
	int var;
	int deps[4];

	snd_pcm_hw_rule_func_t func;
	void *private;
};

struct snd_pcm_hw_constraints {
	struct snd_mask masks[SNDRV_PCM_HW_PARAM_LAST_MASK - 
			 SNDRV_PCM_HW_PARAM_FIRST_MASK + 1];
	struct snd_interval intervals[SNDRV_PCM_HW_PARAM_LAST_INTERVAL -
			     SNDRV_PCM_HW_PARAM_FIRST_INTERVAL + 1];
	unsigned int rules_num;
	unsigned int rules_all;
	struct snd_pcm_hw_rule *rules;
};

static inline struct snd_mask *constrs_mask(struct snd_pcm_hw_constraints *constrs,
					    snd_pcm_hw_param_t var)
{
	return &constrs->masks[var - SNDRV_PCM_HW_PARAM_FIRST_MASK];
}

static inline struct snd_interval *constrs_interval(struct snd_pcm_hw_constraints *constrs,
						    snd_pcm_hw_param_t var)
{
	return &constrs->intervals[var - SNDRV_PCM_HW_PARAM_FIRST_INTERVAL];
}

struct snd_ratnum {
	unsigned int num;
	unsigned int den_min, den_max, den_step;
};

struct snd_ratden {
	unsigned int num_min, num_max, num_step;
	unsigned int den;
};

struct snd_pcm_hw_constraint_ratnums {
	int nrats;
	const struct snd_ratnum *rats;
};

struct snd_pcm_hw_constraint_ratdens {
	int nrats;
	const struct snd_ratden *rats;
};

struct snd_pcm_hw_constraint_list {
	const unsigned int *list;
	unsigned int count;
	unsigned int mask;
};

struct snd_pcm_hw_constraint_ranges {
	unsigned int count;
	const struct snd_interval *ranges;
	unsigned int mask;
};


struct snd_pcm_audio_tstamp_config {
	
	u32 type_requested:4;
	u32 report_delay:1; 
};

static inline void snd_pcm_unpack_audio_tstamp_config(__u32 data,
						struct snd_pcm_audio_tstamp_config *config)
{
	config->type_requested = data & 0xF;
	config->report_delay = (data >> 4) & 1;
}


struct snd_pcm_audio_tstamp_report {
	

	
	u32 valid:1;

	
	u32 actual_type:4;

	
	u32 accuracy_report:1; 
	u32 accuracy; 
};

static inline void snd_pcm_pack_audio_tstamp_report(__u32 *data, __u32 *accuracy,
						const struct snd_pcm_audio_tstamp_report *report)
{
	u32 tmp;

	tmp = report->accuracy_report;
	tmp <<= 4;
	tmp |= report->actual_type;
	tmp <<= 1;
	tmp |= report->valid;

	*data &= 0xffff; 
	*data |= (tmp << 16);
	*accuracy = report->accuracy;
}


struct snd_pcm_runtime {
	
	struct snd_pcm_substream *trigger_master;
	struct timespec trigger_tstamp;	
	bool trigger_tstamp_latched;     
	int overrange;
	snd_pcm_uframes_t avail_max;
	snd_pcm_uframes_t hw_ptr_base;	
	snd_pcm_uframes_t hw_ptr_interrupt; 
	unsigned long hw_ptr_jiffies;	
	unsigned long hw_ptr_buffer_jiffies; 
	snd_pcm_sframes_t delay;	
	u64 hw_ptr_wrap;                

	
	snd_pcm_access_t access;	
	snd_pcm_format_t format;	
	snd_pcm_subformat_t subformat;	
	unsigned int rate;		
	unsigned int channels;		
	snd_pcm_uframes_t period_size;	
	unsigned int periods;		
	snd_pcm_uframes_t buffer_size;	
	snd_pcm_uframes_t min_align;	
	size_t byte_align;
	unsigned int frame_bits;
	unsigned int sample_bits;
	unsigned int info;
	unsigned int rate_num;
	unsigned int rate_den;
	unsigned int no_period_wakeup: 1;

	
	int tstamp_mode;		
  	unsigned int period_step;
	snd_pcm_uframes_t start_threshold;
	snd_pcm_uframes_t stop_threshold;
	snd_pcm_uframes_t silence_threshold; 
	snd_pcm_uframes_t silence_size;	
	snd_pcm_uframes_t boundary;	

	snd_pcm_uframes_t silence_start; 
	snd_pcm_uframes_t silence_filled; 

	union snd_pcm_sync_id sync;	

	
	struct snd_pcm_mmap_status *status;
	struct snd_pcm_mmap_control *control;

	
	snd_pcm_uframes_t twake; 	
	wait_queue_head_t sleep;	
	wait_queue_head_t tsleep;	
	struct fasync_struct *fasync;

	
	void *private_data;
	void (*private_free)(struct snd_pcm_runtime *runtime);

	
	struct snd_pcm_hardware hw;
	struct snd_pcm_hw_constraints hw_constraints;

	
	unsigned int timer_resolution;	
	int tstamp_type;		

	           
	unsigned char *dma_area;	
	dma_addr_t dma_addr;		
	size_t dma_bytes;		

	struct snd_dma_buffer *dma_buffer_p;	

	
	struct snd_pcm_audio_tstamp_config audio_tstamp_config;
	struct snd_pcm_audio_tstamp_report audio_tstamp_report;
	struct timespec driver_tstamp;

#if IS_ENABLED(CONFIG_SND_PCM_OSS)
	
	struct snd_pcm_oss_runtime oss;
#endif
};

struct snd_pcm_group {		
	spinlock_t lock;
	struct mutex mutex;
	struct list_head substreams;
	refcount_t refs;
};

struct pid;

struct snd_pcm_substream {
	struct snd_pcm *pcm;
	struct snd_pcm_str *pstr;
	void *private_data;		
	int number;
	char name[32];			
	int stream;			
	struct pm_qos_request latency_pm_qos_req; 
	size_t buffer_bytes_max;	
	struct snd_dma_buffer dma_buffer;
	size_t dma_max;
	
	const struct snd_pcm_ops *ops;
	
	struct snd_pcm_runtime *runtime;
        
	struct snd_timer *timer;		
	unsigned timer_running: 1;	
	long wait_time;	
	
	struct snd_pcm_substream *next;
	
	struct list_head link_list;	
	struct snd_pcm_group self_group;	
	struct snd_pcm_group *group;		
	
	int ref_count;
	atomic_t mmap_count;
	unsigned int f_flags;
	void (*pcm_release)(struct snd_pcm_substream *);
	struct pid *pid;
#if IS_ENABLED(CONFIG_SND_PCM_OSS)
	
	struct snd_pcm_oss_substream oss;
#endif
#ifdef CONFIG_SND_VERBOSE_PROCFS
	struct snd_info_entry *proc_root;
#endif 
	
	unsigned int hw_opened: 1;
};

#define SUBSTREAM_BUSY(substream) ((substream)->ref_count > 0)


struct snd_pcm_str {
	int stream;				
	struct snd_pcm *pcm;
	
	unsigned int substream_count;
	unsigned int substream_opened;
	struct snd_pcm_substream *substream;
#if IS_ENABLED(CONFIG_SND_PCM_OSS)
	
	struct snd_pcm_oss_stream oss;
#endif
#ifdef CONFIG_SND_VERBOSE_PROCFS
	struct snd_info_entry *proc_root;
#ifdef CONFIG_SND_PCM_XRUN_DEBUG
	unsigned int xrun_debug;	
#endif
#endif
	struct snd_kcontrol *chmap_kctl; 
	struct device dev;
};

struct snd_pcm {
	struct snd_card *card;
	struct list_head list;
	int device; 
	unsigned int info_flags;
	unsigned short dev_class;
	unsigned short dev_subclass;
	char id[64];
	char name[80];
	struct snd_pcm_str streams[2];
	struct mutex open_mutex;
	wait_queue_head_t open_wait;
	void *private_data;
	void (*private_free) (struct snd_pcm *pcm);
	bool internal; 
	bool nonatomic; 
	bool no_device_suspend; 
#if IS_ENABLED(CONFIG_SND_PCM_OSS)
	struct snd_pcm_oss oss;
#endif
};



extern const struct file_operations snd_pcm_f_ops[2];

int snd_pcm_new(struct snd_card *card, const char *id, int device,
		int playback_count, int capture_count,
		struct snd_pcm **rpcm);
int snd_pcm_new_internal(struct snd_card *card, const char *id, int device,
		int playback_count, int capture_count,
		struct snd_pcm **rpcm);
int snd_pcm_new_stream(struct snd_pcm *pcm, int stream, int substream_count);

#if IS_ENABLED(CONFIG_SND_PCM_OSS)
struct snd_pcm_notify {
	int (*n_register) (struct snd_pcm * pcm);
	int (*n_disconnect) (struct snd_pcm * pcm);
	int (*n_unregister) (struct snd_pcm * pcm);
	struct list_head list;
};
int snd_pcm_notify(struct snd_pcm_notify *notify, int nfree);
#endif



int snd_pcm_info(struct snd_pcm_substream *substream, struct snd_pcm_info *info);
int snd_pcm_info_user(struct snd_pcm_substream *substream,
		      struct snd_pcm_info __user *info);
int snd_pcm_status(struct snd_pcm_substream *substream,
		   struct snd_pcm_status *status);
int snd_pcm_start(struct snd_pcm_substream *substream);
int snd_pcm_stop(struct snd_pcm_substream *substream, snd_pcm_state_t status);
int snd_pcm_drain_done(struct snd_pcm_substream *substream);
int snd_pcm_stop_xrun(struct snd_pcm_substream *substream);
#ifdef CONFIG_PM
int snd_pcm_suspend_all(struct snd_pcm *pcm);
#else
static inline int snd_pcm_suspend_all(struct snd_pcm *pcm)
{
	return 0;
}
#endif
int snd_pcm_kernel_ioctl(struct snd_pcm_substream *substream, unsigned int cmd, void *arg);
int snd_pcm_open_substream(struct snd_pcm *pcm, int stream, struct file *file,
			   struct snd_pcm_substream **rsubstream);
void snd_pcm_release_substream(struct snd_pcm_substream *substream);
int snd_pcm_attach_substream(struct snd_pcm *pcm, int stream, struct file *file,
			     struct snd_pcm_substream **rsubstream);
void snd_pcm_detach_substream(struct snd_pcm_substream *substream);
int snd_pcm_mmap_data(struct snd_pcm_substream *substream, struct file *file, struct vm_area_struct *area);


#ifdef CONFIG_SND_DEBUG
void snd_pcm_debug_name(struct snd_pcm_substream *substream,
			   char *name, size_t len);
#else
static inline void
snd_pcm_debug_name(struct snd_pcm_substream *substream, char *buf, size_t size)
{
	*buf = 0;
}
#endif




static inline int snd_pcm_stream_linked(struct snd_pcm_substream *substream)
{
	return substream->group != &substream->self_group;
}

void snd_pcm_stream_lock(struct snd_pcm_substream *substream);
void snd_pcm_stream_unlock(struct snd_pcm_substream *substream);
void snd_pcm_stream_lock_irq(struct snd_pcm_substream *substream);
void snd_pcm_stream_unlock_irq(struct snd_pcm_substream *substream);
unsigned long _snd_pcm_stream_lock_irqsave(struct snd_pcm_substream *substream);


#define snd_pcm_stream_lock_irqsave(substream, flags)		 \
	do {							 \
		typecheck(unsigned long, flags);		 \
		flags = _snd_pcm_stream_lock_irqsave(substream); \
	} while (0)
void snd_pcm_stream_unlock_irqrestore(struct snd_pcm_substream *substream,
				      unsigned long flags);


#define snd_pcm_group_for_each_entry(s, substream) \
	list_for_each_entry(s, &substream->group->substreams, link_list)


static inline int snd_pcm_running(struct snd_pcm_substream *substream)
{
	return (substream->runtime->status->state == SNDRV_PCM_STATE_RUNNING ||
		(substream->runtime->status->state == SNDRV_PCM_STATE_DRAINING &&
		 substream->stream == SNDRV_PCM_STREAM_PLAYBACK));
}


static inline ssize_t bytes_to_samples(struct snd_pcm_runtime *runtime, ssize_t size)
{
	return size * 8 / runtime->sample_bits;
}


static inline snd_pcm_sframes_t bytes_to_frames(struct snd_pcm_runtime *runtime, ssize_t size)
{
	return size * 8 / runtime->frame_bits;
}


static inline ssize_t samples_to_bytes(struct snd_pcm_runtime *runtime, ssize_t size)
{
	return size * runtime->sample_bits / 8;
}


static inline ssize_t frames_to_bytes(struct snd_pcm_runtime *runtime, snd_pcm_sframes_t size)
{
	return size * runtime->frame_bits / 8;
}


static inline int frame_aligned(struct snd_pcm_runtime *runtime, ssize_t bytes)
{
	return bytes % runtime->byte_align == 0;
}


static inline size_t snd_pcm_lib_buffer_bytes(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	return frames_to_bytes(runtime, runtime->buffer_size);
}


static inline size_t snd_pcm_lib_period_bytes(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	return frames_to_bytes(runtime, runtime->period_size);
}


static inline snd_pcm_uframes_t snd_pcm_playback_avail(struct snd_pcm_runtime *runtime)
{
	snd_pcm_sframes_t avail = runtime->status->hw_ptr + runtime->buffer_size - runtime->control->appl_ptr;
	if (avail < 0)
		avail += runtime->boundary;
	else if ((snd_pcm_uframes_t) avail >= runtime->boundary)
		avail -= runtime->boundary;
	return avail;
}


static inline snd_pcm_uframes_t snd_pcm_capture_avail(struct snd_pcm_runtime *runtime)
{
	snd_pcm_sframes_t avail = runtime->status->hw_ptr - runtime->control->appl_ptr;
	if (avail < 0)
		avail += runtime->boundary;
	return avail;
}


static inline snd_pcm_sframes_t snd_pcm_playback_hw_avail(struct snd_pcm_runtime *runtime)
{
	return runtime->buffer_size - snd_pcm_playback_avail(runtime);
}


static inline snd_pcm_sframes_t snd_pcm_capture_hw_avail(struct snd_pcm_runtime *runtime)
{
	return runtime->buffer_size - snd_pcm_capture_avail(runtime);
}


static inline int snd_pcm_playback_ready(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	return snd_pcm_playback_avail(runtime) >= runtime->control->avail_min;
}


static inline int snd_pcm_capture_ready(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	return snd_pcm_capture_avail(runtime) >= runtime->control->avail_min;
}


static inline int snd_pcm_playback_data(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	
	if (runtime->stop_threshold >= runtime->boundary)
		return 1;
	return snd_pcm_playback_avail(runtime) < runtime->buffer_size;
}


static inline int snd_pcm_playback_empty(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	return snd_pcm_playback_avail(runtime) >= runtime->buffer_size;
}


static inline int snd_pcm_capture_empty(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	return snd_pcm_capture_avail(runtime) == 0;
}


static inline void snd_pcm_trigger_done(struct snd_pcm_substream *substream, 
					struct snd_pcm_substream *master)
{
	substream->runtime->trigger_master = master;
}

static inline int hw_is_mask(int var)
{
	return var >= SNDRV_PCM_HW_PARAM_FIRST_MASK &&
		var <= SNDRV_PCM_HW_PARAM_LAST_MASK;
}

static inline int hw_is_interval(int var)
{
	return var >= SNDRV_PCM_HW_PARAM_FIRST_INTERVAL &&
		var <= SNDRV_PCM_HW_PARAM_LAST_INTERVAL;
}

static inline struct snd_mask *hw_param_mask(struct snd_pcm_hw_params *params,
				     snd_pcm_hw_param_t var)
{
	return &params->masks[var - SNDRV_PCM_HW_PARAM_FIRST_MASK];
}

static inline struct snd_interval *hw_param_interval(struct snd_pcm_hw_params *params,
					     snd_pcm_hw_param_t var)
{
	return &params->intervals[var - SNDRV_PCM_HW_PARAM_FIRST_INTERVAL];
}

static inline const struct snd_mask *hw_param_mask_c(const struct snd_pcm_hw_params *params,
					     snd_pcm_hw_param_t var)
{
	return &params->masks[var - SNDRV_PCM_HW_PARAM_FIRST_MASK];
}

static inline const struct snd_interval *hw_param_interval_c(const struct snd_pcm_hw_params *params,
						     snd_pcm_hw_param_t var)
{
	return &params->intervals[var - SNDRV_PCM_HW_PARAM_FIRST_INTERVAL];
}


static inline unsigned int params_channels(const struct snd_pcm_hw_params *p)
{
	return hw_param_interval_c(p, SNDRV_PCM_HW_PARAM_CHANNELS)->min;
}


static inline unsigned int params_rate(const struct snd_pcm_hw_params *p)
{
	return hw_param_interval_c(p, SNDRV_PCM_HW_PARAM_RATE)->min;
}


static inline unsigned int params_period_size(const struct snd_pcm_hw_params *p)
{
	return hw_param_interval_c(p, SNDRV_PCM_HW_PARAM_PERIOD_SIZE)->min;
}


static inline unsigned int params_periods(const struct snd_pcm_hw_params *p)
{
	return hw_param_interval_c(p, SNDRV_PCM_HW_PARAM_PERIODS)->min;
}


static inline unsigned int params_buffer_size(const struct snd_pcm_hw_params *p)
{
	return hw_param_interval_c(p, SNDRV_PCM_HW_PARAM_BUFFER_SIZE)->min;
}


static inline unsigned int params_buffer_bytes(const struct snd_pcm_hw_params *p)
{
	return hw_param_interval_c(p, SNDRV_PCM_HW_PARAM_BUFFER_BYTES)->min;
}

int snd_interval_refine(struct snd_interval *i, const struct snd_interval *v);
int snd_interval_list(struct snd_interval *i, unsigned int count,
		      const unsigned int *list, unsigned int mask);
int snd_interval_ranges(struct snd_interval *i, unsigned int count,
			const struct snd_interval *list, unsigned int mask);
int snd_interval_ratnum(struct snd_interval *i,
			unsigned int rats_count, const struct snd_ratnum *rats,
			unsigned int *nump, unsigned int *denp);

void _snd_pcm_hw_params_any(struct snd_pcm_hw_params *params);
void _snd_pcm_hw_param_setempty(struct snd_pcm_hw_params *params, snd_pcm_hw_param_t var);

int snd_pcm_hw_refine(struct snd_pcm_substream *substream, struct snd_pcm_hw_params *params);

int snd_pcm_hw_constraint_mask64(struct snd_pcm_runtime *runtime, snd_pcm_hw_param_t var,
				 u_int64_t mask);
int snd_pcm_hw_constraint_minmax(struct snd_pcm_runtime *runtime, snd_pcm_hw_param_t var,
				 unsigned int min, unsigned int max);
int snd_pcm_hw_constraint_integer(struct snd_pcm_runtime *runtime, snd_pcm_hw_param_t var);
int snd_pcm_hw_constraint_list(struct snd_pcm_runtime *runtime, 
			       unsigned int cond,
			       snd_pcm_hw_param_t var,
			       const struct snd_pcm_hw_constraint_list *l);
int snd_pcm_hw_constraint_ranges(struct snd_pcm_runtime *runtime,
				 unsigned int cond,
				 snd_pcm_hw_param_t var,
				 const struct snd_pcm_hw_constraint_ranges *r);
int snd_pcm_hw_constraint_ratnums(struct snd_pcm_runtime *runtime, 
				  unsigned int cond,
				  snd_pcm_hw_param_t var,
				  const struct snd_pcm_hw_constraint_ratnums *r);
int snd_pcm_hw_constraint_ratdens(struct snd_pcm_runtime *runtime, 
				  unsigned int cond,
				  snd_pcm_hw_param_t var,
				  const struct snd_pcm_hw_constraint_ratdens *r);
int snd_pcm_hw_constraint_msbits(struct snd_pcm_runtime *runtime, 
				 unsigned int cond,
				 unsigned int width,
				 unsigned int msbits);
int snd_pcm_hw_constraint_step(struct snd_pcm_runtime *runtime,
			       unsigned int cond,
			       snd_pcm_hw_param_t var,
			       unsigned long step);
int snd_pcm_hw_constraint_pow2(struct snd_pcm_runtime *runtime,
			       unsigned int cond,
			       snd_pcm_hw_param_t var);
int snd_pcm_hw_rule_noresample(struct snd_pcm_runtime *runtime,
			       unsigned int base_rate);
int snd_pcm_hw_rule_add(struct snd_pcm_runtime *runtime,
			unsigned int cond,
			int var,
			snd_pcm_hw_rule_func_t func, void *private,
			int dep, ...);


static inline int snd_pcm_hw_constraint_single(
	struct snd_pcm_runtime *runtime, snd_pcm_hw_param_t var,
	unsigned int val)
{
	return snd_pcm_hw_constraint_minmax(runtime, var, val, val);
}

int snd_pcm_format_signed(snd_pcm_format_t format);
int snd_pcm_format_unsigned(snd_pcm_format_t format);
int snd_pcm_format_linear(snd_pcm_format_t format);
int snd_pcm_format_little_endian(snd_pcm_format_t format);
int snd_pcm_format_big_endian(snd_pcm_format_t format);
#if 0 

int snd_pcm_format_cpu_endian(snd_pcm_format_t format);
#endif 
#ifdef SNDRV_LITTLE_ENDIAN
#define snd_pcm_format_cpu_endian(format) snd_pcm_format_little_endian(format)
#else
#define snd_pcm_format_cpu_endian(format) snd_pcm_format_big_endian(format)
#endif
int snd_pcm_format_width(snd_pcm_format_t format);			
int snd_pcm_format_physical_width(snd_pcm_format_t format);		
ssize_t snd_pcm_format_size(snd_pcm_format_t format, size_t samples);
const unsigned char *snd_pcm_format_silence_64(snd_pcm_format_t format);
int snd_pcm_format_set_silence(snd_pcm_format_t format, void *buf, unsigned int frames);

void snd_pcm_set_ops(struct snd_pcm * pcm, int direction,
		     const struct snd_pcm_ops *ops);
void snd_pcm_set_sync(struct snd_pcm_substream *substream);
int snd_pcm_lib_ioctl(struct snd_pcm_substream *substream,
		      unsigned int cmd, void *arg);                      
void snd_pcm_period_elapsed(struct snd_pcm_substream *substream);
snd_pcm_sframes_t __snd_pcm_lib_xfer(struct snd_pcm_substream *substream,
				     void *buf, bool interleaved,
				     snd_pcm_uframes_t frames, bool in_kernel);

static inline snd_pcm_sframes_t
snd_pcm_lib_write(struct snd_pcm_substream *substream,
		  const void __user *buf, snd_pcm_uframes_t frames)
{
	return __snd_pcm_lib_xfer(substream, (void __force *)buf, true, frames, false);
}

static inline snd_pcm_sframes_t
snd_pcm_lib_read(struct snd_pcm_substream *substream,
		 void __user *buf, snd_pcm_uframes_t frames)
{
	return __snd_pcm_lib_xfer(substream, (void __force *)buf, true, frames, false);
}

static inline snd_pcm_sframes_t
snd_pcm_lib_writev(struct snd_pcm_substream *substream,
		   void __user **bufs, snd_pcm_uframes_t frames)
{
	return __snd_pcm_lib_xfer(substream, (void *)bufs, false, frames, false);
}

static inline snd_pcm_sframes_t
snd_pcm_lib_readv(struct snd_pcm_substream *substream,
		  void __user **bufs, snd_pcm_uframes_t frames)
{
	return __snd_pcm_lib_xfer(substream, (void *)bufs, false, frames, false);
}

static inline snd_pcm_sframes_t
snd_pcm_kernel_write(struct snd_pcm_substream *substream,
		     const void *buf, snd_pcm_uframes_t frames)
{
	return __snd_pcm_lib_xfer(substream, (void *)buf, true, frames, true);
}

static inline snd_pcm_sframes_t
snd_pcm_kernel_read(struct snd_pcm_substream *substream,
		    void *buf, snd_pcm_uframes_t frames)
{
	return __snd_pcm_lib_xfer(substream, buf, true, frames, true);
}

static inline snd_pcm_sframes_t
snd_pcm_kernel_writev(struct snd_pcm_substream *substream,
		      void **bufs, snd_pcm_uframes_t frames)
{
	return __snd_pcm_lib_xfer(substream, bufs, false, frames, true);
}

static inline snd_pcm_sframes_t
snd_pcm_kernel_readv(struct snd_pcm_substream *substream,
		     void **bufs, snd_pcm_uframes_t frames)
{
	return __snd_pcm_lib_xfer(substream, bufs, false, frames, true);
}

int snd_pcm_limit_hw_rates(struct snd_pcm_runtime *runtime);
unsigned int snd_pcm_rate_to_rate_bit(unsigned int rate);
unsigned int snd_pcm_rate_bit_to_rate(unsigned int rate_bit);
unsigned int snd_pcm_rate_mask_intersect(unsigned int rates_a,
					 unsigned int rates_b);
unsigned int snd_pcm_rate_range_to_bits(unsigned int rate_min,
					unsigned int rate_max);


static inline void snd_pcm_set_runtime_buffer(struct snd_pcm_substream *substream,
					      struct snd_dma_buffer *bufp)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	if (bufp) {
		runtime->dma_buffer_p = bufp;
		runtime->dma_area = bufp->area;
		runtime->dma_addr = bufp->addr;
		runtime->dma_bytes = bufp->bytes;
	} else {
		runtime->dma_buffer_p = NULL;
		runtime->dma_area = NULL;
		runtime->dma_addr = 0;
		runtime->dma_bytes = 0;
	}
}


static inline void snd_pcm_gettime(struct snd_pcm_runtime *runtime,
				   struct timespec *tv)
{
	switch (runtime->tstamp_type) {
	case SNDRV_PCM_TSTAMP_TYPE_MONOTONIC:
		ktime_get_ts(tv);
		break;
	case SNDRV_PCM_TSTAMP_TYPE_MONOTONIC_RAW:
		getrawmonotonic(tv);
		break;
	default:
		getnstimeofday(tv);
		break;
	}
}



void snd_pcm_lib_preallocate_free(struct snd_pcm_substream *substream);
void snd_pcm_lib_preallocate_free_for_all(struct snd_pcm *pcm);
void snd_pcm_lib_preallocate_pages(struct snd_pcm_substream *substream,
				  int type, struct device *data,
				  size_t size, size_t max);
void snd_pcm_lib_preallocate_pages_for_all(struct snd_pcm *pcm,
					  int type, void *data,
					  size_t size, size_t max);
int snd_pcm_lib_malloc_pages(struct snd_pcm_substream *substream, size_t size);
int snd_pcm_lib_free_pages(struct snd_pcm_substream *substream);

int _snd_pcm_lib_alloc_vmalloc_buffer(struct snd_pcm_substream *substream,
				      size_t size, gfp_t gfp_flags);
int snd_pcm_lib_free_vmalloc_buffer(struct snd_pcm_substream *substream);
struct page *snd_pcm_lib_get_vmalloc_page(struct snd_pcm_substream *substream,
					  unsigned long offset);

static inline int snd_pcm_lib_alloc_vmalloc_buffer
			(struct snd_pcm_substream *substream, size_t size)
{
	return _snd_pcm_lib_alloc_vmalloc_buffer(substream, size,
						 GFP_KERNEL | __GFP_HIGHMEM | __GFP_ZERO);
}


static inline int snd_pcm_lib_alloc_vmalloc_32_buffer
			(struct snd_pcm_substream *substream, size_t size)
{
	return _snd_pcm_lib_alloc_vmalloc_buffer(substream, size,
						 GFP_KERNEL | GFP_DMA32 | __GFP_ZERO);
}

#define snd_pcm_get_dma_buf(substream) ((substream)->runtime->dma_buffer_p)

#ifdef CONFIG_SND_DMA_SGBUF

#define snd_pcm_substream_sgbuf(substream) \
	snd_pcm_get_dma_buf(substream)->private_data

struct page *snd_pcm_sgbuf_ops_page(struct snd_pcm_substream *substream,
				    unsigned long offset);
#else 

#define snd_pcm_sgbuf_ops_page	NULL
#endif 


static inline dma_addr_t
snd_pcm_sgbuf_get_addr(struct snd_pcm_substream *substream, unsigned int ofs)
{
	return snd_sgbuf_get_addr(snd_pcm_get_dma_buf(substream), ofs);
}


static inline void *
snd_pcm_sgbuf_get_ptr(struct snd_pcm_substream *substream, unsigned int ofs)
{
	return snd_sgbuf_get_ptr(snd_pcm_get_dma_buf(substream), ofs);
}


static inline unsigned int
snd_pcm_sgbuf_get_chunk_size(struct snd_pcm_substream *substream,
			     unsigned int ofs, unsigned int size)
{
	return snd_sgbuf_get_chunk_size(snd_pcm_get_dma_buf(substream), ofs, size);
}


static inline void snd_pcm_mmap_data_open(struct vm_area_struct *area)
{
	struct snd_pcm_substream *substream = (struct snd_pcm_substream *)area->vm_private_data;
	atomic_inc(&substream->mmap_count);
}


static inline void snd_pcm_mmap_data_close(struct vm_area_struct *area)
{
	struct snd_pcm_substream *substream = (struct snd_pcm_substream *)area->vm_private_data;
	atomic_dec(&substream->mmap_count);
}

int snd_pcm_lib_default_mmap(struct snd_pcm_substream *substream,
			     struct vm_area_struct *area);

#if defined(CONFIG_X86) || defined(CONFIG_PPC) || defined(CONFIG_ALPHA)
#define SNDRV_PCM_INFO_MMAP_IOMEM	SNDRV_PCM_INFO_MMAP
int snd_pcm_lib_mmap_iomem(struct snd_pcm_substream *substream, struct vm_area_struct *area);
#else
#define SNDRV_PCM_INFO_MMAP_IOMEM	0
#define snd_pcm_lib_mmap_iomem	NULL
#endif


static inline void snd_pcm_limit_isa_dma_size(int dma, size_t *max)
{
	*max = dma < 4 ? 64 * 1024 : 128 * 1024;
}



#define SNDRV_PCM_DEFAULT_CON_SPDIF	(IEC958_AES0_CON_EMPHASIS_NONE|\
					 (IEC958_AES1_CON_ORIGINAL<<8)|\
					 (IEC958_AES1_CON_PCM_CODER<<8)|\
					 (IEC958_AES3_CON_FS_48000<<24))

#define PCM_RUNTIME_CHECK(sub) snd_BUG_ON(!(sub) || !(sub)->runtime)

const char *snd_pcm_format_name(snd_pcm_format_t format);


static inline const char *snd_pcm_stream_str(struct snd_pcm_substream *substream)
{
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		return "Playback";
	else
		return "Capture";
}



struct snd_pcm_chmap_elem {
	unsigned char channels;
	unsigned char map[15];
};


struct snd_pcm_chmap {
	struct snd_pcm *pcm;	
	int stream;		
	struct snd_kcontrol *kctl;
	const struct snd_pcm_chmap_elem *chmap;
	unsigned int max_channels;
	unsigned int channel_mask;	
	void *private_data;	
};


static inline struct snd_pcm_substream *
snd_pcm_chmap_substream(struct snd_pcm_chmap *info, unsigned int idx)
{
	struct snd_pcm_substream *s;
	for (s = info->pcm->streams[info->stream].substream; s; s = s->next)
		if (s->number == idx)
			return s;
	return NULL;
}


extern const struct snd_pcm_chmap_elem snd_pcm_std_chmaps[];

extern const struct snd_pcm_chmap_elem snd_pcm_alt_chmaps[];


#define SND_PCM_CHMAP_MASK_24	((1U << 2) | (1U << 4))
#define SND_PCM_CHMAP_MASK_246	(SND_PCM_CHMAP_MASK_24 | (1U << 6))
#define SND_PCM_CHMAP_MASK_2468	(SND_PCM_CHMAP_MASK_246 | (1U << 8))

int snd_pcm_add_chmap_ctls(struct snd_pcm *pcm, int stream,
			   const struct snd_pcm_chmap_elem *chmap,
			   int max_channels,
			   unsigned long private_value,
			   struct snd_pcm_chmap **info_ret);


static inline u64 pcm_format_to_bits(snd_pcm_format_t pcm_format)
{
	return 1ULL << (__force int) pcm_format;
}


#define pcm_err(pcm, fmt, args...) \
	dev_err((pcm)->card->dev, fmt, ##args)
#define pcm_warn(pcm, fmt, args...) \
	dev_warn((pcm)->card->dev, fmt, ##args)
#define pcm_dbg(pcm, fmt, args...) \
	dev_dbg((pcm)->card->dev, fmt, ##args)

#endif 
