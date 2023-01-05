#include "rfid.h"

// x3 rfid card with 12 digit
const unsigned char RFID[8][12] = {
    "4E005708E9F8",
    "4E0058D2F430",
    "4E005F6F1A64",
    "4E0056B3E54E",
    "4E0058715235",
    "4E005EDECB05",
    "4E00581F3039",
    "4E0057808910"
                    };

enum RFID_CARD_DETECT_FSM mode;
unsigned char rfidBuffer[12];
unsigned char counter;
            unsigned char i, j;
char IS_THIS_RFID_VERIFIED;


void RFID_Init(void){
    mode = INIT;
    counter = 0;
    IS_THIS_RFID_VERIFIED = -1;
}

char RFID_IsVerified(void){
    switch(mode){
        case INIT:
            if(dataReceived == START_RFID_HEX){
                mode = RECEIVE_DATA;
                counter = 0;
            }
            break;
        case RECEIVE_DATA:
            rfidBuffer[counter] = dataReceived;
            counter++;
            if(counter >= NO_OF_RFID_DIGITS){
                mode = DETECT_DATA;
            }
            break;
        case DETECT_DATA:
            for(i = 0; i < NO_OF_CARDS; i++){
                for(j = 0; j < NO_OF_RFID_DIGITS; j++){
                    if(rfidBuffer[j] != RFID[i][j]) break;
                }
                if( j == NO_OF_RFID_DIGITS) {
                    mode = INIT;
                    return i;
                }
            }
            mode = INIT;
            break;
        default:
            mode = INIT;
            break;
    }
    
    return -1;
}

