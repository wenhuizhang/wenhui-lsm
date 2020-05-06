/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef _KEYS_RXRPC_TYPE_H
#define _KEYS_RXRPC_TYPE_H

#include <linux/key.h>


extern struct key_type key_type_rxrpc;

extern struct key *rxrpc_get_null_key(const char *);


struct rxkad_key {
	u32	vice_id;
	u32	start;			
	u32	expiry;			
	u32	kvno;			
	u8	primary_flag;		
	u16	ticket_len;		
	u8	session_key[8];		
	u8	ticket[0];		
};


struct krb5_principal {
	u8	n_name_parts;		
	char	**name_parts;		
	char	*realm;			
};


struct krb5_tagged_data {
	
	s32		tag;
	u32		data_len;
	u8		*data;
};


struct rxk5_key {
	u64			authtime;	
	u64			starttime;	
	u64			endtime;	
	u64			renew_till;	
	s32			is_skey;	
	s32			flags;		
	struct krb5_principal	client;		
	struct krb5_principal	server;		
	u16			ticket_len;	
	u16			ticket2_len;	
	u8			n_authdata;	
	u8			n_addresses;	
	struct krb5_tagged_data	session;	
	struct krb5_tagged_data *addresses;	
	u8			*ticket;	
	u8			*ticket2;	
	struct krb5_tagged_data *authdata;	
};


struct rxrpc_key_token {
	u16	security_index;		
	struct rxrpc_key_token *next;	
	union {
		struct rxkad_key *kad;
		struct rxk5_key *k5;
	};
};


struct rxrpc_key_data_v1 {
	u16		security_index;
	u16		ticket_length;
	u32		expiry;			
	u32		kvno;
	u8		session_key[8];
	u8		ticket[0];
};


#define AFSTOKEN_LENGTH_MAX		16384	
#define AFSTOKEN_STRING_MAX		256	
#define AFSTOKEN_DATA_MAX		64	
#define AFSTOKEN_CELL_MAX		64	
#define AFSTOKEN_MAX			8	
#define AFSTOKEN_BDATALN_MAX		16384	
#define AFSTOKEN_RK_TIX_MAX		12000	
#define AFSTOKEN_GK_KEY_MAX		64	
#define AFSTOKEN_GK_TOKEN_MAX		16384	
#define AFSTOKEN_K5_COMPONENTS_MAX	16	
#define AFSTOKEN_K5_NAME_MAX		128	
#define AFSTOKEN_K5_REALM_MAX		64	
#define AFSTOKEN_K5_TIX_MAX		16384	
#define AFSTOKEN_K5_ADDRESSES_MAX	16	
#define AFSTOKEN_K5_AUTHDATA_MAX	16	


static inline u32 rxrpc_time64_to_u32(time64_t time)
{
	if (time < 0)
		return 0;

	if (time > UINT_MAX)
		return UINT_MAX;

	return (u32)time;
}


static inline time64_t rxrpc_u32_to_time64(u32 time)
{
	return (time64_t)time;
}

#endif 
