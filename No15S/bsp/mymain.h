#ifndef _mymain_h_
#define _mymain_h_

#include "stdio.h"
#include "string.h"
#include "lcd.h"


//led
extern uint16_t led_sta;
extern void led_set();

//key
struct _key{
	uint8_t sta;
	uint8_t judge;
	uint8_t flag;
	uint32_t presstime;
};

extern struct _key key[5];

//IC
extern uint32_t ic2,ic1;

#endif