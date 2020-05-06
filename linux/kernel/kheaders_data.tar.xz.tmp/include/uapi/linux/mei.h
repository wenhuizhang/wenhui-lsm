/* SPDX-License-Identifier: ((GPL-2.0 WITH Linux-syscall-note) OR BSD-3-Clause) */

#ifndef _LINUX_MEI_H
#define _LINUX_MEI_H

#include <linux/uuid.h>


#define IOCTL_MEI_CONNECT_CLIENT \
	_IOWR('H' , 0x01, struct mei_connect_client_data)


struct mei_client {
	__u32 max_msg_length;
	__u8 protocol_version;
	__u8 reserved[3];
};


struct mei_connect_client_data {
	union {
		uuid_le in_client_uuid;
		struct mei_client out_client_properties;
	};
};


#define IOCTL_MEI_NOTIFY_SET _IOW('H', 0x02, __u32)


#define IOCTL_MEI_NOTIFY_GET _IOR('H', 0x03, __u32)

#endif 
