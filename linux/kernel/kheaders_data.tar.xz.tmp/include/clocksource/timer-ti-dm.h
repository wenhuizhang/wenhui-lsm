

#include <linux/delay.h>
#include <linux/io.h>
#include <linux/platform_device.h>

#ifndef __CLOCKSOURCE_DMTIMER_H
#define __CLOCKSOURCE_DMTIMER_H


#define OMAP_TIMER_SRC_SYS_CLK			0x00
#define OMAP_TIMER_SRC_32_KHZ			0x01
#define OMAP_TIMER_SRC_EXT_CLK			0x02


#define OMAP_TIMER_INT_CAPTURE			(1 << 2)
#define OMAP_TIMER_INT_OVERFLOW			(1 << 1)
#define OMAP_TIMER_INT_MATCH			(1 << 0)


#define OMAP_TIMER_TRIGGER_NONE			0x00
#define OMAP_TIMER_TRIGGER_OVERFLOW		0x01
#define OMAP_TIMER_TRIGGER_OVERFLOW_AND_COMPARE	0x02


#define OMAP_TIMER_NONPOSTED			0x00
#define OMAP_TIMER_POSTED			0x01


#define OMAP_TIMER_SECURE				0x80000000
#define OMAP_TIMER_ALWON				0x40000000
#define OMAP_TIMER_HAS_PWM				0x20000000
#define OMAP_TIMER_NEEDS_RESET				0x10000000
#define OMAP_TIMER_HAS_DSP_IRQ				0x08000000


#define OMAP_TIMER_ERRATA_I103_I767			0x80000000

struct timer_regs {
	u32 tidr;
	u32 tier;
	u32 twer;
	u32 tclr;
	u32 tcrr;
	u32 tldr;
	u32 ttrg;
	u32 twps;
	u32 tmar;
	u32 tcar1;
	u32 tsicr;
	u32 tcar2;
	u32 tpir;
	u32 tnir;
	u32 tcvr;
	u32 tocr;
	u32 towr;
};

struct omap_dm_timer {
	int id;
	int irq;
	struct clk *fclk;

	void __iomem	*io_base;
	void __iomem	*irq_stat;	
	void __iomem	*irq_ena;	
	void __iomem	*irq_dis;	
	void __iomem	*pend;		
	void __iomem	*func_base;	

	unsigned long rate;
	unsigned reserved:1;
	unsigned posted:1;
	struct timer_regs context;
	int (*get_context_loss_count)(struct device *);
	int ctx_loss_count;
	int revision;
	u32 capability;
	u32 errata;
	struct platform_device *pdev;
	struct list_head node;
};

int omap_dm_timer_reserve_systimer(int id);
struct omap_dm_timer *omap_dm_timer_request_by_cap(u32 cap);

int omap_dm_timer_get_irq(struct omap_dm_timer *timer);

u32 omap_dm_timer_modify_idlect_mask(u32 inputmask);

int omap_dm_timer_trigger(struct omap_dm_timer *timer);

int omap_dm_timers_active(void);




#define OMAP_TIMER_ID_OFFSET		0x00
#define OMAP_TIMER_OCP_CFG_OFFSET	0x10

#define OMAP_TIMER_V1_SYS_STAT_OFFSET	0x14
#define OMAP_TIMER_V1_STAT_OFFSET	0x18
#define OMAP_TIMER_V1_INT_EN_OFFSET	0x1c

#define OMAP_TIMER_V2_IRQSTATUS_RAW	0x24
#define OMAP_TIMER_V2_IRQSTATUS		0x28
#define OMAP_TIMER_V2_IRQENABLE_SET	0x2c
#define OMAP_TIMER_V2_IRQENABLE_CLR	0x30


#define OMAP_TIMER_V2_FUNC_OFFSET		0x14

#define _OMAP_TIMER_WAKEUP_EN_OFFSET	0x20
#define _OMAP_TIMER_CTRL_OFFSET		0x24
#define		OMAP_TIMER_CTRL_GPOCFG		(1 << 14)
#define		OMAP_TIMER_CTRL_CAPTMODE	(1 << 13)
#define		OMAP_TIMER_CTRL_PT		(1 << 12)
#define		OMAP_TIMER_CTRL_TCM_LOWTOHIGH	(0x1 << 8)
#define		OMAP_TIMER_CTRL_TCM_HIGHTOLOW	(0x2 << 8)
#define		OMAP_TIMER_CTRL_TCM_BOTHEDGES	(0x3 << 8)
#define		OMAP_TIMER_CTRL_SCPWM		(1 << 7)
#define		OMAP_TIMER_CTRL_CE		(1 << 6) 
#define		OMAP_TIMER_CTRL_PRE		(1 << 5) 
#define		OMAP_TIMER_CTRL_PTV_SHIFT	2 
#define		OMAP_TIMER_CTRL_POSTED		(1 << 2)
#define		OMAP_TIMER_CTRL_AR		(1 << 1) 
#define		OMAP_TIMER_CTRL_ST		(1 << 0) 
#define _OMAP_TIMER_COUNTER_OFFSET	0x28
#define _OMAP_TIMER_LOAD_OFFSET		0x2c
#define _OMAP_TIMER_TRIGGER_OFFSET	0x30
#define _OMAP_TIMER_WRITE_PEND_OFFSET	0x34
#define		WP_NONE			0	
#define		WP_TCLR			(1 << 0)
#define		WP_TCRR			(1 << 1)
#define		WP_TLDR			(1 << 2)
#define		WP_TTGR			(1 << 3)
#define		WP_TMAR			(1 << 4)
#define		WP_TPIR			(1 << 5)
#define		WP_TNIR			(1 << 6)
#define		WP_TCVR			(1 << 7)
#define		WP_TOCR			(1 << 8)
#define		WP_TOWR			(1 << 9)
#define _OMAP_TIMER_MATCH_OFFSET	0x38
#define _OMAP_TIMER_CAPTURE_OFFSET	0x3c
#define _OMAP_TIMER_IF_CTRL_OFFSET	0x40
#define _OMAP_TIMER_CAPTURE2_OFFSET		0x44	
#define _OMAP_TIMER_TICK_POS_OFFSET		0x48	
#define _OMAP_TIMER_TICK_NEG_OFFSET		0x4c	
#define _OMAP_TIMER_TICK_COUNT_OFFSET		0x50	
#define _OMAP_TIMER_TICK_INT_MASK_SET_OFFSET	0x54	
#define _OMAP_TIMER_TICK_INT_MASK_COUNT_OFFSET	0x58	


#define	WPSHIFT					16

#define OMAP_TIMER_WAKEUP_EN_REG		(_OMAP_TIMER_WAKEUP_EN_OFFSET \
							| (WP_NONE << WPSHIFT))

#define OMAP_TIMER_CTRL_REG			(_OMAP_TIMER_CTRL_OFFSET \
							| (WP_TCLR << WPSHIFT))

#define OMAP_TIMER_COUNTER_REG			(_OMAP_TIMER_COUNTER_OFFSET \
							| (WP_TCRR << WPSHIFT))

#define OMAP_TIMER_LOAD_REG			(_OMAP_TIMER_LOAD_OFFSET \
							| (WP_TLDR << WPSHIFT))

#define OMAP_TIMER_TRIGGER_REG			(_OMAP_TIMER_TRIGGER_OFFSET \
							| (WP_TTGR << WPSHIFT))

#define OMAP_TIMER_WRITE_PEND_REG		(_OMAP_TIMER_WRITE_PEND_OFFSET \
							| (WP_NONE << WPSHIFT))

#define OMAP_TIMER_MATCH_REG			(_OMAP_TIMER_MATCH_OFFSET \
							| (WP_TMAR << WPSHIFT))

#define OMAP_TIMER_CAPTURE_REG			(_OMAP_TIMER_CAPTURE_OFFSET \
							| (WP_NONE << WPSHIFT))

#define OMAP_TIMER_IF_CTRL_REG			(_OMAP_TIMER_IF_CTRL_OFFSET \
							| (WP_NONE << WPSHIFT))

#define OMAP_TIMER_CAPTURE2_REG			(_OMAP_TIMER_CAPTURE2_OFFSET \
							| (WP_NONE << WPSHIFT))

#define OMAP_TIMER_TICK_POS_REG			(_OMAP_TIMER_TICK_POS_OFFSET \
							| (WP_TPIR << WPSHIFT))

#define OMAP_TIMER_TICK_NEG_REG			(_OMAP_TIMER_TICK_NEG_OFFSET \
							| (WP_TNIR << WPSHIFT))

#define OMAP_TIMER_TICK_COUNT_REG		(_OMAP_TIMER_TICK_COUNT_OFFSET \
							| (WP_TCVR << WPSHIFT))

#define OMAP_TIMER_TICK_INT_MASK_SET_REG				\
		(_OMAP_TIMER_TICK_INT_MASK_SET_OFFSET | (WP_TOCR << WPSHIFT))

#define OMAP_TIMER_TICK_INT_MASK_COUNT_REG				\
		(_OMAP_TIMER_TICK_INT_MASK_COUNT_OFFSET | (WP_TOWR << WPSHIFT))


#if defined(CONFIG_ARCH_OMAP1) || defined(CONFIG_ARCH_OMAP2PLUS)
static inline u32 __omap_dm_timer_read(struct omap_dm_timer *timer, u32 reg,
						int posted)
{
	if (posted)
		while (readl_relaxed(timer->pend) & (reg >> WPSHIFT))
			cpu_relax();

	return readl_relaxed(timer->func_base + (reg & 0xff));
}

static inline void __omap_dm_timer_write(struct omap_dm_timer *timer,
					u32 reg, u32 val, int posted)
{
	if (posted)
		while (readl_relaxed(timer->pend) & (reg >> WPSHIFT))
			cpu_relax();

	writel_relaxed(val, timer->func_base + (reg & 0xff));
}

static inline void __omap_dm_timer_init_regs(struct omap_dm_timer *timer)
{
	u32 tidr;

	
	tidr = readl_relaxed(timer->io_base);
	if (!(tidr >> 16)) {
		timer->revision = 1;
		timer->irq_stat = timer->io_base + OMAP_TIMER_V1_STAT_OFFSET;
		timer->irq_ena = timer->io_base + OMAP_TIMER_V1_INT_EN_OFFSET;
		timer->irq_dis = timer->io_base + OMAP_TIMER_V1_INT_EN_OFFSET;
		timer->pend = timer->io_base + _OMAP_TIMER_WRITE_PEND_OFFSET;
		timer->func_base = timer->io_base;
	} else {
		timer->revision = 2;
		timer->irq_stat = timer->io_base + OMAP_TIMER_V2_IRQSTATUS;
		timer->irq_ena = timer->io_base + OMAP_TIMER_V2_IRQENABLE_SET;
		timer->irq_dis = timer->io_base + OMAP_TIMER_V2_IRQENABLE_CLR;
		timer->pend = timer->io_base +
			_OMAP_TIMER_WRITE_PEND_OFFSET +
				OMAP_TIMER_V2_FUNC_OFFSET;
		timer->func_base = timer->io_base + OMAP_TIMER_V2_FUNC_OFFSET;
	}
}


static inline void __omap_dm_timer_enable_posted(struct omap_dm_timer *timer)
{
	if (timer->posted)
		return;

	if (timer->errata & OMAP_TIMER_ERRATA_I103_I767) {
		timer->posted = OMAP_TIMER_NONPOSTED;
		__omap_dm_timer_write(timer, OMAP_TIMER_IF_CTRL_REG, 0, 0);
		return;
	}

	__omap_dm_timer_write(timer, OMAP_TIMER_IF_CTRL_REG,
			      OMAP_TIMER_CTRL_POSTED, 0);
	timer->context.tsicr = OMAP_TIMER_CTRL_POSTED;
	timer->posted = OMAP_TIMER_POSTED;
}


static inline void __omap_dm_timer_override_errata(struct omap_dm_timer *timer,
						   u32 errata)
{
	timer->errata &= ~errata;
}

static inline void __omap_dm_timer_stop(struct omap_dm_timer *timer,
					int posted, unsigned long rate)
{
	u32 l;

	l = __omap_dm_timer_read(timer, OMAP_TIMER_CTRL_REG, posted);
	if (l & OMAP_TIMER_CTRL_ST) {
		l &= ~0x1;
		__omap_dm_timer_write(timer, OMAP_TIMER_CTRL_REG, l, posted);
#ifdef CONFIG_ARCH_OMAP2PLUS
		
		__omap_dm_timer_read(timer, OMAP_TIMER_CTRL_REG, posted);
		
		udelay(3500000 / rate + 1);
#endif
	}

	
	writel_relaxed(OMAP_TIMER_INT_OVERFLOW, timer->irq_stat);
}

static inline void __omap_dm_timer_load_start(struct omap_dm_timer *timer,
						u32 ctrl, unsigned int load,
						int posted)
{
	__omap_dm_timer_write(timer, OMAP_TIMER_COUNTER_REG, load, posted);
	__omap_dm_timer_write(timer, OMAP_TIMER_CTRL_REG, ctrl, posted);
}

static inline void __omap_dm_timer_int_enable(struct omap_dm_timer *timer,
						unsigned int value)
{
	writel_relaxed(value, timer->irq_ena);
	__omap_dm_timer_write(timer, OMAP_TIMER_WAKEUP_EN_REG, value, 0);
}

static inline unsigned int
__omap_dm_timer_read_counter(struct omap_dm_timer *timer, int posted)
{
	return __omap_dm_timer_read(timer, OMAP_TIMER_COUNTER_REG, posted);
}

static inline void __omap_dm_timer_write_status(struct omap_dm_timer *timer,
						unsigned int value)
{
	writel_relaxed(value, timer->irq_stat);
}
#endif 
#endif 
