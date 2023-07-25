/**
 ******************************************************************************
 * @file			: SM72445_I2C.hpp
 * @brief			: SM72445 Functional Test
 * @author			: Lawrence Stanton
 ******************************************************************************
 */

#pragma once

#include "SM72445.hpp"

#include "stm32g0xx_hal.h"

class SM72445_I2C : public SM72445::I2C {
	I2C_HandleTypeDef *hi2c;

public:
	optional<Register> read(DeviceAddress deviceAddress, MemoryAddress memoryAddress) final;
	optional<Register> write(DeviceAddress deviceAddress, MemoryAddress memoryAddress, Register data) final;

	explicit SM72445_I2C(I2C_HandleTypeDef *hi2c) : hi2c(hi2c) {}
	virtual ~SM72445_I2C() = default;
};
