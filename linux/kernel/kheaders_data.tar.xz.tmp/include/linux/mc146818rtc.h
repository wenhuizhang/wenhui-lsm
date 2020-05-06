


#ifndef _MC146818RTC_H
#define _MC146818RTC_H

#include <asm/io.h>
#include <linux/rtc.h>			
#include <asm/mc146818rtc.h>		
#include <linux/bcd.h>
#include <linux/delay.h>
#include <linux/pm-trace.h>

#ifdef __KERNEL__
#include <linux/spinlock.h>		
extern spinlock_t rtc_lock;		


struct cmos_rtc_board_info {
	void	(*wake_on)(struct device *dev);
	void	(*wake_off)(struct device *dev);

	u32	flags;
#define CMOS_RTC_FLAGS_NOFREQ	(1 << 0)
	int	address_space;

	u8	rtc_day_alarm;		
	u8	rtc_mon_alarm;		
	u8	rtc_century;		
};
#endif


#define RTC_SECONDS		0
#define RTC_SECONDS_ALARM	1
#define RTC_MINUTES		2
#define RTC_MINUTES_ALARM	3
#define RTC_HOURS		4
#define RTC_HOURS_ALARM		5

# define RTC_ALARM_DONT_CARE 	0xC0

#define RTC_DAY_OF_WEEK		6
#define RTC_DAY_OF_MONTH	7
#define RTC_MONTH		8
#define RTC_YEAR		9


#define RTC_REG_A		10
#define RTC_REG_B		11
#define RTC_REG_C		12
#define RTC_REG_D		13


#define RTC_FREQ_SELECT	RTC_REG_A


# define RTC_UIP		0x80
# define RTC_DIV_CTL		0x70
   
#  define RTC_REF_CLCK_4MHZ	0x00
#  define RTC_REF_CLCK_1MHZ	0x10
#  define RTC_REF_CLCK_32KHZ	0x20
   
#  define RTC_DIV_RESET1	0x60
#  define RTC_DIV_RESET2	0x70
  
# define RTC_RATE_SELECT 	0x0F


#define RTC_CONTROL	RTC_REG_B
# define RTC_SET 0x80		
# define RTC_PIE 0x40		
# define RTC_AIE 0x20		
# define RTC_UIE 0x10		
# define RTC_SQWE 0x08		
# define RTC_DM_BINARY 0x04	
# define RTC_24H 0x02		
# define RTC_DST_EN 0x01	


#define RTC_INTR_FLAGS	RTC_REG_C

# define RTC_IRQF 0x80		
# define RTC_PF 0x40
# define RTC_AF 0x20
# define RTC_UF 0x10


#define RTC_VALID	RTC_REG_D
# define RTC_VRT 0x80		


#ifndef ARCH_RTC_LOCATION	

#define RTC_IO_EXTENT	0x8
#define RTC_IO_EXTENT_USED	0x2
#define RTC_IOMAPPED	1	

#else
#define RTC_IO_EXTENT_USED      RTC_IO_EXTENT
#endif 

unsigned int mc146818_get_time(struct rtc_time *time);
int mc146818_set_time(struct rtc_time *time);

#endif 
