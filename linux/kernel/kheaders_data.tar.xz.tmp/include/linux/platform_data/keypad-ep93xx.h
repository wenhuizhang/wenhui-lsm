/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __KEYPAD_EP93XX_H
#define __KEYPAD_EP93XX_H

struct matrix_keymap_data;


#define EP93XX_KEYPAD_DISABLE_3_KEY	(1<<0)	
#define EP93XX_KEYPAD_DIAG_MODE		(1<<1)	
#define EP93XX_KEYPAD_BACK_DRIVE	(1<<2)	
#define EP93XX_KEYPAD_TEST_MODE		(1<<3)	
#define EP93XX_KEYPAD_AUTOREPEAT	(1<<4)	


struct ep93xx_keypad_platform_data {
	struct matrix_keymap_data *keymap_data;
	unsigned int	debounce;
	unsigned int	prescale;
	unsigned int	flags;
	unsigned int	clk_rate;
};

#define EP93XX_MATRIX_ROWS		(8)
#define EP93XX_MATRIX_COLS		(8)

#endif	
