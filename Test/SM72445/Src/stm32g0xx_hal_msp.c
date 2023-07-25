/**
 ******************************************************************************
 * @file			: stm32g0xx_hal_msp.c
 * @brief			: Microcontroller Support Package Initialisation & Deinitialization
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

#include "main.h"

void HAL_MspInit(void) {
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();

	HAL_NVIC_SetPriority(PendSV_IRQn, 3, 0);

	HAL_SYSCFG_StrobeDBattpinsConfig(SYSCFG_CFGR1_UCPD1_STROBE | SYSCFG_CFGR1_UCPD2_STROBE);
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c) {
	GPIO_InitTypeDef		 GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit	 = {0};
	if (hi2c->Instance == I2C2) {
		PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
		PeriphClkInit.I2c1ClockSelection   = RCC_I2C1CLKSOURCE_PCLK1;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) { Error_Handler(); }

		__HAL_RCC_GPIOB_CLK_ENABLE();
		/**I2C2 GPIO Configuration
		PB13     ------> I2C2_SCL
		PB14     ------> I2C2_SDA
		*/
		GPIO_InitStruct.Pin		  = GPIO_PIN_13 | GPIO_PIN_14;
		GPIO_InitStruct.Mode	  = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull	  = GPIO_NOPULL;
		GPIO_InitStruct.Speed	  = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF6_I2C2;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* Peripheral clock enable */
		__HAL_RCC_I2C2_CLK_ENABLE();
		/* I2C2 interrupt Init */
		HAL_NVIC_SetPriority(I2C2_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(I2C2_IRQn);
	}
}
