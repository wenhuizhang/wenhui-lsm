#ifndef _DRM_DEVICE_H_
#define _DRM_DEVICE_H_

#include <linux/list.h>
#include <linux/kref.h>
#include <linux/mutex.h>
#include <linux/idr.h>

#include <drm/drm_hashtab.h>
#include <drm/drm_mode_config.h>

struct drm_driver;
struct drm_minor;
struct drm_master;
struct drm_device_dma;
struct drm_vblank_crtc;
struct drm_sg_mem;
struct drm_local_map;
struct drm_vma_offset_manager;
struct drm_vram_mm;
struct drm_fb_helper;

struct inode;

struct pci_dev;
struct pci_controller;




enum switch_power_state {
	
	DRM_SWITCH_POWER_ON = 0,

	
	DRM_SWITCH_POWER_OFF = 1,

	
	DRM_SWITCH_POWER_CHANGING = 2,

	
	DRM_SWITCH_POWER_DYNAMIC_OFF = 3,
};


struct drm_device {
	
	struct list_head legacy_dev_list;

	
	int if_version;

	
	struct kref ref;

	
	struct device *dev;

	
	struct drm_driver *driver;

	
	void *dev_private;

	
	struct drm_minor *primary;

	
	struct drm_minor *render;

	
	bool registered;

	
	struct drm_master *master;

	
	u32 driver_features;

	
	bool unplugged;

	
	struct inode *anon_inode;

	
	char *unique;

	
	struct mutex struct_mutex;

	
	struct mutex master_mutex;

	
	int open_count;

	
	struct mutex filelist_mutex;
	
	struct list_head filelist;

	
	struct list_head filelist_internal;

	
	struct mutex clientlist_mutex;

	
	struct list_head clientlist;

	
	bool irq_enabled;

	
	int irq;

	
	bool vblank_disable_immediate;

	
	struct drm_vblank_crtc *vblank;

	
	spinlock_t vblank_time_lock;
	
	spinlock_t vbl_lock;

	
	u32 max_vblank_count;

	
	struct list_head vblank_event_list;

	
	spinlock_t event_lock;

	
	struct drm_agp_head *agp;

	
	struct pci_dev *pdev;

#ifdef __alpha__
	
	struct pci_controller *hose;
#endif
	
	unsigned int num_crtcs;

	
	struct drm_mode_config mode_config;

	
	struct mutex object_name_lock;

	
	struct idr object_name_idr;

	
	struct drm_vma_offset_manager *vma_offset_manager;

	
	struct drm_vram_mm *vram_mm;

	
	enum switch_power_state switch_power_state;

	
	struct drm_fb_helper *fb_helper;

	
	
#if IS_ENABLED(CONFIG_DRM_LEGACY)
	
	struct list_head ctxlist;

	
	struct mutex ctxlist_mutex;

	
	struct idr ctx_idr;

	
	struct list_head maplist;

	
	struct drm_open_hash map_hash;

	
	struct list_head vmalist;

	
	struct drm_device_dma *dma;

	
	__volatile__ long context_flag;

	
	int last_context;

	
	spinlock_t buf_lock;

	
	int buf_use;

	
	atomic_t buf_alloc;

	struct {
		int context;
		struct drm_hw_lock *lock;
	} sigdata;

	struct drm_local_map *agp_buffer_map;
	unsigned int agp_buffer_token;

	
	struct drm_sg_mem *sg;
#endif
};

#endif
