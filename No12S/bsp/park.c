#include "mymain.h"

extern UART_HandleTypeDef huart1;

struct _park park[8];
int park_flag;

char tmp_form[10],tmp_id[10];
uint64_t tmp_time;
uint32_t tmp_id_to_num;
uint8_t tmp_year,tmp_month,tmp_day,tmp_hour,tmp_min,tmp_second,tmp_form_to_num;
int time_for_fee;
float tmp_fee;

void park_handler(){
	if(rx_flag==22){
		sscanf(rxdata,"%4s:%4s:%12lld",tmp_form,tmp_id,&tmp_time);
		

		
		if(strlen(tmp_form)!=4||strlen(tmp_id)!=4){
			err_handle();
			return;
		}
		
		
		if(strcmp(tmp_form,"CNBR")!=0&&strcmp(tmp_form,"VNBR")!=0){
			err_handle();
			return;
		}
		
		
		
		tmp_id_to_num=(uint32_t)tmp_id[0]<<24|(uint32_t)tmp_id[1]<<16|(uint32_t)tmp_id[2]<<8|(uint32_t)tmp_id[3];
		
		sprintf(txdata,"%lld",tmp_time);
		sscanf(txdata,"%2d%2d%2d%2d%2d%2d",&tmp_year,&tmp_month,&tmp_day,&tmp_hour,&tmp_min,&tmp_second);
		
		if(tmp_month>12||tmp_day>31||tmp_hour>23||tmp_min>59||tmp_second>59){
			err_handle();
			return;
		}
		

		park_flag=-1;
		for(int i=0;i<=7;i++){//judge if enter or exit
			if(park[i].sta==1&&park[i].id==tmp_id_to_num){
				park_flag=i;
				break;
			}
		}
		
		if(park_flag==-1){//enter
			if(idle_num==0){//no space
				err_handle();
				return;
			}
			
			park_flag=-1;
			for(int i=0;i<=7;i++){
				if(park[i].sta==0){
					park_flag=i;
					break;
				}
			}
			
			park[park_flag].sta=1;
			park[park_flag].time_num=tmp_time;
			if(strcmp(tmp_form,"CNBR")==0){
				park[park_flag].form=1;
				cnbr_num++;
			}
			else {
				park[park_flag].form=2;
				vnbr_num++;
			}
			idle_num--;
			park[park_flag].id=tmp_id_to_num;
			
		}
		else{//exit
			if(strcmp(tmp_form,"CNBR")==0)tmp_form_to_num=1;
			else tmp_form_to_num=2;
			
			if(park[park_flag].form!=tmp_form_to_num){
				err_handle();
				return;
			}
			
			if(park[park_flag].time_num>tmp_time){
				err_handle();
				return;
			}
			
			if(park[park_flag].time_num%10000<tmp_time%10000){
				park[park_flag].time_num=park[park_flag].time_num-(park[park_flag].time_num%10000);
				tmp_time=tmp_time-(tmp_time%10000);
				tmp_time+=10000;
				
//				if(tmp_time%1000000==230000){
//					tmp_time+=1000000;
//					tmp_time-=230000;
//				}
//				else tmp_time+=10000;
			}
			else{
				park[park_flag].time_num=park[park_flag].time_num-(park[park_flag].time_num%10000);
				tmp_time=tmp_time-(tmp_time%10000);
			}
			
			
			//min sec is 0
			park[park_flag].time_num/=10000;
			tmp_time/=10000;
			
			park[park_flag].time_num%=10000;
			tmp_time%=10000;
			
			time_for_fee=(tmp_time/100)*24+(tmp_time%100)-(park[park_flag].time_num/100)*24-(park[park_flag].time_num%100);
			
			if(tmp_form_to_num==1){
				tmp_fee=cnbr_fee*time_for_fee;
				
				sprintf(txdata,"CNBR:%s:%d:%.2f\r\n",tmp_id,time_for_fee,tmp_fee);
				HAL_UART_Transmit(&huart1,(const uint8_t*)txdata,strlen(txdata),50);
				
				cnbr_num--;
			}
			else {
				tmp_fee=vnbr_fee*time_for_fee;
				
				sprintf(txdata,"VNBR:%s:%d:%.2f\r\n",tmp_id,time_for_fee,tmp_fee);
				HAL_UART_Transmit(&huart1,(const uint8_t*)txdata,strlen(txdata),50);
				
				vnbr_num--;
			}
			
			idle_num++;
			
			park[park_flag].sta=0;
			
							
		}
		
		
		
	}
	
	else{
		err_handle();
		return;
	}
	
}




void err_handle(){
	sprintf(txdata,"Error\r\n");
	HAL_UART_Transmit(&huart1,(const uint8_t*)txdata,strlen(txdata),50);
}




