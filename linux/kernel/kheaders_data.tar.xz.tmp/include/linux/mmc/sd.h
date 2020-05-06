/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef LINUX_MMC_SD_H
#define LINUX_MMC_SD_H


  

#define SD_SEND_RELATIVE_ADDR     3   
#define SD_SEND_IF_COND           8   
#define SD_SWITCH_VOLTAGE         11  

  
#define SD_SWITCH                 6   

  
#define SD_ERASE_WR_BLK_START    32   
#define SD_ERASE_WR_BLK_END      33   

  
#define SD_APP_SET_BUS_WIDTH      6   
#define SD_APP_SD_STATUS         13   
#define SD_APP_SEND_NUM_WR_BLKS  22   
#define SD_APP_OP_COND           41   
#define SD_APP_SEND_SCR          51   


#define SD_OCR_S18R		(1 << 24)    
#define SD_ROCR_S18A		SD_OCR_S18R  
#define SD_OCR_XPC		(1 << 28)    
#define SD_OCR_CCS		(1 << 30)    







#define SCR_SPEC_VER_0		0	
#define SCR_SPEC_VER_1		1	
#define SCR_SPEC_VER_2		2	


#define SD_BUS_WIDTH_1		0
#define SD_BUS_WIDTH_4		2


#define SD_SWITCH_CHECK		0
#define SD_SWITCH_SET		1


#define SD_SWITCH_GRP_ACCESS	0


#define SD_SWITCH_ACCESS_DEF	0
#define SD_SWITCH_ACCESS_HS	1


#define SD_ERASE_ARG			0x00000000
#define SD_DISCARD_ARG			0x00000001

#endif 
