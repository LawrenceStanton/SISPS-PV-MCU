/**
 ******************************************************************************
 * @file			: main.cpp
 * @brief			: SM72445 Functional Test for SISPS-PV project.
 * @author			: Lawrence Stanton
 ******************************************************************************
 * @details
 * This file tests the SM72445 peripheral by calling all API functions and
 * checking the results. Where possible, certain values are also checked
 * for typically valid results.
 *
 * A PASS is indicated by the LEDs toggling every 1s.
 * A FAIL is indicated by the LEDs being on solid ON-OFF-ON.
 * An single FAIL will permanently result in a toggling ON-OFF-ON pattern.
 *
 * The test is run recursively in an infinite loop.
 */

#include "main.hpp"

#include "SM72445.hpp"
#include "SM72445_I2C.hpp"

#include <cmath>

#define SM72445_CH0_EXPECTED_VOLTAGE 0.00f
#define SM72445_CH2_EXPECTED_VOLTAGE 1.56f
#define SM72445_CH4_EXPECTED_VOLTAGE 0.00f
#define SM72445_CH6_EXPECTED_VOLTAGE 3.44f

static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);

static inline bool equalFloat(float a, float b, float epsilon);

using Register		  = SM72445::I2C::Register;
using DeviceAddress	  = SM72445::I2C::DeviceAddress;
using AnalogueChannel = SM72445::AnalogueChannel;
using std::optional;

I2C_HandleTypeDef hi2c2;
SM72445_I2C		  sm72445_i2c{&hi2c2};
SM72445_X		  sm72445{
	sm72445_i2c,
	DeviceAddress::ADDR010,
	SM72445_VIN_GAIN,
	SM72445_VOUT_GAIN,
	SM72445_IIN_GAIN,
	SM72445_IOUT_GAIN};

static optional<float> vIn	= .0f;
static optional<float> vOut = .0f;
static optional<float> iIn	= .0f;
static optional<float> iOut = .0f;

static optional<float> ch0 = .0f;
static optional<float> ch2 = .0f;
static optional<float> ch4 = .0f;
static optional<float> ch6 = .0f;

static optional<float> vInOffset  = .0f;
static optional<float> vOutOffset = .0f;
static optional<float> iInOffset  = .0f;
static optional<float> iOutOffset = .0f;

int main(void) {
	HAL_Init();
	SystemClock_Config();

	MX_GPIO_Init();
	MX_I2C2_Init();

	while (true) {
		vIn	 = sm72445.getInputVoltage();
		vOut = sm72445.getOutputVoltage();
		iIn	 = sm72445.getInputCurrent();
		iOut = sm72445.getOutputCurrent();

		bool getElectricalMeasurementsSuccess = vIn.has_value()	 //
											 && vOut.has_value() //
											 && iIn.has_value()	 //
											 && iOut.has_value();

		ch0 = sm72445.getAnalogueChannelVoltage(AnalogueChannel::CH0);
		ch2 = sm72445.getAnalogueChannelVoltage(AnalogueChannel::CH2);
		ch4 = sm72445.getAnalogueChannelVoltage(AnalogueChannel::CH4);
		ch6 = sm72445.getAnalogueChannelVoltage(AnalogueChannel::CH6);

		bool getAnalogueChannelVoltageSuccess =
			ch0.has_value() && equalFloat(ch0.value(), SM72445_CH0_EXPECTED_VOLTAGE, 0.02) && //
			ch2.has_value() && equalFloat(ch2.value(), SM72445_CH2_EXPECTED_VOLTAGE, 0.02) && //
			ch4.has_value() && equalFloat(ch4.value(), SM72445_CH4_EXPECTED_VOLTAGE, 0.02) && //
			ch6.has_value() && equalFloat(ch6.value(), SM72445_CH6_EXPECTED_VOLTAGE, 0.02);

		vInOffset  = sm72445.getOffset(SM72445::ElectricalProperty::VOLTAGE_IN);
		vOutOffset = sm72445.getOffset(SM72445::ElectricalProperty::VOLTAGE_OUT);
		iInOffset  = sm72445.getOffset(SM72445::ElectricalProperty::CURRENT_IN);
		iOutOffset = sm72445.getOffset(SM72445::ElectricalProperty::CURRENT_OUT);

		bool getOffsetsSuccess = vInOffset.has_value() && vInOffset.value() == .0f	 //
							  && vOutOffset.has_value() && vOutOffset.value() == .0f //
							  && iInOffset.has_value() && iInOffset.value() == .0f	 //
							  && iOutOffset.has_value() && iOutOffset.value() == .0f;

		if (getElectricalMeasurementsSuccess && getAnalogueChannelVoltageSuccess && getOffsetsSuccess) {
			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
			HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
			HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
		} else {
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		}

		HAL_Delay(500);
	}
}

static inline bool equalFloat(float a, float b, float epsilon) {
	return std::abs(a - b) < epsilon;
}

static void SystemClock_Config(void) {
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

static void MX_I2C2_Init(void) {
	hi2c2.Instance				= I2C2;
	hi2c2.Init.Timing			= 0x00303D5B;
	hi2c2.Init.OwnAddress1		= 0;
	hi2c2.Init.AddressingMode	= I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode	= I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2		= 0;
	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c2.Init.GeneralCallMode	= I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode	= I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK) { Error_Handler(); }

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK) { Error_Handler(); }

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK) { Error_Handler(); }
}

static void MX_GPIO_Init(void) {
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
