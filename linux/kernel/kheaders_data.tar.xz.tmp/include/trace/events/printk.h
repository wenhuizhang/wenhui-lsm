/* SPDX-License-Identifier: GPL-2.0 */
#undef TRACE_SYSTEM
#define TRACE_SYSTEM printk

#if !defined(_TRACE_PRINTK_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_PRINTK_H

#include <linux/tracepoint.h>

TRACE_EVENT(console,
	TP_PROTO(const char *text, size_t len),

	TP_ARGS(text, len),

	TP_STRUCT__entry(
		__dynamic_array(char, msg, len + 1)
	),

	TP_fast_assign(
		
		if ((len > 0) && (text[len-1] == '\n'))
			len -= 1;

		memcpy(__get_str(msg), text, len);
		__get_str(msg)[len] = 0;
	),

	TP_printk("%s", __get_str(msg))
);
#endif 


#include <trace/define_trace.h>
