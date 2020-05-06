/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _V4L2_IOCTL_H
#define _V4L2_IOCTL_H

#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <linux/sched/signal.h>
#include <linux/compiler.h> 
#include <linux/videodev2.h>

struct v4l2_fh;


struct v4l2_ioctl_ops {
	

	
	int (*vidioc_querycap)(struct file *file, void *fh,
			       struct v4l2_capability *cap);

	
	int (*vidioc_enum_fmt_vid_cap)(struct file *file, void *fh,
				       struct v4l2_fmtdesc *f);
	int (*vidioc_enum_fmt_vid_overlay)(struct file *file, void *fh,
					   struct v4l2_fmtdesc *f);
	int (*vidioc_enum_fmt_vid_out)(struct file *file, void *fh,
				       struct v4l2_fmtdesc *f);
	int (*vidioc_enum_fmt_sdr_cap)(struct file *file, void *fh,
				       struct v4l2_fmtdesc *f);
	int (*vidioc_enum_fmt_sdr_out)(struct file *file, void *fh,
				       struct v4l2_fmtdesc *f);
	int (*vidioc_enum_fmt_meta_cap)(struct file *file, void *fh,
					struct v4l2_fmtdesc *f);
	int (*vidioc_enum_fmt_meta_out)(struct file *file, void *fh,
					struct v4l2_fmtdesc *f);

	
	int (*vidioc_g_fmt_vid_cap)(struct file *file, void *fh,
				    struct v4l2_format *f);
	int (*vidioc_g_fmt_vid_overlay)(struct file *file, void *fh,
					struct v4l2_format *f);
	int (*vidioc_g_fmt_vid_out)(struct file *file, void *fh,
				    struct v4l2_format *f);
	int (*vidioc_g_fmt_vid_out_overlay)(struct file *file, void *fh,
					    struct v4l2_format *f);
	int (*vidioc_g_fmt_vbi_cap)(struct file *file, void *fh,
				    struct v4l2_format *f);
	int (*vidioc_g_fmt_vbi_out)(struct file *file, void *fh,
				    struct v4l2_format *f);
	int (*vidioc_g_fmt_sliced_vbi_cap)(struct file *file, void *fh,
					   struct v4l2_format *f);
	int (*vidioc_g_fmt_sliced_vbi_out)(struct file *file, void *fh,
					   struct v4l2_format *f);
	int (*vidioc_g_fmt_vid_cap_mplane)(struct file *file, void *fh,
					   struct v4l2_format *f);
	int (*vidioc_g_fmt_vid_out_mplane)(struct file *file, void *fh,
					   struct v4l2_format *f);
	int (*vidioc_g_fmt_sdr_cap)(struct file *file, void *fh,
				    struct v4l2_format *f);
	int (*vidioc_g_fmt_sdr_out)(struct file *file, void *fh,
				    struct v4l2_format *f);
	int (*vidioc_g_fmt_meta_cap)(struct file *file, void *fh,
				     struct v4l2_format *f);
	int (*vidioc_g_fmt_meta_out)(struct file *file, void *fh,
				     struct v4l2_format *f);

	
	int (*vidioc_s_fmt_vid_cap)(struct file *file, void *fh,
				    struct v4l2_format *f);
	int (*vidioc_s_fmt_vid_overlay)(struct file *file, void *fh,
					struct v4l2_format *f);
	int (*vidioc_s_fmt_vid_out)(struct file *file, void *fh,
				    struct v4l2_format *f);
	int (*vidioc_s_fmt_vid_out_overlay)(struct file *file, void *fh,
					    struct v4l2_format *f);
	int (*vidioc_s_fmt_vbi_cap)(struct file *file, void *fh,
				    struct v4l2_format *f);
	int (*vidioc_s_fmt_vbi_out)(struct file *file, void *fh,
				    struct v4l2_format *f);
	int (*vidioc_s_fmt_sliced_vbi_cap)(struct file *file, void *fh,
					   struct v4l2_format *f);
	int (*vidioc_s_fmt_sliced_vbi_out)(struct file *file, void *fh,
					   struct v4l2_format *f);
	int (*vidioc_s_fmt_vid_cap_mplane)(struct file *file, void *fh,
					   struct v4l2_format *f);
	int (*vidioc_s_fmt_vid_out_mplane)(struct file *file, void *fh,
					   struct v4l2_format *f);
	int (*vidioc_s_fmt_sdr_cap)(struct file *file, void *fh,
				    struct v4l2_format *f);
	int (*vidioc_s_fmt_sdr_out)(struct file *file, void *fh,
				    struct v4l2_format *f);
	int (*vidioc_s_fmt_meta_cap)(struct file *file, void *fh,
				     struct v4l2_format *f);
	int (*vidioc_s_fmt_meta_out)(struct file *file, void *fh,
				     struct v4l2_format *f);

	
	int (*vidioc_try_fmt_vid_cap)(struct file *file, void *fh,
				      struct v4l2_format *f);
	int (*vidioc_try_fmt_vid_overlay)(struct file *file, void *fh,
					  struct v4l2_format *f);
	int (*vidioc_try_fmt_vid_out)(struct file *file, void *fh,
				      struct v4l2_format *f);
	int (*vidioc_try_fmt_vid_out_overlay)(struct file *file, void *fh,
					     struct v4l2_format *f);
	int (*vidioc_try_fmt_vbi_cap)(struct file *file, void *fh,
				      struct v4l2_format *f);
	int (*vidioc_try_fmt_vbi_out)(struct file *file, void *fh,
				      struct v4l2_format *f);
	int (*vidioc_try_fmt_sliced_vbi_cap)(struct file *file, void *fh,
					     struct v4l2_format *f);
	int (*vidioc_try_fmt_sliced_vbi_out)(struct file *file, void *fh,
					     struct v4l2_format *f);
	int (*vidioc_try_fmt_vid_cap_mplane)(struct file *file, void *fh,
					     struct v4l2_format *f);
	int (*vidioc_try_fmt_vid_out_mplane)(struct file *file, void *fh,
					     struct v4l2_format *f);
	int (*vidioc_try_fmt_sdr_cap)(struct file *file, void *fh,
				      struct v4l2_format *f);
	int (*vidioc_try_fmt_sdr_out)(struct file *file, void *fh,
				      struct v4l2_format *f);
	int (*vidioc_try_fmt_meta_cap)(struct file *file, void *fh,
				       struct v4l2_format *f);
	int (*vidioc_try_fmt_meta_out)(struct file *file, void *fh,
				       struct v4l2_format *f);

	
	int (*vidioc_reqbufs)(struct file *file, void *fh,
			      struct v4l2_requestbuffers *b);
	int (*vidioc_querybuf)(struct file *file, void *fh,
			       struct v4l2_buffer *b);
	int (*vidioc_qbuf)(struct file *file, void *fh,
			   struct v4l2_buffer *b);
	int (*vidioc_expbuf)(struct file *file, void *fh,
			     struct v4l2_exportbuffer *e);
	int (*vidioc_dqbuf)(struct file *file, void *fh,
			    struct v4l2_buffer *b);

	int (*vidioc_create_bufs)(struct file *file, void *fh,
				  struct v4l2_create_buffers *b);
	int (*vidioc_prepare_buf)(struct file *file, void *fh,
				  struct v4l2_buffer *b);

	int (*vidioc_overlay)(struct file *file, void *fh, unsigned int i);
	int (*vidioc_g_fbuf)(struct file *file, void *fh,
			     struct v4l2_framebuffer *a);
	int (*vidioc_s_fbuf)(struct file *file, void *fh,
			     const struct v4l2_framebuffer *a);

		
	int (*vidioc_streamon)(struct file *file, void *fh,
			       enum v4l2_buf_type i);
	int (*vidioc_streamoff)(struct file *file, void *fh,
				enum v4l2_buf_type i);

		
	int (*vidioc_g_std)(struct file *file, void *fh, v4l2_std_id *norm);
	int (*vidioc_s_std)(struct file *file, void *fh, v4l2_std_id norm);
	int (*vidioc_querystd)(struct file *file, void *fh, v4l2_std_id *a);

		
	int (*vidioc_enum_input)(struct file *file, void *fh,
				 struct v4l2_input *inp);
	int (*vidioc_g_input)(struct file *file, void *fh, unsigned int *i);
	int (*vidioc_s_input)(struct file *file, void *fh, unsigned int i);

		
	int (*vidioc_enum_output)(struct file *file, void *fh,
				  struct v4l2_output *a);
	int (*vidioc_g_output)(struct file *file, void *fh, unsigned int *i);
	int (*vidioc_s_output)(struct file *file, void *fh, unsigned int i);

		
	int (*vidioc_queryctrl)(struct file *file, void *fh,
				struct v4l2_queryctrl *a);
	int (*vidioc_query_ext_ctrl)(struct file *file, void *fh,
				     struct v4l2_query_ext_ctrl *a);
	int (*vidioc_g_ctrl)(struct file *file, void *fh,
			     struct v4l2_control *a);
	int (*vidioc_s_ctrl)(struct file *file, void *fh,
			     struct v4l2_control *a);
	int (*vidioc_g_ext_ctrls)(struct file *file, void *fh,
				  struct v4l2_ext_controls *a);
	int (*vidioc_s_ext_ctrls)(struct file *file, void *fh,
				  struct v4l2_ext_controls *a);
	int (*vidioc_try_ext_ctrls)(struct file *file, void *fh,
				    struct v4l2_ext_controls *a);
	int (*vidioc_querymenu)(struct file *file, void *fh,
				struct v4l2_querymenu *a);

	
	int (*vidioc_enumaudio)(struct file *file, void *fh,
				struct v4l2_audio *a);
	int (*vidioc_g_audio)(struct file *file, void *fh,
			      struct v4l2_audio *a);
	int (*vidioc_s_audio)(struct file *file, void *fh,
			      const struct v4l2_audio *a);

	
	int (*vidioc_enumaudout)(struct file *file, void *fh,
				 struct v4l2_audioout *a);
	int (*vidioc_g_audout)(struct file *file, void *fh,
			       struct v4l2_audioout *a);
	int (*vidioc_s_audout)(struct file *file, void *fh,
			       const struct v4l2_audioout *a);
	int (*vidioc_g_modulator)(struct file *file, void *fh,
				  struct v4l2_modulator *a);
	int (*vidioc_s_modulator)(struct file *file, void *fh,
				  const struct v4l2_modulator *a);
	
	int (*vidioc_g_pixelaspect)(struct file *file, void *fh,
				    int buf_type, struct v4l2_fract *aspect);
	int (*vidioc_g_selection)(struct file *file, void *fh,
				  struct v4l2_selection *s);
	int (*vidioc_s_selection)(struct file *file, void *fh,
				  struct v4l2_selection *s);
	
	int (*vidioc_g_jpegcomp)(struct file *file, void *fh,
				 struct v4l2_jpegcompression *a);
	int (*vidioc_s_jpegcomp)(struct file *file, void *fh,
				 const struct v4l2_jpegcompression *a);
	int (*vidioc_g_enc_index)(struct file *file, void *fh,
				  struct v4l2_enc_idx *a);
	int (*vidioc_encoder_cmd)(struct file *file, void *fh,
				  struct v4l2_encoder_cmd *a);
	int (*vidioc_try_encoder_cmd)(struct file *file, void *fh,
				      struct v4l2_encoder_cmd *a);
	int (*vidioc_decoder_cmd)(struct file *file, void *fh,
				  struct v4l2_decoder_cmd *a);
	int (*vidioc_try_decoder_cmd)(struct file *file, void *fh,
				      struct v4l2_decoder_cmd *a);

	
	int (*vidioc_g_parm)(struct file *file, void *fh,
			     struct v4l2_streamparm *a);
	int (*vidioc_s_parm)(struct file *file, void *fh,
			     struct v4l2_streamparm *a);

	
	int (*vidioc_g_tuner)(struct file *file, void *fh,
			      struct v4l2_tuner *a);
	int (*vidioc_s_tuner)(struct file *file, void *fh,
			      const struct v4l2_tuner *a);
	int (*vidioc_g_frequency)(struct file *file, void *fh,
				  struct v4l2_frequency *a);
	int (*vidioc_s_frequency)(struct file *file, void *fh,
				  const struct v4l2_frequency *a);
	int (*vidioc_enum_freq_bands)(struct file *file, void *fh,
				      struct v4l2_frequency_band *band);

	
	int (*vidioc_g_sliced_vbi_cap)(struct file *file, void *fh,
				       struct v4l2_sliced_vbi_cap *a);

	
	int (*vidioc_log_status)(struct file *file, void *fh);

	int (*vidioc_s_hw_freq_seek)(struct file *file, void *fh,
				     const struct v4l2_hw_freq_seek *a);

	
#ifdef CONFIG_VIDEO_ADV_DEBUG
	int (*vidioc_g_register)(struct file *file, void *fh,
				 struct v4l2_dbg_register *reg);
	int (*vidioc_s_register)(struct file *file, void *fh,
				 const struct v4l2_dbg_register *reg);

	int (*vidioc_g_chip_info)(struct file *file, void *fh,
				  struct v4l2_dbg_chip_info *chip);
#endif

	int (*vidioc_enum_framesizes)(struct file *file, void *fh,
				      struct v4l2_frmsizeenum *fsize);

	int (*vidioc_enum_frameintervals)(struct file *file, void *fh,
					  struct v4l2_frmivalenum *fival);

	
	int (*vidioc_s_dv_timings)(struct file *file, void *fh,
				   struct v4l2_dv_timings *timings);
	int (*vidioc_g_dv_timings)(struct file *file, void *fh,
				   struct v4l2_dv_timings *timings);
	int (*vidioc_query_dv_timings)(struct file *file, void *fh,
				       struct v4l2_dv_timings *timings);
	int (*vidioc_enum_dv_timings)(struct file *file, void *fh,
				      struct v4l2_enum_dv_timings *timings);
	int (*vidioc_dv_timings_cap)(struct file *file, void *fh,
				     struct v4l2_dv_timings_cap *cap);
	int (*vidioc_g_edid)(struct file *file, void *fh,
			     struct v4l2_edid *edid);
	int (*vidioc_s_edid)(struct file *file, void *fh,
			     struct v4l2_edid *edid);

	int (*vidioc_subscribe_event)(struct v4l2_fh *fh,
				      const struct v4l2_event_subscription *sub);
	int (*vidioc_unsubscribe_event)(struct v4l2_fh *fh,
					const struct v4l2_event_subscription *sub);

	
	long (*vidioc_default)(struct file *file, void *fh,
			       bool valid_prio, unsigned int cmd, void *arg);
};







#define V4L2_DEV_DEBUG_IOCTL		0x01

#define V4L2_DEV_DEBUG_IOCTL_ARG	0x02

#define V4L2_DEV_DEBUG_FOP		0x04

#define V4L2_DEV_DEBUG_STREAMING	0x08

#define V4L2_DEV_DEBUG_POLL		0x10




const char *v4l2_norm_to_name(v4l2_std_id id);


void v4l2_video_std_frame_period(int id, struct v4l2_fract *frameperiod);


int v4l2_video_std_construct(struct v4l2_standard *vs,
				    int id, const char *name);


int v4l_video_std_enumstd(struct v4l2_standard *vs, v4l2_std_id id);


void v4l_printk_ioctl(const char *prefix, unsigned int cmd);

struct video_device;


extern const char *v4l2_field_names[];
extern const char *v4l2_type_names[];

#ifdef CONFIG_COMPAT

long int v4l2_compat_ioctl32(struct file *file, unsigned int cmd,
			     unsigned long arg);
#endif


typedef long (*v4l2_kioctl)(struct file *file, unsigned int cmd, void *arg);


long int video_usercopy(struct file *file, unsigned int cmd,
			unsigned long int arg, v4l2_kioctl func);


long int video_ioctl2(struct file *file,
		      unsigned int cmd, unsigned long int arg);

#endif 
