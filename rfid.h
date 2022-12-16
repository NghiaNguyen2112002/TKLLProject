/* 
 * File:   rfid.h
 * Author: DELL
 *
 * Created on November 29, 2022, 6:35 PM
 */

#ifndef RFID_H
#define	RFID_H

#include "uart.h"

#define NO_OF_RFID_DIGITS               12 
#define NO_OF_CARDS                     8

#define START_RFID_HEX                  0x02
#define END_RFID_HEX                    0x03

extern char IS_THIS_RFID_VERIFIED;

enum RFID_CARD_DETECT_FSM       {INIT, RECEIVE_DATA, DETECT_DATA};
void init_rfid(void);
char IsRFIDVerified(void);


#endif	/* RFID_H */

