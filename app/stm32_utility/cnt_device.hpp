#ifndef CNT_DEVICE_HPP
#define CNT_DEVICE_HPP

#include "common.hpp"

namespace STM32_Utility {

    struct CNTDevice {
    public:
        CNTDevice() noexcept = default;
        CNTDevice(TIMHandle const timer) noexcept;

        CNTDevice(CNTDevice const& other) = delete;
        CNTDevice(CNTDevice&& other) noexcept = default;

        CNTDevice& operator=(CNTDevice const& other) = delete;
        CNTDevice& operator=(CNTDevice&& other) noexcept = default;

        ~CNTDevice() noexcept;

        std::uint32_t get_count(this CNTDevice const& self) noexcept;
        std::uint32_t get_count_difference(this CNTDevice const& self) noexcept;

    private:
        std::uint32_t get_current_count(this CNTDevice const& self) noexcept;

        void initialize(this CNTDevice const& self) noexcept;
        void deinitialize(this CNTDevice const& self) noexcept;

        TIMHandle timer_{nullptr};
        std::uint32_t mutable count_{};
    };

}; // namespace STM32_Utility

#endif // CNT_DEVICE_HPP