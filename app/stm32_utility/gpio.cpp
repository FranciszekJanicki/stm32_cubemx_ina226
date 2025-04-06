#include "gpio.hpp"
#include <cassert>

namespace STM32_Utility {

    namespace {

        constexpr auto GPIO_PORTS = std::array{GPIOA, GPIOB, GPIOC};

        GPIO_TypeDef* pin_to_port(GPIO const pin) noexcept
        {
            auto const index = std::to_underlying(pin) / 16U;

            return index < GPIO_PORTS.size() ? GPIO_PORTS[index] : nullptr;
        }

        std::uint16_t pin_to_mask(GPIO const pin) noexcept
        {
            return 1U << (std::to_underlying(pin) % 16U);
        }

    }; // namespace

    GPIO_PinState gpio_read_pin(GPIO const pin) noexcept
    {
        assert(pin != GPIO::NC);

        if (pin != GPIO::NC) {
            return HAL_GPIO_ReadPin(pin_to_port(pin), pin_to_mask(pin));
        }
    }

    void gpio_write_pin(GPIO const pin, GPIO_PinState const gpio_state) noexcept
    {
        assert(pin != GPIO::NC);

        if (pin != GPIO::NC) {
            HAL_GPIO_WritePin(pin_to_port(pin), pin_to_mask(pin), gpio_state);
        }
    }

    void gpio_toggle_pin(GPIO const pin) noexcept
    {
        assert(pin != GPIO::NC);

        if (pin != GPIO::NC) {
            HAL_GPIO_TogglePin(pin_to_port(pin), pin_to_mask(pin));
        }
    }

    void gpio_set_pin(GPIO const pin) noexcept
    {
        assert(pin != GPIO::NC);

        if (pin != GPIO::NC) {
            HAL_GPIO_WritePin(pin_to_port(pin), pin_to_mask(pin), GPIO_PIN_SET);
        }
    }

    void gpio_reset_pin(GPIO const pin) noexcept
    {
        assert(pin != GPIO::NC);

        if (pin != GPIO::NC) {
            HAL_GPIO_WritePin(pin_to_port(pin), pin_to_mask(pin), GPIO_PIN_RESET);
        }
    }
}; // namespace STM32_Utility
