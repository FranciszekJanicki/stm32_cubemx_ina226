#include "spi_device.hpp"
#include <cassert>

namespace STM32_Utility {

    SPIDevice::SPIDevice(SPIHandle const spi_bus, GPIO const chip_select) noexcept :
        chip_select_{chip_select}, spi_bus_{spi_bus}
    {
        this->initialize();
    }

    SPIDevice::~SPIDevice() noexcept
    {
        this->deinitialize();
    }

    void
    SPIDevice::transmit_bytes(this SPIDevice const& self, std::uint8_t* const data, std::size_t const size) noexcept
    {
        assert(data);

        HAL_SPI_Transmit(self.spi_bus_, data, size, TIMEOUT);
    }

    void SPIDevice::transmit_byte(this SPIDevice const& self, std::uint8_t const data) noexcept
    {
        self.transmit_bytes(std::array<std::uint8_t, 1UL>{data});
    }

    void SPIDevice::receive_bytes(this SPIDevice const& self, std::uint8_t* const data, std::size_t const size) noexcept
    {
        assert(data);

        HAL_SPI_Receive(self.spi_bus_, data, size, TIMEOUT);
    }

    std::uint8_t SPIDevice::receive_byte(this SPIDevice const& self) noexcept
    {
        return self.receive_bytes<1UL>()[0];
    }

    void SPIDevice::read_bytes(this SPIDevice const& self,
                               std::uint8_t const address,
                               std::uint8_t* const data,
                               std::size_t const size) noexcept
    {
        assert(data);

        auto const command = address_to_read_command(address);
        HAL_SPI_TransmitReceive(self.spi_bus_, &command, data, size, TIMEOUT);
    }

    std::uint8_t SPIDevice::read_byte(this SPIDevice const& self, std::uint8_t const address) noexcept
    {
        return self.read_bytes<1UL>(address)[0];
    }

    void SPIDevice::write_bytes(this SPIDevice const& self,
                                std::uint8_t const address,
                                std::uint8_t* const data,
                                std::size_t const size) noexcept
    {
        assert(data);

        if (auto address_data = static_cast<std::uint8_t*>(std::malloc(size + 1UL))) {
            auto const command = address_to_write_command(address);
            std::memcpy(address_data, &command, 1UL);
            std::memcpy(address_data + 1UL, data, size);

            HAL_SPI_Transmit(self.spi_bus_, address_data, size + 1UL, TIMEOUT);

            std::free(address_data);
        }
    }

    void SPIDevice::write_byte(this SPIDevice const& self, std::uint8_t const address, std::uint8_t const data) noexcept
    {
        self.write_bytes(address, std::array<std::uint8_t, 1UL>{data});
    }

    std::uint8_t SPIDevice::address_to_read_command(std::uint8_t const address) noexcept
    {
        return address & ~(1U << (std::bit_width(address) - 1U));
    }

    std::uint8_t SPIDevice::address_to_write_command(std::uint8_t const address) noexcept
    {
        return address | (1U << (std::bit_width(address) - 1U));
    }

    void SPIDevice::initialize(this SPIDevice const& self) noexcept
    {
        if (self.spi_bus_) {
            gpio_write_pin(self.chip_select_, GPIO_PIN_SET);
        }
    }

    void SPIDevice::deinitialize(this SPIDevice const& self) noexcept
    {
        if (self.spi_bus_) {
            gpio_write_pin(self.chip_select_, GPIO_PIN_RESET);
        }
    }

}; // namespace STM32_Utility