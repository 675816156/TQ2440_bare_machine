#ifndef _SPI_FLASH_H
#define _SPI_FLASH_H

void flash_read(unsigned int addr, char *buf, int len);
void flash_write(unsigned int addr, char *buf, int len);
void flash_erase(unsigned int addr);
void flash_init(void);
#endif
