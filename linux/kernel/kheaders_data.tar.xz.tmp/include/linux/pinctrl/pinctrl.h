/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __LINUX_PINCTRL_PINCTRL_H
#define __LINUX_PINCTRL_PINCTRL_H

#include <linux/radix-tree.h>
#include <linux/list.h>
#include <linux/seq_file.h>
#include <linux/pinctrl/pinctrl-state.h>
#include <linux/pinctrl/devinfo.h>

struct device;
struct pinctrl_dev;
struct pinctrl_map;
struct pinmux_ops;
struct pinconf_ops;
struct pin_config_item;
struct gpio_chip;
struct device_node;


struct pinctrl_pin_desc {
	unsigned number;
	const char *name;
	void *drv_data;
};


#define PINCTRL_PIN(a, b) { .number = a, .name = b }
#define PINCTRL_PIN_ANON(a) { .number = a }


struct pinctrl_gpio_range {
	struct list_head node;
	const char *name;
	unsigned int id;
	unsigned int base;
	unsigned int pin_base;
	unsigned const *pins;
	unsigned int npins;
	struct gpio_chip *gc;
};


struct pinctrl_ops {
	int (*get_groups_count) (struct pinctrl_dev *pctldev);
	const char *(*get_group_name) (struct pinctrl_dev *pctldev,
				       unsigned selector);
	int (*get_group_pins) (struct pinctrl_dev *pctldev,
			       unsigned selector,
			       const unsigned **pins,
			       unsigned *num_pins);
	void (*pin_dbg_show) (struct pinctrl_dev *pctldev, struct seq_file *s,
			  unsigned offset);
	int (*dt_node_to_map) (struct pinctrl_dev *pctldev,
			       struct device_node *np_config,
			       struct pinctrl_map **map, unsigned *num_maps);
	void (*dt_free_map) (struct pinctrl_dev *pctldev,
			     struct pinctrl_map *map, unsigned num_maps);
};


struct pinctrl_desc {
	const char *name;
	const struct pinctrl_pin_desc *pins;
	unsigned int npins;
	const struct pinctrl_ops *pctlops;
	const struct pinmux_ops *pmxops;
	const struct pinconf_ops *confops;
	struct module *owner;
#ifdef CONFIG_GENERIC_PINCONF
	unsigned int num_custom_params;
	const struct pinconf_generic_params *custom_params;
	const struct pin_config_item *custom_conf_items;
#endif
	bool link_consumers;
};



extern int pinctrl_register_and_init(struct pinctrl_desc *pctldesc,
				     struct device *dev, void *driver_data,
				     struct pinctrl_dev **pctldev);
extern int pinctrl_enable(struct pinctrl_dev *pctldev);


extern struct pinctrl_dev *pinctrl_register(struct pinctrl_desc *pctldesc,
				struct device *dev, void *driver_data);

extern void pinctrl_unregister(struct pinctrl_dev *pctldev);

extern int devm_pinctrl_register_and_init(struct device *dev,
				struct pinctrl_desc *pctldesc,
				void *driver_data,
				struct pinctrl_dev **pctldev);


extern struct pinctrl_dev *devm_pinctrl_register(struct device *dev,
				struct pinctrl_desc *pctldesc,
				void *driver_data);

extern void devm_pinctrl_unregister(struct device *dev,
				struct pinctrl_dev *pctldev);

extern void pinctrl_add_gpio_range(struct pinctrl_dev *pctldev,
				struct pinctrl_gpio_range *range);
extern void pinctrl_add_gpio_ranges(struct pinctrl_dev *pctldev,
				struct pinctrl_gpio_range *ranges,
				unsigned nranges);
extern void pinctrl_remove_gpio_range(struct pinctrl_dev *pctldev,
				struct pinctrl_gpio_range *range);

extern struct pinctrl_dev *pinctrl_find_and_add_gpio_range(const char *devname,
		struct pinctrl_gpio_range *range);
extern struct pinctrl_gpio_range *
pinctrl_find_gpio_range_from_pin(struct pinctrl_dev *pctldev,
				 unsigned int pin);
extern int pinctrl_get_group_pins(struct pinctrl_dev *pctldev,
				const char *pin_group, const unsigned **pins,
				unsigned *num_pins);

#ifdef CONFIG_OF
extern struct pinctrl_dev *of_pinctrl_get(struct device_node *np);
#else
static inline
struct pinctrl_dev *of_pinctrl_get(struct device_node *np)
{
	return NULL;
}
#endif 

extern const char *pinctrl_dev_get_name(struct pinctrl_dev *pctldev);
extern const char *pinctrl_dev_get_devname(struct pinctrl_dev *pctldev);
extern void *pinctrl_dev_get_drvdata(struct pinctrl_dev *pctldev);

#endif 
