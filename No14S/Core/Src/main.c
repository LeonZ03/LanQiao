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

//lcd
char text[50];

//DATA 
char pwm_mode='L';
uint8_t mode_change=0;
int cur_duty;
int cur_psc=200;
float cur_v;

//PARA
int K=1,R=1;

//RECD
int N=0;
float MH,ML;
float last_lvm,last_hvm;
struct _count vm[2];


//adc
double adc_val;
int tmp_duty;


//B1
uint8_t lcd_sch=0;
uint32_t tick_mod,shining_mod;
uint8_t tick_l=0,tick_r=0,shining_l=0,shining_r=0;



//B2
uint64_t last_tick=0;
uint8_t RK_select=0;
int K_tmp=1,R_tmp=1;

//B3



//B4
uint8_t R37_lock=0;


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
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim17);
	
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	__HAL_TIM_SET_PRESCALER(&htim2,200);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,10);
	
	HAL_TIM_IC_Start_IT(&htim17,TIM_CHANNEL_1);
	HAL_TIM_IC_Start(&htim17,TIM_CHANNEL_1);
	
	
	
	LCD_Init();
	
  LCD_SetBackColor(Black);
  LCD_SetTextColor(White);
	LCD_Clear(Black);
	

  //LCD_DisplayStringLine(Line4, (unsigned char *)"    Hello,world.   ");

	
	led_sta=0;
	led_set();
	
	
	
	
	
	vm[0].v_id=vm[1].v_id=-1;
	vm[0].start_tick=vm[1].start_tick=0xffffffff;
	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
			
		//R37 control duty
		adc_val=adc_read();
		tmp_duty=v_to_duty(adc_val);
		if(R37_lock==0)cur_duty=tmp_duty;
		
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,cur_duty);
		__HAL_TIM_SET_PRESCALER(&htim2,cur_psc);
		
//		sprintf(text,"frq=%.2f   ",frq);
//		LCD_DisplayStringLine(Line4, (unsigned char *)text);
		
		//led
		led_set();
		
		mode_change_task();
		
		
		key_task();
		
		lcd_task();
		
		recd_task();
		
		
		
		
		
		
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

void lcd_task(){
	//lcd_sch
		if(lcd_sch==0){
			sprintf(text,"        DATA  ");
			LCD_DisplayStringLine(Line1, (unsigned char *)text);
			
			sprintf(text,"     M=%c  ",pwm_mode);
			LCD_DisplayStringLine(Line3, (unsigned char *)text);
			
			sprintf(text,"     P=%d%%   ",cur_duty);
			LCD_DisplayStringLine(Line4, (unsigned char *)text);
			
			sprintf(text,"     V=%.1f    ",cur_v);
			LCD_DisplayStringLine(Line5, (unsigned char *)text);
			
			led_sta|=0x1<<1;
		}
		else if(lcd_sch==1){
			sprintf(text,"        PARA  ");
			LCD_DisplayStringLine(Line1, (unsigned char *)text);
			
			sprintf(text,"     R=%d   ",R_tmp);
			LCD_DisplayStringLine(Line3, (unsigned char *)text);
			
			sprintf(text,"     K=%d   ",K_tmp);
			LCD_DisplayStringLine(Line4, (unsigned char *)text);
			
			led_sta &=~(0x1<<1);
		}
		else{
			K=K_tmp;
			R=R_tmp;
			
			sprintf(text,"        RECD  ");
			LCD_DisplayStringLine(Line1, (unsigned char *)text);
			
			sprintf(text,"     N=%d   ",N);
			LCD_DisplayStringLine(Line3, (unsigned char *)text);
			
			sprintf(text,"     MH=%.1f    ",MH);
			LCD_DisplayStringLine(Line4, (unsigned char *)text);
			
			sprintf(text,"     ML=%.1f    ",ML);
			LCD_DisplayStringLine(Line5, (unsigned char *)text);
			
			//led_sta &=~(0x1<<1);
		
		}
		
}



void key_task(){
	//key
		if(key[1].flag==1){
//			led_sta^=0x1<<1;
//			led_set();
			
			lcd_sch=(lcd_sch+1)%3;
			LCD_Clear(Black);
			led_set();
			
			key[1].flag=0;
		}
		
		if(key[2].flag==1){
			if(lcd_sch==0&&uwTick-last_tick>5000){
				if(pwm_mode=='L')mode_change=2;
				else mode_change=1;
				
				N++;
				
				last_tick=uwTick;
			}
			
			else if(lcd_sch==1){
				RK_select=(RK_select+1)%2;
			}
			
			key[2].flag=0;
		}
		
		if(key[3].flag==1&&lcd_sch==1){
			if(RK_select==0)R_tmp=R_tmp%10+1;
			else K_tmp=K_tmp%10+1;
			
			key[3].flag=0;
		}
		
		if(key[4].flag==1){
			
			if(lcd_sch==1){

				if(RK_select==0)R_tmp=(R_tmp+8)%10+1;
				else K_tmp=(K_tmp+8)%10+1;

			}
			else if(lcd_sch==0){
				R37_lock=0;
				led_sta &= ~(0x4<<1);
			}
			
			key[4].flag=0;
		}
		
		if(key[4].flag==3&&lcd_sch==0){
			led_sta^=0x1<<1;
			led_set();
			R37_lock=1;
			led_sta |= (0x4<<1);
			
			key[4].flag=0;
		}
}



void mode_change_task(){
	if(mode_change!=0){
			if(mode_change==1){
				
				if(uwTick%250<tick_mod)tick_r=0;
				else tick_r=1;
				
				if(uwTick%100<shining_mod)shining_r=0;
				else shining_r=1;
				
				if(cur_psc==100){//start
					tick_mod=125;
					tick_l=0;
					tick_r=1;
					
					shining_mod=50;
					shining_l=0;
					shining_r=1;
				}
	
				
				if(shining_l==0&&shining_r==1){
					led_sta^=(0x2<<1);
					led_set();
				}
				
				shining_l=shining_r;
				
				
					
				if(tick_l==0&&tick_r==1){
					cur_psc+=5;
				}
				
				tick_l=tick_r;
				
				
				
				if(cur_psc>=200){//end
					cur_psc=200;
					mode_change=0;
					pwm_mode='L';
					
					led_sta &= ~(0x2<<1);
					led_set();
				}
				
			}
			else{
				
				if(uwTick%250<tick_mod)tick_r=0;
				else tick_r=1;
				
				if(uwTick%100<shining_mod)shining_r=0;
				else shining_r=1;
				
				if(cur_psc==200){//start
					tick_mod=125;
					tick_l=0;
					tick_r=1;
					
					shining_mod=50;
					shining_l=0;
					shining_r=1;
				}
	
				
				if(shining_l==0&&shining_r==1){
					led_sta^=(0x2<<1);
					led_set();
				}
				
				shining_l=shining_r;
				
				
					
				if(tick_l==0&&tick_r==1){
					cur_psc-=5;
				}
				
				tick_l=tick_r;
				
				
				
				if(cur_psc<=100){//end
					cur_psc=100;
					mode_change=0;
					pwm_mode='H';
					
					led_sta &= ~(0x2<<1);
					led_set();
				}
				
			}
		}
		
}


void recd_task(){
	
		if(pwm_mode=='L'){
			if(ML<cur_v){
				if(vm[0].v_id<0){
					vm[0].v_id=cur_v;
					vm[0].start_tick=uwTick;
				}
				else{
					if((int)(vm[0].v_id*10)==(int)(cur_v*10)){
						if(uwTick-vm[0].start_tick>2000){//change to larger
							ML=cur_v;
							vm[0].v_id=-1;
							vm[0].start_tick=0xffffffff;
						}
					}
					else{
						vm[0].v_id=-1;
						vm[0].start_tick=0xffffffff;
					}
				}
			}
		}
		else{
			if(MH<cur_v){
				if(vm[1].v_id<0){
					vm[1].v_id=cur_v;
					vm[1].start_tick=uwTick;
				}
				else{
					if((int)(vm[1].v_id*10)==(int)(cur_v*10)){
						if(uwTick-vm[1].start_tick>2000){//change to larger
							MH=cur_v;
							vm[1].v_id=-1;
							vm[1].start_tick=0xffffffff;
						}
					}
					else{
						vm[1].v_id=-1;
						vm[1].start_tick=0xffffffff;
					}
				}
			}
		
		}
		
		
}







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
