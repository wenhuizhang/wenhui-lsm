/* SPDX-License-Identifier: GPL-2.0 */
#ifndef LINUX_IOMAP_H
#define LINUX_IOMAP_H 1

#include <linux/atomic.h>
#include <linux/bitmap.h>
#include <linux/mm.h>
#include <linux/types.h>
#include <linux/mm_types.h>
#include <linux/blkdev.h>

struct address_space;
struct fiemap_extent_info;
struct inode;
struct iov_iter;
struct kiocb;
struct page;
struct vm_area_struct;
struct vm_fault;


#define IOMAP_HOLE	0x01	
#define IOMAP_DELALLOC	0x02	
#define IOMAP_MAPPED	0x03	
#define IOMAP_UNWRITTEN	0x04	
#define IOMAP_INLINE	0x05	


#define IOMAP_F_NEW		0x01	
#define IOMAP_F_DIRTY		0x02	
#define IOMAP_F_BUFFER_HEAD	0x04	
#define IOMAP_F_SIZE_CHANGED	0x08	


#define IOMAP_F_MERGED		0x10	
#define IOMAP_F_SHARED		0x20	


#define IOMAP_F_PRIVATE		0x1000



#define IOMAP_NULL_ADDR -1ULL	

struct iomap_page_ops;

struct iomap {
	u64			addr; 
	loff_t			offset;	
	u64			length;	
	u16			type;	
	u16			flags;	
	struct block_device	*bdev;	
	struct dax_device	*dax_dev; 
	void			*inline_data;
	void			*private; 
	const struct iomap_page_ops *page_ops;
};

static inline sector_t
iomap_sector(struct iomap *iomap, loff_t pos)
{
	return (iomap->addr + pos - iomap->offset) >> SECTOR_SHIFT;
}


struct iomap_page_ops {
	int (*page_prepare)(struct inode *inode, loff_t pos, unsigned len,
			struct iomap *iomap);
	void (*page_done)(struct inode *inode, loff_t pos, unsigned copied,
			struct page *page, struct iomap *iomap);
};


#define IOMAP_WRITE		(1 << 0) 
#define IOMAP_ZERO		(1 << 1) 
#define IOMAP_REPORT		(1 << 2) 
#define IOMAP_FAULT		(1 << 3) 
#define IOMAP_DIRECT		(1 << 4) 
#define IOMAP_NOWAIT		(1 << 5) 

struct iomap_ops {
	
	int (*iomap_begin)(struct inode *inode, loff_t pos, loff_t length,
			unsigned flags, struct iomap *iomap);

	
	int (*iomap_end)(struct inode *inode, loff_t pos, loff_t length,
			ssize_t written, unsigned flags, struct iomap *iomap);
};


typedef loff_t (*iomap_actor_t)(struct inode *inode, loff_t pos, loff_t len,
		void *data, struct iomap *iomap);

loff_t iomap_apply(struct inode *inode, loff_t pos, loff_t length,
		unsigned flags, const struct iomap_ops *ops, void *data,
		iomap_actor_t actor);


struct iomap_page {
	atomic_t		read_count;
	atomic_t		write_count;
	DECLARE_BITMAP(uptodate, PAGE_SIZE / 512);
};

static inline struct iomap_page *to_iomap_page(struct page *page)
{
	if (page_has_private(page))
		return (struct iomap_page *)page_private(page);
	return NULL;
}

ssize_t iomap_file_buffered_write(struct kiocb *iocb, struct iov_iter *from,
		const struct iomap_ops *ops);
int iomap_readpage(struct page *page, const struct iomap_ops *ops);
int iomap_readpages(struct address_space *mapping, struct list_head *pages,
		unsigned nr_pages, const struct iomap_ops *ops);
int iomap_set_page_dirty(struct page *page);
int iomap_is_partially_uptodate(struct page *page, unsigned long from,
		unsigned long count);
int iomap_releasepage(struct page *page, gfp_t gfp_mask);
void iomap_invalidatepage(struct page *page, unsigned int offset,
		unsigned int len);
#ifdef CONFIG_MIGRATION
int iomap_migrate_page(struct address_space *mapping, struct page *newpage,
		struct page *page, enum migrate_mode mode);
#else
#define iomap_migrate_page NULL
#endif
int iomap_file_dirty(struct inode *inode, loff_t pos, loff_t len,
		const struct iomap_ops *ops);
int iomap_zero_range(struct inode *inode, loff_t pos, loff_t len,
		bool *did_zero, const struct iomap_ops *ops);
int iomap_truncate_page(struct inode *inode, loff_t pos, bool *did_zero,
		const struct iomap_ops *ops);
vm_fault_t iomap_page_mkwrite(struct vm_fault *vmf,
			const struct iomap_ops *ops);
int iomap_fiemap(struct inode *inode, struct fiemap_extent_info *fieinfo,
		loff_t start, loff_t len, const struct iomap_ops *ops);
loff_t iomap_seek_hole(struct inode *inode, loff_t offset,
		const struct iomap_ops *ops);
loff_t iomap_seek_data(struct inode *inode, loff_t offset,
		const struct iomap_ops *ops);
sector_t iomap_bmap(struct address_space *mapping, sector_t bno,
		const struct iomap_ops *ops);


#define IOMAP_DIO_UNWRITTEN	(1 << 0)	
#define IOMAP_DIO_COW		(1 << 1)	
typedef int (iomap_dio_end_io_t)(struct kiocb *iocb, ssize_t ret,
		unsigned flags);
ssize_t iomap_dio_rw(struct kiocb *iocb, struct iov_iter *iter,
		const struct iomap_ops *ops, iomap_dio_end_io_t end_io);
int iomap_dio_iopoll(struct kiocb *kiocb, bool spin);

#ifdef CONFIG_SWAP
struct file;
struct swap_info_struct;

int iomap_swapfile_activate(struct swap_info_struct *sis,
		struct file *swap_file, sector_t *pagespan,
		const struct iomap_ops *ops);
#else
# define iomap_swapfile_activate(sis, swapfile, pagespan, ops)	(-EIO)
#endif 

#endif 
