/**
 ******************************************************************************
 * @file			: main.cpp
 * @brief			: Main application entry point.
 * @author			: Lawrence Stanton
 ******************************************************************************
 * @attention
 *
 * © LD Stanton 2023
 *
 * This file and its content are the copyright property of the author. All
 * rights are reserved. No warranty is given. No liability is assumed.
 * Confidential unless licensed otherwise. If licensed, refer to the
 * accompanying file "LICENCE" for license details.
 *
 ******************************************************************************
 */

#include "main.hpp"
#include "cmsis_os.h"
#include "init.h"

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

int main(void) {
	HAL_Init();
	SystemClock_Config();

	MX_GPIO_Init();

	MX_I2C1_Init();
	MX_I2C2_Init();

	MX_USART1_UART_Init();
	MX_USART3_UART_Init();

	while (true) {}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM16) HAL_IncTick();
}

void Error_Handler(void) {
	__disable_irq();
	while (true) {}
}
