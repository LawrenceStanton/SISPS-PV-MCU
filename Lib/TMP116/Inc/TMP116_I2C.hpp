/**
 ******************************************************************************
 * @file			: TMP116_I2C.hpp
 * @brief			: I2C interface for the TMP116.
 * @author			: Lawrence Stanton
 ******************************************************************************
 */

#pragma once

#include "TMP116.hpp"

#include "stm32g0xx_hal.h"

class TMP116_I2C : public TMP116::I2C {
	I2C_HandleTypeDef *hi2c;

public:
	virtual std::optional<Register> read(DeviceAddress deviceAddress, MemoryAddress address) final;
	virtual std::optional<Register> write(DeviceAddress deviceAddress, MemoryAddress address, Register data) final;

	TMP116_I2C(I2C_HandleTypeDef *hi2c) : hi2c(hi2c) {}
};
