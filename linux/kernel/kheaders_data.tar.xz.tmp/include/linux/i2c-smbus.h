/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef _LINUX_I2C_SMBUS_H
#define _LINUX_I2C_SMBUS_H

#include <linux/i2c.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>



struct i2c_smbus_alert_setup {
	int			irq;
};

struct i2c_client *i2c_setup_smbus_alert(struct i2c_adapter *adapter,
					 struct i2c_smbus_alert_setup *setup);
int i2c_handle_smbus_alert(struct i2c_client *ara);

#if IS_ENABLED(CONFIG_I2C_SMBUS) && IS_ENABLED(CONFIG_OF)
int of_i2c_setup_smbus_alert(struct i2c_adapter *adap);
#else
static inline int of_i2c_setup_smbus_alert(struct i2c_adapter *adap)
{
	return 0;
}
#endif

#endif 
