// Copyright (C) 2016 S117 <admin@0x10c.pw>
// Bitman Lab.
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt

// In this file, you should implement following five functions for you board

// 1. initialize function which will be called when the very beginnig of driver initializing, return 0 for success
static int inline lcd_1602_4bit_bsp_init();

// 2. delay function which unit is us
static void inline lcd_1602_4bit_udelay(uint32_t us);

// 3. set lcd's enable line
static void inline lcd_1602_4bit_set_en();

// 4. clear lcd's enable line
static void inline lcd_1602_4bit_clr_en();

// 5. write lcd's control line (rs = ctrl[1], rw = ctrl[0])
static void inline lcd_1602_4bit_write_ctrl(uint8_t ctrl);

// 6. write lcd's data bus line (db[7:4] = val[3:0])
static void inline lcd_1602_4bit_write_db(uint8_t val);

// 7. read lcd's data bus line (return[3:0] = db[7:4])
static uint8_t inline lcd_1602_4bit_read_db();

#include "xgpio.h"
#include "xparameters.h"
#include <stdint.h>

void print(char *str);

XGpio g_gpio_lcd_interface;

static const unsigned gpio_channel_1 = 1;
static const unsigned gpio_direction_all_output = 0x00;
static const unsigned gpio_direction_db_input = 0x0f;
static const uint8_t pin_default = 0;

static unsigned current_db_status;
static uint8_t last_pin_status = 0;

static int inline lcd_1602_4bit_bsp_init() {
	int rtn_val;
	rtn_val = XGpio_Initialize(&g_gpio_lcd_interface,
			XPAR_LCD_1602_4BIT_INTERFACE_DEVICE_ID);
	if (XST_SUCCESS != rtn_val) {
		print("Initialize XPAR_LCD_1602_4BIT_INTERFACE fail.\n");
		return -1;
	}

	XGpio_SetDataDirection(&g_gpio_lcd_interface, gpio_channel_1,
			gpio_direction_all_output);
	XGpio_DiscreteWrite(&g_gpio_lcd_interface, gpio_channel_1, pin_default);
	current_db_status = gpio_direction_all_output;
	return 0;
}

static void inline lcd_1602_4bit_udelay(uint32_t us) {
	int i;
	while (us > 0) {
		for (i = 0; i < 8; ++i)
			;
		--us;
	}
}

static void inline lcd_1602_4bit_set_en() {
//	last_en = 1<<6;
//
//	XGpio_DiscreteWrite(&g_gpio_lcd_interface, gpio_channel_1, last_en | last_ctrl | last_db);
	last_pin_status |= 1 << 6;

	XGpio_DiscreteWrite(&g_gpio_lcd_interface, gpio_channel_1, last_pin_status);
}

static void inline lcd_1602_4bit_clr_en() {
//	last_en = 0 << 6;
//
//	XGpio_DiscreteWrite(&g_gpio_lcd_interface, gpio_channel_1,
//			last_en | last_ctrl | last_db);
	last_pin_status &= ~(1 << 6);

	XGpio_DiscreteWrite(&g_gpio_lcd_interface, gpio_channel_1, last_pin_status);
}

// rs = ctrl[1], rw = ctrl[0]
static void inline lcd_1602_4bit_write_ctrl(uint8_t ctrl) {
//	last_ctrl = (ctrl & 0x3) << 4;
//
//	XGpio_DiscreteWrite(&g_gpio_lcd_interface, gpio_channel_1,
//			last_en | last_ctrl | last_db);
	last_pin_status = (last_pin_status & (~(0x3 << 4))) | ((ctrl & 0x3) << 4);

	XGpio_DiscreteWrite(&g_gpio_lcd_interface, gpio_channel_1, last_pin_status);
}

// db[7:4] = ctrl[3:0]
static void inline lcd_1602_4bit_write_db(uint8_t db) {
	if (current_db_status != gpio_direction_all_output) {
		XGpio_SetDataDirection(&g_gpio_lcd_interface, gpio_channel_1,
				gpio_direction_all_output);
		current_db_status = gpio_direction_all_output;
	}

//	last_db = db & 0xf;
//
//	XGpio_DiscreteWrite(&g_gpio_lcd_interface, gpio_channel_1,
//			last_en | last_ctrl | last_db);
	last_pin_status = (last_pin_status & (~0xf)) | (db & 0xf);

	XGpio_DiscreteWrite(&g_gpio_lcd_interface, gpio_channel_1, last_pin_status);
}

// return db[7:4]
static uint8_t inline lcd_1602_4bit_read_db() {
	if (current_db_status != gpio_direction_db_input) {
		XGpio_SetDataDirection(&g_gpio_lcd_interface, gpio_channel_1,
				gpio_direction_db_input);
		current_db_status = gpio_direction_db_input;
	}

	return XGpio_DiscreteRead(&g_gpio_lcd_interface, gpio_channel_1) & 0x0f;
}
