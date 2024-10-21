/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mymain.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char text[50];

uint8_t fh_l=0,fh_r,ah_l=0,ah_r,th_l=0,th_r;
uint8_t lcd_flag,key_lock=0,para_swit=0,fset_swit=0,mem_flag=0,reshowF_flag=0,reshowA_flag=0;
uint32_t F,D,FH=2000,TH=30,FN,AN,TN,FP=1,TT=6,FH_use=2000,TH_use=30,FP_use=1,TT_use=6;
float A,T,AH=3.0,VP=0.9,AH_use=3.0,VP_use=0.9;

uint32_t F_data[100],D_data[100];
float A_data[100];

void reset_sta(){
	fh_l=0,fh_r=0,ah_l=0,ah_r=0,th_l=0,th_r=0;
	lcd_flag=0,key_lock=0,para_swit=0,fset_swit=0,mem_flag=0,reshowF_flag=0,reshowA_flag=0;
	F=0,D=0,FH=2000,TH=30,FN=0,AN=0,TN=0,FP=1,TT=6,FH_use=2000,TH_use=30,FP_use=1,TT_use=6;
	A=0,T=0,AH=3.0,VP=0.9,AH_use=3.0,VP_use=0.9;
}

void data_get(){
	F=1000000/ic_tick;
	D=(float)ic_high/(float)ic_tick*100;
	A=adc_read();
	A=adc_read();
	A=adc_read();
	T=ds_read();
}
void lcd_sch(){
	if(lcd_flag==0){
		sprintf(text,"        DATA  ");
		LCD_DisplayStringLine(Line1, (unsigned char *)text);
		
		sprintf(text,"     F=%d    ",F);
		LCD_DisplayStringLine(Line3, (unsigned char *)text);
		
		sprintf(text,"     D=%d%%    ",D);
		LCD_DisplayStringLine(Line4, (unsigned char *)text);
		
		sprintf(text,"     A=%.1f    ",A);
		LCD_DisplayStringLine(Line5, (unsigned char *)text);
		
		sprintf(text,"     T=%.1f    ",T);
		LCD_DisplayStringLine(Line6, (unsigned char *)text);
	}
	
	if(lcd_flag==1){
		sprintf(text,"        PARA  ");
		LCD_DisplayStringLine(Line1, (unsigned char *)text);
		
		sprintf(text,"     FH=%d    ",FH);
		LCD_DisplayStringLine(Line3, (unsigned char *)text);
		
		sprintf(text,"     AH=%.1f    ",AH);
		LCD_DisplayStringLine(Line4, (unsigned char *)text);
		
		sprintf(text,"     TH=%d    ",TH);
		LCD_DisplayStringLine(Line5, (unsigned char *)text);
	}
	
	
	if(lcd_flag==2){
		sprintf(text,"        RECD  ");
		LCD_DisplayStringLine(Line1, (unsigned char *)text);
		
		sprintf(text,"     FN=%d    ",FN);
		LCD_DisplayStringLine(Line3, (unsigned char *)text);
		
		sprintf(text,"     AN=%d    ",AN);
		LCD_DisplayStringLine(Line4, (unsigned char *)text);
		
		sprintf(text,"     TN=%d    ",TN);
		LCD_DisplayStringLine(Line5, (unsigned char *)text);
	}
	
	if(lcd_flag==3){
		sprintf(text,"        FSET  ");
		LCD_DisplayStringLine(Line1, (unsigned char *)text);
		
		sprintf(text,"     FP=%d    ",FP);
		LCD_DisplayStringLine(Line3, (unsigned char *)text);
		
		sprintf(text,"     VP=%.1f    ",VP);
		LCD_DisplayStringLine(Line4, (unsigned char *)text);
		
		sprintf(text,"     TT=%d    ",TT);
		LCD_DisplayStringLine(Line5, (unsigned char *)text);
	}
}


void key_handle(){
	if(key_lock==1){
		key[1].flag=key[2].flag=key[3].flag=key[4].flag=0;
	}
	
	if(key[1].flag==1){
		if(lcd_flag==1){
			FH_use=FH;
			AH_use=AH;
			TH_use=TH;
		}
		
		if(lcd_flag==3){
			FP_use=FP;
			VP_use=VP;
			TT_use=TT;
		}
		
		lcd_flag=(lcd_flag+1)%4;
		LCD_Clear(Black);
		if(lcd_flag==1)para_swit=0;
		if(lcd_flag==3)fset_swit=0;
		
		key[1].flag=0;
	}
	
	
	if(key[2].flag==1){
		if(lcd_flag==0){
			//key_lock=1;
			mem_flag=1;
		}
		
		if(lcd_flag==1){
			para_swit=(para_swit+1)%3;
		}
		
		if(lcd_flag==2){
			FN=AN=TN=0;
		}
		
		if(lcd_flag==3){
			fset_swit=(fset_swit+1)%3;
		}
		
		key[2].flag=0;
	}
	
	
	if(key[3].flag==1){
		if(lcd_flag==0){
			reshowA_flag=1;
		}
		
		if(lcd_flag==1){
			if(para_swit==0)FH+=1000;
			if(para_swit==1)AH+=0.3;
			if(para_swit==2)TH+=1;
		}
		
		if(lcd_flag==3){
			if(fset_swit==0)FP+=1;
			if(fset_swit==1)VP+=0.3;
			if(fset_swit==2)TT+=2;
		}
		
		key[3].flag=0;
	}
	
	if(key[4].flag==1){
		if(lcd_flag==0){
			reshowF_flag=1;
		}
		
		if(lcd_flag==1){
			if(para_swit==0)FH-=1000;
			if(para_swit==1)AH-=0.3;
			if(para_swit==2)TH-=1;
		}
		
		if(lcd_flag==3){
			if(fset_swit==0)FP-=1;
			if(fset_swit==1)VP-=0.3;
			if(fset_swit==2)TT-=2;
		}
		
		key[4].flag=0;
	}
	
	
	
	if(key[3].flag==4&&key[4].flag==4){
		reset_sta();
		
		key[3].flag=key[4].flag=0;
	}
}

void recd_handle(){
	
	//FH
	if(F>FH_use)fh_r=1;
	else fh_r=0;
	
	if(fh_l==0&&fh_r==1)FN++;
	
	fh_l=fh_r;
	
	
	//AH
	if(A>AH_use)ah_r=1;
	else ah_r=0;
	
	if(ah_l==0&&ah_r==1)AN++;
	
	ah_l=ah_r;
	
	//TH
	if(T>TH_use)th_r=1;
	else th_r=0;
	
	if(th_l==0&&th_r==1)TN++;
	
	th_l=th_r;
}


void data_mem(){
	if(mem_flag==1){
		uint8_t tick_l=1,tick_r=0,mem_p=0;
		while(1){
			
			
			data_get();
			led_handle();
			lcd_sch();
			recd_handle();
			key[1].flag=key[2].flag=key[3].flag=key[4].flag=0;
			
			if(uwTick%100>50)tick_r=1;
			else tick_r=0;
			
			
			if(tick_l==0&&tick_r==1){
				F_data[mem_p]=F;
				D_data[mem_p]=D;
				A_data[mem_p]=A;
				mem_p++;
			}
			
			tick_l=tick_r;
			
			if(mem_p>=TT_use*10){
				mem_flag=0;
				break;
			}
		}
	}
}


void data_reshow(){
	if(reshowF_flag==1){
		uint8_t tick_l=1,tick_r=0,mem_p=0;
		while(1){
		
			data_get();
			led_handle();
			lcd_sch();
			key_handle();
			mem_flag=0;
			recd_handle();
			
			
			
			if(uwTick%100>50)tick_r=1;
			else tick_r=0;
			
			
			if(tick_l==0&&tick_r==1){
				oc_set(F_data[mem_p]/FP_use,D_data[mem_p]);
				mem_p++;
			}
			
			tick_l=tick_r;
			
			if(mem_p>=TT_use*10){
				reshowF_flag=0;
				oc_set(10000,1);
				break;
			}
		}
	}
	
	
	
	if(reshowA_flag==1){
		uint8_t tick_l=1,tick_r=0,mem_p=0;
		while(1){
		
			data_get();
			led_handle();
			lcd_sch();
			key_handle();
			mem_flag=0;
			recd_handle();
			
			
			
			if(uwTick%100>50)tick_r=1;
			else tick_r=0;
			
			
			if(tick_l==0&&tick_r==1){
				uint32_t dut;
				if(A_data[mem_p]<VP_use)dut=10;
				else if(A_data[mem_p]>3.3)dut=100;
				else dut=((float)90/(3.3-VP_use))*A_data[mem_p]+(100.f-297.f/(3.3-VP_use));
				
				oc_set(1000,dut+2);
				mem_p++;
			}
			
			tick_l=tick_r;
			
			if(mem_p>=TT_use*10){
				reshowA_flag=0;
				oc_set(10000,1);
				break;
			}
		}
	}
}

uint8_t memtick_l,memtick_r,ocF_l,ocF_r,ocA_l,ocA_r;
void led_handle(){

	if(mem_flag==1){
			if(uwTick%100>50)memtick_r=1;
			else memtick_r=0;
			
			
			if(memtick_l==0&&memtick_r==1){
				led_sta^=(0x1<<1);
			}
			
			memtick_l=memtick_r;
		}else led_sta &= ~(0x1<<1) ;
	
		if(reshowF_flag==1){
			if(uwTick%100>50)ocF_r=1;
			else ocF_r=0;
			
			
			if(ocF_l==0&&ocF_r==1){
				led_sta^=(0x1<<2);
			}
			
			ocF_l=ocF_r;
		}else led_sta &= ~(0x1<<2) ;
		
		
		if(reshowA_flag==1){
			if(uwTick%100>50)ocA_r=1;
			else ocA_r=0;
			
			
			if(ocA_l==0&&ocA_r==1){
				led_sta^=(0x1<<3);
			}
			
			ocA_l=ocA_r;
		}else led_sta &= ~(0x1<<3) ;
		
		if(F>FH_use)led_sta |= (0x1<<4) ;
		else led_sta &= ~(0x1<<4) ;
		
		if(A>AH_use)led_sta |= (0x1<<5) ;
		else led_sta &= ~(0x1<<5) ;
		
		if(T>TH_use)led_sta |= (0x1<<6) ;
		else led_sta &= ~(0x1<<6) ;
		
		
		led_set();
		
}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */
	
	//lcd
	LCD_Init();
   

	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);


//	LCD_DisplayStringLine(Line4, (unsigned char *)"    Hello,world.   ");

	
	
	//led
	led_sta=0x00;
	led_set();
	
	//key
	HAL_TIM_Base_Start_IT(&htim17);
	
	
	//ds18b20
	ds18b20_init_x();
	
	
	//ic
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);

	HAL_TIM_IC_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_IC_Start(&htim2,TIM_CHANNEL_1);
	
	//oc
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	
	//oc_set(3000,0);
	
	
	
	//HAL_Delay(1000);
	data_get();
	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		
		data_get();
		lcd_sch();
		key_handle();
		data_mem();
		recd_handle();
		led_handle();
		
		data_reshow();
		
//		
//		sprintf(text,"FN=%d AN=%d TN=%d   ",FN,AN,TN);
//		LCD_DisplayStringLine(Line9, (unsigned char *)text);
		
		
		
		
		
		
//		static int cot=0;
//		for(int i=1;i<=4;i++){
//			if(key[i].flag!=0){
//				cot++;
//				sprintf(text,"key%d is %d    %d",i,key[i].flag,cot);
//				LCD_DisplayStringLine(Line4, (unsigned char *)text);
//				
//				key[i].flag=0;
//			}
//		}
//		
		
		
		
//				sprintf(text,"adc is %.2f",adc_read());
//				LCD_DisplayStringLine(Line4, (unsigned char *)text);
//		
//			
//				sprintf(text,"ds is %.2f  ",ds_read());
//				LCD_DisplayStringLine(Line5, (unsigned char *)text);
//				
//				sprintf(text,"ic frq=%d   ",1000000/ic_tick);
//				LCD_DisplayStringLine(Line6, (unsigned char *)text);
//				
//					sprintf(text,"duty=%.2f  ",(float)ic_high/(float)ic_tick);
//				LCD_DisplayStringLine(Line7, (unsigned char *)text);
		
		
		
		
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
