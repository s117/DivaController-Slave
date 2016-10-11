// Copyright (C) 2016 S117 <admin@0x10c.pw>
// Bitman Lab.
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt

#ifndef __H_LCD_1602_DRIVER_4BIT
#define __H_LCD_1602_DRIVER_4BIT

#include <stdint.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef __C_LCD_1602_DRIVER_4BIT
#define EXTERN extern
#else
#define EXTERN
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define LCD_1602_4BIT_PIN_MASK_EN  (1<<6)
#define LCD_1602_4BIT_PIN_MASK_RS  (1<<5)
#define LCD_1602_4BIT_PIN_MASK_RW  (1<<4)
#define LCD_1602_4BIT_PIN_MASK_DB7 (1<<3)
#define LCD_1602_4BIT_PIN_MASK_DB6 (1<<2)
#define LCD_1602_4BIT_PIN_MASK_DB5 (1<<1)
#define LCD_1602_4BIT_PIN_MASK_DB4 (1<<0)
#define LCD_1602_4BIT_PIN_MASK_DB  (LCD_BITMASK_DB7|LCD_BITMASK_DB6|LCD_BITMASK_DB5|LCD_BITMASK_DB4)
// add function here
EXTERN int lcd_initialize();
EXTERN void lcd_writes(uint8_t a, const char *str);
EXTERN void lcd_write(uint8_t addr, char ch);
EXTERN void lcd_add_font(uint8_t cgrom_addr, const uint8_t *bitmask);

#ifdef __cplusplus
}
#endif

#undef EXTERN

#endif
