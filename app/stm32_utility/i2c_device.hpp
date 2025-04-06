#ifndef I2C_DEVICE_HPP
#define I2C_DEVICE_HPP

#include "common.hpp"

namespace STM32_Utility {

    struct I2CDevice {
    public:
        I2CDevice() noexcept = default;
        I2CDevice(I2CHandle const i2c_bus, std::uint16_t const dev_address) noexcept;

        I2CDevice(I2CDevice const& other) = delete;
        I2CDevice(I2CDevice&& other) noexcept = default;

        I2CDevice& operator=(I2CDevice const& other) = delete;
        I2CDevice& operator=(I2CDevice&& other) noexcept = default;

        ~I2CDevice() noexcept = default;

        template <std::size_t SIZE>
        void transmit_bytes(this I2CDevice const& self,
                            std::array<std::uint8_t, SIZE> const& data) noexcept;

        void transmit_bytes(this I2CDevice const& self,
                            std::uint8_t* const data,
                            std::size_t const size) noexcept;

        void transmit_byte(this I2CDevice const& self, std::uint8_t const data) noexcept;

        template <std::size_t SIZE>
        std::array<std::uint8_t, SIZE> receive_bytes(this I2CDevice const& self) noexcept;

        void receive_bytes(this I2CDevice const& self,
                           std::uint8_t* const data,
                           std::size_t const size) noexcept;

        std::uint8_t receive_byte(this I2CDevice const& self) noexcept;

        template <std::size_t SIZE>
        std::array<std::uint8_t, SIZE> read_bytes(this I2CDevice const& self,
                                                  std::uint8_t const address) noexcept;

        void read_bytes(this I2CDevice const& self,
                        std::uint8_t const address,
                        std::uint8_t* const data,
                        std::size_t const size) noexcept;

        std::uint8_t read_byte(this I2CDevice const& self, std::uint8_t const address) noexcept;

        template <std::size_t SIZE>
        void write_bytes(this I2CDevice const& self,
                         std::uint8_t const address,
                         std::array<std::uint8_t, SIZE> const& data) noexcept;

        void write_bytes(this I2CDevice const& self,
                         std::uint8_t const address,
                         std::uint8_t* const data,
                         std::size_t const size) noexcept;

        void write_byte(this I2CDevice const& self,
                        std::uint8_t const address,
                        std::uint8_t const data) noexcept;

        std::uint16_t dev_address(this I2CDevice const& self) noexcept;

        void bus_scan(this I2CDevice const& self) noexcept;

    private:
        static constexpr std::uint32_t TIMEOUT{100U};
        static constexpr std::uint32_t SCAN_RETRIES{10U};

        void initialize(this I2CDevice const& self) noexcept;

        I2CHandle i2c_bus_{nullptr};
        std::uint16_t dev_address_{};
    };

    template <std::size_t SIZE>
    void I2CDevice::transmit_bytes(this I2CDevice const& self,
                                   std::array<std::uint8_t, SIZE> const& data) noexcept
    {
        HAL_I2C_Master_Transmit(self.i2c_bus_,
                                self.dev_address_ << 1,
                                (std::uint8_t*)data.data(),
                                data.size(),
                                TIMEOUT);
    }

    template <std::size_t SIZE>
    std::array<std::uint8_t, SIZE> I2CDevice::receive_bytes(this I2CDevice const& self) noexcept
    {
        auto data = std::array<std::uint8_t, SIZE>{};

        HAL_I2C_Master_Receive(self.i2c_bus_,
                               self.dev_address_ << 1,
                               data.data(),
                               data.size(),
                               TIMEOUT);

        return data;
    }

    template <std::size_t SIZE>
    std::array<std::uint8_t, SIZE> I2CDevice::read_bytes(this I2CDevice const& self,
                                                         std::uint8_t const address) noexcept
    {
        auto data = std::array<std::uint8_t, SIZE>{};

        HAL_I2C_Mem_Read(self.i2c_bus_,
                         self.dev_address_ << 1,
                         address,
                         sizeof(address),
                         data.data(),
                         data.size(),
                         TIMEOUT);

        return data;
    }

    template <std::size_t SIZE>
    void I2CDevice::write_bytes(this I2CDevice const& self,
                                std::uint8_t const address,
                                std::array<std::uint8_t, SIZE> const& data) noexcept
    {
        HAL_I2C_Mem_Write(self.i2c_bus_,
                          self.dev_address_ << 1,
                          address,
                          sizeof(address),
                          (std::uint8_t*)data.data(),
                          data.size(),
                          TIMEOUT);
    }

}; // namespace STM32_Utility

#endif // I2C_DEVICE_HPP
