/*
 * File:   main.c
 * Author: DELL
 *
 * Created on October 27, 2022, 3:45 PM
 */


#include "main.h"


enum elevator {DOWN, IDLE, UP};
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
    InitSystem();
    
    while(1){
        
        
        scan_key_matrix();
        AddFloorBuffer();
        RemoveCurrenFloorBuffer();

        
        DisplayState();           
        Display(floorX);
        //fsm_elevatorState();

        if(flag_timer0){
            flag_timer0 = 0;

            ElevatorOperating();

            if(floor_buffer[floorX]) {
                SetTimer0_ms(2000);
            }
            else SetTimer0_ms(500);
             
        }
        Delay_ms(50);
    }
    return;
}

void InitSystem(void){
    floorX = 0;
    state = IDLE;
    init_key_matrix();
    InitLed();
    InitLed7Seg();
    
    init_interrupt();
    init_timer0(4695);      //1ms
    //init_timer1(9390);      //dinh thoi 2ms
    
    Display(floorX);
    SetTimer0_ms(500);      //0.5s
}


void Delay_ms(unsigned int value)
{
	int i,j;
	for(i = 0; i < value; i++)
		for(j = 0; j < 160; j++);
}

void AddFloorBuffer(void){
    int i;
    for(i = 0; i < MAX_FLOOR; i++){
        if(key_code[i]){
            DisplayFloorDemanded(i);
            floor_buffer[i] = 1;
        }
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
    if(floorX == UP){
        OpenOutput(6);
        CloseOutput(7);
    }
    else if(floorX == DOWN){
        OpenOutput(7);
        CloseOutput(6);
    }
    else {
        CloseOutput(6);
        CloseOutput(7);
    }
}