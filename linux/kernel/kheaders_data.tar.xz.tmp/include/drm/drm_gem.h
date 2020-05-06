#ifndef __DRM_GEM_H__
#define __DRM_GEM_H__



#include <linux/kref.h>
#include <linux/reservation.h>

#include <drm/drm_vma_manager.h>

struct drm_gem_object;


struct drm_gem_object_funcs {
	
	void (*free)(struct drm_gem_object *obj);

	
	int (*open)(struct drm_gem_object *obj, struct drm_file *file);

	
	void (*close)(struct drm_gem_object *obj, struct drm_file *file);

	
	void (*print_info)(struct drm_printer *p, unsigned int indent,
			   const struct drm_gem_object *obj);

	
	struct dma_buf *(*export)(struct drm_gem_object *obj, int flags);

	
	int (*pin)(struct drm_gem_object *obj);

	
	void (*unpin)(struct drm_gem_object *obj);

	
	struct sg_table *(*get_sg_table)(struct drm_gem_object *obj);

	
	void *(*vmap)(struct drm_gem_object *obj);

	
	void (*vunmap)(struct drm_gem_object *obj, void *vaddr);

	
	const struct vm_operations_struct *vm_ops;
};


struct drm_gem_object {
	
	struct kref refcount;

	
	unsigned handle_count;

	
	struct drm_device *dev;

	
	struct file *filp;

	
	struct drm_vma_offset_node vma_node;

	
	size_t size;

	
	int name;

	
	struct dma_buf *dma_buf;

	
	struct dma_buf_attachment *import_attach;

	
	struct reservation_object *resv;

	
	struct reservation_object _resv;

	
	const struct drm_gem_object_funcs *funcs;
};


#define DEFINE_DRM_GEM_FOPS(name) \
	static const struct file_operations name = {\
		.owner		= THIS_MODULE,\
		.open		= drm_open,\
		.release	= drm_release,\
		.unlocked_ioctl	= drm_ioctl,\
		.compat_ioctl	= drm_compat_ioctl,\
		.poll		= drm_poll,\
		.read		= drm_read,\
		.llseek		= noop_llseek,\
		.mmap		= drm_gem_mmap,\
	}

void drm_gem_object_release(struct drm_gem_object *obj);
void drm_gem_object_free(struct kref *kref);
int drm_gem_object_init(struct drm_device *dev,
			struct drm_gem_object *obj, size_t size);
void drm_gem_private_object_init(struct drm_device *dev,
				 struct drm_gem_object *obj, size_t size);
void drm_gem_vm_open(struct vm_area_struct *vma);
void drm_gem_vm_close(struct vm_area_struct *vma);
int drm_gem_mmap_obj(struct drm_gem_object *obj, unsigned long obj_size,
		     struct vm_area_struct *vma);
int drm_gem_mmap(struct file *filp, struct vm_area_struct *vma);


static inline void drm_gem_object_get(struct drm_gem_object *obj)
{
	kref_get(&obj->refcount);
}


static inline void
__drm_gem_object_put(struct drm_gem_object *obj)
{
	kref_put(&obj->refcount, drm_gem_object_free);
}

void drm_gem_object_put_unlocked(struct drm_gem_object *obj);
void drm_gem_object_put(struct drm_gem_object *obj);

int drm_gem_handle_create(struct drm_file *file_priv,
			  struct drm_gem_object *obj,
			  u32 *handlep);
int drm_gem_handle_delete(struct drm_file *filp, u32 handle);


void drm_gem_free_mmap_offset(struct drm_gem_object *obj);
int drm_gem_create_mmap_offset(struct drm_gem_object *obj);
int drm_gem_create_mmap_offset_size(struct drm_gem_object *obj, size_t size);

struct page **drm_gem_get_pages(struct drm_gem_object *obj);
void drm_gem_put_pages(struct drm_gem_object *obj, struct page **pages,
		bool dirty, bool accessed);

int drm_gem_objects_lookup(struct drm_file *filp, void __user *bo_handles,
			   int count, struct drm_gem_object ***objs_out);
struct drm_gem_object *drm_gem_object_lookup(struct drm_file *filp, u32 handle);
long drm_gem_reservation_object_wait(struct drm_file *filep, u32 handle,
				    bool wait_all, unsigned long timeout);
int drm_gem_lock_reservations(struct drm_gem_object **objs, int count,
			      struct ww_acquire_ctx *acquire_ctx);
void drm_gem_unlock_reservations(struct drm_gem_object **objs, int count,
				 struct ww_acquire_ctx *acquire_ctx);
int drm_gem_fence_array_add(struct xarray *fence_array,
			    struct dma_fence *fence);
int drm_gem_fence_array_add_implicit(struct xarray *fence_array,
				     struct drm_gem_object *obj,
				     bool write);
int drm_gem_dumb_map_offset(struct drm_file *file, struct drm_device *dev,
			    u32 handle, u64 *offset);
int drm_gem_dumb_destroy(struct drm_file *file,
			 struct drm_device *dev,
			 uint32_t handle);

#endif 
