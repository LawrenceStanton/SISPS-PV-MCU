/**
 ******************************************************************************
 * @file			: HDC1080_I2C.hpp
 * @brief			: I2C interface for the HDC1080.
 * @author			: Lawrence Stanton
 ******************************************************************************
 */

#pragma once

#include "HDC1080.hpp"

#include "stm32g0xx_hal.h"

class HDC1080_I2C : public HDC1080::I2C {
	I2C_HandleTypeDef &hi2c;

public:
	virtual std::optional<Register> read(MemoryAddress address) noexcept override final;
	virtual std::optional<Register> write(MemoryAddress address, Register data) noexcept override final;
	virtual std::optional<uint8_t>	transmit(uint8_t data) noexcept override final;
	virtual std::optional<uint8_t>	receive() noexcept override final;

	virtual void delay(Duration ms) const noexcept override final;

	/**
	 * @brief Construct a new I2C object
	 *
	 * @param hi2c The I2C handle to use for communication.
	 * @note The I2C handle must be initialized before passing it to this constructor.
	 */
	HDC1080_I2C(I2C_HandleTypeDef &hi2c);

	virtual ~HDC1080_I2C() = default;
};
