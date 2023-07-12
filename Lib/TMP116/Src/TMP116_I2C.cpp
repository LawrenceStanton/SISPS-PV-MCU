/**
 ******************************************************************************
 * @file			: TMP116_I2C.cpp
 * @brief			: Source for TMP116_I2C.hpp
 * @author			: Lawrence Stanton
 ******************************************************************************
 */

#include "TMP116_I2C.hpp"

using I2C			= TMP116_I2C;
using Register		= TMP116::I2C::Register;
using MemoryAddress = TMP116::I2C::MemoryAddress;
using DeviceAddress = TMP116::I2C::DeviceAddress;

std::optional<Register> TMP116_I2C::read(DeviceAddress deviceAddress, MemoryAddress address) {
	uint8_t data[sizeof(Register)];
	auto	resultCode = HAL_I2C_Mem_Read(
		   this->hi2c,								 //
		   static_cast<uint8_t>(deviceAddress) << 1, // I2C Address. Left shift required by HAL.
		   address,									 // Memory Address to Read
		   sizeof(MemoryAddress),					 //
		   data,									 //
		   sizeof(data),							 //
		   10										 // milliseconds
	   );

	if (resultCode != HAL_OK) return std::nullopt;

	Register result = data[0] << 8 | data[1];
	return result;
}

std::optional<Register> TMP116_I2C::write(DeviceAddress deviceAddress, MemoryAddress address, Register data) {
	uint8_t dataBytes[] = {
		static_cast<uint8_t>(data >> 8),  // MSB First
		static_cast<uint8_t>(data & 0xFF) // LSB Second
	};

	auto resultCode = HAL_I2C_Mem_Write(
		this->hi2c,								  //
		static_cast<uint8_t>(deviceAddress) << 1, // I2C Address. Left shift required by HAL.
		address,								  // Memory Address to Write
		sizeof(MemoryAddress),					  //
		dataBytes,								  //
		sizeof(data),							  //
		10										  // milliseconds
	);

	if (resultCode != HAL_OK) return std::nullopt;

	return data;
}
