/**
 ******************************************************************************
 * @file			: SM72445_I2C.cpp
 * @brief			: Source for SM72445_I2C.hpp
 * @author			: Lawrence Stanton
 ******************************************************************************
 */

#include "SM72445_I2C.hpp"

#include "stm32g0xx_hal_i2c.h"

using Register		= SM72445::I2C::Register;
using MemoryAddress = SM72445::I2C::MemoryAddress;
using DeviceAddress = SM72445::I2C::DeviceAddress;

using std::nullopt;

static_assert(sizeof(Register) == 8, "Register expected to be 8 bytes long.");
static_assert(sizeof(MemoryAddress) == 1, "MemoryAddress expected to be 1 byte long.");
static_assert(sizeof(DeviceAddress) == 1, "DeviceAddress expected to be 1 byte long.");

optional<Register> SM72445_I2C::read(DeviceAddress deviceAddress, MemoryAddress memoryAddress) {
	uint8_t data[8]; // 7 bytes plus 1 length byte.

	auto resultCode = HAL_I2C_Mem_Read(
		this->hi2c,								  //
		static_cast<uint8_t>(deviceAddress) << 1, // I2C Address. Left shift required by HAL.
		static_cast<uint8_t>(memoryAddress),	  // Memory Address to Read
		sizeof(MemoryAddress),					  //
		data,									  //
		sizeof(data),							  //
		10										  // milliseconds
	);

	if (resultCode != HAL_OK) return nullopt;

	Register result = 0ull;

	for (auto i = 0u; i < sizeof(data); i++) {
		result |= static_cast<Register>(data[i]) << (i * 8);
	}

	result >>= 8; // Discard the length byte send by SM72445 in LSB (weird behaviour).

	return result;
}

optional<Register> SM72445_I2C::write(DeviceAddress deviceAddress, MemoryAddress memoryAddress, Register data) {
	uint8_t dataBuffer[8]; // 7 bytes plus 1 length byte.

	for (auto i = 0u; i < sizeof(dataBuffer); i++) {
		dataBuffer[i] = static_cast<uint8_t>((data >> (i * 8)) & 0xFFu);
	}

	auto resultCode = HAL_I2C_Mem_Write(
		this->hi2c,								  //
		static_cast<uint8_t>(deviceAddress) << 1, // I2C Address. Left shift required by HAL.
		static_cast<uint8_t>(memoryAddress),	  // Memory Address to Write
		sizeof(MemoryAddress),					  //
		dataBuffer,								  //
		sizeof(dataBuffer),						  //
		10										  // milliseconds
	);

	if (resultCode != HAL_OK) return nullopt;

	return data; // Success
}
