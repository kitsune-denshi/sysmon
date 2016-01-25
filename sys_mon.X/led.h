/* 
 * File:   led.h
 * Author: robert
 *
 * Created on 15 January 2016, 20:44
 */

#ifndef LED_H
#define	LED_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum
{
    LED_NONE,
    LED_D1,
    LED_D2,
    LED_D3,
    LED_D4
} LED;

void LED_Off(LED led);

void LED_On(LED led);

#define LED_USB_DEVICE_STATE 4


#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */

