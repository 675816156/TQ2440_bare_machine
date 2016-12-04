#include "uart.h"
#include "tq2440.h"
#include "led.h"

typedef volatile unsigned int regtype;

void sdram_init(void)
{
	int i;
	unsigned int mem_cfgs[] = {
		0x22011110,
		0x700,
		0x700,
		0x700,
		0x700,
		0x700,
		0x700,
		0x18005,
		0x18005,
		0xac03f4,
		0x31,
		0x30,
		0x30,
	};
	regtype *memcfg_regbase = (regtype *)0x48000000;
	for (i = 0; i < sizeof(mem_cfgs)/sizeof(mem_cfgs[0]); ++i) {
		memcfg_regbase[i] = mem_cfgs[i];
	}
}

//int printf(const char *format, ...);
void main(void)
{
	int i;
	sdram_init();
	uart_init();
	flash_init();
	char buf[] = "Hello This is for flash test String!\n";
	char rd_buf[256];
	flash_erase(0x0);
	flash_write(0x0, buf, sizeof(buf));
	flash_read(0x0, rd_buf, sizeof(buf));
	while (1) {
		for (i = 0; i < sizeof(buf); ++i) {
			if (rd_buf[i] == '\n')
				putc('\r');
			putc(rd_buf[i]);
		}
		//printf("Hello world !!!\n");
		;
		led_shine();
	}
	return;
}
