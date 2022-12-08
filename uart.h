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
void init_uart(void);
void uart_isr(void);
void uart_putchar(unsigned char data);

void Clear_UartBuffer(void);


#endif	/* UART_H */

