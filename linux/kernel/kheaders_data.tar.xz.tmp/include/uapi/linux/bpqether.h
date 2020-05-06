/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef	__BPQETHER_H
#define	__BPQETHER_H



#include <linux/if_ether.h>

#define SIOCSBPQETHOPT		(SIOCDEVPRIVATE+0)	
#define SIOCSBPQETHADDR		(SIOCDEVPRIVATE+1)
 
struct bpq_ethaddr {
	unsigned char destination[ETH_ALEN];
	unsigned char accept[ETH_ALEN];
};



#define SIOCGBPQETHPARAM	0x5000  
#define SIOCSBPQETHPARAM	0x5001  

struct bpq_req  {
    int cmd;
    int speed;			
    int clockmode;		
    int txdelay;
    unsigned char persist;	
    int slotime;		
    int squeldelay;
    int dmachan;		
    int irq;			
};

#endif
