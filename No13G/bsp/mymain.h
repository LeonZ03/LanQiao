#ifndef _mymain_h_
#define _mymain_h_

#include "stdio.h"
#include "string.h"

#include "lcd.h"
#include "i2c_hal.h"





//LCD
extern char text[];
extern uint8_t lcd_show;

extern void my_LCD_Init(uint8_t mode);
extern void my_REG_932X_Init(uint8_t mode);

//LED
extern uint16_t led_sta;
extern void led_set();

//key
struct _key{
	uint8_t sta;
	uint8_t flag;
	uint8_t judge;
	uint32_t presstime;

};

extern struct _key key[];

//adc
extern ADC_HandleTypeDef hadc2;
extern float pa4_adc,pa5_adc;
extern void adc2_read();


//IC
extern uint32_t ic_tick,pa1_ICpulse;

//OC
extern TIM_HandleTypeDef htim3;
extern void PA7_Pulse_Out(uint32_t x);
extern uint8_t oc_mode;

//UART
extern char uart_buf[50];
extern uint8_t rx_flag,rx_single;
extern void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);


//IIC
extern void eeprom_send(uint8_t addr,uint8_t data);
extern uint8_t eeprom_read(uint8_t addr);


//PARA
extern uint8_t X,Y;

//REC
extern uint8_t rec_swit;
extern uint32_t N_pa4,N_pa5;
extern float A_pa4,A_pa5,T_pa4,T_pa5,H_pa4,H_pa5;
extern float pa4_dat[],pa5_dat[];



extern void rec_acc(uint8_t x);

#endif