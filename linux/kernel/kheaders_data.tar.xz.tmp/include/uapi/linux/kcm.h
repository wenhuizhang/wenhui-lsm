/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef KCM_KERNEL_H
#define KCM_KERNEL_H

struct kcm_attach {
	int fd;
	int bpf_fd;
};

struct kcm_unattach {
	int fd;
};

struct kcm_clone {
	int fd;
};

#define SIOCKCMATTACH	(SIOCPROTOPRIVATE + 0)
#define SIOCKCMUNATTACH	(SIOCPROTOPRIVATE + 1)
#define SIOCKCMCLONE	(SIOCPROTOPRIVATE + 2)

#define KCMPROTO_CONNECTED	0


#define KCM_RECV_DISABLE	1

#endif

