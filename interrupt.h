#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <p18f4620.h>
#include "timer.h"
#include "uart.h"

void INT_Init(void);

void low_isr(void);
void high_isr(void);


#endif