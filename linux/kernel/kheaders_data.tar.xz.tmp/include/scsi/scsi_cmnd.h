/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SCSI_SCSI_CMND_H
#define _SCSI_SCSI_CMND_H

#include <linux/dma-mapping.h>
#include <linux/blkdev.h>
#include <linux/t10-pi.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/timer.h>
#include <linux/scatterlist.h>
#include <scsi/scsi_device.h>
#include <scsi/scsi_request.h>

struct Scsi_Host;
struct scsi_driver;


#define MAX_COMMAND_SIZE 16
#if (MAX_COMMAND_SIZE > BLK_MAX_CDB)
# error MAX_COMMAND_SIZE can not be bigger than BLK_MAX_CDB
#endif

struct scsi_data_buffer {
	struct sg_table table;
	unsigned length;
};


struct scsi_pointer {
	char *ptr;		
	int this_residual;	
	struct scatterlist *buffer;	
	int buffers_residual;	

        dma_addr_t dma_handle;

	volatile int Status;
	volatile int Message;
	volatile int have_data_in;
	volatile int sent_command;
	volatile int phase;
};


#define SCMD_TAGGED		(1 << 0)
#define SCMD_UNCHECKED_ISA_DMA	(1 << 1)
#define SCMD_INITIALIZED	(1 << 2)

#define SCMD_PRESERVED_FLAGS	(SCMD_UNCHECKED_ISA_DMA | SCMD_INITIALIZED)


#define SCMD_STATE_COMPLETE	0

struct scsi_cmnd {
	struct scsi_request req;
	struct scsi_device *device;
	struct list_head list;  
	struct list_head eh_entry; 
	struct delayed_work abort_work;

	struct rcu_head rcu;

	int eh_eflags;		

	
	unsigned long jiffies_at_alloc;

	int retries;
	int allowed;

	unsigned char prot_op;
	unsigned char prot_type;
	unsigned char prot_flags;

	unsigned short cmd_len;
	enum dma_data_direction sc_data_direction;

	
	unsigned char *cmnd;


	
	struct scsi_data_buffer sdb;
	struct scsi_data_buffer *prot_sdb;

	unsigned underflow;	

	unsigned transfersize;	

	struct request *request;	

	unsigned char *sense_buffer;
				

	
	void (*scsi_done) (struct scsi_cmnd *);

	
	struct scsi_pointer SCp;	

	unsigned char *host_scribble;	

	int result;		
	int flags;		
	unsigned long state;	

	unsigned char tag;	
};


static inline void *scsi_cmd_priv(struct scsi_cmnd *cmd)
{
	return cmd + 1;
}


static inline struct scsi_driver *scsi_cmd_to_driver(struct scsi_cmnd *cmd)
{
	return *(struct scsi_driver **)cmd->request->rq_disk->private_data;
}

extern void scsi_put_command(struct scsi_cmnd *);
extern void scsi_finish_command(struct scsi_cmnd *cmd);

extern void *scsi_kmap_atomic_sg(struct scatterlist *sg, int sg_count,
				 size_t *offset, size_t *len);
extern void scsi_kunmap_atomic_sg(void *virt);

extern blk_status_t scsi_init_io(struct scsi_cmnd *cmd);

#ifdef CONFIG_SCSI_DMA
extern int scsi_dma_map(struct scsi_cmnd *cmd);
extern void scsi_dma_unmap(struct scsi_cmnd *cmd);
#else 
static inline int scsi_dma_map(struct scsi_cmnd *cmd) { return -ENOSYS; }
static inline void scsi_dma_unmap(struct scsi_cmnd *cmd) { }
#endif 

static inline unsigned scsi_sg_count(struct scsi_cmnd *cmd)
{
	return cmd->sdb.table.nents;
}

static inline struct scatterlist *scsi_sglist(struct scsi_cmnd *cmd)
{
	return cmd->sdb.table.sgl;
}

static inline unsigned scsi_bufflen(struct scsi_cmnd *cmd)
{
	return cmd->sdb.length;
}

static inline void scsi_set_resid(struct scsi_cmnd *cmd, int resid)
{
	cmd->req.resid_len = resid;
}

static inline int scsi_get_resid(struct scsi_cmnd *cmd)
{
	return cmd->req.resid_len;
}

#define scsi_for_each_sg(cmd, sg, nseg, __i)			\
	for_each_sg(scsi_sglist(cmd), sg, nseg, __i)

static inline int scsi_sg_copy_from_buffer(struct scsi_cmnd *cmd,
					   void *buf, int buflen)
{
	return sg_copy_from_buffer(scsi_sglist(cmd), scsi_sg_count(cmd),
				   buf, buflen);
}

static inline int scsi_sg_copy_to_buffer(struct scsi_cmnd *cmd,
					 void *buf, int buflen)
{
	return sg_copy_to_buffer(scsi_sglist(cmd), scsi_sg_count(cmd),
				 buf, buflen);
}


enum scsi_prot_operations {
	
	SCSI_PROT_NORMAL = 0,

	
	SCSI_PROT_READ_INSERT,
	SCSI_PROT_WRITE_STRIP,

	
	SCSI_PROT_READ_STRIP,
	SCSI_PROT_WRITE_INSERT,

	
	SCSI_PROT_READ_PASS,
	SCSI_PROT_WRITE_PASS,
};

static inline void scsi_set_prot_op(struct scsi_cmnd *scmd, unsigned char op)
{
	scmd->prot_op = op;
}

static inline unsigned char scsi_get_prot_op(struct scsi_cmnd *scmd)
{
	return scmd->prot_op;
}

enum scsi_prot_flags {
	SCSI_PROT_TRANSFER_PI		= 1 << 0,
	SCSI_PROT_GUARD_CHECK		= 1 << 1,
	SCSI_PROT_REF_CHECK		= 1 << 2,
	SCSI_PROT_REF_INCREMENT		= 1 << 3,
	SCSI_PROT_IP_CHECKSUM		= 1 << 4,
};


enum scsi_prot_target_type {
	SCSI_PROT_DIF_TYPE0 = 0,
	SCSI_PROT_DIF_TYPE1,
	SCSI_PROT_DIF_TYPE2,
	SCSI_PROT_DIF_TYPE3,
};

static inline void scsi_set_prot_type(struct scsi_cmnd *scmd, unsigned char type)
{
	scmd->prot_type = type;
}

static inline unsigned char scsi_get_prot_type(struct scsi_cmnd *scmd)
{
	return scmd->prot_type;
}

static inline sector_t scsi_get_lba(struct scsi_cmnd *scmd)
{
	return blk_rq_pos(scmd->request);
}

static inline unsigned int scsi_prot_interval(struct scsi_cmnd *scmd)
{
	return scmd->device->sector_size;
}

static inline unsigned scsi_prot_sg_count(struct scsi_cmnd *cmd)
{
	return cmd->prot_sdb ? cmd->prot_sdb->table.nents : 0;
}

static inline struct scatterlist *scsi_prot_sglist(struct scsi_cmnd *cmd)
{
	return cmd->prot_sdb ? cmd->prot_sdb->table.sgl : NULL;
}

static inline struct scsi_data_buffer *scsi_prot(struct scsi_cmnd *cmd)
{
	return cmd->prot_sdb;
}

#define scsi_for_each_prot_sg(cmd, sg, nseg, __i)		\
	for_each_sg(scsi_prot_sglist(cmd), sg, nseg, __i)

static inline void set_msg_byte(struct scsi_cmnd *cmd, char status)
{
	cmd->result = (cmd->result & 0xffff00ff) | (status << 8);
}

static inline void set_host_byte(struct scsi_cmnd *cmd, char status)
{
	cmd->result = (cmd->result & 0xff00ffff) | (status << 16);
}

static inline void set_driver_byte(struct scsi_cmnd *cmd, char status)
{
	cmd->result = (cmd->result & 0x00ffffff) | (status << 24);
}

static inline unsigned scsi_transfer_length(struct scsi_cmnd *scmd)
{
	unsigned int xfer_len = scmd->sdb.length;
	unsigned int prot_interval = scsi_prot_interval(scmd);

	if (scmd->prot_flags & SCSI_PROT_TRANSFER_PI)
		xfer_len += (xfer_len >> ilog2(prot_interval)) * 8;

	return xfer_len;
}

#endif 
