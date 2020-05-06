#ifndef NETDEV_PCS_H
#define NETDEV_PCS_H

#include <linux/phy.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>

struct device_node;
struct ethtool_cmd;
struct fwnode_handle;
struct net_device;

enum {
	MLO_PAUSE_NONE,
	MLO_PAUSE_ASYM = BIT(0),
	MLO_PAUSE_SYM = BIT(1),
	MLO_PAUSE_RX = BIT(2),
	MLO_PAUSE_TX = BIT(3),
	MLO_PAUSE_TXRX_MASK = MLO_PAUSE_TX | MLO_PAUSE_RX,
	MLO_PAUSE_AN = BIT(4),

	MLO_AN_PHY = 0,	
	MLO_AN_FIXED,	
	MLO_AN_INBAND,	
};

static inline bool phylink_autoneg_inband(unsigned int mode)
{
	return mode == MLO_AN_INBAND;
}


struct phylink_link_state {
	__ETHTOOL_DECLARE_LINK_MODE_MASK(advertising);
	__ETHTOOL_DECLARE_LINK_MODE_MASK(lp_advertising);
	phy_interface_t interface;
	int speed;
	int duplex;
	int pause;
	unsigned int link:1;
	unsigned int an_enabled:1;
	unsigned int an_complete:1;
};

enum phylink_op_type {
	PHYLINK_NETDEV = 0,
	PHYLINK_DEV,
};


struct phylink_config {
	struct device *dev;
	enum phylink_op_type type;
};


struct phylink_mac_ops {
	void (*validate)(struct phylink_config *config,
			 unsigned long *supported,
			 struct phylink_link_state *state);
	int (*mac_link_state)(struct phylink_config *config,
			      struct phylink_link_state *state);
	void (*mac_config)(struct phylink_config *config, unsigned int mode,
			   const struct phylink_link_state *state);
	void (*mac_an_restart)(struct phylink_config *config);
	void (*mac_link_down)(struct phylink_config *config, unsigned int mode,
			      phy_interface_t interface);
	void (*mac_link_up)(struct phylink_config *config, unsigned int mode,
			    phy_interface_t interface,
			    struct phy_device *phy);
};

#if 0 

void validate(struct phylink_config *config, unsigned long *supported,
	      struct phylink_link_state *state);


int mac_link_state(struct phylink_config *config,
		   struct phylink_link_state *state);


void mac_config(struct phylink_config *config, unsigned int mode,
		const struct phylink_link_state *state);


void mac_an_restart(struct phylink_config *config);


void mac_link_down(struct phylink_config *config, unsigned int mode,
		   phy_interface_t interface);


void mac_link_up(struct phylink_config *config, unsigned int mode,
		 phy_interface_t interface,
		 struct phy_device *phy);
#endif

struct phylink *phylink_create(struct phylink_config *, struct fwnode_handle *,
			       phy_interface_t iface,
			       const struct phylink_mac_ops *ops);
void phylink_destroy(struct phylink *);

int phylink_connect_phy(struct phylink *, struct phy_device *);
int phylink_of_phy_connect(struct phylink *, struct device_node *, u32 flags);
void phylink_disconnect_phy(struct phylink *);
int phylink_fixed_state_cb(struct phylink *,
			   void (*cb)(struct net_device *dev,
				      struct phylink_link_state *));

void phylink_mac_change(struct phylink *, bool up);

void phylink_start(struct phylink *);
void phylink_stop(struct phylink *);

void phylink_ethtool_get_wol(struct phylink *, struct ethtool_wolinfo *);
int phylink_ethtool_set_wol(struct phylink *, struct ethtool_wolinfo *);

int phylink_ethtool_ksettings_get(struct phylink *,
				  struct ethtool_link_ksettings *);
int phylink_ethtool_ksettings_set(struct phylink *,
				  const struct ethtool_link_ksettings *);
int phylink_ethtool_nway_reset(struct phylink *);
void phylink_ethtool_get_pauseparam(struct phylink *,
				    struct ethtool_pauseparam *);
int phylink_ethtool_set_pauseparam(struct phylink *,
				   struct ethtool_pauseparam *);
int phylink_get_eee_err(struct phylink *);
int phylink_init_eee(struct phylink *, bool);
int phylink_ethtool_get_eee(struct phylink *, struct ethtool_eee *);
int phylink_ethtool_set_eee(struct phylink *, struct ethtool_eee *);
int phylink_mii_ioctl(struct phylink *, struct ifreq *, int);

#define phylink_zero(bm) \
	bitmap_zero(bm, __ETHTOOL_LINK_MODE_MASK_NBITS)
#define __phylink_do_bit(op, bm, mode) \
	op(ETHTOOL_LINK_MODE_ ## mode ## _BIT, bm)

#define phylink_set(bm, mode)	__phylink_do_bit(__set_bit, bm, mode)
#define phylink_clear(bm, mode)	__phylink_do_bit(__clear_bit, bm, mode)
#define phylink_test(bm, mode)	__phylink_do_bit(test_bit, bm, mode)

void phylink_set_port_modes(unsigned long *bits);
void phylink_helper_basex_speed(struct phylink_link_state *state);

#endif
