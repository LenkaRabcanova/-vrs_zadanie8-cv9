/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "HTS221.h"
#include "LPS25HB.h"
#include "stdio.h"
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
uint8_t switch_state = 0;
uint8_t mode = 0;
uint8_t length_retazec = 0;
int tim2count = 0;
int x1=0;
int x2=0;

const unsigned char seven_seg_digits_decode_abcdefg[75]= {
/*  0     1     2     3     4     5     6     7     8     9     :     ;     */
    0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x00, 0x00,
/*  <     =     >     ?     @     A     B     C     D     E     F     G     */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47, 0x5E,
/*  H     I     J     K     L     M     N     O     P     Q     R     S     */
    0x37, 0x06, 0x3C, 0x57, 0x0E, 0x6A, 0x15, 0x1D, 0x67, 0x73, 0x05, 0x5B,
/*  T     U     V     W     X     Y     Z     [     \     ]     ^     _     */
    0x0F, 0x3E, 0x1C, 0x5C, 0x13, 0x3B, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x08,
/*  `     a     b     c     d     e     f     g     h     i     j     k     */
    0x00, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47, 0x5E, 0x37, 0x06, 0x3C, 0x57,
/*  l     m     n     o     p     q     r     s     t     u     v     w     */
    0x0E, 0x6A, 0x15, 0x1D, 0x67, 0x73, 0x05, 0x5B, 0x0F, 0x3E, 0x1C, 0x5C,
/*  x     y     z     */
    0x13, 0x3B, 0x6D
};


//const unsigned char retazec[]="Jakub_Miklus_98350";
//const unsigned char retazec[]="Lenka_Rabcanova_98364";
char retazec[20];
volatile int orientation=0; //0=left ; 1=right
volatile int digit=0;
volatile int textposition = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void resetSegments(void)
{
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_11);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_8);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_5);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_4);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_0);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_1);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_3);
}

/* Reset (turn-off) all digits*/
void resetDigits(void)
{

//	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_2);
//	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
//	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4);
//	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_6);
//	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_7);

	LL_GPIO_ResetOutputPin(digit0_GPIO_Port, digit0_Pin);
	LL_GPIO_ResetOutputPin(digit1_GPIO_Port, digit1_Pin);
	LL_GPIO_ResetOutputPin(digit2_GPIO_Port, digit2_Pin);
	LL_GPIO_ResetOutputPin(digit3_GPIO_Port, digit3_Pin);
	LL_GPIO_ResetOutputPin(digitTime_GPIO_Port, digitTime_Pin);
}





unsigned char decode_7seg(unsigned char chr)
{ /* Implementation uses ASCII */
	if (chr == '.') {
		return (1<<7); //DP
	}
	if (chr == '-') {
		return (1<<0); //segment G
	}

    if (chr > (unsigned char)'z')
        return 0x00;
    return seven_seg_digits_decode_abcdefg[chr - '0'];
}


void display_symbol(char symbol,int digit)
{
	switch (digit)
	{
	    case 0:
                DIGIT_0_ON;
	      break;

	    case 1:
	    	DIGIT_1_ON;
	      break;

	    case 2:
	    	DIGIT_2_ON;
	      break;

	    case 3:
	    	DIGIT_3_ON;
	      break;
	}



	char pomocna=symbol;
	pomocna &= 1;

	if(pomocna==1)
		LL_GPIO_ResetOutputPin(segmentG_GPIO_Port, segmentG_Pin);



	pomocna=symbol;
	pomocna >>= 1;
	pomocna &= 1;

	if(pomocna==1)
		LL_GPIO_ResetOutputPin(segmentF_GPIO_Port, segmentF_Pin);



	pomocna=symbol;
	pomocna >>= 2;
	pomocna &= 1;

	if(pomocna==1)
		LL_GPIO_ResetOutputPin(segmentE_GPIO_Port, segmentE_Pin);



	pomocna=symbol;
	pomocna >>= 3;
	pomocna &= 1;

	if(pomocna==1)
		LL_GPIO_ResetOutputPin(segmentD_GPIO_Port, segmentD_Pin);



	pomocna=symbol;
	pomocna >>= 4;
	pomocna &= 1;

	if(pomocna==1)
		LL_GPIO_ResetOutputPin(segmentC_GPIO_Port, segmentC_Pin);


	pomocna=symbol;
	pomocna >>= 5;
	pomocna &= 1;

	if(pomocna==1)
		LL_GPIO_ResetOutputPin(segmentB_GPIO_Port, segmentB_Pin);



	pomocna=symbol;
	pomocna >>= 6;
	pomocna &= 1;

	if(pomocna==1)
		LL_GPIO_ResetOutputPin(segmentA_GPIO_Port, segmentA_Pin);

}

void display_dot()
{
    LL_GPIO_ResetOutputPin(segmentDP_GPIO_Port, segmentDP_Pin);
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

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/
  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /* USER CODE BEGIN Init */
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  LL_TIM_EnableCounter(TIM2);
  LL_TIM_EnableIT_UPDATE(TIM2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  lps25hb_init();
  hts221_init();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	   if(mode==0){
		   float temperature = hts221_get_temperature();
		   if(temperature>=100) {
			   temperature = 99.9;
		   }
		   if(temperature<=-100) {
			   temperature = -99.9;
		   }
		   sprintf(retazec, "TEMP_%03.1f", temperature);
		   //strcpy(retazec, "TEMP_xx.x");
		   length_retazec = strlen(retazec);

	   }
	   if(mode==1){
		   float humidity = hts221_get_relative_humidity();
		   if(humidity>99) {
			   humidity = 99;
		   }
		   if(humidity<=0) {
		   	  humidity = 0;
		   }
		   sprintf(retazec, "HUM_%02.0f", humidity);
		   //strcpy(retazec, "HUM_xx");
		   length_retazec = strlen(retazec);

	   }
	   if(mode==2){
		   float final_pressure = lps25hb_get_pressure();
		   sprintf(retazec, "BAR_%06.2f", final_pressure);
		   //strcpy(retazec, "BAR_xxxx.xx");
		   length_retazec = strlen(retazec);

	   }
	   if(mode==3){
		   float final_pressure = lps25hb_get_pressure();
		   float alltitude = lps25hb_get_alltitude(final_pressure);
		   sprintf(retazec, "ALT_%05.1f", alltitude);
		   //strcpy(retazec, "ALT_xxxx.x");
		   length_retazec = strlen(retazec);
	   }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_Init1msTick(8000000);
  LL_SetSystemCoreClock(8000000);
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);
}

/* USER CODE BEGIN 4 */

uint8_t checkButtonState(GPIO_TypeDef* PORT, uint8_t PIN, uint8_t edge, uint8_t samples_window, uint8_t samples_required)
{
	  //type your code for "checkButtonState" implementation here:
	uint8_t button_state = 0, timeout = 0;

		while(button_state < samples_required && timeout < samples_window)
		{
			if((LL_GPIO_IsInputPinSet(PORT, PIN) && (edge==TRIGGER_RISE)) || (!LL_GPIO_IsInputPinSet(PORT, PIN) && (edge==TRIGGER_FALL)))/*LL_GPIO_IsInputPinSet(PORT, PIN)*/
			{
				button_state += 1;
			}
			else
			{
				button_state = 0;
			}

			timeout += 1;
			LL_mDelay(1);
		}

		if((button_state >= samples_required) && (timeout <= samples_window))
		{
			return 1;
		}
		else
		{
			return 0;
		}
}

void updateDisplay(void)
{
	resetDigits();
	resetSegments();
	char symbol = retazec[textposition+digit+x2];
        char aux;


        char segments = decode_7seg(symbol);

	display_symbol(segments, digit);

       
        if ((length_retazec-1)>textposition+digit)
             aux=retazec[textposition+digit+1];

	if (aux=='.'){
		display_dot();
                x2=1;
	}


	digit = digit+1;
	if(digit >= 4){
		digit=0;
		x2=0;
        }
}

//Update displayed data and keep display ON
void TIM2_IRQHandler(void)
{
	//1000 za s
	tim2count = tim2count + 1;
	if(tim2count >=500) {
		tim2count = 0; //2 krat za sekundu sa vykona tento if

		//Posun textu
		if (orientation==0){
			textposition = textposition + 1;
			if((textposition+4+x2)>=(length_retazec))
			    orientation=1;
		}

		else{
			textposition = textposition - 1;
			if((textposition)<=0)
			    orientation=0;
		}

	}

	if(LL_TIM_IsActiveFlag_UPDATE(TIM2))
	{
		updateDisplay();
	}

	LL_TIM_ClearFlag_UPDATE(TIM2);


}

void EXTI3_IRQHandler(void)
{
	if(checkButtonState(GPIO_PORT_BUTTON,
						GPIO_PIN_BUTTON,
						BUTTON_EXTI_TRIGGER,
						BUTTON_EXTI_SAMPLES_WINDOW,
						BUTTON_EXTI_SAMPLES_REQUIRED))
	{
		//switch_state ^= 1;
		mode = mode+1;
		if (mode > 3) {
			mode = 0;
		}
		textposition = 0;
		orientation = 0;
	}

	/* Clear EXTI4 pending register flag */

		//type your code for pending register flag clear here:
	EXTI->PR |= (1 << 4);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
