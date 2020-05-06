/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_GENHD_H
#define _LINUX_GENHD_H



#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/rcupdate.h>
#include <linux/slab.h>
#include <linux/percpu-refcount.h>
#include <linux/uuid.h>
#include <linux/blk_types.h>
#include <asm/local.h>

#ifdef CONFIG_BLOCK

#define dev_to_disk(device)	container_of((device), struct gendisk, part0.__dev)
#define dev_to_part(device)	container_of((device), struct hd_struct, __dev)
#define disk_to_dev(disk)	(&(disk)->part0.__dev)
#define part_to_dev(part)	(&((part)->__dev))

extern struct device_type part_type;
extern struct kobject *block_depr;
extern struct class block_class;

enum {

	DOS_EXTENDED_PARTITION = 5,
	LINUX_EXTENDED_PARTITION = 0x85,
	WIN98_EXTENDED_PARTITION = 0x0f,

	SUN_WHOLE_DISK = DOS_EXTENDED_PARTITION,

	LINUX_SWAP_PARTITION = 0x82,
	LINUX_DATA_PARTITION = 0x83,
	LINUX_LVM_PARTITION = 0x8e,
	LINUX_RAID_PARTITION = 0xfd,	

	SOLARIS_X86_PARTITION =	LINUX_SWAP_PARTITION,
	NEW_SOLARIS_X86_PARTITION = 0xbf,

	DM6_AUX1PARTITION = 0x51,	
	DM6_AUX3PARTITION = 0x53,	
	DM6_PARTITION =	0x54,		
	EZD_PARTITION =	0x55,		

	FREEBSD_PARTITION = 0xa5,	
	OPENBSD_PARTITION = 0xa6,	
	NETBSD_PARTITION = 0xa9,	
	BSDI_PARTITION = 0xb7,		
	MINIX_PARTITION = 0x81,		
	UNIXWARE_PARTITION = 0x63,	
};

#define DISK_MAX_PARTS			256
#define DISK_NAME_LEN			32

#include <linux/major.h>
#include <linux/device.h>
#include <linux/smp.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/workqueue.h>

struct partition {
	unsigned char boot_ind;		
	unsigned char head;		
	unsigned char sector;		
	unsigned char cyl;		
	unsigned char sys_ind;		
	unsigned char end_head;		
	unsigned char end_sector;	
	unsigned char end_cyl;		
	__le32 start_sect;	
	__le32 nr_sects;		
} __attribute__((packed));

struct disk_stats {
	u64 nsecs[NR_STAT_GROUPS];
	unsigned long sectors[NR_STAT_GROUPS];
	unsigned long ios[NR_STAT_GROUPS];
	unsigned long merges[NR_STAT_GROUPS];
	unsigned long io_ticks;
	unsigned long time_in_queue;
	local_t in_flight[2];
};

#define PARTITION_META_INFO_VOLNAMELTH	64

#define PARTITION_META_INFO_UUIDLTH	(UUID_STRING_LEN + 1)

struct partition_meta_info {
	char uuid[PARTITION_META_INFO_UUIDLTH];
	u8 volname[PARTITION_META_INFO_VOLNAMELTH];
};

struct hd_struct {
	sector_t start_sect;
	
	sector_t nr_sects;
	seqcount_t nr_sects_seq;
	sector_t alignment_offset;
	unsigned int discard_alignment;
	struct device __dev;
	struct kobject *holder_dir;
	int policy, partno;
	struct partition_meta_info *info;
#ifdef CONFIG_FAIL_MAKE_REQUEST
	int make_it_fail;
#endif
	unsigned long stamp;
#ifdef	CONFIG_SMP
	struct disk_stats __percpu *dkstats;
#else
	struct disk_stats dkstats;
#endif
	struct percpu_ref ref;
	struct rcu_work rcu_work;
};

#define GENHD_FL_REMOVABLE			1

#define GENHD_FL_MEDIA_CHANGE_NOTIFY		4
#define GENHD_FL_CD				8
#define GENHD_FL_UP				16
#define GENHD_FL_SUPPRESS_PARTITION_INFO	32
#define GENHD_FL_EXT_DEVT			64 
#define GENHD_FL_NATIVE_CAPACITY		128
#define GENHD_FL_BLOCK_EVENTS_ON_EXCL_WRITE	256
#define GENHD_FL_NO_PART_SCAN			512
#define GENHD_FL_HIDDEN				1024

enum {
	DISK_EVENT_MEDIA_CHANGE			= 1 << 0, 
	DISK_EVENT_EJECT_REQUEST		= 1 << 1, 
};

enum {
	
	DISK_EVENT_FLAG_POLL			= 1 << 0,
	
	DISK_EVENT_FLAG_UEVENT			= 1 << 1,
};

struct disk_part_tbl {
	struct rcu_head rcu_head;
	int len;
	struct hd_struct __rcu *last_lookup;
	struct hd_struct __rcu *part[];
};

struct disk_events;
struct badblocks;

#if defined(CONFIG_BLK_DEV_INTEGRITY)

struct blk_integrity {
	const struct blk_integrity_profile	*profile;
	unsigned char				flags;
	unsigned char				tuple_size;
	unsigned char				interval_exp;
	unsigned char				tag_size;
};

#endif	

struct gendisk {
	
	int major;			
	int first_minor;
	int minors;                     

	char disk_name[DISK_NAME_LEN];	
	char *(*devnode)(struct gendisk *gd, umode_t *mode);

	unsigned short events;		
	unsigned short event_flags;	

	
	struct disk_part_tbl __rcu *part_tbl;
	struct hd_struct part0;

	const struct block_device_operations *fops;
	struct request_queue *queue;
	void *private_data;

	int flags;
	struct rw_semaphore lookup_sem;
	struct kobject *slave_dir;

	struct timer_rand_state *random;
	atomic_t sync_io;		
	struct disk_events *ev;
#ifdef  CONFIG_BLK_DEV_INTEGRITY
	struct kobject integrity_kobj;
#endif	
	int node_id;
	struct badblocks *bb;
	struct lockdep_map lockdep_map;
};

static inline struct gendisk *part_to_disk(struct hd_struct *part)
{
	if (likely(part)) {
		if (part->partno)
			return dev_to_disk(part_to_dev(part)->parent);
		else
			return dev_to_disk(part_to_dev(part));
	}
	return NULL;
}

static inline int disk_max_parts(struct gendisk *disk)
{
	if (disk->flags & GENHD_FL_EXT_DEVT)
		return DISK_MAX_PARTS;
	return disk->minors;
}

static inline bool disk_part_scan_enabled(struct gendisk *disk)
{
	return disk_max_parts(disk) > 1 &&
		!(disk->flags & GENHD_FL_NO_PART_SCAN);
}

static inline dev_t disk_devt(struct gendisk *disk)
{
	return MKDEV(disk->major, disk->first_minor);
}

static inline dev_t part_devt(struct hd_struct *part)
{
	return part_to_dev(part)->devt;
}

extern struct hd_struct *__disk_get_part(struct gendisk *disk, int partno);
extern struct hd_struct *disk_get_part(struct gendisk *disk, int partno);

static inline void disk_put_part(struct hd_struct *part)
{
	if (likely(part))
		put_device(part_to_dev(part));
}


#define DISK_PITER_REVERSE	(1 << 0) 
#define DISK_PITER_INCL_EMPTY	(1 << 1) 
#define DISK_PITER_INCL_PART0	(1 << 2) 
#define DISK_PITER_INCL_EMPTY_PART0 (1 << 3) 

struct disk_part_iter {
	struct gendisk		*disk;
	struct hd_struct	*part;
	int			idx;
	unsigned int		flags;
};

extern void disk_part_iter_init(struct disk_part_iter *piter,
				 struct gendisk *disk, unsigned int flags);
extern struct hd_struct *disk_part_iter_next(struct disk_part_iter *piter);
extern void disk_part_iter_exit(struct disk_part_iter *piter);

extern struct hd_struct *disk_map_sector_rcu(struct gendisk *disk,
					     sector_t sector);


#ifdef	CONFIG_SMP
#define part_stat_lock()	({ rcu_read_lock(); get_cpu(); })
#define part_stat_unlock()	do { put_cpu(); rcu_read_unlock(); } while (0)

#define part_stat_get_cpu(part, field, cpu)					\
	(per_cpu_ptr((part)->dkstats, (cpu))->field)

#define part_stat_get(part, field)					\
	part_stat_get_cpu(part, field, smp_processor_id())

#define part_stat_read(part, field)					\
({									\
	typeof((part)->dkstats->field) res = 0;				\
	unsigned int _cpu;						\
	for_each_possible_cpu(_cpu)					\
		res += per_cpu_ptr((part)->dkstats, _cpu)->field;	\
	res;								\
})

static inline void part_stat_set_all(struct hd_struct *part, int value)
{
	int i;

	for_each_possible_cpu(i)
		memset(per_cpu_ptr(part->dkstats, i), value,
				sizeof(struct disk_stats));
}

static inline int init_part_stats(struct hd_struct *part)
{
	part->dkstats = alloc_percpu(struct disk_stats);
	if (!part->dkstats)
		return 0;
	return 1;
}

static inline void free_part_stats(struct hd_struct *part)
{
	free_percpu(part->dkstats);
}

#else 
#define part_stat_lock()	({ rcu_read_lock(); 0; })
#define part_stat_unlock()	rcu_read_unlock()

#define part_stat_get(part, field)		((part)->dkstats.field)
#define part_stat_get_cpu(part, field, cpu)	part_stat_get(part, field)
#define part_stat_read(part, field)		part_stat_get(part, field)

static inline void part_stat_set_all(struct hd_struct *part, int value)
{
	memset(&part->dkstats, value, sizeof(struct disk_stats));
}

static inline int init_part_stats(struct hd_struct *part)
{
	return 1;
}

static inline void free_part_stats(struct hd_struct *part)
{
}

#endif 

#define part_stat_read_msecs(part, which)				\
	div_u64(part_stat_read(part, nsecs[which]), NSEC_PER_MSEC)

#define part_stat_read_accum(part, field)				\
	(part_stat_read(part, field[STAT_READ]) +			\
	 part_stat_read(part, field[STAT_WRITE]) +			\
	 part_stat_read(part, field[STAT_DISCARD]))

#define __part_stat_add(part, field, addnd)				\
	(part_stat_get(part, field) += (addnd))

#define part_stat_add(part, field, addnd)	do {			\
	__part_stat_add((part), field, addnd);				\
	if ((part)->partno)						\
		__part_stat_add(&part_to_disk((part))->part0,		\
				field, addnd);				\
} while (0)

#define part_stat_dec(gendiskp, field)					\
	part_stat_add(gendiskp, field, -1)
#define part_stat_inc(gendiskp, field)					\
	part_stat_add(gendiskp, field, 1)
#define part_stat_sub(gendiskp, field, subnd)				\
	part_stat_add(gendiskp, field, -subnd)

#define part_stat_local_dec(gendiskp, field)				\
	local_dec(&(part_stat_get(gendiskp, field)))
#define part_stat_local_inc(gendiskp, field)				\
	local_inc(&(part_stat_get(gendiskp, field)))
#define part_stat_local_read(gendiskp, field)				\
	local_read(&(part_stat_get(gendiskp, field)))
#define part_stat_local_read_cpu(gendiskp, field, cpu)			\
	local_read(&(part_stat_get_cpu(gendiskp, field, cpu)))

unsigned int part_in_flight(struct request_queue *q, struct hd_struct *part);
void part_in_flight_rw(struct request_queue *q, struct hd_struct *part,
		       unsigned int inflight[2]);
void part_dec_in_flight(struct request_queue *q, struct hd_struct *part,
			int rw);
void part_inc_in_flight(struct request_queue *q, struct hd_struct *part,
			int rw);

static inline struct partition_meta_info *alloc_part_info(struct gendisk *disk)
{
	if (disk)
		return kzalloc_node(sizeof(struct partition_meta_info),
				    GFP_KERNEL, disk->node_id);
	return kzalloc(sizeof(struct partition_meta_info), GFP_KERNEL);
}

static inline void free_part_info(struct hd_struct *part)
{
	kfree(part->info);
}

void update_io_ticks(struct hd_struct *part, unsigned long now);


extern void device_add_disk(struct device *parent, struct gendisk *disk,
			    const struct attribute_group **groups);
static inline void add_disk(struct gendisk *disk)
{
	device_add_disk(NULL, disk, NULL);
}
extern void device_add_disk_no_queue_reg(struct device *parent, struct gendisk *disk);
static inline void add_disk_no_queue_reg(struct gendisk *disk)
{
	device_add_disk_no_queue_reg(NULL, disk);
}

extern void del_gendisk(struct gendisk *gp);
extern struct gendisk *get_gendisk(dev_t dev, int *partno);
extern struct block_device *bdget_disk(struct gendisk *disk, int partno);

extern void set_device_ro(struct block_device *bdev, int flag);
extern void set_disk_ro(struct gendisk *disk, int flag);

static inline int get_disk_ro(struct gendisk *disk)
{
	return disk->part0.policy;
}

extern void disk_block_events(struct gendisk *disk);
extern void disk_unblock_events(struct gendisk *disk);
extern void disk_flush_events(struct gendisk *disk, unsigned int mask);
extern unsigned int disk_clear_events(struct gendisk *disk, unsigned int mask);


extern void add_disk_randomness(struct gendisk *disk) __latent_entropy;
extern void rand_initialize_disk(struct gendisk *disk);

static inline sector_t get_start_sect(struct block_device *bdev)
{
	return bdev->bd_part->start_sect;
}
static inline sector_t get_capacity(struct gendisk *disk)
{
	return disk->part0.nr_sects;
}
static inline void set_capacity(struct gendisk *disk, sector_t size)
{
	disk->part0.nr_sects = size;
}

#ifdef CONFIG_SOLARIS_X86_PARTITION

#define SOLARIS_X86_NUMSLICE	16
#define SOLARIS_X86_VTOC_SANE	(0x600DDEEEUL)

struct solaris_x86_slice {
	__le16 s_tag;		
	__le16 s_flag;		
	__le32 s_start;		
	__le32 s_size;		
};

struct solaris_x86_vtoc {
	unsigned int v_bootinfo[3];	
	__le32 v_sanity;		
	__le32 v_version;		
	char	v_volume[8];		
	__le16	v_sectorsz;		
	__le16	v_nparts;		
	unsigned int v_reserved[10];	
	struct solaris_x86_slice
		v_slice[SOLARIS_X86_NUMSLICE]; 
	unsigned int timestamp[SOLARIS_X86_NUMSLICE]; 
	char	v_asciilabel[128];	
};

#endif 

#ifdef CONFIG_BSD_DISKLABEL




#define BSD_DISKMAGIC	(0x82564557UL)	
#define BSD_MAXPARTITIONS	16
#define OPENBSD_MAXPARTITIONS	16
#define BSD_FS_UNUSED		0	
struct bsd_disklabel {
	__le32	d_magic;		
	__s16	d_type;			
	__s16	d_subtype;		
	char	d_typename[16];		
	char	d_packname[16];			 
	__u32	d_secsize;		
	__u32	d_nsectors;		
	__u32	d_ntracks;		
	__u32	d_ncylinders;		
	__u32	d_secpercyl;		
	__u32	d_secperunit;		
	__u16	d_sparespertrack;	
	__u16	d_sparespercyl;		
	__u32	d_acylinders;		
	__u16	d_rpm;			
	__u16	d_interleave;		
	__u16	d_trackskew;		
	__u16	d_cylskew;		
	__u32	d_headswitch;		
	__u32	d_trkseek;		
	__u32	d_flags;		
#define NDDATA 5
	__u32	d_drivedata[NDDATA];	
#define NSPARE 5
	__u32	d_spare[NSPARE];	
	__le32	d_magic2;		
	__le16	d_checksum;		

			
	__le16	d_npartitions;		
	__le32	d_bbsize;		
	__le32	d_sbsize;		
	struct	bsd_partition {		
		__le32	p_size;		
		__le32	p_offset;	
		__le32	p_fsize;	
		__u8	p_fstype;	
		__u8	p_frag;		
		__le16	p_cpg;		
	} d_partitions[BSD_MAXPARTITIONS];	
};

#endif	

#ifdef CONFIG_UNIXWARE_DISKLABEL


#define UNIXWARE_DISKMAGIC     (0xCA5E600DUL)	
#define UNIXWARE_DISKMAGIC2    (0x600DDEEEUL)	
#define UNIXWARE_NUMSLICE      16
#define UNIXWARE_FS_UNUSED     0		

struct unixware_slice {
	__le16   s_label;	
	__le16   s_flags;	
	__le32   start_sect;	
	__le32   nr_sects;	
};

struct unixware_disklabel {
	__le32   d_type;               	
	__le32   d_magic;                
	__le32   d_version;              
	char    d_serial[12];           
	__le32   d_ncylinders;           
	__le32   d_ntracks;              
	__le32   d_nsectors;             
	__le32   d_secsize;              
	__le32   d_part_start;           
	__le32   d_unknown1[12];         
 	__le32	d_alt_tbl;              
 	__le32	d_alt_len;              
 	__le32	d_phys_cyl;             
 	__le32	d_phys_trk;             
 	__le32	d_phys_sec;             
 	__le32	d_phys_bytes;           
 	__le32	d_unknown2;             
	__le32   d_unknown3;             
	__le32	d_pad[8];               

	struct unixware_vtoc {
		__le32	v_magic;		
		__le32	v_version;		
		char	v_name[8];		
		__le16	v_nslices;		
		__le16	v_unknown1;		
		__le32	v_reserved[10];		
		struct unixware_slice
			v_slice[UNIXWARE_NUMSLICE];	
	} vtoc;

};  

#endif 

#ifdef CONFIG_MINIX_SUBPARTITION
#   define MINIX_NR_SUBPARTITIONS  4
#endif 

#define ADDPART_FLAG_NONE	0
#define ADDPART_FLAG_RAID	1
#define ADDPART_FLAG_WHOLEDISK	2

extern int blk_alloc_devt(struct hd_struct *part, dev_t *devt);
extern void blk_free_devt(dev_t devt);
extern void blk_invalidate_devt(dev_t devt);
extern dev_t blk_lookup_devt(const char *name, int partno);
extern char *disk_name (struct gendisk *hd, int partno, char *buf);

extern int disk_expand_part_tbl(struct gendisk *disk, int target);
extern int rescan_partitions(struct gendisk *disk, struct block_device *bdev);
extern int invalidate_partitions(struct gendisk *disk, struct block_device *bdev);
extern struct hd_struct * __must_check add_partition(struct gendisk *disk,
						     int partno, sector_t start,
						     sector_t len, int flags,
						     struct partition_meta_info
						       *info);
extern void __delete_partition(struct percpu_ref *);
extern void delete_partition(struct gendisk *, int);
extern void printk_all_partitions(void);

extern struct gendisk *__alloc_disk_node(int minors, int node_id);
extern struct kobject *get_disk_and_module(struct gendisk *disk);
extern void put_disk(struct gendisk *disk);
extern void put_disk_and_module(struct gendisk *disk);
extern void blk_register_region(dev_t devt, unsigned long range,
			struct module *module,
			struct kobject *(*probe)(dev_t, int *, void *),
			int (*lock)(dev_t, void *),
			void *data);
extern void blk_unregister_region(dev_t devt, unsigned long range);

extern ssize_t part_size_show(struct device *dev,
			      struct device_attribute *attr, char *buf);
extern ssize_t part_stat_show(struct device *dev,
			      struct device_attribute *attr, char *buf);
extern ssize_t part_inflight_show(struct device *dev,
			      struct device_attribute *attr, char *buf);
#ifdef CONFIG_FAIL_MAKE_REQUEST
extern ssize_t part_fail_show(struct device *dev,
			      struct device_attribute *attr, char *buf);
extern ssize_t part_fail_store(struct device *dev,
			       struct device_attribute *attr,
			       const char *buf, size_t count);
#endif 

#define alloc_disk_node(minors, node_id)				\
({									\
	static struct lock_class_key __key;				\
	const char *__name;						\
	struct gendisk *__disk;						\
									\
	__name = "(gendisk_completion)"#minors"("#node_id")";		\
									\
	__disk = __alloc_disk_node(minors, node_id);			\
									\
	if (__disk)							\
		lockdep_init_map(&__disk->lockdep_map, __name, &__key, 0); \
									\
	__disk;								\
})

#define alloc_disk(minors) alloc_disk_node(minors, NUMA_NO_NODE)

static inline int hd_ref_init(struct hd_struct *part)
{
	if (percpu_ref_init(&part->ref, __delete_partition, 0,
				GFP_KERNEL))
		return -ENOMEM;
	return 0;
}

static inline void hd_struct_get(struct hd_struct *part)
{
	percpu_ref_get(&part->ref);
}

static inline int hd_struct_try_get(struct hd_struct *part)
{
	return percpu_ref_tryget_live(&part->ref);
}

static inline void hd_struct_put(struct hd_struct *part)
{
	percpu_ref_put(&part->ref);
}

static inline void hd_struct_kill(struct hd_struct *part)
{
	percpu_ref_kill(&part->ref);
}

static inline void hd_free_part(struct hd_struct *part)
{
	free_part_stats(part);
	free_part_info(part);
	percpu_ref_exit(&part->ref);
}


static inline sector_t part_nr_sects_read(struct hd_struct *part)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	sector_t nr_sects;
	unsigned seq;
	do {
		seq = read_seqcount_begin(&part->nr_sects_seq);
		nr_sects = part->nr_sects;
	} while (read_seqcount_retry(&part->nr_sects_seq, seq));
	return nr_sects;
#elif BITS_PER_LONG==32 && defined(CONFIG_PREEMPT)
	sector_t nr_sects;

	preempt_disable();
	nr_sects = part->nr_sects;
	preempt_enable();
	return nr_sects;
#else
	return part->nr_sects;
#endif
}


static inline void part_nr_sects_write(struct hd_struct *part, sector_t size)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	write_seqcount_begin(&part->nr_sects_seq);
	part->nr_sects = size;
	write_seqcount_end(&part->nr_sects_seq);
#elif BITS_PER_LONG==32 && defined(CONFIG_PREEMPT)
	preempt_disable();
	part->nr_sects = size;
	preempt_enable();
#else
	part->nr_sects = size;
#endif
}

#if defined(CONFIG_BLK_DEV_INTEGRITY)
extern void blk_integrity_add(struct gendisk *);
extern void blk_integrity_del(struct gendisk *);
#else	
static inline void blk_integrity_add(struct gendisk *disk) { }
static inline void blk_integrity_del(struct gendisk *disk) { }
#endif	

#else 

static inline void printk_all_partitions(void) { }

static inline dev_t blk_lookup_devt(const char *name, int partno)
{
	dev_t devt = MKDEV(0, 0);
	return devt;
}
#endif 

#endif 
