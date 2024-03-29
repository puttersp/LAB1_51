/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
TIM_HandleTypeDef htim11;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

static int b[16];
static int bp[16];

GPIO_PinState HIGH = 1;
GPIO_PinState LOW = 0;

static int b_state_curr,b_state_last;

static uint32_t timestamp = 100;

//fix_init

void   set(int x){switch(x){case 0:HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,HIGH);break;		//R1
							case 1:HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9 ,HIGH);break;		//R2
							case 2:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5 ,HIGH);break;		//R3
							case 3:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8 ,HIGH);break;}	//R4
}

void reset(int x){switch(x){case 0:HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,LOW);break;		//R1
							case 1:HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9 ,LOW);break;		//R2
							case 2:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5 ,LOW);break;		//R3
							case 3:HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8 ,LOW);break;}		//R4
}

//fix_end

int get_readButton(){
	for(int i =0;i<16;i++){
		if(b[i] == 0){
			return i;
		}
	}
	return 99;
}

int get_savedButton(){
	for(int i =0;i<16;i++){
		if(bp[i] == 0){
			return i;
		}
	}
	return 99;
}

void readButton(){
	static int x = 0;
	b_state_curr = 0;

	b[(x*4)  ]   = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9); //L1
	b[(x*4)+1]   = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7); //L2
	b[(x*4)+2]   = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6); //L3
	b[(x*4)+3]   = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7); //L4


	x++;
	x = x % 4;

	if(get_readButton() != 99){b_state_curr = 1;}

}

void statecheck(){

	  static int state = 0;
	  static int fault = 0;
	  static int faultState = 0;

	  switch(state){
	  case 0:	//non pressed
		  	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
			  if(get_savedButton() == 9){state++;}
			  else if(get_savedButton() == 12){state=0;}
			  else if(get_savedButton() == 99){state=0;}
			  else{state = 99;fault++;faultState=0;}
			  break;
	  case 1:	// 6
			  if(get_savedButton() == 1){state++;}
			  else if(get_savedButton() == 12){state=0;}
			  else if(get_savedButton() == 13){state--;}
			  else if(get_savedButton() == 99){state=1;}
			  else{state = 99;fault++;faultState=1;}
			  break;
	  case 2:	// 64
			  if(get_savedButton() == 10){state++;}
			  else if(get_savedButton() == 12){state=0;}
			  else if(get_savedButton() == 13){state--;}
			  else if(get_savedButton() == 99){state=2;}
			  else{state = 99;fault++;faultState=2;}
			  break;
	  case 3:	// 643
			  if(get_savedButton() == 1){state++;}
			  else if(get_savedButton() == 12){state=0;}
			  else if(get_savedButton() == 13){state--;}
			  else if(get_savedButton() == 99){state=3;}
			  else{state = 99;fault++;faultState=3;}
			  break;
	  case 4:	// 6434
			  if(get_savedButton() == 3){state++;}
			  else if(get_savedButton() == 12){state=0;}
			  else if(get_savedButton() == 13){state--;}
			  else if(get_savedButton() == 99){state=4;}
			  else{state = 99;fault++;faultState=4;}
			  break;
	  case 5:	// 64340
			  if(get_savedButton() == 5){state++;}
			  else if(get_savedButton() == 12){state=0;}
			  else if(get_savedButton() == 13){state--;}
			  else if(get_savedButton() == 99){state=5;}
			  else{state = 99;fault++;faultState=5;}
			  break;
	  case 6:	//643405
			  if(get_savedButton() == 3){state++;}
			  else if(get_savedButton() == 12){state=0;}
			  else if(get_savedButton() == 13){state--;}
			  else if(get_savedButton() == 99){state=6;}
			  else{state = 99;fault++;faultState=6;}
			  break;
	  case 7:	//6434050
			  if(get_savedButton() == 3){state++;}
			  else if(get_savedButton() == 12){state=0;}
			  else if(get_savedButton() == 13){state--;}
			  else if(get_savedButton() == 99){state=7;}
			  else{state = 99;fault++;faultState=7;}
			  break;
	  case 8:	//64340500
			  if(get_savedButton() == 3){state++;}
			  else if(get_savedButton() == 12){state=0;}
			  else if(get_savedButton() == 13){state--;}
			  else if(get_savedButton() == 99){state=8;}
			  else{state = 99;fault++;faultState=8;}
			  break;
	  case 9:	//643405000
			  if(get_savedButton() == 5){state++;}
			  else if(get_savedButton() == 12){state=0;}
			  else if(get_savedButton() == 13){state--;}
			  else if(get_savedButton() == 99){state=9;}
			  else{state = 99;fault++;faultState=9;}
			  break;
	  case 10:	//6434050005
			  if(get_savedButton() == 2){state++;}
			  else if(get_savedButton() == 12){state=0;}
			  else if(get_savedButton() == 13){state--;}
			  else if(get_savedButton() == 99){state=10;}
			  else{state = 99;fault++;faultState=10;}
			  break;
	  case 11:	//64340500051
			  if(get_savedButton() == 15){HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1);state++;}
			  else if(get_savedButton() == 12){state=0;}
			  else if(get_savedButton() == 13){state--;}
			  else if(get_savedButton() == 99){state=11;}
			  else{state = 99;fault++;faultState=11;}
			  break;
	  case 12: //64340500051 OK
	  	  if(get_savedButton() == 99){state=12;}
	  	  else if(get_savedButton() == 12){HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);state=0;}
	  	  break;
	  case 99:
		  if(get_savedButton() == 12){state=0;fault=0;}
		  if(get_savedButton() == 13){fault--;}
		  else{fault++;}

		  if(fault == 0){
			  state = faultState;
		  }

		  break;
	  }

}

void saveButton(){
	for(int i=0;i<16;i++){
		bp[i] = b[i];
	}
}
//void b_to_16(){}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM11_Init(void);
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

	static int process = 0;

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
  MX_USART2_UART_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  b[100] = 0;
  bp[100] = 0;

  while (1)
  {
	  if(HAL_GetTick()>timestamp){
		  timestamp = HAL_GetTick()+10;

		  switch(process){
		  case 0:
			  reset(0);
			  readButton();
			  set(0);
			  reset(1);
			  process = 1;
			  break;

		  case 1:
		  	  readButton();
		      set(1);
		  	  reset(2);
		  	  process = 2;
		  	  break;

		  case 2:
		  	  readButton();
		      set(2);
		  	  reset(3);
		  	  process = 3;
		  	  break;

	  	  case 3:
	  		  readButton();
	  		  set(3);
	  		  reset(0);

	  		  if(b_state_curr == 1 && b_state_last == 0){
	  			  saveButton();
	  			  statecheck();
	  		  }

	  		  b_state_last = b_state_curr;
	  		  process = 0;
	  		  break;
		  }

	  }
  }



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */



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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM11 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 0;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 65535;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim11, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin PA10 */
  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA7 PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB5 PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
