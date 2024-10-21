#ifndef _mymain_h_
#define _mymain_h_

#include "stdio.h"
#include "string.h"

#include "lcd.h"

#define Pi 3.14

//extern __IO uint32_t uwTick;

//DATA 
extern char pwm_mode;
extern int cur_duty;
extern float cur_v;

//PARA
extern int K,R;

//RECD
extern int N;
extern float MH,ML;



//led
extern volatile uint32_t led_sta;
void led_set(void);

//key
struct _key{
	uint8_t sta;
	uint8_t flag;
	uint8_t judge;
	uint32_t press_time;
	uint8_t gap;
	uint8_t dou_flag;
};
extern struct _key key[5];

//Cap
extern double frq,entr;



//adc
extern double adc_read(void);
extern int v_to_duty(double v);


//main
extern void lcd_task();
extern void key_task();
extern void mode_change_task();
extern void recd_task();

struct _count{
			float v_id;
			uint32_t start_tick;
};

#endif
