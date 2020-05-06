/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_ONCE_H
#define _LINUX_ONCE_H

#include <linux/types.h>
#include <linux/jump_label.h>

bool __do_once_start(bool *done, unsigned long *flags);
void __do_once_done(bool *done, struct static_key_true *once_key,
		    unsigned long *flags);


#define DO_ONCE(func, ...)						     \
	({								     \
		bool ___ret = false;					     \
		static bool ___done = false;				     \
		static DEFINE_STATIC_KEY_TRUE(___once_key);		     \
		if (static_branch_unlikely(&___once_key)) {		     \
			unsigned long ___flags;				     \
			___ret = __do_once_start(&___done, &___flags);	     \
			if (unlikely(___ret)) {				     \
				func(__VA_ARGS__);			     \
				__do_once_done(&___done, &___once_key,	     \
					       &___flags);		     \
			}						     \
		}							     \
		___ret;							     \
	})

#define get_random_once(buf, nbytes)					     \
	DO_ONCE(get_random_bytes, (buf), (nbytes))
#define get_random_once_wait(buf, nbytes)                                    \
	DO_ONCE(get_random_bytes_wait, (buf), (nbytes))                      \

#endif 
