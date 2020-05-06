

#ifndef LINUX_MMC_MMC_H
#define LINUX_MMC_MMC_H

#include <linux/types.h>


   
#define MMC_GO_IDLE_STATE         0   
#define MMC_SEND_OP_COND          1   
#define MMC_ALL_SEND_CID          2   
#define MMC_SET_RELATIVE_ADDR     3   
#define MMC_SET_DSR               4   
#define MMC_SLEEP_AWAKE		  5   
#define MMC_SWITCH                6   
#define MMC_SELECT_CARD           7   
#define MMC_SEND_EXT_CSD          8   
#define MMC_SEND_CSD              9   
#define MMC_SEND_CID             10   
#define MMC_READ_DAT_UNTIL_STOP  11   
#define MMC_STOP_TRANSMISSION    12   
#define MMC_SEND_STATUS          13   
#define MMC_BUS_TEST_R           14   
#define MMC_GO_INACTIVE_STATE    15   
#define MMC_BUS_TEST_W           19   
#define MMC_SPI_READ_OCR         58   
#define MMC_SPI_CRC_ON_OFF       59   

  
#define MMC_SET_BLOCKLEN         16   
#define MMC_READ_SINGLE_BLOCK    17   
#define MMC_READ_MULTIPLE_BLOCK  18   
#define MMC_SEND_TUNING_BLOCK    19   
#define MMC_SEND_TUNING_BLOCK_HS200	21	

  
#define MMC_WRITE_DAT_UNTIL_STOP 20   

  
#define MMC_SET_BLOCK_COUNT      23   
#define MMC_WRITE_BLOCK          24   
#define MMC_WRITE_MULTIPLE_BLOCK 25   
#define MMC_PROGRAM_CID          26   
#define MMC_PROGRAM_CSD          27   

  
#define MMC_SET_WRITE_PROT       28   
#define MMC_CLR_WRITE_PROT       29   
#define MMC_SEND_WRITE_PROT      30   

  
#define MMC_ERASE_GROUP_START    35   
#define MMC_ERASE_GROUP_END      36   
#define MMC_ERASE                38   

  
#define MMC_FAST_IO              39   
#define MMC_GO_IRQ_STATE         40   

  
#define MMC_LOCK_UNLOCK          42   

  
#define MMC_APP_CMD              55   
#define MMC_GEN_CMD              56   

  
#define MMC_QUE_TASK_PARAMS      44   
#define MMC_QUE_TASK_ADDR        45   
#define MMC_EXECUTE_READ_TASK    46   
#define MMC_EXECUTE_WRITE_TASK   47   
#define MMC_CMDQ_TASK_MGMT       48   

static inline bool mmc_op_multi(u32 opcode)
{
	return opcode == MMC_WRITE_MULTIPLE_BLOCK ||
	       opcode == MMC_READ_MULTIPLE_BLOCK;
}





#define R1_OUT_OF_RANGE		(1 << 31)	
#define R1_ADDRESS_ERROR	(1 << 30)	
#define R1_BLOCK_LEN_ERROR	(1 << 29)	
#define R1_ERASE_SEQ_ERROR      (1 << 28)	
#define R1_ERASE_PARAM		(1 << 27)	
#define R1_WP_VIOLATION		(1 << 26)	
#define R1_CARD_IS_LOCKED	(1 << 25)	
#define R1_LOCK_UNLOCK_FAILED	(1 << 24)	
#define R1_COM_CRC_ERROR	(1 << 23)	
#define R1_ILLEGAL_COMMAND	(1 << 22)	
#define R1_CARD_ECC_FAILED	(1 << 21)	
#define R1_CC_ERROR		(1 << 20)	
#define R1_ERROR		(1 << 19)	
#define R1_UNDERRUN		(1 << 18)	
#define R1_OVERRUN		(1 << 17)	
#define R1_CID_CSD_OVERWRITE	(1 << 16)	
#define R1_WP_ERASE_SKIP	(1 << 15)	
#define R1_CARD_ECC_DISABLED	(1 << 14)	
#define R1_ERASE_RESET		(1 << 13)	
#define R1_STATUS(x)            (x & 0xFFF9A000)
#define R1_CURRENT_STATE(x)	((x & 0x00001E00) >> 9)	
#define R1_READY_FOR_DATA	(1 << 8)	
#define R1_SWITCH_ERROR		(1 << 7)	
#define R1_EXCEPTION_EVENT	(1 << 6)	
#define R1_APP_CMD		(1 << 5)	

#define R1_STATE_IDLE	0
#define R1_STATE_READY	1
#define R1_STATE_IDENT	2
#define R1_STATE_STBY	3
#define R1_STATE_TRAN	4
#define R1_STATE_DATA	5
#define R1_STATE_RCV	6
#define R1_STATE_PRG	7
#define R1_STATE_DIS	8


#define R1_SPI_IDLE		(1 << 0)
#define R1_SPI_ERASE_RESET	(1 << 1)
#define R1_SPI_ILLEGAL_COMMAND	(1 << 2)
#define R1_SPI_COM_CRC		(1 << 3)
#define R1_SPI_ERASE_SEQ	(1 << 4)
#define R1_SPI_ADDRESS		(1 << 5)
#define R1_SPI_PARAMETER	(1 << 6)

#define R2_SPI_CARD_LOCKED	(1 << 8)
#define R2_SPI_WP_ERASE_SKIP	(1 << 9)	
#define R2_SPI_LOCK_UNLOCK_FAIL	R2_SPI_WP_ERASE_SKIP
#define R2_SPI_ERROR		(1 << 10)
#define R2_SPI_CC_ERROR		(1 << 11)
#define R2_SPI_CARD_ECC_ERROR	(1 << 12)
#define R2_SPI_WP_VIOLATION	(1 << 13)
#define R2_SPI_ERASE_PARAM	(1 << 14)
#define R2_SPI_OUT_OF_RANGE	(1 << 15)	
#define R2_SPI_CSD_OVERWRITE	R2_SPI_OUT_OF_RANGE


#define MMC_CARD_BUSY	0x80000000	


#define CCC_BASIC		(1<<0)	
					
					
#define CCC_STREAM_READ		(1<<1)	
					
#define CCC_BLOCK_READ		(1<<2)	
					
#define CCC_STREAM_WRITE	(1<<3)	
					
#define CCC_BLOCK_WRITE		(1<<4)	
					
#define CCC_ERASE		(1<<5)	
					
#define CCC_WRITE_PROT		(1<<6)	
					
#define CCC_LOCK_CARD		(1<<7)	
					
#define CCC_APP_SPEC		(1<<8)	
					
#define CCC_IO_MODE		(1<<9)	
					
#define CCC_SWITCH		(1<<10)	
					
					
					



#define CSD_STRUCT_VER_1_0  0           
#define CSD_STRUCT_VER_1_1  1           
#define CSD_STRUCT_VER_1_2  2           
#define CSD_STRUCT_EXT_CSD  3           

#define CSD_SPEC_VER_0      0           
#define CSD_SPEC_VER_1      1           
#define CSD_SPEC_VER_2      2           
#define CSD_SPEC_VER_3      3           
#define CSD_SPEC_VER_4      4           



#define EXT_CSD_CMDQ_MODE_EN		15	
#define EXT_CSD_FLUSH_CACHE		32      
#define EXT_CSD_CACHE_CTRL		33      
#define EXT_CSD_POWER_OFF_NOTIFICATION	34	
#define EXT_CSD_PACKED_FAILURE_INDEX	35	
#define EXT_CSD_PACKED_CMD_STATUS	36	
#define EXT_CSD_EXP_EVENTS_STATUS	54	
#define EXT_CSD_EXP_EVENTS_CTRL		56	
#define EXT_CSD_DATA_SECTOR_SIZE	61	
#define EXT_CSD_GP_SIZE_MULT		143	
#define EXT_CSD_PARTITION_SETTING_COMPLETED 155	
#define EXT_CSD_PARTITION_ATTRIBUTE	156	
#define EXT_CSD_PARTITION_SUPPORT	160	
#define EXT_CSD_HPI_MGMT		161	
#define EXT_CSD_RST_N_FUNCTION		162	
#define EXT_CSD_BKOPS_EN		163	
#define EXT_CSD_BKOPS_START		164	
#define EXT_CSD_SANITIZE_START		165     
#define EXT_CSD_WR_REL_PARAM		166	
#define EXT_CSD_RPMB_MULT		168	
#define EXT_CSD_FW_CONFIG		169	
#define EXT_CSD_BOOT_WP			173	
#define EXT_CSD_ERASE_GROUP_DEF		175	
#define EXT_CSD_PART_CONFIG		179	
#define EXT_CSD_ERASED_MEM_CONT		181	
#define EXT_CSD_BUS_WIDTH		183	
#define EXT_CSD_STROBE_SUPPORT		184	
#define EXT_CSD_HS_TIMING		185	
#define EXT_CSD_POWER_CLASS		187	
#define EXT_CSD_REV			192	
#define EXT_CSD_STRUCTURE		194	
#define EXT_CSD_CARD_TYPE		196	
#define EXT_CSD_DRIVER_STRENGTH		197	
#define EXT_CSD_OUT_OF_INTERRUPT_TIME	198	
#define EXT_CSD_PART_SWITCH_TIME        199     
#define EXT_CSD_PWR_CL_52_195		200	
#define EXT_CSD_PWR_CL_26_195		201	
#define EXT_CSD_PWR_CL_52_360		202	
#define EXT_CSD_PWR_CL_26_360		203	
#define EXT_CSD_SEC_CNT			212	
#define EXT_CSD_S_A_TIMEOUT		217	
#define EXT_CSD_REL_WR_SEC_C		222	
#define EXT_CSD_HC_WP_GRP_SIZE		221	
#define EXT_CSD_ERASE_TIMEOUT_MULT	223	
#define EXT_CSD_HC_ERASE_GRP_SIZE	224	
#define EXT_CSD_BOOT_MULT		226	
#define EXT_CSD_SEC_TRIM_MULT		229	
#define EXT_CSD_SEC_ERASE_MULT		230	
#define EXT_CSD_SEC_FEATURE_SUPPORT	231	
#define EXT_CSD_TRIM_MULT		232	
#define EXT_CSD_PWR_CL_200_195		236	
#define EXT_CSD_PWR_CL_200_360		237	
#define EXT_CSD_PWR_CL_DDR_52_195	238	
#define EXT_CSD_PWR_CL_DDR_52_360	239	
#define EXT_CSD_BKOPS_STATUS		246	
#define EXT_CSD_POWER_OFF_LONG_TIME	247	
#define EXT_CSD_GENERIC_CMD6_TIME	248	
#define EXT_CSD_CACHE_SIZE		249	
#define EXT_CSD_PWR_CL_DDR_200_360	253	
#define EXT_CSD_FIRMWARE_VERSION	254	
#define EXT_CSD_PRE_EOL_INFO		267	
#define EXT_CSD_DEVICE_LIFE_TIME_EST_TYP_A	268	
#define EXT_CSD_DEVICE_LIFE_TIME_EST_TYP_B	269	
#define EXT_CSD_CMDQ_DEPTH		307	
#define EXT_CSD_CMDQ_SUPPORT		308	
#define EXT_CSD_SUPPORTED_MODE		493	
#define EXT_CSD_TAG_UNIT_SIZE		498	
#define EXT_CSD_DATA_TAG_SUPPORT	499	
#define EXT_CSD_MAX_PACKED_WRITES	500	
#define EXT_CSD_MAX_PACKED_READS	501	
#define EXT_CSD_BKOPS_SUPPORT		502	
#define EXT_CSD_HPI_FEATURES		503	



#define EXT_CSD_WR_REL_PARAM_EN		(1<<2)

#define EXT_CSD_BOOT_WP_B_PWR_WP_DIS	(0x40)
#define EXT_CSD_BOOT_WP_B_PERM_WP_DIS	(0x10)
#define EXT_CSD_BOOT_WP_B_PERM_WP_EN	(0x04)
#define EXT_CSD_BOOT_WP_B_PWR_WP_EN	(0x01)

#define EXT_CSD_PART_CONFIG_ACC_MASK	(0x7)
#define EXT_CSD_PART_CONFIG_ACC_BOOT0	(0x1)
#define EXT_CSD_PART_CONFIG_ACC_RPMB	(0x3)
#define EXT_CSD_PART_CONFIG_ACC_GP0	(0x4)

#define EXT_CSD_PART_SETTING_COMPLETED	(0x1)
#define EXT_CSD_PART_SUPPORT_PART_EN	(0x1)

#define EXT_CSD_CMD_SET_NORMAL		(1<<0)
#define EXT_CSD_CMD_SET_SECURE		(1<<1)
#define EXT_CSD_CMD_SET_CPSECURE	(1<<2)

#define EXT_CSD_CARD_TYPE_HS_26	(1<<0)	
#define EXT_CSD_CARD_TYPE_HS_52	(1<<1)	
#define EXT_CSD_CARD_TYPE_HS	(EXT_CSD_CARD_TYPE_HS_26 | \
				 EXT_CSD_CARD_TYPE_HS_52)
#define EXT_CSD_CARD_TYPE_DDR_1_8V  (1<<2)   
					     
#define EXT_CSD_CARD_TYPE_DDR_1_2V  (1<<3)   
					     
#define EXT_CSD_CARD_TYPE_DDR_52       (EXT_CSD_CARD_TYPE_DDR_1_8V  \
					| EXT_CSD_CARD_TYPE_DDR_1_2V)
#define EXT_CSD_CARD_TYPE_HS200_1_8V	(1<<4)	
#define EXT_CSD_CARD_TYPE_HS200_1_2V	(1<<5)	
						
#define EXT_CSD_CARD_TYPE_HS200		(EXT_CSD_CARD_TYPE_HS200_1_8V | \
					 EXT_CSD_CARD_TYPE_HS200_1_2V)
#define EXT_CSD_CARD_TYPE_HS400_1_8V	(1<<6)	
#define EXT_CSD_CARD_TYPE_HS400_1_2V	(1<<7)	
#define EXT_CSD_CARD_TYPE_HS400		(EXT_CSD_CARD_TYPE_HS400_1_8V | \
					 EXT_CSD_CARD_TYPE_HS400_1_2V)
#define EXT_CSD_CARD_TYPE_HS400ES	(1<<8)	

#define EXT_CSD_BUS_WIDTH_1	0	
#define EXT_CSD_BUS_WIDTH_4	1	
#define EXT_CSD_BUS_WIDTH_8	2	
#define EXT_CSD_DDR_BUS_WIDTH_4	5	
#define EXT_CSD_DDR_BUS_WIDTH_8	6	
#define EXT_CSD_BUS_WIDTH_STROBE BIT(7)	

#define EXT_CSD_TIMING_BC	0	
#define EXT_CSD_TIMING_HS	1	
#define EXT_CSD_TIMING_HS200	2	
#define EXT_CSD_TIMING_HS400	3	
#define EXT_CSD_DRV_STR_SHIFT	4	

#define EXT_CSD_SEC_ER_EN	BIT(0)
#define EXT_CSD_SEC_BD_BLK_EN	BIT(2)
#define EXT_CSD_SEC_GB_CL_EN	BIT(4)
#define EXT_CSD_SEC_SANITIZE	BIT(6)  

#define EXT_CSD_RST_N_EN_MASK	0x3
#define EXT_CSD_RST_N_ENABLED	1	

#define EXT_CSD_NO_POWER_NOTIFICATION	0
#define EXT_CSD_POWER_ON		1
#define EXT_CSD_POWER_OFF_SHORT		2
#define EXT_CSD_POWER_OFF_LONG		3

#define EXT_CSD_PWR_CL_8BIT_MASK	0xF0	
#define EXT_CSD_PWR_CL_4BIT_MASK	0x0F	
#define EXT_CSD_PWR_CL_8BIT_SHIFT	4
#define EXT_CSD_PWR_CL_4BIT_SHIFT	0

#define EXT_CSD_PACKED_EVENT_EN	BIT(3)


#define EXT_CSD_URGENT_BKOPS		BIT(0)
#define EXT_CSD_DYNCAP_NEEDED		BIT(1)
#define EXT_CSD_SYSPOOL_EXHAUSTED	BIT(2)
#define EXT_CSD_PACKED_FAILURE		BIT(3)

#define EXT_CSD_PACKED_GENERIC_ERROR	BIT(0)
#define EXT_CSD_PACKED_INDEXED_ERROR	BIT(1)


#define EXT_CSD_BKOPS_LEVEL_2		0x2


#define EXT_CSD_MANUAL_BKOPS_MASK	0x01
#define EXT_CSD_AUTO_BKOPS_MASK		0x02


#define EXT_CSD_CMDQ_MODE_ENABLED	BIT(0)
#define EXT_CSD_CMDQ_DEPTH_MASK		GENMASK(4, 0)
#define EXT_CSD_CMDQ_SUPPORTED		BIT(0)


#define MMC_SWITCH_MODE_CMD_SET		0x00	
#define MMC_SWITCH_MODE_SET_BITS	0x01	
#define MMC_SWITCH_MODE_CLEAR_BITS	0x02	
#define MMC_SWITCH_MODE_WRITE_BYTE	0x03	


#define MMC_ERASE_ARG			0x00000000
#define MMC_SECURE_ERASE_ARG		0x80000000
#define MMC_TRIM_ARG			0x00000001
#define MMC_DISCARD_ARG			0x00000003
#define MMC_SECURE_TRIM1_ARG		0x80000001
#define MMC_SECURE_TRIM2_ARG		0x80008000
#define MMC_SECURE_ARGS			0x80000000
#define MMC_TRIM_ARGS			0x00008001

#define mmc_driver_type_mask(n)		(1 << (n))

#endif 
