#ifndef PWM_DEVICE_HPP
#define PWM_DEVICE_HPP

#include "common.hpp"

namespace STM32_Utility {

    struct PWMDevice {
    public:
        PWMDevice() noexcept = default;
        PWMDevice(TIMHandle const timer, std::uint32_t const channel_mask) noexcept;

        PWMDevice(PWMDevice const& other) = delete;
        PWMDevice(PWMDevice&& other) noexcept = default;

        PWMDevice& operator=(PWMDevice const& other) = delete;
        PWMDevice& operator=(PWMDevice&& other) noexcept = default;

        ~PWMDevice() noexcept;

        std::uint32_t get_period(this PWMDevice const& self) noexcept;

        void set_frequency(this PWMDevice const& self, std::uint32_t const frequency) noexcept;
        void set_prescaler(this PWMDevice const& self, std::uint32_t const prescaler) noexcept;
        void set_period(this PWMDevice const& self, std::uint32_t const period) noexcept;

        void set_compare_min(this PWMDevice const& self) noexcept;
        void set_compare_max(this PWMDevice const& self) noexcept;
        void set_compare_half(this PWMDevice const& self) noexcept;

        void set_compare_raw(this PWMDevice const& self, std::uint32_t const raw) noexcept;
        void set_compare_voltage(this PWMDevice const& self, std::float32_t const voltage) noexcept;

    private:
        std::uint32_t voltage_to_raw(this PWMDevice const& self,
                                     std::float32_t const voltage) noexcept;

        void initialize(this PWMDevice const& self) noexcept;
        void deinitialize(this PWMDevice const& self) noexcept;

        TIMHandle timer_{nullptr};
        std::uint32_t channel_mask_{};
    };

}; // namespace STM32_Utility

#endif // PWM_DEVICE_HPP