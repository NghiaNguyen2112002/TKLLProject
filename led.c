#include "led.h"

unsigned char arrayMapOfOutput [8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

void InitLed(void){
    TRIS_LED = 0x00;
    PORT_LED = 0x00;
}

void OpenOutput(int index)
{
    if (index >= 0 && index <= 7)
    {
        PORT_LED = PORT_LED | arrayMapOfOutput[index];
    }
}

void CloseOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		PORT_LED = PORT_LED & ~arrayMapOfOutput[index];
	}
}