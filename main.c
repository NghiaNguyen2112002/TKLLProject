/*
 * File:   main.c
 * Author: DELL
 *
 * Created on October 27, 2022, 3:45 PM
 */


#include "main.h"

#define NO_OF_PASS_DIGITS               4
unsigned char PASSWORD[4] = {0, 1, 2, 3};

enum elevatorMode {INIT, NORMAL, ENTER_PASS, SECURE};
enum elevatorMode eleMode;

enum elevator {UP, DOWN, IDLE};
enum elevator state;

char floorX;
char floor_buffer[6] = {0, 0, 0, 0, 0, 0};

unsigned char isEnterMode;
unsigned char isSecureMode;

unsigned char temp, index_button;
void InitSystem(void);
void Delay_ms(unsigned int value);


void AddButtonFloorBuffer(void);
void AddRFIDBuffer(void);
void RemoveCurrenFloorBuffer(void);
void ClearFloorBuffer(void);
char IsUpFloorDemanded(void);
char IsDownFloorDemanded(void);
void DisplayFloorDemanded(char floor);
void DisplayState(void);


void fsm_elevatorState(void);
void fsm_elevatorMode(void);

void main(void) {
    InitSystem();

    for(temp = 0; temp < 10; temp++){
        Delay_ms(50);
        Display(temp);
    }
    PORTAbits.RA0 = 0;
    Delay_ms(100);
    PORTAbits.RA0 = 1;
    Delay_ms(50);
    
    floorX = 0;
    while(1){

        DisplayState();
        Display(floorX);
      
        fsm_elevatorMode();
        
        if(flag_timer0){
            flag_timer0 = 0;

            fsm_elevatorState();
//            ElevatorOperating();

            if(floor_buffer[floorX]) {
                PORTAbits.RA0 = 0;
                Delay_ms(100);
                PORTAbits.RA0 = 1;
                SetTimer0_ms(4000);
            }
            else {
                SetTimer0_ms(500);
            }
        }
    }
    return;
}

void InitSystem(void){
    //internal OSC 4MHz
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;       
    
//    RCONbits.NOT_POR = 1;
    floorX = 0;
    state = IDLE;
    eleMode = INIT;
    
    isEnterMode = 0;
    isSecureMode = 0;
    
    //BUZZER
    TRISAbits.RA0 = 0;
    PORTAbits.RA0 = 1;          //low level trigger => 1 == OFF
    
    InitLed();
    InitLed7Seg();
//
    InitButtonReading();
    init_interrupt();
    init_uart(); 
    init_rfid();
   
    init_timer0(10000);      //10ms
//    init_timer1(4695);      //dinh thoi 1ms
//    
    
    Display(floorX);
    DisplayState(); 
    SetTimer0_ms(1000);      //1000ms
//    SetTimer1_ms(10);
}


void Delay_ms(unsigned int value)
{
	unsigned int i,j;
	for(i = 0; i < value; i++)
		for(j = 0; j < 160; j++);
}

void AddButtonFloorBuffer(void){
    int i;
//  floor  added from button
    for(i = 0; i < MAX_FLOOR; i++){
        if(is_button_pressed(i)){
            DisplayFloorDemanded(i);
            floor_buffer[i] = 1;
        }
    }

    
}

void AddRFIDBuffer(void){
    if(IS_THIS_RFID_VERIFIED != -1 && IS_THIS_RFID_VERIFIED < MAX_FLOOR) {

        uart_putchar('a');
        uart_putchar(IS_THIS_RFID_VERIFIED + '0');
        DisplayFloorDemanded(IS_THIS_RFID_VERIFIED);
        floor_buffer[IS_THIS_RFID_VERIFIED] = 1;
        IS_THIS_RFID_VERIFIED = -1;     //turn off flag
        
    }
    else if(IS_THIS_RFID_VERIFIED == 6){
        PORTAbits.RA0 = 0;
        Delay_ms(100);
        PORTAbits.RA0 = 1;
        isEnterMode = 1;
        IS_THIS_RFID_VERIFIED = -1;
    }
//    else if(IS_THIS_RFID_VERIFIED == 7){
//        isSecureMode = 1;
//        IS_THIS_RFID_VERIFIED = -1;
//    }
}
void RemoveCurrenFloorBuffer(void){
    //elevator at floorX -> buffer at that floor = 0
    floor_buffer[floorX] = 0;
    CloseOutput(floorX);
}

void ClearFloorBuffer(void){
    unsigned char i;
    for(i = 0; i < MAX_FLOOR; i++) {
        floor_buffer[i] = 0;
        CloseOutput(i);
    }
}

char IsUpFloorDemanded(void){
    char i;
    for(i = floorX+1; i < MAX_FLOOR; i++){
        if(floor_buffer[i]) return 1;
    }
    return 0;
}
char IsDownFloorDemanded(void){
    char i;
    for(i = floorX-1; i > -1; i--){
        if(floor_buffer[i]) return 1;
    }
    return 0;
}


void fsm_elevatorMode(void){
    switch(eleMode){
        case INIT:
            PORTAbits.RA0 = 0;
            Delay_ms(100);
            PORTAbits.RA0 = 1;
            ClearFloorBuffer();
            
            if(isSecureMode == 0) {
                eleMode = NORMAL;
            }
            else {
                eleMode = SECURE;
            }
            break;
        case NORMAL:
            
            AddButtonFloorBuffer();
            RemoveCurrenFloorBuffer();

            AddRFIDBuffer();
            if(isEnterMode){
                temp = 0;
                ClearFloorBuffer();

                eleMode = ENTER_PASS;
            }
            break;
        case ENTER_PASS:
            isEnterMode = 0;
            
            for(index_button = 0; index_button < NO_OF_BUTTONS; index_button++){
                if(is_button_pressed(index_button)){
                    DisplayFloorDemanded(index_button);
                    if(index_button == temp) temp++;
                    else eleMode = INIT;
                }
               
            }
            if(temp == NO_OF_PASS_DIGITS) {
                isSecureMode = !isSecureMode;
                eleMode = INIT;
            }
            break;
        case SECURE:
            
            AddRFIDBuffer();
            RemoveCurrenFloorBuffer();

            if(isEnterMode){
                temp = 0;
                ClearFloorBuffer();

                eleMode = ENTER_PASS;
            }
            break;
    }
}


void fsm_elevatorState(void){
    switch(state){
        case UP:
            floorX++;
            if(floorX > MAX_FLOOR - 1){
                floorX = MAX_FLOOR - 1;    
            }
            
            if(IsUpFloorDemanded()) state = UP;
            else if(IsDownFloorDemanded()) state = DOWN;
            else state = IDLE;
            break;
        case DOWN:
            floorX--;
            if(floorX < 0){
                floorX = 0;    
            }
            
            if(IsDownFloorDemanded()) state = DOWN;
            else if(IsUpFloorDemanded()) state = UP;
            else state = IDLE;
            break;
        case IDLE:
            if(IsUpFloorDemanded()) state = UP;
            else if(IsDownFloorDemanded()) state = DOWN;
            else state = IDLE;
            break;
        default:
            break;
    }
}


void DisplayFloorDemanded(char floor){
    OpenOutput(floor);
}
void DisplayState(void){  
    if(state == UP){
        OpenOutput(7);
        CloseOutput(6);
    }
    else if(state == DOWN){
        OpenOutput(6);
        CloseOutput(7);
    }
    else{
        CloseOutput(6);
        CloseOutput(7);
    }
}