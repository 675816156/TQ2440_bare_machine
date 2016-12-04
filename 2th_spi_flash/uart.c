#include "tq2440.h"

#define UART_BAUD_RATE (115200)
#define UART_BRD ((PCLK  / (UART_BAUD_RATE * 16)) - 1)

#define TXD0READY   (1<<2)
#define RXD0READY   (1)

void putc(int c)
{
	UTXH0_L = (unsigned char)c;
	while (!(UTRSTAT0 & TXD0READY));
}

unsigned char getc(void)
{
	while (!(UTRSTAT0 & RXD0READY));
	return URXH0_L;
}

void puthex(unsigned char c)
{
	int i, val;
	for (i = 4; i >= 0; i -= 4) {
		val = (c >> i) & 0xF;
		if (val >= 0 && val <= 9)
			putc(val + '0');
		if (val >= 10 && val <= 15)
			putc(val - 10 + 'A');
	}
}

void uart_init(void)
{
	GPHCON &= ~(3 << (2 * 2) | 3 << (2 * 3));
	GPHCON |=  (2 << (2 * 2) | 2 << (2 * 3));

	GPHUP  |=  ((1 << 2) | (1 << 3));

	ULCON0  = 0x03;     // 8N1(8个数据位，无较验，1个停止位)
	UCON0   = 0x05;     // 查询方式，UART时钟源为PCLK
	UFCON0  = 0x00;     // 不使用FIFO
	UMCON0  = 0x00;     // 不使用流控
	/* UBRDIVn = (int)(selected clock / (baudrate x 16) ) –1 */
	UBRDIV0 = UART_BRD; // 波特率为115200
}
