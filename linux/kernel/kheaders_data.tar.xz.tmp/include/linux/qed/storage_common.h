

#ifndef __STORAGE_COMMON__
#define __STORAGE_COMMON__





#define SCSI_MAX_NUM_OF_CMDQS		(NUM_OF_GLOBAL_QUEUES / 2)
#define BDQ_NUM_RESOURCES		(4)

#define BDQ_ID_RQ			(0)
#define BDQ_ID_IMM_DATA			(1)
#define BDQ_ID_TQ			(2)
#define BDQ_NUM_IDS			(3)

#define SCSI_NUM_SGES_SLOW_SGL_THR	8

#define BDQ_MAX_EXTERNAL_RING_SIZE	BIT(15)


#define SCSI_OPCODE_COMPARE_AND_WRITE	(0x89)
#define SCSI_OPCODE_READ_10		(0x28)
#define SCSI_OPCODE_WRITE_6		(0x0A)
#define SCSI_OPCODE_WRITE_10		(0x2A)
#define SCSI_OPCODE_WRITE_12		(0xAA)
#define SCSI_OPCODE_WRITE_16		(0x8A)
#define SCSI_OPCODE_WRITE_AND_VERIFY_10	(0x2E)
#define SCSI_OPCODE_WRITE_AND_VERIFY_12	(0xAE)
#define SCSI_OPCODE_WRITE_AND_VERIFY_16	(0x8E)


struct iscsi_drv_opaque {
	__le16 reserved_zero[3];
	__le16 opaque;
};


union scsi_opaque {
	struct regpair fcoe_opaque;
	struct iscsi_drv_opaque iscsi_opaque;
};


struct scsi_bd {
	struct regpair address;
	union scsi_opaque opaque;
};


struct scsi_bdq_ram_drv_data {
	__le16 external_producer;
	__le16 reserved0[3];
};


struct scsi_sge {
	struct regpair sge_addr;
	__le32 sge_len;
	__le32 reserved;
};


struct scsi_cached_sges {
	struct scsi_sge sge[4];
};


struct scsi_drv_cmdq {
	__le16 cmdq_cons;
	__le16 reserved0;
	__le32 reserved1;
};


struct scsi_init_func_params {
	__le16 num_tasks;
	u8 log_page_size;
	u8 debug_mode;
	u8 reserved2[12];
};


struct scsi_init_func_queues {
	struct regpair glbl_q_params_addr;
	__le16 rq_buffer_size;
	__le16 cq_num_entries;
	__le16 cmdq_num_entries;
	u8 bdq_resource_id;
	u8 q_validity;
#define SCSI_INIT_FUNC_QUEUES_RQ_VALID_MASK			0x1
#define SCSI_INIT_FUNC_QUEUES_RQ_VALID_SHIFT			0
#define SCSI_INIT_FUNC_QUEUES_IMM_DATA_VALID_MASK		0x1
#define SCSI_INIT_FUNC_QUEUES_IMM_DATA_VALID_SHIFT		1
#define SCSI_INIT_FUNC_QUEUES_CMD_VALID_MASK			0x1
#define SCSI_INIT_FUNC_QUEUES_CMD_VALID_SHIFT			2
#define SCSI_INIT_FUNC_QUEUES_TQ_VALID_MASK			0x1
#define SCSI_INIT_FUNC_QUEUES_TQ_VALID_SHIFT			3
#define SCSI_INIT_FUNC_QUEUES_SOC_EN_MASK			0x1
#define SCSI_INIT_FUNC_QUEUES_SOC_EN_SHIFT			4
#define SCSI_INIT_FUNC_QUEUES_SOC_NUM_OF_BLOCKS_LOG_MASK	0x7
#define SCSI_INIT_FUNC_QUEUES_SOC_NUM_OF_BLOCKS_LOG_SHIFT	5
	__le16 cq_cmdq_sb_num_arr[SCSI_MAX_NUM_OF_CMDQS];
	u8 num_queues;
	u8 queue_relative_offset;
	u8 cq_sb_pi;
	u8 cmdq_sb_pi;
	u8 bdq_pbl_num_entries[BDQ_NUM_IDS];
	u8 reserved1;
	struct regpair bdq_pbl_base_address[BDQ_NUM_IDS];
	__le16 bdq_xoff_threshold[BDQ_NUM_IDS];
	__le16 cmdq_xoff_threshold;
	__le16 bdq_xon_threshold[BDQ_NUM_IDS];
	__le16 cmdq_xon_threshold;
};


struct scsi_ram_per_bdq_resource_drv_data {
	struct scsi_bdq_ram_drv_data drv_data_per_bdq_id[BDQ_NUM_IDS];
};


enum scsi_sgl_mode {
	SCSI_TX_SLOW_SGL,
	SCSI_FAST_SGL,
	MAX_SCSI_SGL_MODE
};


struct scsi_sgl_params {
	struct regpair sgl_addr;
	__le32 sgl_total_length;
	__le32 sge_offset;
	__le16 sgl_num_sges;
	u8 sgl_index;
	u8 reserved;
};


struct scsi_terminate_extra_params {
	__le16 unsolicited_cq_count;
	__le16 cmdq_count;
	u8 reserved[4];
};


struct scsi_tqe {
	__le16 itid;
};

#endif 
