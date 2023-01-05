#include "led7seg.h"

//RB:   7 6 5 4    3 2 1 0
//led:  0 g f e    d c b a
//LED ANODE
unsigned char LEDDisplay[10] = {0x40, 0x79, 0x24, 0x30, 
                                0x19, 0x12, 0x02, 0x78, 
                                0x00, 0x10};
void LED7_Init(void){
    TRIS_7SEG = 0x00;
    PORT_7SEG = 0x00;
}

void LED7_Display(unsigned char number){
    if(number <= 9){
        PORTB = LEDDisplay[number];
    }
}

