/* SPDX-License-Identifier: GPL-2.0-only */


#ifndef __LINUX_LM3642_H
#define __LINUX_LM3642_H

#define LM3642_NAME "leds-lm3642"

enum lm3642_torch_pin_enable {
	LM3642_TORCH_PIN_DISABLE = 0x00,
	LM3642_TORCH_PIN_ENABLE = 0x10,
};

enum lm3642_strobe_pin_enable {
	LM3642_STROBE_PIN_DISABLE = 0x00,
	LM3642_STROBE_PIN_ENABLE = 0x20,
};

enum lm3642_tx_pin_enable {
	LM3642_TX_PIN_DISABLE = 0x00,
	LM3642_TX_PIN_ENABLE = 0x40,
};

struct lm3642_platform_data {
	enum lm3642_torch_pin_enable torch_pin;
	enum lm3642_strobe_pin_enable strobe_pin;
	enum lm3642_tx_pin_enable tx_pin;
};

#endif 
