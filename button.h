#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <p18f4620.h>

#define PORT_BUTTON		PORTC
#define TRIS_BUTTON		TRISC


#define NO_OF_BUTTONS                       6
#define TRIS_BUTTON                         TRISC
#define PORT_BUTTON                         PORTC

#define DURATION_FOR_AUTO_INCREASING        200   //2s

#define BUTTON_IS_PRESSED                   0
#define BUTTON_IS_RELEASED                  1

void InitButtonReading(void);
void button_reading(void);

unsigned char is_button_held(unsigned char index);
unsigned char is_button_pressed(unsigned char index);
unsigned char is_button_pressed_s(unsigned char index);

#endif	/* BUTTON_H */
