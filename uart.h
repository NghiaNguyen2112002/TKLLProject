/* 
 * File:   uart.h
 * Author: DELL
 *
 * Created on November 29, 2022, 5:49 PM
 */

#ifndef UART_H
#define	UART_H

#include <p18f4620.h>
#include "rfid.h"

extern unsigned char dataReceived;
void UART_Init(void);
void UART_Isr(void);
void UART_PutChar(unsigned char data);

void UART_ClearBuffer(void);


#endif	/* UART_H */

