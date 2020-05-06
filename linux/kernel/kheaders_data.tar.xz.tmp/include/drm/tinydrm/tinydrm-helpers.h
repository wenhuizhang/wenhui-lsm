/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef __LINUX_TINYDRM_HELPERS_H
#define __LINUX_TINYDRM_HELPERS_H

struct backlight_device;
struct drm_device;
struct drm_display_mode;
struct drm_framebuffer;
struct drm_rect;
struct drm_simple_display_pipe;
struct drm_simple_display_pipe_funcs;
struct spi_transfer;
struct spi_message;
struct spi_device;
struct device;


static inline bool tinydrm_machine_little_endian(void)
{
#if defined(__LITTLE_ENDIAN)
	return true;
#else
	return false;
#endif
}

int tinydrm_display_pipe_init(struct drm_device *drm,
			      struct drm_simple_display_pipe *pipe,
			      const struct drm_simple_display_pipe_funcs *funcs,
			      int connector_type,
			      const uint32_t *formats,
			      unsigned int format_count,
			      const struct drm_display_mode *mode,
			      unsigned int rotation);

size_t tinydrm_spi_max_transfer_size(struct spi_device *spi, size_t max_len);
bool tinydrm_spi_bpw_supported(struct spi_device *spi, u8 bpw);
int tinydrm_spi_transfer(struct spi_device *spi, u32 speed_hz,
			 struct spi_transfer *header, u8 bpw, const void *buf,
			 size_t len);
void _tinydrm_dbg_spi_message(struct spi_device *spi, struct spi_message *m);

#ifdef DEBUG

static inline void tinydrm_dbg_spi_message(struct spi_device *spi,
					   struct spi_message *m)
{
	if (drm_debug & DRM_UT_DRIVER)
		_tinydrm_dbg_spi_message(spi, m);
}
#else
static inline void tinydrm_dbg_spi_message(struct spi_device *spi,
					   struct spi_message *m)
{
}
#endif 

#endif 
