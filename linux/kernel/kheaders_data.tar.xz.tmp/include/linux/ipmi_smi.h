/* SPDX-License-Identifier: GPL-2.0+ */


#ifndef __LINUX_IPMI_SMI_H
#define __LINUX_IPMI_SMI_H

#include <linux/ipmi_msgdefs.h>
#include <linux/proc_fs.h>
#include <linux/platform_device.h>
#include <linux/ipmi.h>

struct device;




struct ipmi_smi;


#define IPMI_WATCH_MASK_CHECK_MESSAGES	(1 << 0)
#define IPMI_WATCH_MASK_CHECK_WATCHDOG	(1 << 1)
#define IPMI_WATCH_MASK_CHECK_COMMANDS	(1 << 2)


struct ipmi_smi_msg {
	struct list_head link;

	long    msgid;
	void    *user_data;

	int           data_size;
	unsigned char data[IPMI_MAX_MSG_LENGTH];

	int           rsp_size;
	unsigned char rsp[IPMI_MAX_MSG_LENGTH];

	
	void (*done)(struct ipmi_smi_msg *msg);
};

struct ipmi_smi_handlers {
	struct module *owner;

	
	int (*start_processing)(void            *send_info,
				struct ipmi_smi *new_intf);

	
	void (*shutdown)(void *send_info);

	
	int (*get_smi_info)(void *send_info, struct ipmi_smi_info *data);

	
	void (*sender)(void                *send_info,
		       struct ipmi_smi_msg *msg);

	
	void (*request_events)(void *send_info);

	
	void (*set_need_watch)(void *send_info, unsigned int watch_mask);

	
	void (*flush_messages)(void *send_info);

	
	void (*set_run_to_completion)(void *send_info, bool run_to_completion);

	
	void (*poll)(void *send_info);

	
	void (*set_maintenance_mode)(void *send_info, bool enable);
};

struct ipmi_device_id {
	unsigned char device_id;
	unsigned char device_revision;
	unsigned char firmware_revision_1;
	unsigned char firmware_revision_2;
	unsigned char ipmi_version;
	unsigned char additional_device_support;
	unsigned int  manufacturer_id;
	unsigned int  product_id;
	unsigned char aux_firmware_revision[4];
	unsigned int  aux_firmware_revision_set : 1;
};

#define ipmi_version_major(v) ((v)->ipmi_version & 0xf)
#define ipmi_version_minor(v) ((v)->ipmi_version >> 4)


static inline int ipmi_demangle_device_id(uint8_t netfn, uint8_t cmd,
					  const unsigned char *data,
					  unsigned int data_len,
					  struct ipmi_device_id *id)
{
	if (data_len < 7)
		return -EINVAL;
	if (netfn != IPMI_NETFN_APP_RESPONSE || cmd != IPMI_GET_DEVICE_ID_CMD)
		
		return -EINVAL;
	if (data[0] != 0)
		
		return -EINVAL;

	data++;
	data_len--;

	id->device_id = data[0];
	id->device_revision = data[1];
	id->firmware_revision_1 = data[2];
	id->firmware_revision_2 = data[3];
	id->ipmi_version = data[4];
	id->additional_device_support = data[5];
	if (data_len >= 11) {
		id->manufacturer_id = (data[6] | (data[7] << 8) |
				       (data[8] << 16));
		id->product_id = data[9] | (data[10] << 8);
	} else {
		id->manufacturer_id = 0;
		id->product_id = 0;
	}
	if (data_len >= 15) {
		memcpy(id->aux_firmware_revision, data+11, 4);
		id->aux_firmware_revision_set = 1;
	} else
		id->aux_firmware_revision_set = 0;

	return 0;
}


int ipmi_register_smi(const struct ipmi_smi_handlers *handlers,
		      void                     *send_info,
		      struct device            *dev,
		      unsigned char            slave_addr);


void ipmi_unregister_smi(struct ipmi_smi *intf);


void ipmi_smi_msg_received(struct ipmi_smi     *intf,
			   struct ipmi_smi_msg *msg);


void ipmi_smi_watchdog_pretimeout(struct ipmi_smi *intf);

struct ipmi_smi_msg *ipmi_alloc_smi_msg(void);
static inline void ipmi_free_smi_msg(struct ipmi_smi_msg *msg)
{
	msg->done(msg);
}

#endif 
