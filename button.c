#include "button.h"

unsigned char   buttonInputMask[NO_OF_BUTTONS] = {0x01, 0x02, 0x04,
                                                  0x08, 0x10, 0x20};

static unsigned char buttonBuffer[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED,
                                                    BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};

static unsigned char debounceButtonBuffer1[NO_OF_BUTTONS];
static unsigned char debounceButtonBuffer2[NO_OF_BUTTONS];

static unsigned char flagForButtonPress[NO_OF_BUTTONS];
static unsigned char flagForButtonPress_s[NO_OF_BUTTONS];
static unsigned int counterForButtonPress[NO_OF_BUTTONS];

void InitButtonReading(void){
    TRIS_BUTTON = 0xFF;
    PORT_BUTTON = 0xFF;
}

void button_reading(void){
    unsigned char i;
    for(i = 0; i < NO_OF_BUTTONS; i++){
        debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
        debounceButtonBuffer1[i] = PORT_BUTTON & buttonInputMask[i];
        
        if(debounceButtonBuffer1[i] == debounceButtonBuffer2[i]){
            buttonBuffer[i] = debounceButtonBuffer1[i];
            if(buttonBuffer[i] == BUTTON_IS_PRESSED){
                if(counterForButtonPress[i] < DURATION_FOR_AUTO_INCREASING){
                    counterForButtonPress[i]++;
                }
                else flagForButtonPress_s[i] = 1;
            }
            else{
//              releasing button after press => pressed flag = 1
                flagForButtonPress[i] = counterForButtonPress[i] && (!flagForButtonPress_s[i]);
                counterForButtonPress[i] = 0;
                flagForButtonPress_s[i] = 0;
            }
        }
    }
}


unsigned char is_button_held(unsigned char index){
    if(index >= NO_OF_BUTTONS) return 0;
    
    return counterForButtonPress[index];
}

unsigned char is_button_pressed(unsigned char index){
    unsigned char result = flagForButtonPress[index];
    if(index >= NO_OF_BUTTONS) return 0;
    
    flagForButtonPress[index] = 0;
    return result; 
}
unsigned char is_button_pressed_s(unsigned char index){
    if(index >= NO_OF_BUTTONS) return 0xFF;
    return flagForButtonPress_s[index]; 
    ///reading invoked every 10ms
}



