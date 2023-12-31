/**
 ******************************************************************************
 * @file			: HDC1080_I2C.cpp
 * @brief			: Source for HDC1080_I2C.hpp
 * @author			: Lawrence Stanton
 ******************************************************************************
 */

#include "HDC1080_I2C.hpp"

#include "stm32g0xx_hal_i2c.h"

using Register		= HDC1080::I2C::Register;
using MemoryAddress = HDC1080::I2C::MemoryAddress;

HDC1080_I2C::HDC1080_I2C(I2C_HandleTypeDef &hi2c) : hi2c(hi2c) {}

std::optional<Register> HDC1080_I2C::read(MemoryAddress address) {
	uint8_t data[sizeof(Register)];
	auto	resultCode = HAL_I2C_Mem_Read(
		   &this->hi2c,			  //
		   HDC1080_I2C_ADDR << 1, // I2C Address. Left shift required by HAL.
		   address,				  // Memory Address to Read
		   sizeof(MemoryAddress), //
		   data,				  //
		   sizeof(data),		  //
		   10					  // milliseconds
	   );

	if (resultCode != HAL_OK) return std::nullopt;

	Register result = data[0] << 8 | data[1];
	return result;
}

std::optional<Register> HDC1080_I2C::write(MemoryAddress address, Register data) {
	uint8_t dataBytes[] = {
		static_cast<uint8_t>(data >> 8),  // MSB First
		static_cast<uint8_t>(data & 0xFF) // LSB Second
	};

	auto resultCode = HAL_I2C_Mem_Write(
		&this->hi2c,		   //
		HDC1080_I2C_ADDR << 1, // I2C Address. Left shift required by HAL.
		address,			   // Memory Address to Write
		sizeof(MemoryAddress), //
		dataBytes,			   //
		sizeof(data),		   //
		10					   // milliseconds
	);

	if (resultCode != HAL_OK) return std::nullopt;

	return data;
}

std::optional<uint8_t> HDC1080_I2C::transmit(uint8_t data) {
	auto resultCode = HAL_I2C_Master_Transmit(
		&this->hi2c,		   //
		HDC1080_I2C_ADDR << 1, // I2C Address. Left shift required by HAL.
		&data,				   //
		sizeof(data),		   //
		10					   // milliseconds
	);

	if (resultCode != HAL_OK) return std::nullopt;

	return data;
}

std::optional<uint8_t> HDC1080_I2C::receive() {
	uint8_t data;
	auto	resultCode = HAL_I2C_Master_Receive(
		   &this->hi2c,			  //
		   HDC1080_I2C_ADDR << 1, // I2C Address. Left shift required by HAL.
		   &data,				  //
		   sizeof(data),		  //
		   10					  // milliseconds
	   );

	if (resultCode != HAL_OK) return std::nullopt;

	return data;
}

void HDC1080_I2C::delay(Duration ms) const noexcept {
	static_assert(
		std::is_same_v<Duration, std::chrono::milliseconds>,
		"SISPS: HDC1080_I2C::delay() expected duration in milliseconds."
	);
	HAL_Delay(static_cast<uint32_t>(ms.count()));
}
