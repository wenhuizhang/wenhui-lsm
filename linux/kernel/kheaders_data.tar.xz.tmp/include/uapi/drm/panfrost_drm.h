/* SPDX-License-Identifier: MIT */

#ifndef _PANFROST_DRM_H_
#define _PANFROST_DRM_H_

#include "drm.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define DRM_PANFROST_SUBMIT			0x00
#define DRM_PANFROST_WAIT_BO			0x01
#define DRM_PANFROST_CREATE_BO			0x02
#define DRM_PANFROST_MMAP_BO			0x03
#define DRM_PANFROST_GET_PARAM			0x04
#define DRM_PANFROST_GET_BO_OFFSET		0x05
#define DRM_PANFROST_PERFCNT_ENABLE		0x06
#define DRM_PANFROST_PERFCNT_DUMP		0x07

#define DRM_IOCTL_PANFROST_SUBMIT		DRM_IOW(DRM_COMMAND_BASE + DRM_PANFROST_SUBMIT, struct drm_panfrost_submit)
#define DRM_IOCTL_PANFROST_WAIT_BO		DRM_IOW(DRM_COMMAND_BASE + DRM_PANFROST_WAIT_BO, struct drm_panfrost_wait_bo)
#define DRM_IOCTL_PANFROST_CREATE_BO		DRM_IOWR(DRM_COMMAND_BASE + DRM_PANFROST_CREATE_BO, struct drm_panfrost_create_bo)
#define DRM_IOCTL_PANFROST_MMAP_BO		DRM_IOWR(DRM_COMMAND_BASE + DRM_PANFROST_MMAP_BO, struct drm_panfrost_mmap_bo)
#define DRM_IOCTL_PANFROST_GET_PARAM		DRM_IOWR(DRM_COMMAND_BASE + DRM_PANFROST_GET_PARAM, struct drm_panfrost_get_param)
#define DRM_IOCTL_PANFROST_GET_BO_OFFSET	DRM_IOWR(DRM_COMMAND_BASE + DRM_PANFROST_GET_BO_OFFSET, struct drm_panfrost_get_bo_offset)


#define DRM_IOCTL_PANFROST_PERFCNT_ENABLE	DRM_IOW(DRM_COMMAND_BASE + DRM_PANFROST_PERFCNT_ENABLE, struct drm_panfrost_perfcnt_enable)
#define DRM_IOCTL_PANFROST_PERFCNT_DUMP		DRM_IOW(DRM_COMMAND_BASE + DRM_PANFROST_PERFCNT_DUMP, struct drm_panfrost_perfcnt_dump)

#define PANFROST_JD_REQ_FS (1 << 0)

struct drm_panfrost_submit {

	
	__u64 jc;

	
	__u64 in_syncs;

	
	__u32 in_sync_count;

	
	__u32 out_sync;

	
	__u64 bo_handles;

	
	__u32 bo_handle_count;

	
	__u32 requirements;
};


struct drm_panfrost_wait_bo {
	__u32 handle;
	__u32 pad;
	__s64 timeout_ns;	
};


struct drm_panfrost_create_bo {
	__u32 size;
	__u32 flags;
	
	__u32 handle;
	
	__u32 pad;
	
	__u64 offset;
};


struct drm_panfrost_mmap_bo {
	
	__u32 handle;
	__u32 flags;
	
	__u64 offset;
};

enum drm_panfrost_param {
	DRM_PANFROST_PARAM_GPU_PROD_ID,
};

struct drm_panfrost_get_param {
	__u32 param;
	__u32 pad;
	__u64 value;
};


struct drm_panfrost_get_bo_offset {
	__u32 handle;
	__u32 pad;
	__u64 offset;
};

struct drm_panfrost_perfcnt_enable {
	__u32 enable;
	
	__u32 counterset;
};

struct drm_panfrost_perfcnt_dump {
	__u64 buf_ptr;
};

#if defined(__cplusplus)
}
#endif

#endif 
