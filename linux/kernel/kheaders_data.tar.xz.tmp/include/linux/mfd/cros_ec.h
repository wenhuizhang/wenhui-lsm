/* SPDX-License-Identifier: GPL-2.0-only */


#ifndef __LINUX_MFD_CROS_EC_H
#define __LINUX_MFD_CROS_EC_H

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/notifier.h>
#include <linux/mfd/cros_ec_commands.h>
#include <linux/mutex.h>

#define CROS_EC_DEV_NAME "cros_ec"
#define CROS_EC_DEV_FP_NAME "cros_fp"
#define CROS_EC_DEV_PD_NAME "cros_pd"
#define CROS_EC_DEV_TP_NAME "cros_tp"
#define CROS_EC_DEV_ISH_NAME "cros_ish"
#define CROS_EC_DEV_SCP_NAME "cros_scp"


#define EC_REBOOT_DELAY_MS             50


#define EC_PROTO_VERSION_UNKNOWN	0
#define EC_MAX_REQUEST_OVERHEAD		1
#define EC_MAX_RESPONSE_OVERHEAD	32


enum {
	EC_MSG_TX_HEADER_BYTES	= 3,
	EC_MSG_TX_TRAILER_BYTES	= 1,
	EC_MSG_TX_PROTO_BYTES	= EC_MSG_TX_HEADER_BYTES +
					EC_MSG_TX_TRAILER_BYTES,
	EC_MSG_RX_PROTO_BYTES	= 3,

	
	EC_PROTO2_MSG_BYTES		= EC_PROTO2_MAX_PARAM_SIZE +
					EC_MSG_TX_PROTO_BYTES,

	EC_MAX_MSG_BYTES		= 64 * 1024,
};


struct cros_ec_command {
	uint32_t version;
	uint32_t command;
	uint32_t outsize;
	uint32_t insize;
	uint32_t result;
	uint8_t data[0];
};


struct cros_ec_device {
	
	const char *phys_name;
	struct device *dev;
	bool was_wake_device;
	struct class *cros_class;
	int (*cmd_readmem)(struct cros_ec_device *ec, unsigned int offset,
			   unsigned int bytes, void *dest);

	
	u16 max_request;
	u16 max_response;
	u16 max_passthru;
	u16 proto_version;
	void *priv;
	int irq;
	u8 *din;
	u8 *dout;
	int din_size;
	int dout_size;
	bool wake_enabled;
	bool suspended;
	int (*cmd_xfer)(struct cros_ec_device *ec,
			struct cros_ec_command *msg);
	int (*pkt_xfer)(struct cros_ec_device *ec,
			struct cros_ec_command *msg);
	struct mutex lock;
	bool mkbp_event_supported;
	bool host_sleep_v1;
	struct blocking_notifier_head event_notifier;

	struct ec_response_get_next_event_v1 event_data;
	int event_size;
	u32 host_event_wake_mask;
	u32 last_resume_result;
};


struct cros_ec_sensor_platform {
	u8 sensor_num;
};


struct cros_ec_platform {
	const char *ec_name;
	u16 cmd_offset;
};

struct cros_ec_debugfs;


struct cros_ec_dev {
	struct device class_dev;
	struct cdev cdev;
	struct cros_ec_device *ec_dev;
	struct device *dev;
	struct cros_ec_debugfs *debug_info;
	bool has_kb_wake_angle;
	u16 cmd_offset;
	u32 features[2];
};

#define to_cros_ec_dev(dev)  container_of(dev, struct cros_ec_dev, class_dev)


int cros_ec_suspend(struct cros_ec_device *ec_dev);


int cros_ec_resume(struct cros_ec_device *ec_dev);


int cros_ec_prepare_tx(struct cros_ec_device *ec_dev,
		       struct cros_ec_command *msg);


int cros_ec_check_result(struct cros_ec_device *ec_dev,
			 struct cros_ec_command *msg);


int cros_ec_cmd_xfer(struct cros_ec_device *ec_dev,
		     struct cros_ec_command *msg);


int cros_ec_cmd_xfer_status(struct cros_ec_device *ec_dev,
			    struct cros_ec_command *msg);


int cros_ec_register(struct cros_ec_device *ec_dev);


int cros_ec_query_all(struct cros_ec_device *ec_dev);


int cros_ec_get_next_event(struct cros_ec_device *ec_dev, bool *wake_event);


u32 cros_ec_get_host_event(struct cros_ec_device *ec_dev);

#endif 
