/* SPDX-License-Identifier: GPL-2.0-only */


#define C2PORT_NAME_LEN			32

struct device;




struct c2port_ops;
struct c2port_device {
	unsigned int access:1;
	unsigned int flash_access:1;

	int id;
	char name[C2PORT_NAME_LEN];
	struct c2port_ops *ops;
	struct mutex mutex;		

	struct device *dev;

	void *private_data;
};


struct c2port_ops {
	
	unsigned short block_size;	
	unsigned short blocks_num;	

	
	void (*access)(struct c2port_device *dev, int status);

	
	void (*c2d_dir)(struct c2port_device *dev, int dir);

	
	int (*c2d_get)(struct c2port_device *dev);
	void (*c2d_set)(struct c2port_device *dev, int status);

	
	void (*c2ck_set)(struct c2port_device *dev, int status);
};



extern struct c2port_device *c2port_device_register(char *name,
					struct c2port_ops *ops, void *devdata);
extern void c2port_device_unregister(struct c2port_device *dev);
