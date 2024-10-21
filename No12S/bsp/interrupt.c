#include "mymain.h"




void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance==TIM3){
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
					if(key[i].sta==1){
						key[i].flag=1;
						key[i].judge=0;
					}
					break;
				
				default:
					break;
			}
		}
	}
}


char rxdata[50],txdata[50];
uint8_t rx_flag=0,rx_sigl;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	rxdata[rx_flag++]=rx_sigl;
	
	HAL_UART_Receive_IT(huart,&rx_sigl,1);
}
