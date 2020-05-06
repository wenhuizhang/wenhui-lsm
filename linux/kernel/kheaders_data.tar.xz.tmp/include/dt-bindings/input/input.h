/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _DT_BINDINGS_INPUT_INPUT_H
#define _DT_BINDINGS_INPUT_INPUT_H

#include "linux-event-codes.h"

#define MATRIX_KEY(row, col, code)	\
	((((row) & 0xFF) << 24) | (((col) & 0xFF) << 16) | ((code) & 0xFFFF))

#endif 
