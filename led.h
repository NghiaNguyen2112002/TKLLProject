/* 
 * File:   led.h
 * Author: DELL
 *
 * Created on November 2, 2022, 4:40 PM
 */

#ifndef LED_H
#define	LED_H

#include <p18f4620.h>

#define TRIS_LED    TRISD
#define PORT_LED    PORTD

void LED_Init(void);
void LED_Open(int index);
void LED_Close(int index);


#endif	/* LED_H */

