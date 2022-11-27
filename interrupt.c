#include "interrupt.h"



void init_interrupt(void)
{
	INTCONbits.GIE = 1;  //enable global interrupt
	RCONbits.IPEN = 1;   //enable interrupt priority
//	INTCONbits.PEIE = 1; //Peripheral External Interupt Enable
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
		stop_timer0();
		INTCONbits.T0IF = 0;
		start_timer0();
		timer0_isr();
	}

	if (PIR1bits.TMR1IF) // timer1
	{
		stop_timer1();
		PIR1bits.TMR1IF = 0;
		start_timer1();	
		timer1_isr();
	}
}

#pragma code
#pragma interrupt		high_isr
void high_isr(void)
{
	if (INTCONbits.T0IF) // timer0
	{
		stop_timer0();
		INTCONbits.T0IF = 0;
		start_timer0();
		timer0_isr();
	}

	if (PIR1bits.TMR1IF) // timer1
	{
		stop_timer1();
		PIR1bits.TMR1IF = 0;
		start_timer1();	
		timer1_isr();
	}
}

