#include "mymain.h"




struct _key key[5];
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
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
					if(key[i].press_time<200){
						if(key[i].gap==0)key[i].dou_flag=1;
						else {
							key[i].flag=2;
							key[i].dou_flag=0;
							key[i].gap=0;
						}
					}
					else {
						key[i].flag=3;
						key[i].dou_flag=0;
						key[i].gap=0;
					}
					
					key[i].judge=0;
					key[i].press_time=0;
				}
				else{
					key[i].press_time++;
				}
			
				break;
			
			default:
				break;

		}
		
		
		if(key[i].dou_flag==1)key[i].gap++;
		if(key[i].gap>20){
			key[i].flag=1;
			key[i].dou_flag=0;
			key[i].gap=0;
		}
		
	}
	
}




double frq,entr;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance==TIM17&&htim->Channel==HAL_TIM_ACTIVE_CHANNEL_1){
		entr=1+HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		__HAL_TIM_SetCounter(htim,0);
		
		frq=1000000/entr;
		
		cur_v=frq*2*Pi*R/(100*K);
	}

}