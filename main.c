/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "MPU6050.h"
#include <stdio.h>
#include <math.h>

/* Private variables ---------------------------------------------------------*/
int16_t ax, ay, az;
float prevMagnitude = 0.0f;
uint32_t lastStepTime = 0;
uint32_t stepCount = 0;
const float threshold = 7000.0f;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();

  // Başlangıç mesajı
  char startMsg[] = "Step Counter Starting...\r\n";
  HAL_UART_Transmit(&huart2, (uint8_t*)startMsg, sizeof(startMsg)-1, HAL_MAX_DELAY);

  // MPU6050 başlat
  if (MPU6050_Init(&hi2c1) != HAL_OK) {
    char errorMsg[] = "MPU6050 init FAILED!\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)errorMsg, sizeof(errorMsg)-1, HAL_MAX_DELAY);
    //Error_Handler();
  }

  // İlk ölçüm
  MPU6050_Read_Accel(&hi2c1, &ax, &ay, &az);
  prevMagnitude = sqrt(ax * ax + ay * ay + az * az);

  /* Infinite loop */
  while (1)
  {
    // MPU6050 verilerini oku
    if (MPU6050_Read_Accel(&hi2c1, &ax, &ay, &az) == HAL_OK) {
      float magnitude = sqrt(ax * ax + ay * ay + az * az);
      float diff = fabs(magnitude - prevMagnitude);
      uint32_t now = HAL_GetTick();

      // Adım algıla
      if (diff > threshold && (now - lastStepTime > 400)) {
        stepCount++;
        lastStepTime = now;
      }

      prevMagnitude = magnitude;
    }

    // UART ile her 1 saniyede bir adım sayısı gönder
    static uint32_t lastPrint = 0;
    uint32_t now = HAL_GetTick();

    if (now - lastPrint >= 1000) {
      lastPrint = now;
      char msg[50];
      sprintf(msg, "Step Count: %lu\r\n", stepCount);
      HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    }

    HAL_Delay(50);
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1) {
    // Hata sırasında burada kalır
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  // Gelişmiş hata ayıklama için kullanılır (isteğe bağlı)
}
#endif /* USE_FULL_ASSERT */
