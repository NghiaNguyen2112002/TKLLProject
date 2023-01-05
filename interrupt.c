#include "interrupt.h"

void INT_Init(void)
{
	INTCONbits.GIE = 1;  //enable global interrupt
	RCONbits.IPEN = 1;   //enable interrupt priority
	INTCONbits.PEIE = 1; //Peripheral External Interupt Enable
}


#pragma code InterruptVectorLow		= 0x18
void InterruptVectorLow(void)
{
	_asm
		GOTO	low_isr
	_endasm
}

#pragma code InterruptVectorHigh 	= 0x08
void InterruptVectorHigh(void)
{
	_asm
		GOTO	high_isr
	_endasm
}

#pragma code
#pragma interruptlow	low_isr
void low_isr(void)
{
	if (INTCONbits.T0IF) // timer0
	{
		TMR0_Stop();
		INTCONbits.T0IF = 0;
		TMR0_Start();
		TMR0_Isr();
	}

	if (PIR1bits.TMR1IF) // timer1
	{
		TMR1_Stop();
		PIR1bits.TMR1IF = 0;
		TMR1_Start();	
		TMR1_Isr();
	}
    if(PIR1bits.RCIF){  //uart receive isr
        PIR1bits.RCIF = 0;
        UART_Isr(); 
    }
}

#pragma code
#pragma interrupt		high_isr
void high_isr(void)
{
	if (INTCONbits.T0IF) // timer0
	{
		TMR0_Stop();
		INTCONbits.T0IF = 0;
		TMR0_Start();
		TMR0_Isr();
	}

	if (PIR1bits.TMR1IF) // timer1
	{
		TMR1_Stop();
		PIR1bits.TMR1IF = 0;
		TMR1_Start();	
		TMR1_Isr();
	}
    if(PIR1bits.RCIF){  //uart receive isr
        PIR1bits.RCIF = 0;
        UART_Isr(); 
    }
}

