/* SPDX-License-Identifier: GPL-2.0 */


#include <linux/interrupt.h>	
#include <linux/sched/signal.h>
#include <linux/delay.h>
#include <linux/io-64-nonatomic-lo-hi.h>


#define DRM_CURRENTPID			task_pid_nr(current)
#define DRM_UDELAY(d)			udelay(d)

#define DRM_READ8(map, offset)		readb(((void __iomem *)(map)->handle) + (offset))

#define DRM_READ16(map, offset)         readw(((void __iomem *)(map)->handle) + (offset))

#define DRM_READ32(map, offset)		readl(((void __iomem *)(map)->handle) + (offset))

#define DRM_WRITE8(map, offset, val)	writeb(val, ((void __iomem *)(map)->handle) + (offset))

#define DRM_WRITE16(map, offset, val)   writew(val, ((void __iomem *)(map)->handle) + (offset))

#define DRM_WRITE32(map, offset, val)	writel(val, ((void __iomem *)(map)->handle) + (offset))


#define DRM_READ64(map, offset)		readq(((void __iomem *)(map)->handle) + (offset))

#define DRM_WRITE64(map, offset, val)	writeq(val, ((void __iomem *)(map)->handle) + (offset))

#define DRM_WAIT_ON( ret, queue, timeout, condition )		\
do {								\
	DECLARE_WAITQUEUE(entry, current);			\
	unsigned long end = jiffies + (timeout);		\
	add_wait_queue(&(queue), &entry);			\
								\
	for (;;) {						\
		__set_current_state(TASK_INTERRUPTIBLE);	\
		if (condition)					\
			break;					\
		if (time_after_eq(jiffies, end)) {		\
			ret = -EBUSY;				\
			break;					\
		}						\
		schedule_timeout((HZ/100 > 1) ? HZ/100 : 1);	\
		if (signal_pending(current)) {			\
			ret = -EINTR;				\
			break;					\
		}						\
	}							\
	__set_current_state(TASK_RUNNING);			\
	remove_wait_queue(&(queue), &entry);			\
} while (0)
