#include "mymain.h"


//led
uint16_t led_sta;
void led_set(){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,led_sta<<7,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}



//key
struct _key key[5];
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance==TIM17){
		key[1].sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
		key[2].sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
		key[3].sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
		key[4].sta=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
		
		
		for(int i=1;i<=4;i++){
			switch(key[i].judge){
				case 0:
					if(key[i].sta==0)key[i].judge=1;
					break;
				
				case 1:
					if(key[i].sta==0)key[i].judge=2;
					else key[i].judge=0;
					break;
				
				case 2:
					if(key[i].sta==0){
						key[i].presstime++;
						if(key[i].presstime>200&&key[i].keep==0){
							key[i].keep=1;
							key[i].flag=4;
						}
					}
					else{
						if(key[i].presstime<190)key[i].flag=1;
						else key[i].flag=0;
						
						key[i].judge=0;
						key[i].presstime=0;
						key[i].keep=0;
						
					}
					break;
				
				default:
					break;
			}
		}
	}
}



//adc
float adc_read(){
	HAL_ADC_Start(&hadc2);
	int u=HAL_ADC_GetValue(&hadc2);
	return (float)u*3.3/4096;
}



//IC
uint32_t ic_tick,ic_high;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance==TIM2&&htim->Channel==HAL_TIM_ACTIVE_CHANNEL_2){
		ic_tick=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
		ic_high=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		
		__HAL_TIM_SetCounter(htim,0);
	}
}

//oc
extern TIM_HandleTypeDef htim3;
void oc_set(uint32_t frq,uint32_t duty){
		
	__HAL_TIM_SET_PRESCALER(&htim3,(800000/frq)-1);
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2, duty-1);
}

