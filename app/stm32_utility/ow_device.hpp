#ifndef OW_DEVICE_HPP
#define OW_DEVICE_HPP

#include "common.hpp"
#include "gpio.hpp"

namespace STM32_Utility {

    struct OWDevice {
    public:
        OWDevice() noexcept = default;
        OWDevice(TIMHandle const timer,
                 GPIO const dev_pin,
                 std::uint64_t const dev_address) noexcept;

        OWDevice(OWDevice const& other) = delete;
        OWDevice(OWDevice&& other) noexcept = default;

        OWDevice& operator=(OWDevice const& other) = delete;
        OWDevice& operator=(OWDevice&& other) noexcept = default;

        ~OWDevice() noexcept;

        template <std::size_t SIZE>
        void transmit_bytes(this OWDevice const& self,
                            std::array<std::uint8_t, SIZE> const& data) noexcept;

        void transmit_bytes(this OWDevice const& self,
                            std::uint8_t* const data,
                            std::size_t const size) noexcept;

        void transmit_byte(this OWDevice const& self, std::uint8_t const data) noexcept;

        template <std::size_t SIZE>
        std::array<std::uint8_t, SIZE> receive_bytes(this OWDevice const& self) noexcept;

        void receive_bytes(this OWDevice const& self,
                           std::uint8_t* const data,
                           std::size_t const size) noexcept;

        std::uint8_t receive_byte(this OWDevice const& self) noexcept;

        template <std::size_t SIZE>
        std::array<std::uint8_t, SIZE> read_bytes(this OWDevice const& self,
                                                  std::uint8_t const address) noexcept;

        void read_bytes(this OWDevice const& self,
                        std::uint8_t const address,
                        std::uint8_t* const data,
                        std::size_t const size) noexcept;

        std::uint8_t read_byte(this OWDevice const& self, std::uint8_t const address) noexcept;

        template <std::size_t SIZE>
        void write_bytes(this OWDevice const& self,
                         std::uint8_t const address,
                         std::array<std::uint8_t, SIZE> const& data) noexcept;

        void write_bytes(this OWDevice const& self,
                         std::uint8_t const address,
                         std::uint8_t* const data,
                         std::size_t const size) noexcept;

        void write_byte(this OWDevice const& self,
                        std::uint8_t const address,
                        std::uint8_t const data) noexcept;

        std::uint64_t dev_address(this OWDevice const& self) noexcept;

    private:
        static std::uint64_t get_counter_microseconds(TIMHandle const timer) noexcept;
        static std::uint64_t get_counter_miliseconds(TIMHandle const timer) noexcept;

        static void delay_microseconds(TIMHandle const timer, std::uint64_t const delay) noexcept;

        static constexpr std::uint32_t TIMEOUT{100U};
        static constexpr std::uint32_t DELAY_80_US{80U};
        static constexpr std::uint32_t DELAY_320_US{320U};

        void initialize(this OWDevice const& self) noexcept;
        void deinitialize(this OWDevice const& self) noexcept;

        TIMHandle timer_{nullptr};

        GPIO dev_pin_{};
        std::uint64_t dev_address_{};
    };

    template <std::size_t SIZE>
    void OWDevice::transmit_bytes(this OWDevice const& self,
                                  std::array<std::uint8_t, SIZE> const& data) noexcept
    {}

    template <std::size_t SIZE>
    std::array<std::uint8_t, SIZE> OWDevice::receive_bytes(this OWDevice const& self) noexcept
    {
        auto data = std::array<std::uint8_t, SIZE>{};

        return data;
    }

    template <std::size_t SIZE>
    std::array<std::uint8_t, SIZE> OWDevice::read_bytes(this OWDevice const& self,
                                                        std::uint8_t const address) noexcept
    {
        auto data = std::array<std::uint8_t, SIZE>{};

        return data;
    }

    template <std::size_t SIZE>
    void OWDevice::write_bytes(this OWDevice const& self,
                               std::uint8_t const address,
                               std::array<std::uint8_t, SIZE> const& data) noexcept
    {}

}; // namespace STM32_Utility

#endif // OW_DEVICE_HPP