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
#include "usart.h"
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
uint8_t lcd_flag=0;
void lcd_sch(){
	if(lcd_flag==0){
		LCD_DisplayStringLine(Line1, (unsigned char *)"        DATA  ");
		
		sprintf(text,"     PA4=%.2f   ",pa4_adc);
		LCD_DisplayStringLine(Line3, (unsigned char *)text);
		
		sprintf(text,"     PA5=%.2f   ",pa5_adc);
		LCD_DisplayStringLine(Line4, (unsigned char *)text);
		
		sprintf(text,"     PA1=%d   ",pa1_ICpulse);
		LCD_DisplayStringLine(Line5, (unsigned char *)text);
	}
	else if(lcd_flag==1){
		LCD_DisplayStringLine(Line1, (unsigned char *)"        PARA   ");
		
		sprintf(text,"     X=%d   ",X);
		LCD_DisplayStringLine(Line3, (unsigned char *)text);
		
		sprintf(text,"     Y=%d   ",Y);
		LCD_DisplayStringLine(Line4, (unsigned char *)text);
	}
	else if(lcd_flag==2){
		if(rec_swit==0){
			rec_acc(0);
			
			LCD_DisplayStringLine(Line1, (unsigned char *)"        REC-PA4   ");
			
			sprintf(text,"     N=%d   ",N_pa4);
			LCD_DisplayStringLine(Line3, (unsigned char *)text);
			
			sprintf(text,"     A=%.2f   ",A_pa4);
			LCD_DisplayStringLine(Line4, (unsigned char *)text);
			
			sprintf(text,"     T=%.2f   ",T_pa4);
			LCD_DisplayStringLine(Line5, (unsigned char *)text);
			
			sprintf(text,"     H=%.2f   ",H_pa4);
			LCD_DisplayStringLine(Line6, (unsigned char *)text);
		}
		
		else{
			rec_acc(1);
			
			LCD_DisplayStringLine(Line1, (unsigned char *)"        REC-PA5   ");
			
			sprintf(text,"     N=%d   ",N_pa5);
			LCD_DisplayStringLine(Line3, (unsigned char *)text);
			
			sprintf(text,"     A=%.2f   ",A_pa5);
			LCD_DisplayStringLine(Line4, (unsigned char *)text);
			
			sprintf(text,"     T=%.2f   ",T_pa5);
			LCD_DisplayStringLine(Line5, (unsigned char *)text);
			
			sprintf(text,"     H=%.2f   ",H_pa5);
			LCD_DisplayStringLine(Line6, (unsigned char *)text);
		}
		
	}
}


void rec_acc(uint8_t x){
	if(x==0&&N_pa4==0){A_pa4=T_pa4=H_pa4=0;return;}
	if(x==1&&N_pa5==0){A_pa5=T_pa5=H_pa5=0;return;}
	
	float min=10.f,max=0.f,sum=0.f;
	
	if(x==0){
		for(int i=1;i<=N_pa4;i++){
			if(pa4_dat[i]>=max)max=pa4_dat[i];
			if(pa4_dat[i]<=min)min=pa4_dat[i];
			sum+=pa4_dat[i];
			
		}
		
		A_pa4=max;
		T_pa4=min;
		H_pa4=sum/N_pa4;
	}
	
	else{
		for(int i=1;i<=N_pa5;i++){
			if(pa5_dat[i]>=max)max=pa5_dat[i];
			if(pa5_dat[i]<=min)min=pa5_dat[i];
			sum+=pa5_dat[i];
			
		}
		
		A_pa5=max;
		T_pa5=min;
		H_pa5=sum/N_pa5;
	}
	
}


void key_handle(){
	if(key[1].flag==1){
		lcd_flag=(lcd_flag+1)%3;
		if(lcd_flag==2)rec_swit=0;
		LCD_Clear(Black);
		
		key[1].flag=0;
	}
	
	if(key[2].flag==1){
		if(lcd_flag==1){
			X++;
			if(X>=5)X=1;
			
			eeprom_send(0x01,X);
			HAL_Delay(10);
		}
		
		key[2].flag=0;
	}
	
	if(key[3].flag==1){
		if(lcd_flag==1){
			Y++;
			if(Y>=5)Y=1;
			
			eeprom_send(0x00,Y);
			HAL_Delay(10);
		}
		
		key[3].flag=0;
	}
	
	if(key[4].flag==1){
		if(lcd_flag==0){
			adc2_read();
			HAL_Delay(1);
			adc2_read();
			N_pa4++;
			N_pa5++;
			
			pa4_dat[N_pa4]=pa4_adc;
			pa5_dat[N_pa5]=pa5_adc;
		}
		
		if(lcd_flag==1){
			oc_mode=(oc_mode+1)%2;
		}
		
		if(lcd_flag==2){
			rec_swit=(rec_swit+1)%2;
		}
		
		key[4].flag=0;
	}
	
	if(key[4].flag==3){
		if(lcd_flag==2){
			if(rec_swit==0){
				N_pa4=0;
				A_pa4=T_pa4=H_pa4=0;
			}
			
			else{
				N_pa5=0;
				A_pa5=T_pa5=H_pa5=0;
			}
		}
		
		key[4].flag=0;
	}
}

void OC_act(){
	if(oc_mode==0){
		PA7_Pulse_Out(pa1_ICpulse*X);
//		PA7_Pulse_Out(10000*X);
	}
	else{
		PA7_Pulse_Out(pa1_ICpulse/X);
//		PA7_Pulse_Out(10000/X);
	}
}

uint8_t tick_l,tick_r;
void led_handle(){
	if(oc_mode==0)led_sta|=(0x1<<1);
	else led_sta &= ~(0x1<<1);
	
	
	if(oc_mode==1)led_sta|=(0x1<<2);
	else led_sta &= ~(0x1<<2);
	
	
	if(uwTick%100>50)tick_r=1;
	else tick_r=0;

	if(pa4_adc>pa5_adc*Y){
		
		if(tick_l==0&&tick_r==1)led_sta ^= (0x1<<3);
	}
	else{
		led_sta &= ~(0x1<<3);
	}
	
	tick_l=tick_r;
	
	
	
	if(lcd_show==0)led_sta|= (0x1<<4);
	else led_sta &= ~(0x1<<4);
	
}


void uart_handle(){
	if(rx_flag>0){
		HAL_Delay(50);
		
		if(strcmp(uart_buf,"X")==0){
			sprintf(text,"X:%d\r\n",X);
			HAL_UART_Transmit(&huart1,text,strlen(text),50);
		}
		
		if(strcmp(uart_buf,"Y")==0){
			sprintf(text,"Y:%d\r\n",Y);
			HAL_UART_Transmit(&huart1,text,strlen(text),50);
		}
		
		if(strcmp(uart_buf,"PA1")==0){
			sprintf(text,"PA1:%d\r\n",pa1_ICpulse);
			HAL_UART_Transmit(&huart1,text,strlen(text),50);
		}
		
		if(strcmp(uart_buf,"PA4")==0){
			sprintf(text,"PA4:%.2f\r\n",pa4_adc);
			HAL_UART_Transmit(&huart1,text,strlen(text),50);
		}
		
		if(strcmp(uart_buf,"PA5")==0){
			sprintf(text,"PA5:%.2f\r\n",pa5_adc);
			HAL_UART_Transmit(&huart1,text,strlen(text),50);
		}
		
		if(strcmp(uart_buf,"#")==0){
			lcd_show=(lcd_show+1)%2;
			my_LCD_Init(lcd_show);
			LCD_Clear(Black);
		}
		
		rx_flag=0;
		memset(uart_buf,0,sizeof(uart_buf));
	}
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
	//LCD
	 LCD_Init();
//	 my_LCD_Init(1);
  
   LCD_Clear(Black);
   LCD_SetBackColor(Black);
   LCD_SetTextColor(White);

//   LCD_DisplayStringLine(Line4, (unsigned char *)"    Hello,world.   ");

	//key
	HAL_TIM_Base_Start_IT(&htim17);
	
	
	
	//led
	led_sta=0x00;
//	led_set();
	
	//IC
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_2);
	HAL_TIM_IC_Start(&htim2,TIM_CHANNEL_2);
	
	//OC
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	
	
//	PA7_Pulse_Out(2500);
	__HAL_TIM_SET_PRESCALER(&htim3,320);
	
	
	//UART
	HAL_UART_Receive_IT(&huart1,&rx_single,1);
	
	sprintf(text," Hello,world.\r\n");
	HAL_UART_Transmit(&huart1,text,strlen(text),50);
	
	
	//I2C
//	eeprom_send(0x00,0x99);
//	HAL_Delay(50);
//	sprintf(text,"eeprom is %x   ",eeprom_read(0x00));
//	LCD_DisplayStringLine(Line0, (unsigned char *)text);
	Y=eeprom_read(0x00);
	HAL_Delay(50);
	X=eeprom_read(0x01);
	HAL_Delay(50);
	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		

		led_handle();
		led_set();
		key_handle();
		lcd_sch();
		OC_act();
		uart_handle();
		
		
//		adc2_read();
//		sprintf(text,"pa4=%.2f pa5=%.2f  ",pa4_adc,pa5_adc);
//		LCD_DisplayStringLine(Line7, (unsigned char *)text);
//		
//		
//		sprintf(text,"%d     ",pa1_ICpulse);
//		LCD_DisplayStringLine(Line7, (unsigned char *)text);
//		
//		
//		if(rx_flag>0){
//			HAL_Delay(50);
//			LCD_DisplayStringLine(Line6, (unsigned char *)uart_buf);
//			memset(uart_buf,0,sizeof(uart_buf));
//			rx_flag=0;
//		}
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
