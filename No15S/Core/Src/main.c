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
int A_frq,B_frq;
int A_pri,B_pri;
int PX=0,PD=1000,PH=5000;
int NDA=0,NDB=0,NHA,NHB;
int A_data[30],B_data[30];
uint8_t aflag=0,bflag=0;

void IC_handle(){
	static uint8_t ic_l=1,ic_r=0;
	if(uwTick%100>50)ic_r=1;
	else ic_r=0;
	
	if(ic_l==0&&ic_r==1){
		A_frq=(1000000/ic1)+PX;
		B_frq=(1000000/ic2)+PX;
		

		A_pri=1000000/A_frq;
		B_pri=1000000/B_frq;
		
		
		
		
		//PH
		static uint8_t pha_l=1,pha_r=0,phb_l=1,phb_r=0;
		//A
		if(A_frq>PH)pha_r=1;
		else pha_r=0;
		
		if(pha_l==0&&pha_r==1)NHA++;
		pha_l=pha_r;
		
		//B
		if(B_frq>PH)phb_r=1;
		else phb_r=0;
		
		if(phb_l==0&&phb_r==1)NHB++;
		phb_l=phb_r;
		
		
		//PD
		A_data[aflag]=A_frq;
		aflag=(aflag+1)%30;
		B_data[bflag]=B_frq;
		bflag=(bflag+1)%30;
		
		int amin,amax,bmin,bmax;
		uint8_t amark,bmark;
		amark=bmark=0;
//		amin=bmin=50000;
//		amax=bmax=-50000;
		for(int i=0;i<30;i++){
			if(A_data[i]==0x01010101)continue;
			
			amark++;
			if(amark==1)amin=amax=A_data[i];
			if(A_data[i]<amin)amin=A_data[i];
			if(A_data[i]>amax)amax=A_data[i];
		}
		if(amax-amin>PD&&amark>0){
			NDA++;
			memset(A_data,1,sizeof(A_data));
			aflag=0;
		}
		
		
		for(int i=0;i<30;i++){
			if(B_data[i]==0x01010101)continue;
			
			bmark++;
			if(bmark==1)bmin=bmax=B_data[i];
			if(B_data[i]<bmin)bmin=B_data[i];
			if(B_data[i]>bmax)bmax=B_data[i];
		}
		if(bmax-bmin>PD&&bmark>0){
			NDB++;
			memset(B_data,1,sizeof(B_data));
			bflag=0;
		}
	}
	
	ic_l=ic_r;
	
	
	
	
	
	
	
}

uint8_t lcd_flag=0;
uint8_t data_swit=0,para_swit=0;
void lcd_sch(){
	if(lcd_flag==0){
		
		if(data_swit==0){
			sprintf(text,"        DATA  ");
			LCD_DisplayStringLine(Line1, (unsigned char *)text);
			
			
			if(A_frq<0)sprintf(text,"     A=NULL    ");
			else if(A_frq<1000)sprintf(text,"     A=%dHz    ",A_frq);
			else	sprintf(text,"     A=%.2fKHz    ",(float)A_frq/1000);
			LCD_DisplayStringLine(Line3, (unsigned char *)text);
			
			if(B_frq<0)sprintf(text,"     B=NULL    ");
			else if(B_frq<1000)sprintf(text,"     B=%dHz    ",B_frq);
			else	sprintf(text,"     B=%.2fKHz    ",(float)B_frq/1000);
			LCD_DisplayStringLine(Line4, (unsigned char *)text);
		}
		
		else{
			sprintf(text,"        DATA  ");
			LCD_DisplayStringLine(Line1, (unsigned char *)text);
			
			if(A_pri<0)sprintf(text,"     A=NULL    ");
			else if(A_pri<1000)sprintf(text,"     A=%dus    ",A_pri);
			else	sprintf(text,"     A=%.2fms    ",(float)A_pri/1000);
			LCD_DisplayStringLine(Line3, (unsigned char *)text);
			
			if(B_pri<0)sprintf(text,"     B=NULL    ");
			else if(B_pri<1000)sprintf(text,"     B=%dus    ",B_pri);
			else	sprintf(text,"     B=%.2fms    ",(float)B_pri/1000);
			LCD_DisplayStringLine(Line4, (unsigned char *)text);
		}
	}
	
	if(lcd_flag==1){
		sprintf(text,"        PARA  ");
		LCD_DisplayStringLine(Line1, (unsigned char *)text);
			
		sprintf(text,"     PD=%dHz  ",PD);
		LCD_DisplayStringLine(Line3, (unsigned char *)text);
		
		sprintf(text,"     PH=%dHz  ",PH);
		LCD_DisplayStringLine(Line4, (unsigned char *)text);
		
		sprintf(text,"     PX=%dHz  ",PX);
		LCD_DisplayStringLine(Line5, (unsigned char *)text);
			
	}
	
	if(lcd_flag==2){
		sprintf(text,"        RECD  ");
		LCD_DisplayStringLine(Line1, (unsigned char *)text);
			
		sprintf(text,"     NDA=%d  ",NDA);
		LCD_DisplayStringLine(Line3, (unsigned char *)text);
		
		sprintf(text,"     NDB=%d  ",NDB);
		LCD_DisplayStringLine(Line4, (unsigned char *)text);
		
		sprintf(text,"     NHA=%d  ",NHA);
		LCD_DisplayStringLine(Line5, (unsigned char *)text);
		
		sprintf(text,"     NHB=%d  ",NHB);
		LCD_DisplayStringLine(Line6, (unsigned char *)text);
	}
}

void key_handle(){
	if(key[4].flag==1){
		lcd_flag=(lcd_flag+1)%3;
		LCD_Clear(Black);
		
		if(lcd_flag==0)data_swit=0;
		if(lcd_flag==1)para_swit=0;
		
		key[4].flag=0;
	}
	
	if(key[3].flag==1){
		if(lcd_flag==0){
			data_swit=(data_swit+1)%2;
			LCD_Clear(Black);
		}
		
		if(lcd_flag==1){
			para_swit=(para_swit+1)%3;
		}
		
		key[3].flag=0;
	}
	
	if(key[3].flag==3){
		if(lcd_flag==2){
			NDA=NDB=NHA=NHB=0;
			memset(A_data,1,sizeof(A_data));
			aflag=0;
			
			memset(B_data,1,sizeof(B_data));
			bflag=0;
		}
		
		key[3].flag=0;
	}
	
	if(key[1].flag==1){
		if(lcd_flag==1){
			if(para_swit==0)PD+=100;
			if(para_swit==1)PH+=100;
			if(para_swit==2)PX+=100;
		}
		
		key[1].flag=0;
	}
	
	if(key[2].flag==1){
		if(lcd_flag==1){
			if(para_swit==0)PD-=100;
			if(para_swit==1)PH-=100;
			if(para_swit==2)PX-=100;
		}
		
		key[2].flag=0;
	}
}


void led_handle(){
	if(lcd_flag==0)led_sta|=(0x1<<1);
	else led_sta &=~(0x1<<1);
	
	if(A_frq>PH)led_sta|=(0x1<<2);
	else led_sta &=~(0x1<<2);
	
	if(B_frq>PH)led_sta|=(0x1<<3);
	else led_sta &=~(0x1<<3);
	
	if(NDA>=3||NDB>=3)led_sta|=(0x1<<8);
	else led_sta &=~(0x1<<8);
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
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */
	
	//lcd
	LCD_Init();
   
  LCD_Clear(Black);
  LCD_SetBackColor(Black);
  LCD_SetTextColor(White);

//  LCD_DisplayStringLine(Line4, (unsigned char *)"    Hello,world.   ");

	
	//led
	led_sta=0x00;
	led_set();
	
	//key
	HAL_TIM_Base_Start_IT(&htim17);
	
	//IC
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
	HAL_TIM_IC_Start(&htim2,TIM_CHANNEL_1);
	
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);
	HAL_TIM_IC_Start(&htim3,TIM_CHANNEL_1);
	
	
	
	
	//PD
	memset(A_data,1,sizeof(A_data));
	memset(B_data,1,sizeof(B_data));
	
	
	
	
	
	
	HAL_Delay(10);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		
		
//		for(int i=1;i<=4;i++){
//			if(key[i].flag!=0){
//				sprintf(text,"B%d is %d  ",i,key[i].flag);
//				LCD_DisplayStringLine(Line4, (unsigned char *)text);

//				
//				key[i].flag=0;
//			}
//		}
//		
//		
//		sprintf(text,"ic1=%d ic2=%d  ",ic1,ic2);
//		LCD_DisplayStringLine(Line5, (unsigned char *)text);
		
		
		IC_handle();
		lcd_sch();
		key_handle();
		led_handle();
		led_set();
		
//		sprintf(text,"NDA=%d NDB=%d  ",NDA,NDB);
//		LCD_DisplayStringLine(Line7, (unsigned char *)text);
//		
//		sprintf(text,"NHA=%d NHB=%d  ",NHA,NHB);
//		LCD_DisplayStringLine(Line8, (unsigned char *)text);
//		
//		
		
		
		
		
		
		
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
