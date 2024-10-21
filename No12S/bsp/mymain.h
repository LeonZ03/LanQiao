#ifndef _mymain_h_
#define _mymain_h_

#include "main.h"
#include "lcd.h"
#include "stdio.h"
#include "string.h"
//#include "fonts.h"


//LCD
extern char text[50];
extern uint8_t menu;
extern int cnbr_num,vnbr_num,idle_num;
extern float cnbr_fee,vnbr_fee;

//led
extern volatile int led_sta;
extern void led_set();

//key
struct _key{
	int sta;
	int presstime;
	int flag;
	int judge;
};
extern struct _key key[5];



//UART
extern char rxdata[50],txdata[50];
extern uint8_t rx_flag,rx_sigl;



//park
struct _park{
	uint8_t sta;
	uint32_t id;
	uint8_t form;
	uint64_t time_num;
//	uint8_t year;
//	uint8_t month;
//	uint8_t day;
//	uint8_t hour;
//	uint8_t min;
//	uint8_t second;
};


extern void err_handle();
extern void park_handler();



#endif


