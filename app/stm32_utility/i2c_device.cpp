#include "i2c_device.hpp"
#include <cassert>
#include <cstdio>

namespace STM32_Utility {

    I2CDevice::I2CDevice(I2CHandle const i2c_bus, std::uint16_t const dev_address) noexcept :
        i2c_bus_{i2c_bus}, dev_address_{dev_address}
    {
        this->initialize();
    }

    void
    I2CDevice::transmit_bytes(this I2CDevice const& self, std::uint8_t* const data, std::size_t const size) noexcept
    {
        assert(data);

        HAL_I2C_Master_Transmit(self.i2c_bus_, self.dev_address_ << 1, data, size, TIMEOUT);
    }

    void I2CDevice::transmit_byte(this I2CDevice const& self, std::uint8_t const data) noexcept
    {
        self.transmit_bytes(std::array<std::uint8_t, 1UL>{data});
    }

    void I2CDevice::receive_bytes(this I2CDevice const& self, std::uint8_t* const data, std::size_t const size) noexcept
    {
        assert(data);

        HAL_I2C_Master_Receive(self.i2c_bus_, self.dev_address_ << 1, data, size, TIMEOUT);
    }

    std::uint8_t I2CDevice::receive_byte(this I2CDevice const& self) noexcept
    {
        return self.receive_bytes<1UL>()[0];
    }

    void I2CDevice::read_bytes(this I2CDevice const& self,
                               std::uint8_t const address,
                               std::uint8_t* const data,
                               std::size_t const size) noexcept
    {
        assert(data);

        HAL_I2C_Mem_Read(self.i2c_bus_, self.dev_address_ << 1, address, sizeof(address), data, size, TIMEOUT);
    }

    std::uint8_t I2CDevice::read_byte(this I2CDevice const& self, std::uint8_t const address) noexcept
    {
        return self.read_bytes<1UL>(address)[0];
    }

    void I2CDevice::write_bytes(this I2CDevice const& self,
                                std::uint8_t const address,
                                std::uint8_t* const data,
                                std::size_t const size) noexcept
    {
        assert(data);

        HAL_I2C_Mem_Write(self.i2c_bus_, self.dev_address_ << 1, address, sizeof(address), data, size, TIMEOUT);
    }

    void I2CDevice::write_byte(this I2CDevice const& self, std::uint8_t const address, std::uint8_t const data) noexcept
    {
        self.write_bytes(address, std::array<std::uint8_t, 1UL>{data});
    }

    std::uint16_t I2CDevice::dev_address(this I2CDevice const& self) noexcept
    {
        return self.dev_address_;
    }

    void I2CDevice::bus_scan(this I2CDevice const& self) noexcept
    {
        if (self.i2c_bus_) {
            for (std::uint8_t i = 0U; i < (1U << 7U); ++i) {
                if (HAL_I2C_IsDeviceReady(self.i2c_bus_, i << 1U, SCAN_RETRIES, TIMEOUT) == HAL_OK) {
                    std::printf("address: %u\n\r", i);
                }
            }
        }
    }

    void I2CDevice::initialize(this I2CDevice const& self) noexcept
    {
        if (self.i2c_bus_) {
            if (HAL_I2C_IsDeviceReady(self.i2c_bus_, self.dev_address_ << 1, SCAN_RETRIES, TIMEOUT) != HAL_OK) {
                std::puts("I2C ERROR\n\r");
            }
        }
    }

}; // namespace STM32_Utility
