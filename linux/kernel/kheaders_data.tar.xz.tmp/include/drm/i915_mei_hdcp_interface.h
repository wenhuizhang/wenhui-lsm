/* SPDX-License-Identifier: (GPL-2.0+) */


#ifndef _I915_MEI_HDCP_INTERFACE_H_
#define _I915_MEI_HDCP_INTERFACE_H_

#include <linux/mutex.h>
#include <linux/device.h>
#include <drm/drm_hdcp.h>
#include <drm/i915_drm.h>


enum hdcp_port_type {
	HDCP_PORT_TYPE_INVALID,
	HDCP_PORT_TYPE_INTEGRATED,
	HDCP_PORT_TYPE_LSPCON,
	HDCP_PORT_TYPE_CPDP
};


enum hdcp_wired_protocol {
	HDCP_PROTOCOL_INVALID,
	HDCP_PROTOCOL_HDMI,
	HDCP_PROTOCOL_DP
};


struct hdcp_port_data {
	enum port port;
	u8 port_type;
	u8 protocol;
	u16 k;
	u32 seq_num_m;
	struct hdcp2_streamid_type *streams;
};


struct i915_hdcp_component_ops {
	
	struct module *owner;

	int (*initiate_hdcp2_session)(struct device *dev,
				      struct hdcp_port_data *data,
				      struct hdcp2_ake_init *ake_data);
	int (*verify_receiver_cert_prepare_km)(struct device *dev,
					       struct hdcp_port_data *data,
					       struct hdcp2_ake_send_cert
								*rx_cert,
					       bool *km_stored,
					       struct hdcp2_ake_no_stored_km
								*ek_pub_km,
					       size_t *msg_sz);
	int (*verify_hprime)(struct device *dev,
			     struct hdcp_port_data *data,
			     struct hdcp2_ake_send_hprime *rx_hprime);
	int (*store_pairing_info)(struct device *dev,
				  struct hdcp_port_data *data,
				  struct hdcp2_ake_send_pairing_info
								*pairing_info);
	int (*initiate_locality_check)(struct device *dev,
				       struct hdcp_port_data *data,
				       struct hdcp2_lc_init *lc_init_data);
	int (*verify_lprime)(struct device *dev,
			     struct hdcp_port_data *data,
			     struct hdcp2_lc_send_lprime *rx_lprime);
	int (*get_session_key)(struct device *dev,
			       struct hdcp_port_data *data,
			       struct hdcp2_ske_send_eks *ske_data);
	int (*repeater_check_flow_prepare_ack)(struct device *dev,
					       struct hdcp_port_data *data,
					       struct hdcp2_rep_send_receiverid_list
								*rep_topology,
					       struct hdcp2_rep_send_ack
								*rep_send_ack);
	int (*verify_mprime)(struct device *dev,
			     struct hdcp_port_data *data,
			     struct hdcp2_rep_stream_ready *stream_ready);
	int (*enable_hdcp_authentication)(struct device *dev,
					  struct hdcp_port_data *data);
	int (*close_hdcp_session)(struct device *dev,
				  struct hdcp_port_data *data);
};


struct i915_hdcp_comp_master {
	struct device *mei_dev;
	const struct i915_hdcp_component_ops *ops;

	
	struct mutex mutex;
};

#endif 
