/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _VIDEOBUF2_DVB_H_
#define	_VIDEOBUF2_DVB_H_

#include <media/dvbdev.h>
#include <media/dmxdev.h>
#include <media/dvb_demux.h>
#include <media/dvb_net.h>
#include <media/dvb_frontend.h>
#include <media/videobuf2-v4l2.h>


struct media_device;



struct vb2_dvb {
	
	char			*name;
	struct dvb_frontend	*frontend;
	struct vb2_queue	dvbq;

	
	struct mutex		lock;
	int			nfeeds;

	
	struct dvb_demux	demux;
	struct dmxdev		dmxdev;
	struct dmx_frontend	fe_hw;
	struct dmx_frontend	fe_mem;
	struct dvb_net		net;
};

struct vb2_dvb_frontend {
	struct list_head felist;
	int id;
	struct vb2_dvb dvb;
};

struct vb2_dvb_frontends {
	struct list_head felist;
	struct mutex lock;
	struct dvb_adapter adapter;
	int active_fe_id; 
	int gate; 
};

int vb2_dvb_register_bus(struct vb2_dvb_frontends *f,
			 struct module *module,
			 void *adapter_priv,
			 struct device *device,
			 struct media_device *mdev,
			 short *adapter_nr,
			 int mfe_shared);

void vb2_dvb_unregister_bus(struct vb2_dvb_frontends *f);

struct vb2_dvb_frontend *vb2_dvb_alloc_frontend(struct vb2_dvb_frontends *f, int id);
void vb2_dvb_dealloc_frontends(struct vb2_dvb_frontends *f);

struct vb2_dvb_frontend *vb2_dvb_get_frontend(struct vb2_dvb_frontends *f, int id);
int vb2_dvb_find_frontend(struct vb2_dvb_frontends *f, struct dvb_frontend *p);

#endif			
