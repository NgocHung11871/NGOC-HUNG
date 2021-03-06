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
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
uint16_t data[2];
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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
int k = 0;              /* k = 1 _ quay thuan  ,  k = 0 quay nghich*/
  int duty_set = 650;       /* duty_set = ((dien_ap / 24 )*1000)  < 1000*/
  int run = 0 , duty_out = 0 ;
	uint16_t duty, duty_1, duty_2  ;
  
HAL_ADC_Start_DMA(&hadc1, (uint32_t*)data, 2);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {   
		if(  (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)==0) && (duty_set  <= 820))  //CHâN PA3 tang toc
		{
			HAL_Delay(20);
			while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)==0);
			duty_set =duty_set  + 170;
			HAL_Delay(50);
		}
		if(  (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)==0) && (duty_set >= 480) )   //CHÂN PA4 giam toc
		{
			HAL_Delay(20);
			while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)==0);
			duty_set =duty_set  - 170;
			HAL_Delay(50);
		}
		
		// Can Joystick khong dich chuyen
if( (data[0] <= 3000) && (data[0] >= 1000) && (data[1] <= 3000) && (data[1] >= 1000) )            /* Stop */
				 {
				   HAL_Delay(50);
					  if( (data[0] <= 3000) && (data[0] >= 1000) && (data[1] <= 3000) && (data[1] >= 1000) )	
						 {   
									
            for(duty=duty_out;duty > 0 ;duty=duty - 10)                   /* Nguon 24 V */
			         {
						__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,duty);
						__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,duty);   
						HAL_Delay(20); 
			          }
							  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);  //Van toc dong co ve 0
							  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);	//Van toc dong co ve 0
								duty_out =0;   								
						 }
						 
				 }
			// Câu lenh tien thanh 
				 
if( data[0] > 3000)                                                   /* forward */
				 {
				      HAL_Delay(50);
					    if(( data[0] > 3000))
						    {
	
										HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);   // Quay thuan dong co
										HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);   // Quay thuan dong co
								
										HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
										HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
				             for(duty=130;duty <= duty_set ;duty=duty+10)                 /* Nguon 24 V */
			                 {
								__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,duty);
								__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,duty);
												/* if( (data[0] <= 3000) && (data[0] >= 1000) && (data[1] <= 3000) && (data[1] >= 1000) )
												   {break;}  */
      	                  HAL_Delay(20); 
			                 }
											 duty_out =duty;			 
				        } 
              							
				 }
while((data[0] > 3000)&& (data[1] <= 3000) && (data[1] >= 1000))
             {
							 k++;
							 if(k>1000)
                  {
								     while( (data[0] >= 1000) || (data[1] > 3000) || (data[1] < 1000) )	{HAL_Delay(10);}
									}
						   HAL_Delay(10);
						 }
				
// Di lui thang						 
 if( data[0] < 1000)                                                    /*  backward  */
				   {
				    HAL_Delay(50);
					    if( data[0] < 1000)
						    {
									for(duty=duty_out;duty > 0 ;duty=duty - 10)                   /* Nguon 24 V */
										{
											__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,duty);
											__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,duty);   
											HAL_Delay(20); 
										}
							  
										__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
										__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);
										duty_out =0; 
								
								  	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);   //quay nguoc dong co
										HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);	 // quay nguoc dong co
										
										HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
										HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
										
				             for(duty=0;duty <= duty_set ;duty=duty+10)                 /* Nguon 24 V */
			                 {
                        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,duty);
											  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,duty);
											if( (data[0] <= 3000) && (data[0] >= 1000) && (data[1] <= 3000) && (data[1] >= 1000) )
												   {break;}     
      	                  HAL_Delay(20); 
			                 }
											 duty_out = duty; 		 
				        }     
				 }
while((data[0] < 1000)&& (data[1] <= 3000) && (data[1] >= 1000)){HAL_Delay(10);}	
				 

// Quay phai
if(data[1] > 3000)
					 {{
					   HAL_Delay(50);
						 if( data[1] > 3000)
						    {
						if(duty_out >300)
							{		
								for(duty=duty_out;duty > 0 ;duty = duty - 10)                   /* forward => right*/
			               {                  
  							 	    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,duty);   // dong co phai giam dan van toc
      	                HAL_Delay(20); 
							       }
								
							}
							
							else{ 
								    for(duty=duty_out;duty < 300 ;duty=duty+10)                 /* stop => right  */
			                 {
                        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,duty);   // dong co trai tang van toc
											  HAL_Delay(20); 
											 } 
								     		 
					         }
							   }
			   	}
					while(data[1] > 3000){HAL_Delay(10);}	 
		
		if(duty_out > 300){			if( (data[0] <= 3000) && (data[0] >= 1000) && (data[1] <= 3000) && (data[1] >= 1000) )            // can o vi tri giua
			                 	 {
				                    HAL_Delay(50);
					                  if( (data[0] <= 3000) && (data[0] >= 1000) && (data[1] <= 3000) && (data[1] >= 1000) )	
						                  { 
												        	for(duty=duty_out;duty > 300 ;duty=duty - 10)                
			                             {                  
  							 	                  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,duty);   // Sau khi quay thi giam toc do dong co ben trai 
      	                              HAL_Delay(20);  
												           }
												      }
			                   }duty_out =duty;
										}
		if((duty_out > 300) &&  (data[0] > 3000))                   //Neu có tin hieu tien                 /* forward */
				                       {
				                        HAL_Delay(50);
					                       if( data[0] > 3000)
						                               { 
															                 for(duty=300;duty < duty_out ;duty=duty + 10)                   /* forward => right*/
			                                                   {                  
  							 	                                       __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,duty);   //Tang toc do ben phai
      	                                                   HAL_Delay(20); 
							                                            }
															             }
															 }
		if(duty_out < 300){			if( (data[0] <= 3000) && (data[0] >= 1000) && (data[1] <= 3000) && (data[1] >= 1000) )            //can o vi tri giua
			                 	 {
				                    HAL_Delay(50);
					                  if( (data[0] <= 3000) && (data[0] >= 1000) && (data[1] <= 3000) && (data[1] >= 1000) )	
						                  { 
												        	for(duty=300;duty > duty_out ;duty=duty-10)                 /* stop => right  */
			                               {
                                     __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,duty);  //dong co trai giam
											                HAL_Delay(20); 
											               } 
												      }
			                   }
									    	}													 
		if(duty_out < 300){  if( data[0] > 3000)                                                   /* forward */
				                   {
				                   HAL_Delay(50);
					                  if( data[0] > 3000)
						                 { 
													     for(duty=duty_out;duty < 300 ;duty=duty+10)                 /* stop => right  */
			                               {
                                       __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,duty);  //dong co phai tang
											                   HAL_Delay(20); 
											               }
													   }
													 }   
		                  }duty_out =duty;
					 }

if(data[1] < 1000)
					{ {
					   HAL_Delay(50);
						 if( data[1] < 1000)
						    {
									for(duty=duty_out;duty > 300 ;duty=duty - 10)                   /* Nguon 24 V */
			          {
                   __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,duty);
  						  
      	            HAL_Delay(20); 
			          }
								duty_out =duty;
								  for(duty=duty_out;duty < 300 ;duty=duty+10)                 /* Nguon 24 V */
			                 {
                        
											 __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,duty);
											} 
									 
					 }
								
				} while(	data[1] < 1000){HAL_Delay(10);}	
				if(duty_out > 300){			if( (data[0] <= 3000) && (data[0] >= 1000) && (data[1] <= 3000) && (data[1] >= 1000) )            
			                 	 {
				                    HAL_Delay(50);
					                  if( (data[0] <= 3000) && (data[0] >= 1000) && (data[1] <= 3000) && (data[1] >= 1000) )	
						                  { 
												        	for(duty=duty_out;duty > 300 ;duty=duty - 10)                
			                             {                  
  							 	                  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,duty);   
      	                              HAL_Delay(20);  
												           }
												      }
			                   }duty_out =duty;
										}
		if((duty_out > 300) &&  (data[0] < 1000))                                                   /* forward */
				                       {
				                        HAL_Delay(50);
					                       if( data[0] < 1000)
						                               { 
															                 for(duty=300;duty < duty_out ;duty=duty + 10)                   /* forward => right*/
			                                                   {                  
  							 	                                       __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,duty);   
      	                                                   HAL_Delay(20); 
							                                            }
															             }
															 }
		if(duty_out < 300){			if( (data[0] <= 3000) && (data[0] >= 1000) && (data[1] <= 3000) && (data[1] >= 1000) )            
			                 	 {
				                    HAL_Delay(50);
					                  if( (data[0] <= 3000) && (data[0] >= 1000) && (data[1] <= 3000) && (data[1] >= 1000) )	
						                  { 
												        	for(duty=300;duty > duty_out ;duty=duty-10)                 /* stop => right  */
			                               {
                                     __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,duty);
											                HAL_Delay(20); 
											               } 
												      }
			                   }
									    	}													 
		if(duty_out < 300){  if( data[0] < 1000)                                                   /* forward */
				                   {
				                   HAL_Delay(50);
					                  if( data[0] < 1000)
						                 { 
													     for(duty=duty_out;duty < 300 ;duty=duty+10)                 /* stop => right  */
			                               {
                                       __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,duty);
											                   HAL_Delay(20); 
											               }
													   }
													 }   
		                  }duty_out =duty;
				}
				
				 
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 7;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA3 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
