#ifndef _UART_H
#define _UART_H

/* leds GPB5 6 7 8 */

/* TXD0 - GPH2 */
/* RXD0 - GPH3 */

/*
Mpll = (2 * m * Fin) / (p * 2^s)
m = M (the value for divider M)+ 8, p = P (the value for divider P) + 2

MPLL Control Register
Mpll = (2 * m * Fin) / (p * 2^s)
m = (MDIV + 8), p = (PDIV + 2), s = SDIV
UPLL Control Register
Upll = (m * Fin) / (p * 2^s)
m = (MDIV + 8), p = (PDIV + 2), s = SDIV

If HDIVN is not 0, the CPU bus mode has to be changed from the fast bus mode to the asynchronous
bus mode using following instructions(S3C2440 does not support synchronous bus mode).
MMU_SetAsyncBusMode
mrc p15,0,r0,c1,c0,0
orr r0,r0,#R1_nF:OR:R1_iA
mcr p15,0,r0,c1,c0,0

PLL Value Selection Guide (MPLLCON)
1. Fout = 2 * m * Fin / (p*2^s),
   Fvco = 2 * m * Fin / p
where: m=MDIV+8, p=PDIV+2, s=SDIV
2. 600MHz <= Fvco <= 1.2GHz
3. 200MHz <= FCLKout <= 600MHz
4. Don't set the P or M value as zero, that is, setting the P=000000, M=00000000 can cause malfunction of
the PLL.
5. The proper range of P and M: 1 <= P <= 62, 1 <= M <= 248

M_MDIV  92 M_PDIV 1 M_SDIV 1 - FCLK = 400MHz,HCLK = 100Mhz,PCLK = 50Mhz
M_MDIV 125 M_PDIV 1 M_SDIV 1 - FCLK = 532Mhz,HCLK = 133Mhz,PCLK = 66.5Mhz
M_MDIV 124 M_PDIV 1 M_SDIV 1 - FCLK = 528Mhz,HCLK = 132Mhz,PCLK = 66Mhz

*/

void uart_init(void);
void putc(int);
void puthex(unsigned char);
unsigned char getc(void);
#endif
