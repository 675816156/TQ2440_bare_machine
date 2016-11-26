/*
 * nLED		GPIO	GPBCON		GPBDAT	GPBUP
 * nLED1	GPB5	[11:10] 	[5]		[5]
 * nLED2	GPB6	[13:12]		[6]		[6]
 * nLED3	GPB7	[15:14]		[7]		[7]
 * nLED4	GPB8	[17:16]		[8]		[8]
 */

#define FUNC_MSK 	(0x3)
#define FUNC_OUT 	(0x1)
#define PULL_UP 	(0x0)
#define PULL_NO 	(0x1)

#define GPBCON	(*((volatile unsigned *)0x56000010))
#define GPBDAT	(*((volatile unsigned *)0x56000014))
#define GPBUP	(*((volatile unsigned *)0x56000018))

typedef volatile unsigned long regtype;
#if 0
void nled_on(int n, int on)
{
	int offset = n + 4;
	if (n < 1 || n > 4) return;
	if (on != 0 && on != 1) return;

	GPBCON &= ~(FUNC_MSK << (2 * offset));
	GPBCON |=  (FUNC_OUT << (2 * offset));
	GPBUP  &= ~(1 << offset);
	GPBUP  |=  (PULL_NO << offset);
	GPBDAT &= ~(1 << offset);
	if (!on)
		GPBDAT |=  (1 << offset);
}

void flow_led(void)
{
	int i = 0;
	nled_on(1, 0);
	nled_on(2, 0);
	nled_on(3, 0);
	nled_on(4, 0);
	while (1) {
		if (i == 0x1000000) {
			i = 0;
			nled_on(4, 0);
		}
		if (i == 0x800000) {
			nled_on(4, 1);
		}
		i++;
	}
}
#endif
void ram_init(void)
{
	int i;
	unsigned long mem_cfgs[] = {
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

#if 0
void OLEDInit(void);
void SPI_GPIO_Init(void);
void OLEDPrint(int, int, char *);
#else
void spi_gpio_init(void);
void oled_init(void);
void oled_puts(int page, int col, char *s);
#endif

void main(void)
{
	ram_init();
	spi_gpio_init();
	oled_init();
#if 0
	oled_puts(3, 64, "ChenJun Say Hello World!");
#else
	void show_pic(void);
	show_pic();
#endif
	return;
}
