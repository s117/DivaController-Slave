// Copyright (C) 2016 S117 <admin@0x10c.pw>
// Bitman Lab.
// Licensed under the terms of the GNU GPL, version 2
// http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt

#define __C_LCD_1602_DRIVER_4BIT
#include "lcd_1602_driver_4bit.h"
#include "bsp.h"

#define DELAY_tAS    100
#define DELAY_tDSW   100
#define DELAY_tDDR   100
#define DELAY_tAH    100
#define DELAY_tH     100

#define enable_lcd() do{ \
	lcd_1602_4bit_udelay(DELAY_tAS); \
	lcd_1602_4bit_set_en(); \
	lcd_1602_4bit_udelay(DELAY_tDSW); \
	lcd_1602_4bit_clr_en(); \
	lcd_1602_4bit_udelay(DELAY_tH); \
}while(0) \

static inline void wait_lcd_busy() {
	char db70 = 0xff;
	lcd_1602_4bit_write_ctrl(0x01);
	while (db70 & 0x80){
		lcd_1602_4bit_udelay(DELAY_tAS);
		lcd_1602_4bit_set_en();
		lcd_1602_4bit_udelay(DELAY_tDDR);
		db70 = lcd_1602_4bit_read_db() << 4;
		lcd_1602_4bit_clr_en();
		lcd_1602_4bit_udelay(DELAY_tAH);

		lcd_1602_4bit_udelay(DELAY_tAS);
		lcd_1602_4bit_set_en();
		lcd_1602_4bit_udelay(DELAY_tDDR);
		db70 |= lcd_1602_4bit_read_db()&0xf;
		lcd_1602_4bit_clr_en();
		lcd_1602_4bit_udelay(DELAY_tH);
	}
}


static inline void lcd_commit(uint8_t dat, uint8_t is_data) {
	wait_lcd_busy();

	// R/W = 0 (write mode)
	// RS: 0-instruction, 1-data
	if (is_data)
		lcd_1602_4bit_write_ctrl(0x2);
	else
		lcd_1602_4bit_write_ctrl(0x0);

	// send higher 4 bit first
	lcd_1602_4bit_write_db((dat >> 4) & 0x0f);
	// send a EN pulse
	enable_lcd();

	// send lower 4 bit now
	lcd_1602_4bit_write_db(dat & 0x0f);
	// send a EN pulse
	enable_lcd();
}

int lcd_initialize() {
	if (lcd_1602_4bit_bsp_init())
		return -1;

	lcd_1602_4bit_write_ctrl(0x0);
	lcd_1602_4bit_write_db(0x3);

	lcd_1602_4bit_udelay(20000); // wait 15ms
	enable_lcd();

	lcd_1602_4bit_udelay(5000); // wait 4.1ms
	enable_lcd();

	lcd_1602_4bit_udelay(500);  // wait 100us
	enable_lcd();

	lcd_1602_4bit_udelay(2000); // wait 2ms
	lcd_1602_4bit_write_ctrl(0x0);
	lcd_1602_4bit_write_db(0x2);
	enable_lcd();

	lcd_commit(0x28, 0);        // set 4-bit mode
	lcd_commit(0x08, 0);        // display off
	lcd_commit(0x01, 0);        // clear display and move cursor to home
	lcd_commit(0x06, 0);        // cursor: increment, shift: false
	lcd_commit(0x0c, 0);        // display on, no cursor, no blink
	return 0;
}

//送入显示的数据
void lcd_writes(uint8_t addr, const char *str) { //函数参数说明，a你要送入的地址，看下图 如果是在第一行第二个显示就是0x01，*str这个就填你要显示的字
	//lcd_commit(0x02, 0);
	lcd_commit(addr | (1<<7), 0);
	while (*str != '\0') {
		lcd_commit(*str++, 1);  //因为送入的是显示的字所以第二个参数为1
	}
}

void lcd_write(uint8_t addr, char ch) {
	lcd_commit(0x02, 0);
	lcd_commit(addr | 0x80, 0);
	lcd_commit(ch, 1);
}

void lcd_add_font(uint8_t cgram_baseaddr, const uint8_t *bitmask) {
	int line;
//	for(line = 0;line < 8;++line){
//		udelay(50000);
//		lcd_commit_directly(cgram_baseaddr++, 0);
//		udelay(50000);
//		lcd_commit_directly(*bitmask++, 1);
//	}
	cgram_baseaddr &= ~(1<<7);
	cgram_baseaddr |= (1<<6);
	lcd_commit(cgram_baseaddr, 0);
	for(line = 0;line < 8;++line){
		lcd_commit(*bitmask++, 1);
	}
}
