#include "rfid.h"

// x3 rfid card with 12 digit
const unsigned char RFID[5][12] = {
    "870092771371",
    "870092DC7CB5",
    "870092C338EE",
    "870092A76DDF",
    "8700929CC049"
                    };

enum RFID_CARD_DETECT_FSM mode;
unsigned char rfidBuffer[12];
unsigned char counter;
            unsigned char i, j;
char IS_THIS_RFID_VERIFIED;


void init_rfid(void){
    mode = INIT;
    counter = 0;
    IS_THIS_RFID_VERIFIED = -1;
}

char IsRFIDVerified(void){
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

