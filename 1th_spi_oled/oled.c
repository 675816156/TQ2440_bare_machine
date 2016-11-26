#include "oled.h"

#define oled_send_cmd(cmd) oled_send_byte(1, (cmd))
#define oled_send_dat(dat) oled_send_byte(0, (dat))

void spi_gpio_init(void)
{
	GPEDAT &= ~BIT(11); /* SPIMISO output 0 */
	GPEDAT &= ~BIT(12); /* SPIMOSI output 0 */
	GPEDAT &= ~BIT(13); /* SPICLK  output 0 */

	GPECON &= ~(3 << (2 * 11) | 3 << (2 * 12) | 3 << (2 * 13));
	GPECON |= (1 << (2 * 12) | 1 << (2 * 13));

	//GPEUP |= (BIT(11) | BIT(12) | BIT(13)); /* disable pull up */

	GPFDAT &= ~BIT(3); /* OLED_DC output 0 */

	GPFCON &= ~(3 << (2 * 3));
	GPFCON |= (1 << (2 * 3));

	//GPFUP |= (BIT(3)); /* disable pull up */

	GPGDAT |= BIT(1); /* OLED_CSn  deselect */
	GPGDAT |= BIT(10); /* FLASH_CSn deselect */

	GPGCON &= (3 << (2 * 1) | 3 << (2 * 10));
	GPGCON |= (1 << (2 * 1) | 1 << (2 * 10));
	
	//GPGUP |= (BIT(1) | BIT(10)); /* disable pull up */
}

void select_oled(unsigned char sel)
{
	if (sel)
		GPGDAT &= ~BIT(1);
	else
		GPGDAT |= BIT(1);
}

void oled_data_cmd(unsigned char cmd)
{
	if (cmd)
		GPFDAT &= ~BIT(3);
	else
		GPFDAT |= BIT(3);
}

void oled_send_byte(char iscmd, unsigned char byte)
{
	int i;
	select_oled(1);
	oled_data_cmd(iscmd);
	for (i = 0; i < 8; ++i) {
		GPEDAT &= ~BIT(13); /* clk low */		
		if (byte & 0x80)
			GPEDAT |= BIT(12);
		else
			GPEDAT &= ~BIT(12);
		GPEDAT |= BIT(13);  /* clk high */
		byte <<= 1;
	}
	select_oled(0);
}

void oled_set_position(int page, int col)
{
	oled_send_cmd(0xB0 + page); /* page address */
	oled_send_cmd(col & 0xF); /* Lower Column Start Address */
	oled_send_cmd(0x10 + (col >> 4)); /* Lower Higher Start Address */
}

void oled_clear(void)
{
	int page, col;
	for (page = 0; page < 8; page++) {
		for (col = 0; col < 128; col++) {
			oled_set_position(page, col);
			oled_send_dat(0x0);
		}
	}
}

void oled_init(void)
{
	/* 向OLED发命令以初始化 */
	oled_send_cmd(0xAE); /*display off*/ 
	oled_send_cmd(0x00); /*set lower column address*/ 
	oled_send_cmd(0x10); /*set higher column address*/ 
	oled_send_cmd(0x40); /*set display start line*/ 
	oled_send_cmd(0xB0); /*set page address*/ 
	oled_send_cmd(0x81); /*contract control*/ 
	oled_send_cmd(0x66); /*128*/ 
	oled_send_cmd(0xA1); /*set segment remap*/ 
	oled_send_cmd(0xA6); /*normal / reverse*/ 
	oled_send_cmd(0xA8); /*multiplex ratio*/ 
	oled_send_cmd(0x3F); /*duty = 1/64*/ 
	oled_send_cmd(0xC8); /*Com scan direction*/ 
	oled_send_cmd(0xD3); /*set display offset*/ 
	oled_send_cmd(0x00); 
	oled_send_cmd(0xD5); /*set osc division*/ 
	oled_send_cmd(0x80); 
	oled_send_cmd(0xD9); /*set pre-charge period*/ 
	oled_send_cmd(0x1f); 
	oled_send_cmd(0xDA); /*set COM pins*/ 
	oled_send_cmd(0x12); 
	oled_send_cmd(0xdb); /*set vcomh*/ 
	oled_send_cmd(0x30); 
	oled_send_cmd(0x8d); /*set charge pump enable*/ 
	oled_send_cmd(0x14); 

	/* oled set page address mode */
	oled_send_cmd(0x20);
	oled_send_cmd(0x02);

	oled_clear();

	oled_send_cmd(0xAF); /* display on */
}

#if 0
/* show 8x16 ASCII */
void oled_puts(int page, int col, char *s)
{
	int i;
	while (*s) {
		const unsigned char *dots = oled_asc2_8x16[*s++ - ' '];
		oled_set_position(page, col);
		for (i = 0; i < 16; ++i) {
			if (i == 8) {
				oled_set_position(page + 1, col);
			}
			oled_send_dat(dots[i]);
		}
		col += 8;
		if (col > 127) {
			col = 0;
			page += 2;
		}
	}
}
#else
void show_pic(void)
{
	int i;
	int col = 0;
	int page = 0;
	oled_set_position(page, col);
	for (i = 0; i < 64 * 128 / 8; ++i) {
		if (i && (i % 128 == 0)) {
			oled_set_position(++page, col);
		}
		oled_send_dat(gImage_n[i]);
	}
}
#endif
