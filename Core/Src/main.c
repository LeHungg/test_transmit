/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "user_tcp.h"
#include "user_uart.h"
#include "user_util.h"
#include "Meter103_init_Meter.h"
#include "Meter103_Read_Meter.h"
#include "User_Readmeter.h"
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
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

osThreadId Uart_SimHandle;
osThreadId TaskControl_SIMHandle;
osThreadId Read_MeterHandle;
osMessageQId Meter_SIMHandle;
osSemaphoreId bsUART2PendingMessHandle;
/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
void Task_uart_sim(void const * argument);
void Control_Sim(void const * argument);
void Task_Read_Meter(void const * argument);

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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	Init_UART(&sUartDebugControl, &aRecei_UartDebug[0]);  
	Init_UART(&sUartSIMControl, &aRecei_UartSIM[0]);
  Init_Struct_UART_Meter();

//  osSemaphoreDef(bsUART2PendingMess);
//  bsUART2PendingMessHandle = osSemaphoreCreate(osSemaphore(bsUART2PendingMess), 0);
//  if (bsUART2PendingMessHandle == NULL)
//  {
//    Error_Handler();
//  }

	__HAL_UART_ENABLE_IT(&UART_METER, UART_IT_RXNE);    // Debug - Read Meter
	__HAL_UART_ENABLE_IT(&UART_SIM, UART_IT_RXNE);      // Serial - Sim800
	__HAL_UART_ENABLE_IT(&UART_SERIAL, UART_IT_RXNE);   // Sim800 - Serial
    
    HAL_GPIO_WritePin(NET485IO_GPIO_Port, NET485IO_Pin, GPIO_PIN_SET);            //RS485 send/receive. cac tbi dung 232 phai set moi nhan dc
	//HAL_GPIO_WritePin(MCU_232_CTS_GPIO_Port, MCU_232_CTS_Pin, GPIO_PIN_RESET);    //CTS pin RS232. ch?n?con start cu moi can phia set len
	HAL_GPIO_WritePin(PWRKEY_GPIO_Port, PWRKEY_Pin, GPIO_PIN_RESET);
    
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of bsUART2PendingMess */
  osSemaphoreDef(bsUART2PendingMess);
  bsUART2PendingMessHandle = osSemaphoreCreate(osSemaphore(bsUART2PendingMess), 0);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
//  osSemaphoreDef(bsUART2PendingMess);
//  bsUART2PendingMessHandle = osSemaphoreCreate(osSemaphore(bsUART2PendingMess), 1);
//  if (bsUART2PendingMessHandle == NULL)
//  {
//    Error_Handler();
//  }
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of Meter_SIM */
  osMessageQDef(Meter_SIM, 16, uint16_t);
  Meter_SIMHandle = osMessageCreate(osMessageQ(Meter_SIM), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of Uart_Sim */
//  osThreadDef(Uart_Sim, Task_uart_sim, osPriorityHigh, 0, 256);
//  Uart_SimHandle = osThreadCreate(osThread(Uart_Sim), NULL);
//
//  /* definition and creation of TaskControl_SIM */
//  osThreadDef(TaskControl_SIM, Control_Sim, osPriorityNormal, 0, 256);
//  TaskControl_SIMHandle = osThreadCreate(osThread(TaskControl_SIM), NULL);

  /* definition and creation of Read_Meter */
  osThreadDef(Read_Meter, Task_Read_Meter, osPriorityHigh, 0, 256);
  Read_MeterHandle = osThreadCreate(osThread(Read_Meter), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 32;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 300;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, RESET_L501_Pin|MCU_232_CTS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PWRKEY_Pin|SIM_DTR_Pin|NET485IO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MCU_232_RTS_GPIO_Port, MCU_232_RTS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ON_OFF_SIM_GPIO_Port, ON_OFF_SIM_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RESET_L501_Pin MCU_232_CTS_Pin */
  GPIO_InitStruct.Pin = RESET_L501_Pin|MCU_232_CTS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PWRKEY_Pin SIM_DTR_Pin NET485IO_Pin */
  GPIO_InitStruct.Pin = PWRKEY_Pin|SIM_DTR_Pin|NET485IO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : MCU_232_RTS_Pin */
  GPIO_InitStruct.Pin = MCU_232_RTS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MCU_232_RTS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ON_OFF_SIM_Pin */
  GPIO_InitStruct.Pin = ON_OFF_SIM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ON_OFF_SIM_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PGOOD_Pin */
  GPIO_InitStruct.Pin = PGOOD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PGOOD_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_Task_uart_sim */
/**
  * @brief  Function implementing the Uart_Sim thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Task_uart_sim */
void Task_uart_sim(void const * argument)
{
  /* USER CODE BEGIN 5 */
  for(;;)
  {
//    if(Check_Time_Out_2(sUartDebugControl.LandMarkRecei_u32 , 50))
//    {
//        sUartDebugControl.Pending = 1;
//        sUartDebugControl.LandMarkRecei_u32=0;
//        HAL_UART_Transmit(&UART_SERIAL, sUartDebugControl.strRecei.Data_a8 , sUartDebugControl.strRecei.Length_u16 , 1000);
//    }
    TCP_ProcessResponse();
    osDelay(20);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Control_Sim */
/**
* @brief Function implementing the TaskControl_SIM thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Control_Sim */
void Control_Sim(void const * argument)
{
  /* USER CODE BEGIN Control_Sim */
  for(;;)
  {
    TCP_StateMachineProcess();
    osDelay(20);
  }
  /* USER CODE END Control_Sim */
}

/* USER CODE BEGIN Header_Task_Read_Meter */
/**
* @brief Function implementing the Read_Meter thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_Read_Meter */
void Task_Read_Meter(void const * argument)
{
  /* USER CODE BEGIN Task_Read_Meter */
  /* Infinite loop */
  for(;;)
  {

    METER103_Handshake(0); // read
      
//     __HAL_UART_DISABLE_IT(&huart3, UART_IT_RXNE);
//     HAL_StatusTypeDef status_1 = UART_Change_Baudrate(&huart3, 4800U);
//    if (status_1 == HAL_OK)
//    {
//        osDelay(100);
//    }
//   
//    __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);
//     osDelay(500);
//    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart3,(uint8_t*)"a\r\n",3,1000);
//    if (status == HAL_OK)
//    {
//        HAL_UART_Transmit(&huart1,(uint8_t*)"\r\nUART 3 OK",11,1000);
//    }
    osDelay(10000);
  }
  /* USER CODE END Task_Read_Meter */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
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
