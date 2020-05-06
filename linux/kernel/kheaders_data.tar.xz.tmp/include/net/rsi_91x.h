

#ifndef __RSI_HEADER_H__
#define __RSI_HEADER_H__

#include <linux/skbuff.h>


#define RSI_COEX_Q			0x0
#define RSI_BT_Q			0x2
#define RSI_WLAN_Q                      0x3
#define RSI_WIFI_MGMT_Q                 0x4
#define RSI_WIFI_DATA_Q                 0x5
#define RSI_BT_MGMT_Q			0x6
#define RSI_BT_DATA_Q			0x7

enum rsi_coex_queues {
	RSI_COEX_Q_INVALID = -1,
	RSI_COEX_Q_COMMON = 0,
	RSI_COEX_Q_BT,
	RSI_COEX_Q_WLAN
};

enum rsi_host_intf {
	RSI_HOST_INTF_SDIO = 0,
	RSI_HOST_INTF_USB
};

struct rsi_proto_ops {
	int (*coex_send_pkt)(void *priv, struct sk_buff *skb, u8 hal_queue);
	enum rsi_host_intf (*get_host_intf)(void *priv);
	void (*set_bt_context)(void *priv, void *context);
};

struct rsi_mod_ops {
	int (*attach)(void *priv, struct rsi_proto_ops *ops);
	void (*detach)(void *priv);
	int (*recv_pkt)(void *priv, const u8 *msg);
};

extern const struct rsi_mod_ops rsi_bt_ops;
#endif
