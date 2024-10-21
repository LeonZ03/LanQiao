#include "mymain.h"




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
					break;
				
				case 2:
					if(key[i].sta==0)key[i].presstime++;
					else{
						if(key[i].presstime>=100)key[i].flag=3;
						else key[i].flag=1;
						
						key[i].judge=0;
						key[i].presstime=0;
					}
					break;
				
				default:
					break;
				
			}
		}
	}
}


uint32_t ic_tick,pa1_ICpulse;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance==TIM2){
		ic_tick=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
		__HAL_TIM_SetCounter(htim,0);
		
		pa1_ICpulse=1000000/ic_tick;
	}
}



//UART
char uart_buf[50];
uint8_t rx_flag,rx_single;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	uart_buf[rx_flag++]=rx_single;
	HAL_UART_Receive_IT(huart,&rx_single,1);
	
}