#ifndef COMMON_HPP
#define COMMON_HPP

#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_i2c.h"
#include "stm32l4xx_hal_spi.h"
#include "stm32l4xx_hal_tim.h"
#include "stm32l4xx_hal_usart.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdfloat>
#include <utility>

namespace STM32_Utility {

    using SPIHandle = SPI_HandleTypeDef*;
    using TIMHandle = TIM_HandleTypeDef*;
    using GPIOHandle = GPIO_TypeDef*;
    using UARTHandle = USART_HandleTypeDef*;
    using I2CHandle = I2C_HandleTypeDef*;

}; // namespace STM32_Utility

#endif // COMMON_HPP