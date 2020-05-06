/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef __SOUND_PCM_INDIRECT_H
#define __SOUND_PCM_INDIRECT_H

#include <sound/pcm.h>

struct snd_pcm_indirect {
	unsigned int hw_buffer_size;	
	unsigned int hw_queue_size;	
	unsigned int hw_data;	
	unsigned int hw_io;	
	int hw_ready;		
	unsigned int sw_buffer_size;	
	unsigned int sw_data;	
	unsigned int sw_io;	
	int sw_ready;		
	snd_pcm_uframes_t appl_ptr;	
};

typedef void (*snd_pcm_indirect_copy_t)(struct snd_pcm_substream *substream,
					struct snd_pcm_indirect *rec, size_t bytes);


static inline int
snd_pcm_indirect_playback_transfer(struct snd_pcm_substream *substream,
				   struct snd_pcm_indirect *rec,
				   snd_pcm_indirect_copy_t copy)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	snd_pcm_uframes_t appl_ptr = runtime->control->appl_ptr;
	snd_pcm_sframes_t diff = appl_ptr - rec->appl_ptr;
	int qsize;

	if (diff) {
		if (diff < -(snd_pcm_sframes_t) (runtime->boundary / 2))
			diff += runtime->boundary;
		if (diff < 0)
			return -EINVAL;
		rec->sw_ready += (int)frames_to_bytes(runtime, diff);
		rec->appl_ptr = appl_ptr;
	}
	qsize = rec->hw_queue_size ? rec->hw_queue_size : rec->hw_buffer_size;
	while (rec->hw_ready < qsize && rec->sw_ready > 0) {
		unsigned int hw_to_end = rec->hw_buffer_size - rec->hw_data;
		unsigned int sw_to_end = rec->sw_buffer_size - rec->sw_data;
		unsigned int bytes = qsize - rec->hw_ready;
		if (rec->sw_ready < (int)bytes)
			bytes = rec->sw_ready;
		if (hw_to_end < bytes)
			bytes = hw_to_end;
		if (sw_to_end < bytes)
			bytes = sw_to_end;
		if (! bytes)
			break;
		copy(substream, rec, bytes);
		rec->hw_data += bytes;
		if (rec->hw_data == rec->hw_buffer_size)
			rec->hw_data = 0;
		rec->sw_data += bytes;
		if (rec->sw_data == rec->sw_buffer_size)
			rec->sw_data = 0;
		rec->hw_ready += bytes;
		rec->sw_ready -= bytes;
	}
	return 0;
}


static inline snd_pcm_uframes_t
snd_pcm_indirect_playback_pointer(struct snd_pcm_substream *substream,
				  struct snd_pcm_indirect *rec, unsigned int ptr)
{
	int bytes = ptr - rec->hw_io;
	if (bytes < 0)
		bytes += rec->hw_buffer_size;
	rec->hw_io = ptr;
	rec->hw_ready -= bytes;
	rec->sw_io += bytes;
	if (rec->sw_io >= rec->sw_buffer_size)
		rec->sw_io -= rec->sw_buffer_size;
	if (substream->ops->ack)
		substream->ops->ack(substream);
	return bytes_to_frames(substream->runtime, rec->sw_io);
}



static inline int
snd_pcm_indirect_capture_transfer(struct snd_pcm_substream *substream,
				  struct snd_pcm_indirect *rec,
				  snd_pcm_indirect_copy_t copy)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	snd_pcm_uframes_t appl_ptr = runtime->control->appl_ptr;
	snd_pcm_sframes_t diff = appl_ptr - rec->appl_ptr;

	if (diff) {
		if (diff < -(snd_pcm_sframes_t) (runtime->boundary / 2))
			diff += runtime->boundary;
		if (diff < 0)
			return -EINVAL;
		rec->sw_ready -= frames_to_bytes(runtime, diff);
		rec->appl_ptr = appl_ptr;
	}
	while (rec->hw_ready > 0 && 
	       rec->sw_ready < (int)rec->sw_buffer_size) {
		size_t hw_to_end = rec->hw_buffer_size - rec->hw_data;
		size_t sw_to_end = rec->sw_buffer_size - rec->sw_data;
		size_t bytes = rec->sw_buffer_size - rec->sw_ready;
		if (rec->hw_ready < (int)bytes)
			bytes = rec->hw_ready;
		if (hw_to_end < bytes)
			bytes = hw_to_end;
		if (sw_to_end < bytes)
			bytes = sw_to_end;
		if (! bytes)
			break;
		copy(substream, rec, bytes);
		rec->hw_data += bytes;
		if ((int)rec->hw_data == rec->hw_buffer_size)
			rec->hw_data = 0;
		rec->sw_data += bytes;
		if (rec->sw_data == rec->sw_buffer_size)
			rec->sw_data = 0;
		rec->hw_ready -= bytes;
		rec->sw_ready += bytes;
	}
	return 0;
}


static inline snd_pcm_uframes_t
snd_pcm_indirect_capture_pointer(struct snd_pcm_substream *substream,
				 struct snd_pcm_indirect *rec, unsigned int ptr)
{
	int qsize;
	int bytes = ptr - rec->hw_io;
	if (bytes < 0)
		bytes += rec->hw_buffer_size;
	rec->hw_io = ptr;
	rec->hw_ready += bytes;
	qsize = rec->hw_queue_size ? rec->hw_queue_size : rec->hw_buffer_size;
	if (rec->hw_ready > qsize)
		return SNDRV_PCM_POS_XRUN;
	rec->sw_io += bytes;
	if (rec->sw_io >= rec->sw_buffer_size)
		rec->sw_io -= rec->sw_buffer_size;
	if (substream->ops->ack)
		substream->ops->ack(substream);
	return bytes_to_frames(substream->runtime, rec->sw_io);
}

#endif 
