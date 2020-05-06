/* SPDX-License-Identifier: GPL-2.0-only */


#ifndef ST_SENSORS_PDATA_H
#define ST_SENSORS_PDATA_H


struct st_sensors_platform_data {
	u8 drdy_int_pin;
	bool open_drain;
	bool spi_3wire;
	bool pullups;
};

#endif 
