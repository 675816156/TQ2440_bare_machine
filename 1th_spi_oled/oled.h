#ifndef _OLED_H
#define _OLED_H

/* GPF3  - OLED_DC
 * GPG1  - OLED_CSn
 * GPG10 - FLASH_CSn
 * GPE13 - SPICLK
 * GPE12 - SPIMOSI
 * GPE11 - SPIMISO
 */
/* OLED need  OLED_CSn  OLED_DC SPICLK SPIMOSI */
/* FLASH need FLASH_CSn SPIMISO SPICLK SPIMOSI*/
#if 0
#include "oledfont.h"
#else
#include "pic.h"
#endif

#define GPECON (*((volatile unsigned long *)0x56000040))
#define GPEDAT (*((volatile unsigned long *)0x56000044))
#define GPEUP  (*((volatile unsigned long *)0x56000048))

#define GPFCON (*((volatile unsigned long *)0x56000050))
#define GPFDAT (*((volatile unsigned long *)0x56000054))
#define GPFUP  (*((volatile unsigned long *)0x56000058))

#define GPGCON (*((volatile unsigned long *)0x56000060))
#define GPGDAT (*((volatile unsigned long *)0x56000064))
#define GPGUP  (*((volatile unsigned long *)0x56000068))

#define BIT(x) (1 << (x))

extern void spi_gpio_init(void);
extern void oled_init(void);
extern void oled_puts(int, int, char *);

#endif
