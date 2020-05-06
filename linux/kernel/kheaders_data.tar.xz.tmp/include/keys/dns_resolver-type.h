/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef _KEYS_DNS_RESOLVER_TYPE_H
#define _KEYS_DNS_RESOLVER_TYPE_H

#include <linux/key-type.h>

extern struct key_type key_type_dns_resolver;

extern int request_dns_resolver_key(const char *description,
				    const char *callout_info,
				    char **data);

#endif 
