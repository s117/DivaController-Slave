#include <stdio.h>
#include "platform.h"
#include "xgpio.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xdsdac.h"
#include "lcd_1602/lcd_1602_driver_4bit.h"

void print(char *str);

// LCD 1602 related defines
#define LCD_FONT_OPEN_CHAR  (0)
#define LCD_FONT_LINK_CHAR  (1)
#define LCD_FONT_DOWN_CHAR  (2)
#define LCD_FONT_UP_CHAR    (3)
#define LCD_FONT_TRIG_CHAR  (4)
#define LCD_FONT_CIRC_CHAR  (5)
#define LCD_FONT_CROS_CHAR  (6)
#define LCD_FONT_SQRE_CHAR  (7)
#define LCD_FONT_LEFT_CHAR  (0x7f)
#define LCD_FONT_RIGHT_CHAR (0x7e)

const uint8_t LCD_FONT_TRIG[8] = { 0x04, 0x04, 0x0A, 0x0A, 0x11, 0x1F, 0x00,
		0x00 };
const uint8_t LCD_FONT_CIRC[8] = { 0x00, 0x0E, 0x11, 0x11, 0x11, 0x0E, 0x00,
		0x00 };
const uint8_t LCD_FONT_CROS[8] = { 0x00, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x00,
		0x00 };
const uint8_t LCD_FONT_SQRE[8] = { 0x00, 0x1F, 0x11, 0x11, 0x11, 0x1F, 0x00,
		0x00 };
const uint8_t LCD_FONT_DOWN[8] = { 0x00, 0x04, 0x04, 0x15, 0x0E, 0x04, 0x00,
		0x00 };
const uint8_t LCD_FONT_UP[8] =
		{ 0x00, 0x04, 0x0E, 0x15, 0x04, 0x04, 0x00, 0x00 };
const uint8_t LCD_FONT_OPEN[8] = { 0x00, 0x00, 0x02, 0x04, 0x08, 0x13, 0x00,
		0x00 };
const uint8_t LCD_FONT_LINK[8] = { 0x00, 0x00, 0x00, 0x00, 0x0E, 0x13, 0x00,
		0x00 };

const uint8_t TBL_LCD_BTN_FONT_CGRAM_ADDR[8] = { 0x40, 0x48, 0x50, 0x58, 0x60,
		0x68, 0x70, 0x78 };

const uint8_t TBL_HOST_INST_TO_LCD_OFFSET[] = { 0, 0x9, 0xb, 0xd, 0xf, 0, 0,
		0x1, 0x3, 0x5, 0x7, 0, 0, 0x42, 0x46, 0x4a, 0x4e, 0, 0, 0 };

// DS4 controlling related defines
#define DAC_MAX (255)
#define DAC_MID (127)
#define DAC_MIN (0)

#define GPIO_DIR_OUT_ALL (0)
#define GPIO_CHANNEL_1  (1)

enum AXIS_ID {
	L_X = (1 << 5),
	L_Y = (1 << 4),
	R_X = (1 << 3),
	R_Y = (1 << 2),
	LT = (1 << 1),
	RT = (1 << 0),
};

#define BTN_ID_TO_GPIO_PIN(ID) (15-(ID))

enum BUTTON_ID {
	DOWN = (0),
	LEFT = (1),
	UP = (2),
	RIGHT = (3),
	TRIG = (4),
	CIRC = (5),
	CROS = (6),
	SQRE = (7),
	L1_N = (8),
	LS_N = (9),
	R1_N = (10),
	RS_N = (11),
	PS = (12),
	RESERVED = (13),
};

enum HOST_KEY_CODE {
	HOST_PS = 0,
	HOST_BTN_TRIANGLE,
	HOST_BTN_CIRCLE,
	HOST_BTN_CROSS,
	HOST_BTN_SQUARE,
	HOST_SHLD_L1,
	HOST_SHLD_R1,
	HOST_DPAD_UP,
	HOST_DPAD_RIGHT,
	HOST_DPAD_DOWN,
	HOST_DPAD_LEFT,
	HOST_STICK_LS,
	HOST_STICK_RS,
	HOST_STICK_L_X_ANALOG,
	HOST_STICK_L_Y_ANALOG,
	HOST_STICK_R_X_ANALOG,
	HOST_STICK_R_Y_ANALOG,
	HOST_TRIGR_LT_ANALOG,
	HOST_TRIGR_RT_ANALOG,
	HOST_RESERVED,
};

static const int TBL_KEYCODE_TRANSFORM[] = { PS, TRIG, CIRC, CROS, SQRE, L1_N,
		R1_N, UP, RIGHT, DOWN, LEFT, LS_N, RS_N, L_X, L_Y, R_X, R_Y, LT, RT,
		RESERVED, };

struct DS4_Data {
	XGpio gpio_btn;
	XGpio gpio_dac_read_en;
	XDsDac dac_L_X;
	XDsDac dac_L_Y;
	XDsDac dac_R_X;
	XDsDac dac_R_Y;
	XDsDac dac_LT;
	XDsDac dac_RT;
};
struct DS4_Data g_ds4_data;

void delay() {
	unsigned int c = 0;
	for (c = 0; c < 0xff; ++c)
		;
}

static void inline udelay(u32 us) {
	int i;
	while (us > 0) {
		for (i = 0; i < 8; ++i)
			;
		--us;
	}
}

void update_axis(struct DS4_Data* data, int axis_id, u16 val) {
	XDsDac *target;
	switch (axis_id) {
	case L_X:
		target = &data->dac_L_X;
		break;
	case L_Y:
		target = &data->dac_L_Y;
		break;
	case R_X:
		target = &data->dac_R_X;
		break;
	case R_Y:
		target = &data->dac_R_Y;
		break;
	case LT:
		target = &data->dac_LT;
		break;
	case RT:
		target = &data->dac_RT;
		break;
	}
	XDsDac_WriteOutput(target, &val, 1);
	XDsDac_Start(target);
	delay();
	XGpio_DiscreteWrite(&data->gpio_dac_read_en, GPIO_CHANNEL_1, axis_id);
	delay();
	XGpio_DiscreteWrite(&data->gpio_dac_read_en, GPIO_CHANNEL_1, 0x00000000);
}

#define MASK_DEFAULT (0x0000ff0f)
static u32 current_set = MASK_DEFAULT;
void update_button(struct DS4_Data* data, int btn_id, int is_push) {
	//is_push = is_push || is_push; // set is_push to 0 or 1
	if (is_push)
		is_push = 1;
	if (!((L1_N <= btn_id) && (btn_id <= RS_N))) {
		is_push = !is_push;
	}
	//u32 current_set;
	//current_set = XGpio_DiscreteRead(&data->gpio_btn, GPIO_CHANNEL_1);
	if (is_push) {
		current_set |= (1 << BTN_ID_TO_GPIO_PIN(btn_id));
		XGpio_DiscreteWrite(&data->gpio_btn, GPIO_CHANNEL_1, current_set);
	} else {
		current_set &= ~(1 << BTN_ID_TO_GPIO_PIN(btn_id));
		XGpio_DiscreteWrite(&data->gpio_btn, GPIO_CHANNEL_1, current_set);
	}

}

int init_ds4(struct DS4_Data* data) {
	int rtn_val;

	rtn_val = XGpio_Initialize(&data->gpio_btn,
			XPAR_CTRL_BUTTONS_16BIT_DEVICE_ID);
	if (XST_SUCCESS != rtn_val) {
		print("Initialize gpio_ctrl_buttons_16bit fail.\n");
		return rtn_val;
	}
	XGpio_SetDataDirection(&data->gpio_btn, GPIO_CHANNEL_1, GPIO_DIR_OUT_ALL);
	XGpio_DiscreteWrite(&data->gpio_btn, GPIO_CHANNEL_1, MASK_DEFAULT);

	rtn_val = XGpio_Initialize(&data->gpio_dac_read_en,
			XPAR_CTRL_DAC_READ_EN_DEVICE_ID);
	if (XST_SUCCESS != rtn_val) {
		print("Initialize gpio_dac_read_en fail.\n");
		return rtn_val;
	}
	XGpio_SetDataDirection(&data->gpio_dac_read_en, GPIO_CHANNEL_1,
			GPIO_DIR_OUT_ALL);

	rtn_val = XDsDac_Initialize(&data->dac_L_X, XPAR_CTRL_STICK_L_X_DEVICE_ID);
	if (XST_SUCCESS != rtn_val) {
		print("Initialize dac_L_X fail.\n");
		return rtn_val;
	}
	XDsDac_Start(&data->dac_L_X);
	update_axis(data, L_X, DAC_MID);

	rtn_val = XDsDac_Initialize(&data->dac_L_Y, XPAR_CTRL_STICK_L_Y_DEVICE_ID);
	if (XST_SUCCESS != rtn_val) {
		print("Initialize dac_L_Y fail.\n");
		return rtn_val;
	}
	XDsDac_Start(&data->dac_L_Y);
	update_axis(data, L_Y, DAC_MID);

	rtn_val = XDsDac_Initialize(&data->dac_R_X, XPAR_CTRL_STICK_R_X_DEVICE_ID);
	if (XST_SUCCESS != rtn_val) {
		print("Initialize dac_R_X fail.\n");
		return rtn_val;
	}
	XDsDac_Start(&data->dac_R_X);
	update_axis(data, R_X, DAC_MID);

	rtn_val = XDsDac_Initialize(&data->dac_R_Y, XPAR_CTRL_STICK_R_Y_DEVICE_ID);
	if (XST_SUCCESS != rtn_val) {
		print("Initialize dac_R_Y fail.\n");
		return rtn_val;
	}
	XDsDac_Start(&data->dac_R_Y);
	update_axis(data, R_Y, DAC_MID);

	rtn_val = XDsDac_Initialize(&data->dac_LT, XPAR_CTRL_TRIGGER_LT_DEVICE_ID);
	if (XST_SUCCESS != rtn_val) {
		print("Initialize dac_LT fail.\n");
		return rtn_val;
	}
	XDsDac_Start(&data->dac_LT);
	update_axis(data, LT, DAC_MID);

	rtn_val = XDsDac_Initialize(&data->dac_RT, XPAR_CTRL_TRIGGER_RT_DEVICE_ID);
	if (XST_SUCCESS != rtn_val) {
		print("Initialize dac_RT fail.\n");
		return rtn_val;
	}
	XDsDac_Start(&data->dac_RT);
	update_axis(data, RT, DAC_MID);

	return rtn_val;
}

static inline u16 convert_val(char digit_3bit[]) {
	return ((digit_3bit[0] - '0') * 100 + (digit_3bit[1] - '0') * 10
			+ (digit_3bit[2] - '0'));
}

static inline int is_hex_number(char c) {
	if (!((('0' <= c) && (c <= '9')) || (('a' <= c) && (c <= 'f')))) {
		return 0;
	}
	return 1;
}

static inline u8 convert_val_hex(char c) {
	u8 rtnval;
	if (('0' <= c) && (c <= '9')) {
		rtnval = c - '0';
	} else if (('a' <= c) && (c <= 'f')) {
		rtnval = c - 'a' + 10;
	} else {
		return 0x10;
	}
	return rtnval;
}

static inline int is_uppercase_alpha(char c) {
	return (('A' <= c) && (c <= 'Z'));
}

static inline int is_decimal_number(char c) {
	return (('0' <= c) && (c <= '9'));
}

static inline int is_button_command(char inst) {
	return ((inst - 'A') < HOST_STICK_L_X_ANALOG);
}

char g_sprintf_buffer[16];

void lcd_update_display_btn(uint8_t code, uint8_t val) {
	switch (code) {
	case HOST_BTN_TRIANGLE:
	case HOST_BTN_CIRCLE:
	case HOST_BTN_CROSS:
	case HOST_BTN_SQUARE:
	case HOST_DPAD_UP:
	case HOST_DPAD_RIGHT:
	case HOST_DPAD_DOWN:
	case HOST_DPAD_LEFT:
		if (val)
			lcd_write(TBL_HOST_INST_TO_LCD_OFFSET[code], LCD_FONT_LINK_CHAR);
		else
			lcd_write(TBL_HOST_INST_TO_LCD_OFFSET[code], LCD_FONT_OPEN_CHAR);
		break;
	}

}

void convert_u8_to_hex(uint8_t val, char buffer[]) {
	uint8_t high = (val >> 4) & 0xf;
	uint8_t low = val & 0xf;
	if (high >= 0xa)
		buffer[0] = 'a' + high - 0xa;
	else
		buffer[0] = '0' + high;
	if (low >= 0xa)
		buffer[1] = 'a' + low - 0xa;
	else
		buffer[1] = '0' + low;
}

void lcd_update_display_stick(uint8_t code, uint8_t val) {
	if ((HOST_STICK_L_X_ANALOG <= code) && (code <= HOST_STICK_R_Y_ANALOG)) {
		convert_u8_to_hex(val, g_sprintf_buffer);
		//printf("%s\n", g_sprintf_buffer);
		lcd_write(TBL_HOST_INST_TO_LCD_OFFSET[code], g_sprintf_buffer[0]);
		lcd_write(TBL_HOST_INST_TO_LCD_OFFSET[code] + 1, g_sprintf_buffer[1]);
	}
}

int initialize_display() {
	if (lcd_initialize() != 0) {
		return -1;
	}

	print("LCD initialized.\n");

	lcd_add_font(TBL_LCD_BTN_FONT_CGRAM_ADDR[LCD_FONT_OPEN_CHAR],
			LCD_FONT_OPEN);
	lcd_add_font(TBL_LCD_BTN_FONT_CGRAM_ADDR[LCD_FONT_LINK_CHAR],
			LCD_FONT_LINK);
	lcd_add_font(TBL_LCD_BTN_FONT_CGRAM_ADDR[LCD_FONT_DOWN_CHAR],
			LCD_FONT_DOWN);
	lcd_add_font(TBL_LCD_BTN_FONT_CGRAM_ADDR[LCD_FONT_UP_CHAR], LCD_FONT_UP);
	lcd_add_font(TBL_LCD_BTN_FONT_CGRAM_ADDR[LCD_FONT_TRIG_CHAR],
			LCD_FONT_TRIG);
	lcd_add_font(TBL_LCD_BTN_FONT_CGRAM_ADDR[LCD_FONT_CIRC_CHAR],
			LCD_FONT_CIRC);
	lcd_add_font(TBL_LCD_BTN_FONT_CGRAM_ADDR[LCD_FONT_CROS_CHAR],
			LCD_FONT_CROS);
	lcd_add_font(TBL_LCD_BTN_FONT_CGRAM_ADDR[LCD_FONT_SQRE_CHAR],
			LCD_FONT_SQRE);

	print("LCD Font added.\n");

	lcd_writes(0, "                ");
	lcd_writes(40, "DS4 Auto Control");

	lcd_write(0, LCD_FONT_UP_CHAR);
	lcd_write(2, LCD_FONT_RIGHT_CHAR);
	lcd_write(4, LCD_FONT_DOWN_CHAR);
	lcd_write(6, LCD_FONT_LEFT_CHAR);
	lcd_write(8, LCD_FONT_TRIG_CHAR);
	lcd_write(10, LCD_FONT_CIRC_CHAR);
	lcd_write(12, LCD_FONT_SQRE_CHAR);
	lcd_write(14, LCD_FONT_CROS_CHAR);
	udelay(3000000);

	lcd_update_display_btn(HOST_BTN_TRIANGLE, 0);
	lcd_update_display_btn(HOST_BTN_CIRCLE, 0);
	lcd_update_display_btn(HOST_BTN_CROSS, 0);
	lcd_update_display_btn(HOST_BTN_SQUARE, 0);
	lcd_update_display_btn(HOST_DPAD_UP, 0);
	lcd_update_display_btn(HOST_DPAD_RIGHT, 0);
	lcd_update_display_btn(HOST_DPAD_DOWN, 0);
	lcd_update_display_btn(HOST_DPAD_LEFT, 0);

	lcd_writes(40, "LX00LY00RX00RY00");
	lcd_update_display_stick(HOST_STICK_L_X_ANALOG, DAC_MID);
	lcd_update_display_stick(HOST_STICK_L_Y_ANALOG, DAC_MID);
	lcd_update_display_stick(HOST_STICK_R_X_ANALOG, DAC_MID);
	lcd_update_display_stick(HOST_STICK_R_Y_ANALOG, DAC_MID);

	print("LCD displayed.\n");

	return 0;
}

int main() {
	char inst;
	char val_buf[3];
	int code;
	u16 val, val_tmp;
	u32 offset;

	val_buf[0] = '0';
	val_buf[1] = '0';
	val_buf[2] = '0';

	init_platform();
	if (init_ds4(&g_ds4_data) == XST_SUCCESS) {
		print("System initialized.\n");
	}

	initialize_display();

	while (1) {
		do { // read instruction byte
			inst = getchar();
			getchar();
			offset = (inst - 'A');
			if (offset >= HOST_RESERVED) { // invalid instruction, drop
				printf("invalid instruction:%c\n", inst);
				continue;
			}
			printf("read instruction:%d\n", inst);
			break;
		} while (1);

		do { // read value[0]
			val_buf[0] = getchar();
			getchar();
			val_tmp = convert_val_hex(val_buf[0]);
			if (val_tmp > 0xf) {
				printf("invalid value:%c\n", val_buf[0]);
				continue;
			}
			val = val_tmp << 4;
			printf("read value[0]:%c\n", val_buf[0]);
			break;
		} while (1);

		do { // read value[1]
			val_buf[1] = getchar();
			getchar();
			val_tmp = convert_val_hex(val_buf[1]);
			if (val_tmp > 0xf) {
				printf("invalid value:%c\n", val_buf[1]);
				continue;
			}
			val |= val_tmp;
			printf("read value[1]:%c\n", val_buf[1]);
			break;
		} while (1);

		code = TBL_KEYCODE_TRANSFORM[offset];

		if (is_button_command(inst)) {
			update_button(&g_ds4_data, code, val);
			lcd_update_display_btn(offset, val);
		} else {
			update_axis(&g_ds4_data, code, val);
			lcd_update_display_stick(offset, val);
		}
	}
	return 0;
}
