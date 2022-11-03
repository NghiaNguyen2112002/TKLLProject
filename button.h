#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <p18f4620.h>

#define PORT_BUTTON		PORTC
#define TRIS_BUTTON		TRISC
#define	MAX_COL			2   //RC4 RC5 RC6
#define MAX_ROW			3   //RC0 RC1

extern unsigned int key_code[];

void init_key_matrix(void);
void scan_key_matrix(void);

#endif