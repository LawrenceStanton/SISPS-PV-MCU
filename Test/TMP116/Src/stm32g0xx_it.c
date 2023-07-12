/**
 ******************************************************************************
 * @file			: stm32g0xx_it.c
 * @brief			: Interrupt Service Routines
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

#include "stm32g0xx_it.h"

#include "main.h"

extern I2C_HandleTypeDef hi2c1;

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
void NMI_Handler(void) {
	while (1) {}
}

void HardFault_Handler(void) {
	while (1) {}
}

void SysTick_Handler(void) {
	HAL_IncTick();
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

void I2C1_IRQHandler(void) {
	if (hi2c1.Instance->ISR & (I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_OVR)) {
		HAL_I2C_ER_IRQHandler(&hi2c1);
	} else {
		HAL_I2C_EV_IRQHandler(&hi2c1);
	}
}

/*** END OF FILE ***/
