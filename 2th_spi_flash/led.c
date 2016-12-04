#include "led.h"

void led_shine(void)
{
	int i, j, k;
	GPBCON = 0x015551;
	GPBUP  = 0x7ff;
	GPBDAT |= ((1 << 5) | (1 << 6) | (1 << 7) | (1 << 8));
	for (i = 0; i < 10; ++i) {
		for (k = 1; k < 5; ++k) {
			led_on(k);
			for (j = 0; j < 0xFFFFF; ++j);
		}
		for (k = 4; k > 0; --k) {
			led_off(k);
			for (j = 0; j < 0xFFFFF; ++j);
		}
	}
}
