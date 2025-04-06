#ifndef SPI_DEVICE_HPP
#define SPI_DEVICE_HPP

#include "common.hpp"
#include "gpio.hpp"

namespace STM32_Utility {

    struct SPIDevice {
    public:
        SPIDevice() noexcept = default;
        SPIDevice(SPIHandle const spi_bus, GPIO const chip_select) noexcept;

        SPIDevice(SPIDevice const& other) = delete;
        SPIDevice(SPIDevice&& other) noexcept = default;

        SPIDevice& operator=(SPIDevice const& other) = delete;
        SPIDevice& operator=(SPIDevice&& other) noexcept = default;

        ~SPIDevice() noexcept;

        template <std::size_t SIZE>
        void transmit_bytes(this SPIDevice const& self, std::array<std::uint8_t, SIZE> const& data) noexcept;

        void transmit_bytes(this SPIDevice const& self, std::uint8_t* const data, std::size_t const size) noexcept;

        void transmit_byte(this SPIDevice const& self, std::uint8_t const data) noexcept;

        template <std::size_t SIZE>
        std::array<std::uint8_t, SIZE> receive_bytes(this SPIDevice const& self) noexcept;

        void receive_bytes(this SPIDevice const& self, std::uint8_t* const data, std::size_t const size) noexcept;

        std::uint8_t receive_byte(this SPIDevice const& self) noexcept;

        template <std::size_t SIZE>
        std::array<std::uint8_t, SIZE> read_bytes(this SPIDevice const& self, std::uint8_t const address) noexcept;

        void read_bytes(this SPIDevice const& self,
                        std::uint8_t const address,
                        std::uint8_t* const data,
                        std::size_t const size) noexcept;

        std::uint8_t read_byte(this SPIDevice const& self, std::uint8_t const address) noexcept;

        template <std::size_t SIZE>
        void write_bytes(this SPIDevice const& self,
                         std::uint8_t const address,
                         std::array<std::uint8_t, SIZE> const& data) noexcept;

        void write_bytes(this SPIDevice const& self,
                         std::uint8_t const address,
                         std::uint8_t* const data,
                         std::size_t const size) noexcept;

        void write_byte(this SPIDevice const& self, std::uint8_t const address, std::uint8_t const data) noexcept;

    private:
        static std::uint8_t address_to_read_command(std::uint8_t const address) noexcept;
        static std::uint8_t address_to_write_command(std::uint8_t const address) noexcept;

        static constexpr std::uint32_t TIMEOUT{100U};

        void initialize(this SPIDevice const& self) noexcept;
        void deinitialize(this SPIDevice const& self) noexcept;

        GPIO chip_select_{};

        SPIHandle spi_bus_{nullptr};
    };

    template <std::size_t SIZE>
    void SPIDevice::transmit_bytes(this SPIDevice const& self, std::array<std::uint8_t, SIZE> const& data) noexcept
    {
        gpio_write_pin(self.chip_select_, GPIO_PIN_RESET);
        HAL_SPI_Transmit(self.spi_bus_, data.data(), data.size(), TIMEOUT);
        gpio_write_pin(self.chip_select_, GPIO_PIN_SET);
    }

    template <std::size_t SIZE>
    std::array<std::uint8_t, SIZE> SPIDevice::receive_bytes(this SPIDevice const& self) noexcept
    {
        auto data = std::array<std::uint8_t, SIZE>{};

        gpio_write_pin(self.chip_select_, GPIO_PIN_RESET);
        HAL_SPI_Receive(self.spi_bus_, data.data(), data.size(), TIMEOUT);
        gpio_write_pin(self.chip_select_, GPIO_PIN_SET);

        return data;
    }

    template <std::size_t SIZE>
    std::array<std::uint8_t, SIZE> SPIDevice::read_bytes(this SPIDevice const& self,
                                                         std::uint8_t const address) noexcept
    {
        auto data = std::array<std::uint8_t, SIZE>{};
        auto command = address_to_read_command(address);

        gpio_write_pin(self.chip_select_, GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive(self.spi_bus_, &command, data.data(), data.size(), TIMEOUT);
        gpio_write_pin(self.chip_select_, GPIO_PIN_SET);

        return data;
    }

    template <std::size_t SIZE>
    void SPIDevice::write_bytes(this SPIDevice const& self,
                                std::uint8_t const address,
                                std::array<std::uint8_t, SIZE> const& data) noexcept
    {
        auto const command = address_to_write_command(address);
        auto address_data = std::array<std::uint8_t, 1UL + SIZE>{};
        std::memcpy(address_data.data(), &command, 1UL);
        std::memcpy(address_data.data() + 1UL, data.data(), data.size());

        gpio_write_pin(self.chip_select_, GPIO_PIN_RESET);
        HAL_SPI_Transmit(self.spi_bus_, address_data.data(), address_data.size(), TIMEOUT);
        gpio_write_pin(self.chip_select_, GPIO_PIN_SET);
    }

}; // namespace STM32_Utility

#endif // SPI_DEVICE_HPP