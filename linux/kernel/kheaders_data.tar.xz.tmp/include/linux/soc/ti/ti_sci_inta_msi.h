/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __INCLUDE_LINUX_TI_SCI_INTA_MSI_H
#define __INCLUDE_LINUX_TI_SCI_INTA_MSI_H

#include <linux/msi.h>
#include <linux/soc/ti/ti_sci_protocol.h>

struct irq_domain
*ti_sci_inta_msi_create_irq_domain(struct fwnode_handle *fwnode,
				   struct msi_domain_info *info,
				   struct irq_domain *parent);
int ti_sci_inta_msi_domain_alloc_irqs(struct device *dev,
				      struct ti_sci_resource *res);
unsigned int ti_sci_inta_msi_get_virq(struct device *dev, u32 index);
void ti_sci_inta_msi_domain_free_irqs(struct device *dev);
#endif 
