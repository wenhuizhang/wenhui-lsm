/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef __LINUX_MIPI_DBI_H
#define __LINUX_MIPI_DBI_H

#include <linux/mutex.h>
#include <drm/drm_device.h>
#include <drm/drm_simple_kms_helper.h>

struct drm_rect;
struct spi_device;
struct gpio_desc;
struct regulator;


struct mipi_dbi {
	
	struct drm_device drm;

	
	struct drm_simple_display_pipe pipe;

	struct spi_device *spi;
	bool enabled;
	struct mutex cmdlock;
	int (*command)(struct mipi_dbi *mipi, u8 *cmd, u8 *param, size_t num);
	const u8 *read_commands;
	struct gpio_desc *dc;
	u16 *tx_buf;
	void *tx_buf9;
	size_t tx_buf9_len;
	bool swap_bytes;
	struct gpio_desc *reset;
	unsigned int rotation;
	struct backlight_device *backlight;
	struct regulator *regulator;
};

static inline struct mipi_dbi *drm_to_mipi_dbi(struct drm_device *drm)
{
	return container_of(drm, struct mipi_dbi, drm);
}

int mipi_dbi_spi_init(struct spi_device *spi, struct mipi_dbi *mipi,
		      struct gpio_desc *dc);
int mipi_dbi_init(struct mipi_dbi *mipi,
		  const struct drm_simple_display_pipe_funcs *funcs,
		  const struct drm_display_mode *mode, unsigned int rotation);
void mipi_dbi_release(struct drm_device *drm);
void mipi_dbi_pipe_update(struct drm_simple_display_pipe *pipe,
			  struct drm_plane_state *old_state);
void mipi_dbi_enable_flush(struct mipi_dbi *mipi,
			   struct drm_crtc_state *crtc_state,
			   struct drm_plane_state *plan_state);
void mipi_dbi_pipe_disable(struct drm_simple_display_pipe *pipe);
void mipi_dbi_hw_reset(struct mipi_dbi *mipi);
bool mipi_dbi_display_is_on(struct mipi_dbi *mipi);
int mipi_dbi_poweron_reset(struct mipi_dbi *mipi);
int mipi_dbi_poweron_conditional_reset(struct mipi_dbi *mipi);
u32 mipi_dbi_spi_cmd_max_speed(struct spi_device *spi, size_t len);

int mipi_dbi_command_read(struct mipi_dbi *mipi, u8 cmd, u8 *val);
int mipi_dbi_command_buf(struct mipi_dbi *mipi, u8 cmd, u8 *data, size_t len);
int mipi_dbi_command_stackbuf(struct mipi_dbi *mipi, u8 cmd, u8 *data, size_t len);
int mipi_dbi_buf_copy(void *dst, struct drm_framebuffer *fb,
		      struct drm_rect *clip, bool swap);

#define mipi_dbi_command(mipi, cmd, seq...) \
({ \
	u8 d[] = { seq }; \
	mipi_dbi_command_stackbuf(mipi, cmd, d, ARRAY_SIZE(d)); \
})

#ifdef CONFIG_DEBUG_FS
int mipi_dbi_debugfs_init(struct drm_minor *minor);
#else
#define mipi_dbi_debugfs_init		NULL
#endif

#endif 
