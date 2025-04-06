#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "i2c_device.hpp"
#include "ina226.hpp"
#include "usart.h"
#include <cstdio>

namespace {

inline auto volatile gpio_exti5 = false;

};

extern "C" {

    void HAL_GPIO_EXIT_Callback(uint16_t GPIO_Pin)
    {
        if (GPIO_Pin == (1U << 5U)) {
            gpio_exti5 = true;
        }
    }
}

int main()
{
    HAL_Init();
    SystemClock_Config();

    MX_USART2_UART_Init();

    using namespace INA226;

    auto i2c_device = I2CDevice{&hi2c1, std::to_underlying(SA::A1_GND_A0_GND)};

    auto config = Config{};

    auto ina226 = INA226::INA226{std::move(i2c_device), config};

    while (1) {
        if (gpio_exti5) {
            auto const& current = ina226.get_current_scaled();
            auto const& bus_voltage = ina226.get_bus_voltage_scaled();
            auto const& shunt_voltage = ina226.get_shunt_voltage_scaled();
            auto const& power = ina226.get_power_scaled();

            std::printf("Current: %f [A], Bus Voltage: %f [V], Shunt Voltage: %f [V], Power: %f [W]\n\r",
                        current,
                        bus_voltage,
                        shunt_voltage,
                        power);

            gpio_exti5 = false;
        }
    }
}
