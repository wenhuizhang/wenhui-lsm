/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef DRM_VRAM_MM_HELPER_H
#define DRM_VRAM_MM_HELPER_H

#include <drm/ttm/ttm_bo_driver.h>

struct drm_device;


struct drm_vram_mm_funcs {
	void (*evict_flags)(struct ttm_buffer_object *bo,
			    struct ttm_placement *placement);
	int (*verify_access)(struct ttm_buffer_object *bo, struct file *filp);
};


struct drm_vram_mm {
	uint64_t vram_base;
	size_t vram_size;

	struct ttm_bo_device bdev;

	const struct drm_vram_mm_funcs *funcs;
};


static inline struct drm_vram_mm *drm_vram_mm_of_bdev(
	struct ttm_bo_device *bdev)
{
	return container_of(bdev, struct drm_vram_mm, bdev);
}

int drm_vram_mm_init(struct drm_vram_mm *vmm, struct drm_device *dev,
		     uint64_t vram_base, size_t vram_size,
		     const struct drm_vram_mm_funcs *funcs);
void drm_vram_mm_cleanup(struct drm_vram_mm *vmm);

int drm_vram_mm_mmap(struct file *filp, struct vm_area_struct *vma,
		     struct drm_vram_mm *vmm);



struct drm_vram_mm *drm_vram_helper_alloc_mm(
	struct drm_device *dev, uint64_t vram_base, size_t vram_size,
	const struct drm_vram_mm_funcs *funcs);
void drm_vram_helper_release_mm(struct drm_device *dev);



int drm_vram_mm_file_operations_mmap(
	struct file *filp, struct vm_area_struct *vma);


#define DRM_VRAM_MM_FILE_OPERATIONS \
	.llseek		= no_llseek, \
	.read		= drm_read, \
	.poll		= drm_poll, \
	.unlocked_ioctl = drm_ioctl, \
	.compat_ioctl	= drm_compat_ioctl, \
	.mmap		= drm_vram_mm_file_operations_mmap, \
	.open		= drm_open, \
	.release	= drm_release \

#endif
