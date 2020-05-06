/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LINUX_SOCKET_H
#define _UAPI_LINUX_SOCKET_H


#define _K_SS_MAXSIZE	128	

typedef unsigned short __kernel_sa_family_t;


struct __kernel_sockaddr_storage {
	union {
		struct {
			__kernel_sa_family_t	ss_family; 
			
			char __data[_K_SS_MAXSIZE - sizeof(unsigned short)];
				
				
		};
		void *__align; 
	};
};

#endif 
