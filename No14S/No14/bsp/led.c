#include "mymain.h"

#define led_all_pin 0xff<<8
volatile uint32_t led_sta=0;

void led_set(void){
	HAL_GPIO_WritePin(GPIOC,led_all_pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,led_sta<<7,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}