#include "led.h"
#include <xc.h>



void LED_Off(LED led)
{
    LATA &= ~0x10;
    return;
}

void LED_On(LED led)
{
    LATA |= 0x10;
    return;
}

