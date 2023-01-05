#ifndef _TIMER_H_
#define _TIMER_H_

#include <p18f4620.h>

#include "button.h"

void TMR0_Init(unsigned int count);
void TMR0_Start(void);
void TMR0_Stop(void);

void TMR1_Init(unsigned int count);
void TMR1_Start(void);
void TMR1_Stop(void);


void TMR0_Isr(void);
void TMR1_Isr(void);

// xay dung ham cap cao cho timer, nhung ham se duoc lop user goi
void TMR0_Set_ms(int time);
void TMR1_Set_ms(int time);



extern unsigned char flag_timer0,flag_timer1;
// extern la khai bao bien toan cuc chuong trinh, khong duoc gan gia tri

#endif