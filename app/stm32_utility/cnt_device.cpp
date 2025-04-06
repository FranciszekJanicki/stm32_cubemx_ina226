#include "cnt_device.hpp"
#include <cstdio>

namespace STM32_Utility {

    CNTDevice::CNTDevice(TIMHandle const timer) noexcept : timer_{timer}
    {
        this->initialize();
    }

    CNTDevice::~CNTDevice() noexcept
    {
        this->deinitialize();
    }

    std::uint32_t CNTDevice::get_count(this CNTDevice const& self) noexcept
    {
        return self.count_ = (self.get_current_count() % __HAL_TIM_GET_AUTORELOAD(self.timer_));
    }

    std::uint32_t CNTDevice::get_count_difference(this CNTDevice const& self) noexcept
    {
        auto const prev_count = self.count_;

        return self.get_count() - prev_count;
    }

    void CNTDevice::initialize(this CNTDevice const& self) noexcept
    {
        if (self.timer_) {
            if (HAL_TIM_Encoder_Start(self.timer_, TIM_CHANNEL_ALL) != HAL_OK) {
                std::puts("ENCODER ERROR\n\r");
            }
        }
    }

    void CNTDevice::deinitialize(this CNTDevice const& self) noexcept
    {
        if (self.timer_) {
            if (HAL_TIM_Encoder_Stop(self.timer_, TIM_CHANNEL_ALL) != HAL_OK) {
                std::puts("ENCODER ERROR\n\r");
            }
        }
    }

    std::uint32_t CNTDevice::get_current_count(this CNTDevice const& self) noexcept
    {
        return static_cast<std::uint32_t>(__HAL_TIM_GET_COUNTER(self.timer_));
    }

}; // namespace STM32_Utility