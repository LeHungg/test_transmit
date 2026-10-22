/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l4xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32l4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "user_uart.h"
#include "Meter103_Read_Meter.h"
#include "Meter103_init_Meter.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void UART_ClearErrorFlags(UART_HandleTypeDef *huart)
{
  if (__HAL_UART_GET_FLAG(huart, UART_FLAG_PE) != RESET)
  {
    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_PEF);
  }
  if (__HAL_UART_GET_FLAG(huart, UART_FLAG_FE) != RESET)
  {
    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_FEF);
  }
  if (__HAL_UART_GET_FLAG(huart, UART_FLAG_NE) != RESET)
  {
    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_NEF);
  }
  if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET)
  {
    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF);
  }
  if (__HAL_UART_GET_FLAG(huart, UART_FLAG_RTOF) != RESET)
  {
    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_RTOF);
  }
}

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim1;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line4 interrupt.
  */
void EXTI4_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_IRQn 0 */

  /* USER CODE END EXTI4_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(PGOOD_Pin);
  /* USER CODE BEGIN EXTI4_IRQn 1 */

  /* USER CODE END EXTI4_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update interrupt and TIM16 global interrupt.
  */
void TIM1_UP_TIM16_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 0 */
    RtCountSystick_u32++;
  /* USER CODE END TIM1_UP_TIM16_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM16_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
  uint8_t temp_recieve;

  if (__HAL_UART_GET_FLAG(&UART_SERIAL, UART_FLAG_RXNE) != RESET)
  {
    temp_recieve = (uint8_t)(UART_SERIAL.Instance->RDR & 0x00FFU);
    if (sUartDebugControl.Pending == 0U)
    {
      *(sUartDebugControl.strRecei.Data_a8 + sUartDebugControl.strRecei.Length_u16++) = temp_recieve;
      if (sUartDebugControl.strRecei.Length_u16 > (MAX_LENGTH_UART_DEBUG - 1U))
      {
        sUartDebugControl.OverStack = 1U;
        sUartDebugControl.strRecei.Length_u16 = 0U;
      }
      sUartDebugControl.LandMarkRecei_u32 = RtCountSystick_u32;
    }
  }
  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
  uint8_t temp_recieve;
  uint16_t receive_length;

  if (__HAL_UART_GET_FLAG(&UART_SIM, UART_FLAG_RXNE) != RESET)
  {
    temp_recieve = (uint8_t)(UART_SIM.Instance->RDR & 0x00FFU);
    if (sUartSIMControl.Pending == 0U)
    {
      receive_length = sUartSIMControl.strRecei.Length_u16;
      if (receive_length < (MAX_LENGTH_UART_SIM - 1U))
      {
        sUartSIMControl.strRecei.Data_a8[receive_length] = temp_recieve;
        sUartSIMControl.strRecei.Length_u16 = receive_length + 1U;
        sUartSIMControl.strRecei.Data_a8[receive_length + 1U] = 0U;
        sUartSIMControl.LandMarkRecei_u32 = RtCountSystick_u32;
      }
      else
      {
        sUartSIMControl.OverStack = 1U;
      }
    }
  }
  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */
  if (__HAL_UART_GET_FLAG(&UART_METER, UART_FLAG_RXNE) != RESET)
  {
    METER103_GetUART2Data();
  }
  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
