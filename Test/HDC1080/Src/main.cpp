/**
 ******************************************************************************
 * @file			: main.cpp
 * @brief			: Test of HDC1080 Peripheral
 * @author			: Lawrence Stanton
 ******************************************************************************
 * @details
 * This file tests the HDC1080 peripheral by calling all API functions and
 * checking the results. Where possible, certain values are also checked
 * for typically valid results.
 *
 * A PASS is indicated by the LEDs flashing every 1s.
 * A FAIL is indicated by the LEDs being on solid on.
 *
 * The test is run recursively in an infinite loop.
 */

#include "main.h"
#include "stm32g0xx_hal.h"

#include "HDC1080.hpp"
#include "HDC1080_I2C.hpp"

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_I2C1_Init(void);

I2C_HandleTypeDef hi2c1;
I2C_HDC1080		  hdc1080_i2c{&hi2c1};
HDC1080			  hdc1080{&hdc1080_i2c};

using Register = HDC1080::I2C::Register;

static float				   temperature	  = -40.0;
static float				   humidity		  = 0.0;
static std::optional<Register> deviceID		  = 0x0000u;
static std::optional<Register> manufacturerID = 0x0000u;
static std::optional<uint64_t> serialID		  = 0x0ul;

const auto deviceIDExpected		  = 0x1050u;
const auto manufacturerIDExpected = 0x5449u;

int main() {
	HAL_Init();
	SystemClock_Config();

	MX_GPIO_Init();
	MX_I2C1_Init();

	HAL_Delay(1000);

	while (true) {
		temperature	   = hdc1080.getTemperature();
		humidity	   = hdc1080.getHumidity();
		deviceID	   = hdc1080.getDeviceID();
		manufacturerID = hdc1080.getManufacturerID();
		serialID	   = hdc1080.getSerialID();

		if (temperature != -40.0 &&															  //
			humidity != 0.0 &&																  //
			deviceID.has_value() && deviceID.value() == deviceIDExpected &&					  //
			manufacturerID.has_value() && manufacturerID.value() == manufacturerIDExpected && //
			serialID.has_value()) {
			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
			HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
			HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
		} else {
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
		}

		HAL_Delay(500);
	}
}

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

void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, LED1_Pin | LED2_Pin | LED3_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : LED1_Pin LED2_Pin LED3_Pin PM_FORCE_Pin STOP_Pin */
	GPIO_InitStruct.Pin	  = LED1_Pin | LED2_Pin | LED3_Pin;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Error_Handler(void) {
	__disable_irq();
	while (1) {}
}
