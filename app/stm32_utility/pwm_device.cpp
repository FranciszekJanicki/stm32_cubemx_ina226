#include "pwm_device.hpp"
#include <cstdio>

namespace STM32_Utility {

    void frequency_to_prescaler_counter_period(std::uint32_t const clock_hz,
                                               std::uint32_t const clock_div,
                                               std::uint32_t const frequency,
                                               std::uint32_t& prescaler,
                                               std::uint32_t& period) noexcept
    {
        if (frequency > 0UL) {
            period = clock_hz / frequency;
            prescaler = 0UL;

            while (period > 0xFFFF && prescaler < 0xFFFF) {
                prescaler = prescaler + 1UL;
                period = clock_hz / ((prescaler + 1UL) * (clock_div + 1UL) * frequency) - 1UL;
            }
        }
    }

    PWMDevice::PWMDevice(TIMHandle const timer, std::uint32_t const channel_mask) noexcept :
        timer_{timer}, channel_mask_{channel_mask}
    {
        this->initialize();
    }

    PWMDevice::~PWMDevice() noexcept
    {
        this->deinitialize();
    }

    std::uint32_t PWMDevice::get_period(this PWMDevice const& self) noexcept
    {
        return __HAL_TIM_GET_AUTORELOAD(self.timer_);
    }

    void PWMDevice::set_compare_raw(this PWMDevice const& self, std::uint32_t const raw) noexcept
    {
        auto const min_raw = 0UL;
        auto const max_raw = self.get_period();
        auto const clamped_raw = std::clamp(raw, min_raw, max_raw);

        std::printf("DUTY: %.2f\n\r",
                    100 * static_cast<std::float32_t>(clamped_raw) / static_cast<std::float32_t>(max_raw));

        __HAL_TIM_SET_COMPARE(self.timer_, self.channel_mask_, clamped_raw);
    }

    void PWMDevice::set_compare_voltage(this PWMDevice const& self, std::float32_t const voltage) noexcept
    {
        self.set_compare_raw(self.voltage_to_raw(voltage));
    }

    void PWMDevice::set_frequency(this PWMDevice const& self, std::uint32_t const frequency) noexcept
    {
        auto period = 0UL;
        auto prescaler = 0UL;

        frequency_to_prescaler_counter_period(84000000UL, 0UL, frequency, prescaler, period);
        std::printf("FREQ: %u, PSC: %u, CP: %u\n\r", frequency, prescaler, period);

        self.set_prescaler(prescaler);
        self.set_period(period);
    }

    void PWMDevice::set_prescaler(this PWMDevice const& self, std::uint32_t const prescaler) noexcept
    {
        __HAL_TIM_SET_PRESCALER(self.timer_, prescaler);
    }

    void PWMDevice::set_period(this PWMDevice const& self, std::uint32_t const period) noexcept
    {
        __HAL_TIM_SET_AUTORELOAD(self.timer_, period);
    }

    void PWMDevice::set_compare_min(this PWMDevice const& self) noexcept
    {
        self.set_compare_raw(0U);
    }

    void PWMDevice::set_compare_max(this PWMDevice const& self) noexcept
    {
        self.set_compare_raw(self.get_period());
    }

    void PWMDevice::set_compare_half(this PWMDevice const& self) noexcept
    {
        auto compare = static_cast<std::float32_t>(self.get_period()) / 2.0F;
        self.set_compare_raw(static_cast<std::int16_t>(compare));
    }

    void PWMDevice::initialize(this PWMDevice const& self) noexcept
    {
        if (self.timer_) {
            if (HAL_TIM_PWM_Start(self.timer_, self.channel_mask_) != HAL_OK) {
                std::puts("PWM ERROR\n\r");
            }
        }
    }

    void PWMDevice::deinitialize(this PWMDevice const& self) noexcept
    {
        if (self.timer_) {
            if (HAL_TIM_PWM_Stop(self.timer_, self.channel_mask_) != HAL_OK) {
                std::puts("PWM ERROR\n\r");
            }
        }
    }

    std::uint32_t PWMDevice::voltage_to_raw(this PWMDevice const& self, std::float32_t const voltage) noexcept
    {
        return std::clamp(voltage, 0.0F32, 3.3F32) * static_cast<std::float32_t>(self.get_period()) / 3.3F32;
    }

}; // namespace STM32_Utility
