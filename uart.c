#include "uart.h"


unsigned char dataReceived;

void init_uart(void){
    SPBRGH = 0;          //Baud Rate Generator Register
//    sync = 0; BRG16 = 1; SPBRGH = 0;   
//    Baud Rate = F / [16(SPBRG + 1)]
//    internal F = 4 Mhz
    SPBRG = 25;     //Baud Rate 9600
    
   	TXSTAbits.TXEN = 1; //Transmit enabled
	TXSTAbits.BRGH = 0; //Low Speed
	BAUDCONbits.BRG16 = 1; //16-bit Baud Rate Generator
    
    TXSTAbits.SYNC = 0; //Asynchronous mode
	RCSTAbits.SPEN = 1; //Enables
	RCSTAbits.CREN = 1; //Enables receive
    
	PIR1bits.RCIF = 0;  // clear interrupt flag uart
	PIE1bits.RCIE = 1;	//enable interrupt uart
	IPR1bits.RCIP = 0;	// Priority: low
	TRISCbits.RC6 = 0; // RC6 output
	TRISCbits.RC7 = 1; // RC7 input
}

void uart_putchar(unsigned char data)
{
	while(PIR1bits.TXIF == 0);
        TXREG = data;
	PIR1bits.TXIF = 0;
}

void uart_isr(void){
    dataReceived = RCREG;
    IS_THIS_RFID_VERIFIED = IsRFIDVerified();
       
    uart_putchar(dataReceived);

}
