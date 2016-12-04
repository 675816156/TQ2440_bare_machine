#ifndef _LED_H
#define _LED_H

#include "tq2440.h"

#define led_off(n) (GPBDAT |= (1 << (n + 4)))
#define led_on(n)  (GPBDAT &= ~(1 << (n + 4)))

void led_shine(void);
#endif
