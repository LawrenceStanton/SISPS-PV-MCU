/**
 ******************************************************************************
 * @file			: init.c
 * @brief			: Peripheral Initialisation Functions
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

#include "init.h"

extern I2C_HandleTypeDef  hi2c1;
extern I2C_HandleTypeDef  hi2c2;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType	  = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState			  = RCC_HSI_ON;
	RCC_OscInitStruct.HSIDiv			  = RCC_HSI_DIV1;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState		  = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { Error_Handler(); }

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType		 = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource	 = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider	 = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) { Error_Handler(); }
}

void MX_I2C1_Init(void) {
	hi2c1.Instance				= I2C1;
	hi2c1.Init.Timing			= 0x00'30'3D'5B;
	hi2c1.Init.OwnAddress1		= 0;
	hi2c1.Init.AddressingMode	= I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode	= I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2		= 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode	= I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode	= I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) { Error_Handler(); }

	/* Configure Analogue filter */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK) { Error_Handler(); }

	/* Configure Digital filter */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) { Error_Handler(); }
}

void MX_I2C2_Init(void) {
	hi2c2.Instance				= I2C2;
	hi2c2.Init.Timing			= 0x00'30'3D'5B;
	hi2c2.Init.OwnAddress1		= 0;
	hi2c2.Init.AddressingMode	= I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode	= I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2		= 0;
	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c2.Init.GeneralCallMode	= I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode	= I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK) { Error_Handler(); }

	/* Configure Analogue filter */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK) { Error_Handler(); }

	/* Configure Digital filter */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK) { Error_Handler(); }
}

void MX_USART1_UART_Init(void) {
	huart1.Instance					   = USART1;
	huart1.Init.BaudRate			   = 115'200;
	huart1.Init.WordLength			   = UART_WORDLENGTH_8B;
	huart1.Init.StopBits			   = UART_STOPBITS_1;
	huart1.Init.Parity				   = UART_PARITY_NONE;
	huart1.Init.Mode				   = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl			   = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling		   = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling		   = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.Init.ClockPrescaler		   = UART_PRESCALER_DIV1;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart1) != HAL_OK) { Error_Handler(); }
	if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) { Error_Handler(); }
	if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) { Error_Handler(); }
	if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK) { Error_Handler(); }
}

void MX_USART3_UART_Init(void) {
	huart3.Instance					   = USART3;
	huart3.Init.BaudRate			   = 115'200;
	huart3.Init.WordLength			   = UART_WORDLENGTH_8B;
	huart3.Init.StopBits			   = UART_STOPBITS_1;
	huart3.Init.Parity				   = UART_PARITY_NONE;
	huart3.Init.Mode				   = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl			   = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling		   = UART_OVERSAMPLING_16;
	huart3.Init.OneBitSampling		   = UART_ONE_BIT_SAMPLE_DISABLE;
	huart3.Init.ClockPrescaler		   = UART_PRESCALER_DIV1;
	huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
	huart3.AdvancedInit.Swap		   = UART_ADVFEATURE_SWAP_ENABLE;
	if (HAL_UART_Init(&huart3) != HAL_OK) { Error_Handler(); }
}

void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, LED1_Pin | LED2_Pin | LED3_Pin | PM_FORCE_Pin | STOP_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(RDY_GPIO_Port, RDY_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : LED1_Pin LED2_Pin LED3_Pin PM_FORCE_Pin STOP_Pin */
	GPIO_InitStruct.Pin	  = LED1_Pin | LED2_Pin | LED3_Pin | PM_FORCE_Pin | STOP_Pin;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : FUSE_OC_Pin FUSE_OK_Pin PGOOD_Pin SMBA_Pin */
	GPIO_InitStruct.Pin	 = FUSE_OC_Pin | FUSE_OK_Pin | PGOOD_Pin | SMBA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : RDY_Pin */
	GPIO_InitStruct.Pin	  = RDY_Pin;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(RDY_GPIO_Port, &GPIO_InitStruct);
}

/*** END OF FILE ***/
