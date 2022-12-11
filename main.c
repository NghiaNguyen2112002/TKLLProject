/*
 * File:   main.c
 * Author: DELL
 *
 * Created on October 27, 2022, 3:45 PM
 */


#include "main.h"


enum elevator {UP, DOWN, IDLE};
enum elevator state;

char floorX;
char floor_buffer[6] = {0, 0, 0, 0, 0, 0};


void InitSystem(void);
void Delay_ms(unsigned int value);


void AddFloorBuffer(void);
void RemoveCurrenFloorBuffer(void);
char IsUpFloorDemanded(void);
char IsDownFloorDemanded(void);
void DisplayFloorDemanded(char floor);
void DisplayState(void);

void ElevatorOperating(void);

void fsm_elevatorState(void);

void main(void) {
    unsigned char  x;
    InitSystem();

    for(x = 0; x < 10; x++){
        Delay_ms(50);
        Display(x);
    }
    Delay_ms(50);

    floorX = 0;
    while(1){
        AddFloorBuffer();
        RemoveCurrenFloorBuffer();

        DisplayState();
        Display(floorX);
        fsm_elevatorState();
      
        
        
        if(flag_timer0){
            flag_timer0 = 0;

            ElevatorOperating();

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

void AddFloorBuffer(void){
    int i;
//  floor  added from button
    for(i = 0; i < MAX_FLOOR; i++){
        if(is_button_pressed(i)){
            DisplayFloorDemanded(i);
            floor_buffer[i] = 1;
        }
    }

    if(IS_THIS_RFID_VERIFIED != -1) {

        uart_putchar('a');
        uart_putchar(IS_THIS_RFID_VERIFIED + '0');
        DisplayFloorDemanded(IS_THIS_RFID_VERIFIED);
        floor_buffer[IS_THIS_RFID_VERIFIED] = 1;
        IS_THIS_RFID_VERIFIED = -1;     //turn off flag
        
    }
}

void RemoveCurrenFloorBuffer(void){
    //elevator at floorX -> buffer at that floor = 0
    floor_buffer[floorX] = 0;
    CloseOutput(floorX);
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

void ElevatorOperating(void){
    if(state == UP) {
        floorX++;
        if(floorX > MAX_FLOOR - 1){
            floorX = MAX_FLOOR - 1;    
        }
    }
    else if(state == DOWN){
         floorX--;
        if(floorX < 0){
            floorX = 0;    
        }
    }
    else if(state == IDLE){
        
    }
}

void fsm_elevatorState(void){
    switch(state){
        case UP:
            if(IsUpFloorDemanded()) state = UP;
            else if(IsDownFloorDemanded()) state = DOWN;
            else state = IDLE;
            break;
        case DOWN:
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