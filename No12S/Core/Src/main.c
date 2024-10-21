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
  MX_TIM3_Init();
  MX_TIM17_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim3);
	
	HAL_TIM_Base_Start(&htim17);
	HAL_TIM_Base_Start_IT(&htim17);
	HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);
	uint8_t pwm_sta=0;
	
	HAL_UART_Receive_IT(&huart1,&rx_sigl,1);

	LCD_Init();
  
	
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	
	//LCD_DisplayStringLine(Line0, (uint8_t *)"hello");




//	led_sta=0x55<<1;
//	led_set();



//	sprintf(txdata,"hello world\r\n");
//	HAL_UART_Transmit(&huart1,(const uint8_t*)txdata,strlen(txdata),50);







  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		if(key[1].flag==1){
			menu=(menu+1)%2;
			LCD_Clear(Black);
			key[1].flag=0;
		}
		
		if(key[2].flag==1){
			cnbr_fee+=0.5;
			vnbr_fee+=0.5;
			key[2].flag=0;
		}
		
		if(key[3].flag==1){
//			if(cnbr_fee>=0.5)cnbr_fee-=0.5;
//			else cnbr_fee=0;
			
			cnbr_fee-=0.5;
				
//			if(vnbr_fee>=0.5)vnbr_fee-=0.5;
//			else vnbr_fee=0;
			
			vnbr_fee-=0.5;
			
			key[3].flag=0;
		}
		
		
		if(key[4].flag==1){
			pwm_sta=(pwm_sta+1)%2;
			key[4].flag=0;
		}
		
		
		
		if(rx_flag>0){
			HAL_Delay(50);
			park_handler();
			rx_flag=0;
			memset(rxdata,0,sizeof(rxdata));
			
		}
		
		
		if(menu==0){
			sprintf(text,"       Data");
			LCD_DisplayStringLine(Line1, (uint8_t *)text);
			sprintf(text,"   CNBR:%d",cnbr_num);
			LCD_DisplayStringLine(Line3, (uint8_t *)text);
			sprintf(text,"   VNBR:%d",vnbr_num);
			LCD_DisplayStringLine(Line5, (uint8_t *)text);
			sprintf(text,"   IDLE:%d",idle_num);
			LCD_DisplayStringLine(Line7, (uint8_t *)text);
		}
		else{
			sprintf(text,"       Para");
			LCD_DisplayStringLine(Line1, (uint8_t *)text);
			sprintf(text,"   CNBR:%.2f",cnbr_fee);
			LCD_DisplayStringLine(Line3, (uint8_t *)text);
			sprintf(text,"   VNBR:%.2f",vnbr_fee);
			LCD_DisplayStringLine(Line5, (uint8_t *)text);
		}
		
		
		if(pwm_sta==0){
			__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,0);
			led_sta&=~(1<<2);
		}
		else {
			__HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,20);
			led_sta|=(1<<2);
		}
		
		if(idle_num>0)led_sta|=(1<<1);
		else led_sta&=~(1<<1);
		
		led_set();

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
