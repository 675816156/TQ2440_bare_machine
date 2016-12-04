#ifndef _TQ2440_H
#define _TQ2440_H

#define RH(x)       (*((volatile unsigned int *)(x)))

#define BIT(x)      (1 << (x))

/* UART0 Registers */
#define ULCON0      RH(0x50000000)
#define UCON0       RH(0x50000004)
#define UFCON0      RH(0x50000008)
#define UMCON0      RH(0x5000000c)
#define UTRSTAT0    RH(0x50000010)
#define UBRDIV0     RH(0x50000028)

#define UTXH0_B     RH(0x50000023)
#define URXH0_B     RH(0x50000027)
#define UTXH0_L     RH(0x50000020)
#define URXH0_L     RH(0x50000024)

/* gpio-b */
#define GPBCON      RH(0x56000010)
#define GPBDAT      RH(0x56000014)
#define GPBUP       RH(0x56000018)
/* gpio-e */
#define GPECON RH(0x56000040)
#define GPEDAT RH(0x56000044)
#define GPEUP  RH(0x56000048)
/* gpio-f */
#define GPFCON RH(0x56000050)
#define GPFDAT RH(0x56000054)
#define GPFUP  RH(0x56000058)
/* gpio-g */
#define GPGCON RH(0x56000060)
#define GPGDAT RH(0x56000064)
#define GPGUP  RH(0x56000068)
/* gpio-h */
#define GPHCON  RH(0x56000070)
#define GPHDAT  RH(0x56000074)
#define GPHUP   RH(0x56000078)

#define LOCKTIME    RH(0x4c000000)
#define MPLLCON     RH(0x4c000004)
#define CLKDIVN     RH(0x4c000014)
/* if you change PCLK, please update this value */
#define PCLK (66500000)
#endif
