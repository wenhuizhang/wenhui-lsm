/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __LINUX_USB_CH9_H
#define __LINUX_USB_CH9_H

#include <linux/device.h>
#include <uapi/linux/usb/ch9.h>


extern const char *usb_ep_type_string(int ep_type);


extern const char *usb_speed_string(enum usb_device_speed speed);


extern enum usb_device_speed usb_get_maximum_speed(struct device *dev);


extern const char *usb_state_string(enum usb_device_state state);

#endif 
