/* 
 * File:   led7seg.h
 * Author: DELL
 *
 * Created on October 11, 2022, 5:13 PM
 */

#ifndef LED7SEG_H
#define	LED7SEG_H
#include <p18f4620.h>

#define TRIS_7SEG TRISB
#define PORT_7SEG PORTB

void LED7_Init(void);
void LED7_Display(unsigned char number);


#endif	/* LED7SEG_H */

