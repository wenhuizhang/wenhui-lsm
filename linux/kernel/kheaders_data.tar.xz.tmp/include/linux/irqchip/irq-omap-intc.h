/* SPDX-License-Identifier: GPL-2.0-only */


#ifndef __INCLUDE_LINUX_IRQCHIP_IRQ_OMAP_INTC_H
#define __INCLUDE_LINUX_IRQCHIP_IRQ_OMAP_INTC_H

int omap_irq_pending(void);
void omap_intc_save_context(void);
void omap_intc_restore_context(void);
void omap3_intc_suspend(void);
void omap3_intc_prepare_idle(void);
void omap3_intc_resume_idle(void);

#endif 
