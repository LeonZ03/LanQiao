#ifndef _mymain_h_
#define _mymain_h_


#include "stdio.h"
#include "string.h"
#include "lcd.h"
#include "ds18b20_hal.h"





//led
extern uint16_t led_sta;
extern void led_set();

//key
struct _key{
	uint8_t flag;
	uint8_t sta;
	uint8_t judge;
	uint8_t keep;
	uint32_t presstime;
};

extern struct _key key[];


//adc
extern ADC_HandleTypeDef hadc2;
extern float adc_read();

//IC
extern uint32_t ic_tick,ic_high;


//oc
extern TIM_HandleTypeDef htim3;
extern void oc_set(uint32_t frq,uint32_t duty);



extern void led_handle();


#endif