/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __SOUNDWIRE_TYPES_H
#define __SOUNDWIRE_TYPES_H

extern struct bus_type sdw_bus_type;

#define drv_to_sdw_driver(_drv) container_of(_drv, struct sdw_driver, driver)

#define sdw_register_driver(drv) \
	__sdw_register_driver(drv, THIS_MODULE)

int __sdw_register_driver(struct sdw_driver *drv, struct module *owner);
void sdw_unregister_driver(struct sdw_driver *drv);

int sdw_slave_modalias(const struct sdw_slave *slave, char *buf, size_t size);


#define module_sdw_driver(__sdw_driver) \
	module_driver(__sdw_driver, sdw_register_driver, \
			sdw_unregister_driver)
#endif 
