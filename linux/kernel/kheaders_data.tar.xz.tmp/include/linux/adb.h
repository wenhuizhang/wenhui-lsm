/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __ADB_H
#define __ADB_H

#include <uapi/linux/adb.h>


struct adb_request {
	unsigned char data[32];
	int nbytes;
	unsigned char reply[32];
	int reply_len;
	unsigned char reply_expected;
	unsigned char sent;
	unsigned char complete;
	void (*done)(struct adb_request *);
	void *arg;
	struct adb_request *next;
};

struct adb_ids {
	int nids;
	unsigned char id[16];
};



struct adb_driver {
	char name[16];
	int (*probe)(void);
	int (*init)(void);
	int (*send_request)(struct adb_request *req, int sync);
	int (*autopoll)(int devs);
	void (*poll)(void);
	int (*reset_bus)(void);
};


#define ADBREQ_REPLY	1	
#define ADBREQ_SYNC	2	
#define ADBREQ_NOSEND	4	


enum adb_message {
    ADB_MSG_POWERDOWN,	
    ADB_MSG_PRE_RESET,	
    ADB_MSG_POST_RESET	
};
extern struct blocking_notifier_head adb_client_list;

int adb_request(struct adb_request *req, void (*done)(struct adb_request *),
		int flags, int nbytes, ...);
int adb_register(int default_id,int handler_id,struct adb_ids *ids,
		 void (*handler)(unsigned char *, int, int));
int adb_unregister(int index);
void adb_poll(void);
void adb_input(unsigned char *, int, int);
int adb_reset_bus(void);

int adb_try_handler_change(int address, int new_id);
int adb_get_infos(int address, int *original_address, int *handler_id);

#endif 
