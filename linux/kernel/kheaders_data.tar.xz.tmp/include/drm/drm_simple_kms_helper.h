/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef __LINUX_DRM_SIMPLE_KMS_HELPER_H
#define __LINUX_DRM_SIMPLE_KMS_HELPER_H

#include <drm/drm_crtc.h>
#include <drm/drm_encoder.h>
#include <drm/drm_plane.h>

struct drm_simple_display_pipe;


struct drm_simple_display_pipe_funcs {
	
	enum drm_mode_status (*mode_valid)(struct drm_crtc *crtc,
					   const struct drm_display_mode *mode);

	
	void (*enable)(struct drm_simple_display_pipe *pipe,
		       struct drm_crtc_state *crtc_state,
		       struct drm_plane_state *plane_state);
	
	void (*disable)(struct drm_simple_display_pipe *pipe);

	
	int (*check)(struct drm_simple_display_pipe *pipe,
		     struct drm_plane_state *plane_state,
		     struct drm_crtc_state *crtc_state);
	
	void (*update)(struct drm_simple_display_pipe *pipe,
		       struct drm_plane_state *old_plane_state);

	
	int (*prepare_fb)(struct drm_simple_display_pipe *pipe,
			  struct drm_plane_state *plane_state);

	
	void (*cleanup_fb)(struct drm_simple_display_pipe *pipe,
			   struct drm_plane_state *plane_state);

	
	int (*enable_vblank)(struct drm_simple_display_pipe *pipe);

	
	void (*disable_vblank)(struct drm_simple_display_pipe *pipe);
};


struct drm_simple_display_pipe {
	struct drm_crtc crtc;
	struct drm_plane plane;
	struct drm_encoder encoder;
	struct drm_connector *connector;

	const struct drm_simple_display_pipe_funcs *funcs;
};

int drm_simple_display_pipe_attach_bridge(struct drm_simple_display_pipe *pipe,
					  struct drm_bridge *bridge);

int drm_simple_display_pipe_init(struct drm_device *dev,
			struct drm_simple_display_pipe *pipe,
			const struct drm_simple_display_pipe_funcs *funcs,
			const uint32_t *formats, unsigned int format_count,
			const uint64_t *format_modifiers,
			struct drm_connector *connector);

#endif 
