/**
 ******************************************************************************
 * @file			: init.h
 * @brief			: Peripheral Initialisation Function Prototypes
 * @author			: Lawrence Stanton
 ******************************************************************************
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void SystemClock_Config(void);
void MX_GPIO_Init(void);

void MX_I2C1_Init(void);
void MX_I2C2_Init(void);

void MX_USART1_UART_Init(void);
void MX_USART3_UART_Init(void);

#ifdef __cplusplus
}
#endif
