/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef _INPUT_POLLDEV_H
#define _INPUT_POLLDEV_H



#include <linux/input.h>
#include <linux/workqueue.h>


struct input_polled_dev {
	void *private;

	void (*open)(struct input_polled_dev *dev);
	void (*close)(struct input_polled_dev *dev);
	void (*poll)(struct input_polled_dev *dev);
	unsigned int poll_interval; 
	unsigned int poll_interval_max; 
	unsigned int poll_interval_min; 

	struct input_dev *input;


	struct delayed_work work;

	bool devres_managed;
};

struct input_polled_dev *input_allocate_polled_device(void);
struct input_polled_dev *devm_input_allocate_polled_device(struct device *dev);
void input_free_polled_device(struct input_polled_dev *dev);
int input_register_polled_device(struct input_polled_dev *dev);
void input_unregister_polled_device(struct input_polled_dev *dev);

#endif
