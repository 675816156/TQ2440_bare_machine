#include "tq2440.h"

static void flash_gpio_init(void)
{
	GPEDAT &= ~BIT(11); /* SPIMISO input 0 */
	GPEDAT &= ~BIT(12); /* SPIMOSI output 0 */
	GPEDAT &= ~BIT(13); /* SPICLK  output 0 */

	GPECON &= ~(3 << (2 * 11) | 3 << (2 * 12) | 3 << (2 * 13));
	GPECON |=  (1 << (2 * 12) | 1 << (2 * 13));

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

static unsigned char spi_rdwr_byte(int read, unsigned char byte)
{
	int i;

	if (read)
		byte = 0;

	for (i = 0; i < 8; ++i) {
		if (read) {
			byte <<= 1;
			/* rising edge of CLK */
			GPEDAT &= ~BIT(13);
			byte |= ((GPEDAT & BIT(11)) ? 1 : 0);
			GPEDAT |= BIT(13);
		} else {
			/* rising edge of CLK */
			GPEDAT &= ~BIT(13);
			if (byte & 0x80)
				GPEDAT |= BIT(12);
			else
				GPEDAT &= ~BIT(12);
			GPEDAT |= BIT(13);
			byte <<= 1;
		}
	}

	return byte;
}

#define SPI_WRITE (0)
#define SPI_READ  (1)
static void spi_send_address(unsigned int addr)
{
	spi_rdwr_byte(SPI_WRITE, (addr >> 16) & 0xFF);
	spi_rdwr_byte(SPI_WRITE, (addr >> 8) & 0xFF);
	spi_rdwr_byte(SPI_WRITE, (addr >> 0) & 0xFF);
}

void puthex(unsigned char);
static void get_flash_id(void)
{
	unsigned char val;
	/* spi flash chip select */
	GPGDAT &= ~BIT(10);
	spi_rdwr_byte(SPI_WRITE, 0x90);
	spi_send_address(0x0);
	val = spi_rdwr_byte(SPI_READ, 0x0);
	puthex(val);
	val = spi_rdwr_byte(SPI_READ, 0x0);
	GPGDAT |= BIT(10);
	puthex(val);
}

static unsigned char read_flash_reg(unsigned char cmd)
{
	unsigned char val;
	/* spi flash chip select */
	GPGDAT &= ~BIT(10);
	spi_rdwr_byte(SPI_WRITE, cmd);
	val = spi_rdwr_byte(SPI_READ, 0x0);
	GPGDAT |= BIT(10);
	return val;
}

static void wait_for_flash_finish(void)
{
	while (read_flash_reg(0x05) & 0x1);
}

static void flash_write_enable(unsigned char enable)
{
	/* spi flash chip select */
	GPGDAT &= ~BIT(10);
	if (enable)
		spi_rdwr_byte(SPI_WRITE, 0x06);
	else
		spi_rdwr_byte(SPI_WRITE, 0x04);
	GPGDAT |= BIT(10);
}

static void flash_update_reg(unsigned char reg1, unsigned char reg2)
{
	flash_write_enable(1);
	/* spi flash chip select */
	GPGDAT &= ~BIT(10);
	spi_rdwr_byte(SPI_WRITE, 0x01);
	spi_rdwr_byte(SPI_WRITE, reg1);
	spi_rdwr_byte(SPI_WRITE, reg2);
	GPGDAT |= BIT(10);
	wait_for_flash_finish();
}

static void clear_status_reg_protect(void)
{
	unsigned char reg1, reg2;
	reg1 = read_flash_reg(0x05);
	reg2 = read_flash_reg(0x35);
	reg1 &= ~(1 << 7);
	reg2 &= ~(1 << 0);
	flash_update_reg(reg1, reg2);
}

static void clear_data_protect(void)
{
	unsigned char reg1, reg2;
	reg1 = read_flash_reg(0x05);
	reg2 = read_flash_reg(0x35);
	reg1 &= ~(7 << 2);
	reg2 &= ~(1 << 6);
	flash_update_reg(reg1, reg2);
}

void flash_read(unsigned int addr, char *buf, int len)
{
	int i;
	/* spi flash chip select */
	GPGDAT &= ~BIT(10);
	spi_rdwr_byte(SPI_WRITE, 0x03);
	spi_send_address(addr);
	for (i = 0; i < len; ++i) {
		buf[i] = spi_rdwr_byte(SPI_READ, 0x0);
	}
	GPGDAT |= BIT(10);
}

/* 4k */
void flash_erase(unsigned int addr)
{
	flash_write_enable(1);
	/* spi flash chip select */
	GPGDAT &= ~BIT(10);
	spi_rdwr_byte(SPI_WRITE, 0x20);
	spi_send_address(addr);
	GPGDAT |= BIT(10);
	wait_for_flash_finish();
}

void flash_write(unsigned int addr, char *buf, int len)
{
	int i;
	flash_write_enable(1);
	/* spi flash chip select */
	GPGDAT &= ~BIT(10);
	spi_rdwr_byte(SPI_WRITE, 0x02);
	spi_send_address(addr);
	for (i = 0; i < len; ++i)
		spi_rdwr_byte(SPI_WRITE, buf[i]);
	GPGDAT |= BIT(10);
	wait_for_flash_finish();
}

void flash_init(void)
{
	flash_gpio_init();
	clear_status_reg_protect();
	clear_data_protect();
}
