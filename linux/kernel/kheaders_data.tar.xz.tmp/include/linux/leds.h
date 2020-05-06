/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __LINUX_LEDS_H_INCLUDED
#define __LINUX_LEDS_H_INCLUDED

#include <linux/device.h>
#include <linux/kernfs.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/rwsem.h>
#include <linux/spinlock.h>
#include <linux/timer.h>
#include <linux/workqueue.h>

struct device;
struct led_pattern;


enum led_brightness {
	LED_OFF		= 0,
	LED_ON		= 1,
	LED_HALF	= 127,
	LED_FULL	= 255,
};

struct led_classdev {
	const char		*name;
	enum led_brightness	 brightness;
	enum led_brightness	 max_brightness;
	int			 flags;

	
#define LED_SUSPENDED		BIT(0)
#define LED_UNREGISTERING	BIT(1)
	
#define LED_CORE_SUSPENDRESUME	BIT(16)
#define LED_SYSFS_DISABLE	BIT(17)
#define LED_DEV_CAP_FLASH	BIT(18)
#define LED_HW_PLUGGABLE	BIT(19)
#define LED_PANIC_INDICATOR	BIT(20)
#define LED_BRIGHT_HW_CHANGED	BIT(21)
#define LED_RETAIN_AT_SHUTDOWN	BIT(22)
#define LED_INIT_DEFAULT_TRIGGER BIT(23)

	
	unsigned long		work_flags;

#define LED_BLINK_SW			0
#define LED_BLINK_ONESHOT		1
#define LED_BLINK_ONESHOT_STOP		2
#define LED_BLINK_INVERT		3
#define LED_BLINK_BRIGHTNESS_CHANGE 	4
#define LED_BLINK_DISABLE		5

	
	void		(*brightness_set)(struct led_classdev *led_cdev,
					  enum led_brightness brightness);
	
	int (*brightness_set_blocking)(struct led_classdev *led_cdev,
				       enum led_brightness brightness);
	
	enum led_brightness (*brightness_get)(struct led_classdev *led_cdev);

	
	int		(*blink_set)(struct led_classdev *led_cdev,
				     unsigned long *delay_on,
				     unsigned long *delay_off);

	int (*pattern_set)(struct led_classdev *led_cdev,
			   struct led_pattern *pattern, u32 len, int repeat);
	int (*pattern_clear)(struct led_classdev *led_cdev);

	struct device		*dev;
	const struct attribute_group	**groups;

	struct list_head	 node;			
	const char		*default_trigger;	

	unsigned long		 blink_delay_on, blink_delay_off;
	struct timer_list	 blink_timer;
	int			 blink_brightness;
	int			 new_blink_brightness;
	void			(*flash_resume)(struct led_classdev *led_cdev);

	struct work_struct	set_brightness_work;
	int			delayed_set_value;

#ifdef CONFIG_LEDS_TRIGGERS
	
	struct rw_semaphore	 trigger_lock;

	struct led_trigger	*trigger;
	struct list_head	 trig_list;
	void			*trigger_data;
	
	bool			activated;
#endif

#ifdef CONFIG_LEDS_BRIGHTNESS_HW_CHANGED
	int			 brightness_hw_changed;
	struct kernfs_node	*brightness_hw_changed_kn;
#endif

	
	struct mutex		led_access;
};

extern int of_led_classdev_register(struct device *parent,
				    struct device_node *np,
				    struct led_classdev *led_cdev);
#define led_classdev_register(parent, led_cdev)				\
	of_led_classdev_register(parent, NULL, led_cdev)
extern int devm_of_led_classdev_register(struct device *parent,
					 struct device_node *np,
					 struct led_classdev *led_cdev);
#define devm_led_classdev_register(parent, led_cdev)			\
	devm_of_led_classdev_register(parent, NULL, led_cdev)
extern void led_classdev_unregister(struct led_classdev *led_cdev);
extern void devm_led_classdev_unregister(struct device *parent,
					 struct led_classdev *led_cdev);
extern void led_classdev_suspend(struct led_classdev *led_cdev);
extern void led_classdev_resume(struct led_classdev *led_cdev);


extern void led_blink_set(struct led_classdev *led_cdev,
			  unsigned long *delay_on,
			  unsigned long *delay_off);

extern void led_blink_set_oneshot(struct led_classdev *led_cdev,
				  unsigned long *delay_on,
				  unsigned long *delay_off,
				  int invert);

extern void led_set_brightness(struct led_classdev *led_cdev,
			       enum led_brightness brightness);


extern int led_set_brightness_sync(struct led_classdev *led_cdev,
				   enum led_brightness value);


extern int led_update_brightness(struct led_classdev *led_cdev);


extern u32 *led_get_default_pattern(struct led_classdev *led_cdev,
				    unsigned int *size);


extern void led_sysfs_disable(struct led_classdev *led_cdev);


extern void led_sysfs_enable(struct led_classdev *led_cdev);


static inline bool led_sysfs_is_disabled(struct led_classdev *led_cdev)
{
	return led_cdev->flags & LED_SYSFS_DISABLE;
}



#define DEFINE_LED_TRIGGER(x)		static struct led_trigger *x;
#define DEFINE_LED_TRIGGER_GLOBAL(x)	struct led_trigger *x;

#ifdef CONFIG_LEDS_TRIGGERS

#define TRIG_NAME_MAX 50

struct led_trigger {
	
	const char	 *name;
	int		(*activate)(struct led_classdev *led_cdev);
	void		(*deactivate)(struct led_classdev *led_cdev);

	
	rwlock_t	  leddev_list_lock;
	struct list_head  led_cdevs;

	
	struct list_head  next_trig;

	const struct attribute_group **groups;
};


#define led_trigger_get_led(dev)	((struct led_classdev *)dev_get_drvdata((dev)))
#define led_trigger_get_drvdata(dev)	(led_get_trigger_data(led_trigger_get_led(dev)))

ssize_t led_trigger_store(struct device *dev, struct device_attribute *attr,
			const char *buf, size_t count);
ssize_t led_trigger_show(struct device *dev, struct device_attribute *attr,
			char *buf);


extern int led_trigger_register(struct led_trigger *trigger);
extern void led_trigger_unregister(struct led_trigger *trigger);
extern int devm_led_trigger_register(struct device *dev,
				     struct led_trigger *trigger);

extern void led_trigger_register_simple(const char *name,
				struct led_trigger **trigger);
extern void led_trigger_unregister_simple(struct led_trigger *trigger);
extern void led_trigger_event(struct led_trigger *trigger,
				enum led_brightness event);
extern void led_trigger_blink(struct led_trigger *trigger,
			      unsigned long *delay_on,
			      unsigned long *delay_off);
extern void led_trigger_blink_oneshot(struct led_trigger *trigger,
				      unsigned long *delay_on,
				      unsigned long *delay_off,
				      int invert);
extern void led_trigger_set_default(struct led_classdev *led_cdev);
extern int led_trigger_set(struct led_classdev *led_cdev,
			   struct led_trigger *trigger);
extern void led_trigger_remove(struct led_classdev *led_cdev);

static inline void led_set_trigger_data(struct led_classdev *led_cdev,
					void *trigger_data)
{
	led_cdev->trigger_data = trigger_data;
}

static inline void *led_get_trigger_data(struct led_classdev *led_cdev)
{
	return led_cdev->trigger_data;
}


extern void led_trigger_rename_static(const char *name,
				      struct led_trigger *trig);

#define module_led_trigger(__led_trigger) \
	module_driver(__led_trigger, led_trigger_register, \
		      led_trigger_unregister)

#else


struct led_trigger {};


static inline void led_trigger_register_simple(const char *name,
					struct led_trigger **trigger) {}
static inline void led_trigger_unregister_simple(struct led_trigger *trigger) {}
static inline void led_trigger_event(struct led_trigger *trigger,
				enum led_brightness event) {}
static inline void led_trigger_blink(struct led_trigger *trigger,
				      unsigned long *delay_on,
				      unsigned long *delay_off) {}
static inline void led_trigger_blink_oneshot(struct led_trigger *trigger,
				      unsigned long *delay_on,
				      unsigned long *delay_off,
				      int invert) {}
static inline void led_trigger_set_default(struct led_classdev *led_cdev) {}
static inline int led_trigger_set(struct led_classdev *led_cdev,
				  struct led_trigger *trigger)
{
	return 0;
}

static inline void led_trigger_remove(struct led_classdev *led_cdev) {}
static inline void led_set_trigger_data(struct led_classdev *led_cdev) {}
static inline void *led_get_trigger_data(struct led_classdev *led_cdev)
{
	return NULL;
}

#endif 


#ifdef CONFIG_LEDS_TRIGGER_DISK
extern void ledtrig_disk_activity(bool write);
#else
static inline void ledtrig_disk_activity(bool write) {}
#endif

#ifdef CONFIG_LEDS_TRIGGER_MTD
extern void ledtrig_mtd_activity(void);
#else
static inline void ledtrig_mtd_activity(void) {}
#endif

#if defined(CONFIG_LEDS_TRIGGER_CAMERA) || defined(CONFIG_LEDS_TRIGGER_CAMERA_MODULE)
extern void ledtrig_flash_ctrl(bool on);
extern void ledtrig_torch_ctrl(bool on);
#else
static inline void ledtrig_flash_ctrl(bool on) {}
static inline void ledtrig_torch_ctrl(bool on) {}
#endif


struct led_info {
	const char	*name;
	const char	*default_trigger;
	int		flags;
};

struct led_platform_data {
	int		num_leds;
	struct led_info	*leds;
};

struct gpio_desc;
typedef int (*gpio_blink_set_t)(struct gpio_desc *desc, int state,
				unsigned long *delay_on,
				unsigned long *delay_off);


struct gpio_led {
	const char *name;
	const char *default_trigger;
	unsigned 	gpio;
	unsigned	active_low : 1;
	unsigned	retain_state_suspended : 1;
	unsigned	panic_indicator : 1;
	unsigned	default_state : 2;
	unsigned	retain_state_shutdown : 1;
	
	struct gpio_desc *gpiod;
};
#define LEDS_GPIO_DEFSTATE_OFF		0
#define LEDS_GPIO_DEFSTATE_ON		1
#define LEDS_GPIO_DEFSTATE_KEEP		2

struct gpio_led_platform_data {
	int 		num_leds;
	const struct gpio_led *leds;

#define GPIO_LED_NO_BLINK_LOW	0	
#define GPIO_LED_NO_BLINK_HIGH	1	
#define GPIO_LED_BLINK		2	
	gpio_blink_set_t	gpio_blink_set;
};

#ifdef CONFIG_NEW_LEDS
struct platform_device *gpio_led_register_device(
		int id, const struct gpio_led_platform_data *pdata);
#else
static inline struct platform_device *gpio_led_register_device(
		int id, const struct gpio_led_platform_data *pdata)
{
	return 0;
}
#endif

enum cpu_led_event {
	CPU_LED_IDLE_START,	
	CPU_LED_IDLE_END,	
	CPU_LED_START,		
	CPU_LED_STOP,		
	CPU_LED_HALTED,		
};
#ifdef CONFIG_LEDS_TRIGGER_CPU
extern void ledtrig_cpu(enum cpu_led_event evt);
#else
static inline void ledtrig_cpu(enum cpu_led_event evt)
{
	return;
}
#endif

#ifdef CONFIG_LEDS_BRIGHTNESS_HW_CHANGED
extern void led_classdev_notify_brightness_hw_changed(
	struct led_classdev *led_cdev, enum led_brightness brightness);
#else
static inline void led_classdev_notify_brightness_hw_changed(
	struct led_classdev *led_cdev, enum led_brightness brightness) { }
#endif


struct led_pattern {
	u32 delta_t;
	int brightness;
};

enum led_audio {
	LED_AUDIO_MUTE,		
	LED_AUDIO_MICMUTE,	
	NUM_AUDIO_LEDS
};

#if IS_ENABLED(CONFIG_LEDS_TRIGGER_AUDIO)
enum led_brightness ledtrig_audio_get(enum led_audio type);
void ledtrig_audio_set(enum led_audio type, enum led_brightness state);
#else
static inline enum led_brightness ledtrig_audio_get(enum led_audio type)
{
	return LED_OFF;
}
static inline void ledtrig_audio_set(enum led_audio type,
				     enum led_brightness state)
{
}
#endif

#endif		
