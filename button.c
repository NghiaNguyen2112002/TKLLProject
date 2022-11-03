#include "button.h"
#include "led7seg.h"

unsigned int key_code[6] = {0,0,0,0,0,0};

unsigned char arrayMaskOutputOfKey [3] = {0x10,0x20,0x40};
unsigned char arrayMaskInputOfKey [2] = {0x01,0x02};

void init_key_matrix()
{
	TRIS_BUTTON = 0x0F; 
	PORT_BUTTON = 0xFF;
}

void scan_key_matrix()
{
	int i,j;
	for(i=0;i<MAX_ROW;i++)     
	{
		PORT_BUTTON = ~arrayMaskOutputOfKey[i];
		for(j=0;j<MAX_COL;j++)
		{ 
			if((PORT_BUTTON & arrayMaskInputOfKey[j]) == 0)  {
				key_code[i*MAX_COL+j] = 1;
            }
			else
				key_code[i*MAX_ROW+j] = 0;   
		}
	}
}
