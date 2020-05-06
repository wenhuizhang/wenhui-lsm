/* SPDX-License-Identifier: GPL-2.0-only */


#ifndef __LINUX_MTD_XIP_H__
#define __LINUX_MTD_XIP_H__


#ifdef CONFIG_MTD_XIP


#include <linux/compiler.h>


#ifdef CONFIG_XIP_KERNEL
#define __xipram noinline __attribute__ ((__section__ (".xiptext")))
#endif



#include <asm/mtd-xip.h>

#ifndef xip_irqpending

#warning "missing IRQ and timer primitives for XIP MTD support"
#warning "some of the XIP MTD support code will be disabled"
#warning "your system will therefore be unresponsive when writing or erasing flash"

#define xip_irqpending()	(0)
#define xip_currtime()		(0)
#define xip_elapsed_since(x)	(0)

#endif

#ifndef xip_iprefetch
#define xip_iprefetch()		do { } while (0)
#endif


#ifndef xip_cpu_idle
#define xip_cpu_idle()  do { } while (0)
#endif

#endif 

#ifndef __xipram
#define __xipram
#endif

#endif 
