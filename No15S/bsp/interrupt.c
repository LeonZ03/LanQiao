#include "mymain.h"



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
					if(key[i].sta==0)key[i].presstime++;
					else{
						if(key[i].presstime<100)key[i].flag=1;
						else key[i].flag=3;
						
						key[i].presstime=0;
						key[i].judge=0;
					}
					break;
				
				default:
					break;
			}
		}
	}
}


uint32_t ic2,ic1;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance==TIM2){
		ic1=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		
		__HAL_TIM_SET_COUNTER(htim,0);
	}
	
	if(htim->Instance==TIM3){
		ic2=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		
		__HAL_TIM_SET_COUNTER(htim,0);
	}
}