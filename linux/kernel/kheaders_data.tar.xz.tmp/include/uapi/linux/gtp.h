/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LINUX_GTP_H_
#define _UAPI_LINUX_GTP_H_

enum gtp_genl_cmds {
	GTP_CMD_NEWPDP,
	GTP_CMD_DELPDP,
	GTP_CMD_GETPDP,

	GTP_CMD_MAX,
};

enum gtp_version {
	GTP_V0 = 0,
	GTP_V1,
};

enum gtp_attrs {
	GTPA_UNSPEC = 0,
	GTPA_LINK,
	GTPA_VERSION,
	GTPA_TID,	
	GTPA_PEER_ADDRESS,	
#define GTPA_SGSN_ADDRESS GTPA_PEER_ADDRESS 
	GTPA_MS_ADDRESS,
	GTPA_FLOW,
	GTPA_NET_NS_FD,
	GTPA_I_TEI,	
	GTPA_O_TEI,	
	GTPA_PAD,
	__GTPA_MAX,
};
#define GTPA_MAX (__GTPA_MAX + 1)

#endif 
