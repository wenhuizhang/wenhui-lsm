/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_INDIRECT_CALL_WRAPPER_H
#define _LINUX_INDIRECT_CALL_WRAPPER_H

#ifdef CONFIG_RETPOLINE


#define INDIRECT_CALL_1(f, f1, ...)					\
	({								\
		likely(f == f1) ? f1(__VA_ARGS__) : f(__VA_ARGS__);	\
	})
#define INDIRECT_CALL_2(f, f2, f1, ...)					\
	({								\
		likely(f == f2) ? f2(__VA_ARGS__) :			\
				  INDIRECT_CALL_1(f, f1, __VA_ARGS__);	\
	})

#define INDIRECT_CALLABLE_DECLARE(f)	f
#define INDIRECT_CALLABLE_SCOPE

#else
#define INDIRECT_CALL_1(f, f1, ...) f(__VA_ARGS__)
#define INDIRECT_CALL_2(f, f2, f1, ...) f(__VA_ARGS__)
#define INDIRECT_CALLABLE_DECLARE(f)
#define INDIRECT_CALLABLE_SCOPE		static
#endif


#if IS_BUILTIN(CONFIG_IPV6)
#define INDIRECT_CALL_INET(f, f2, f1, ...) \
	INDIRECT_CALL_2(f, f2, f1, __VA_ARGS__)
#elif IS_ENABLED(CONFIG_INET)
#define INDIRECT_CALL_INET(f, f2, f1, ...) INDIRECT_CALL_1(f, f1, __VA_ARGS__)
#else
#define INDIRECT_CALL_INET(f, f2, f1, ...) f(__VA_ARGS__)
#endif

#endif
