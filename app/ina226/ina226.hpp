#ifndef INA226_HPP
#define INA226_HPP

#include "i2c_device.hpp"
#include "ina226_config.hpp"
#include "ina226_registers.hpp"
#include <optional>

namespace INA226 {

    struct INA226 {
    public:
        INA226() noexcept = default;
        INA226(I2CDevice&& i2c_device, Config const& config) noexcept;

        INA226(INA226 const& other) = delete;
        INA226(INA226&& other) noexcept = default;

        INA226& operator=(INA226 const& other) = delete;
        INA226& operator=(INA226&& other) noexcept = default;

        ~INA226() noexcept;

        std::optional<std::float32_t> get_current_scaled(this INA226 const& self) noexcept;
        std::optional<std::float32_t> get_bus_voltage_scaled(this INA226 const& self) noexcept;
        std::optional<std::float32_t> get_shunt_voltage_scaled(this INA226 const& self) noexcept;
        std::optional<std::float32_t> get_power_scaled(this INA226 const& self) noexcept;

    private:
        void write_byte(this INA226 const& self, std::uint8_t const address, std::uint8_t const data) noexcept;

        template <std::size_t SIZE>
        void write_bytes(this INA226 const& self,
                         std::uint8_t const address,
                         std::array<std::uint8_t, SIZE> const& data) noexcept;

        std::uint8_t read_byte(this INA226 const& self, std::uint8_t const address) noexcept;

        template <std::size_t SIZE>
        std::array<std::uint8_t, SIZE> read_bytes(this INA226 const& self, std::uint8_t const address) noexcept;

        std::optional<std::int16_t> get_current_raw(this INA226 const& self) noexcept;
        std::optional<std::int16_t> get_bus_voltage_raw(this INA226 const& self) noexcept;
        std::optional<std::int16_t> get_shunt_voltage_raw(this INA226 const& self) noexcept;
        std::optional<std::int16_t> get_power_raw(this INA226 const& self) noexcept;

        bool is_valid_manufacturer_id(this INA226 const& self) noexcept;
        std::uint16_t get_manufacturer_id(this INA226 const& self) noexcept;

        void initialize(this INA226& self, Config const& config) noexcept;
        void deinitialize(this INA226& self) noexcept;

        CONFIG get_config_register(this INA226 const& self) noexcept;
        void set_config_register(this INA226 const& self, CONFIG const config) noexcept;

        SHUNT_VOLTAGE get_shunt_voltage_register(this INA226 const& self) noexcept;

        BUS_VOLTAGE get_bus_voltage_register(this INA226 const& self) noexcept;

        POWER get_power_register(this INA226 const& self) noexcept;

        CURRENT get_current_register(this INA226 const& self) noexcept;

        CALIBRATION get_calibration_register(this INA226 const& self) noexcept;
        void set_calibration_register(this INA226 const& self, CALIBRATION const calibration) noexcept;

        MASK_ENABLE get_mask_enable_register(this INA226 const& self) noexcept;
        void set_mask_enable_register(this INA226 const& self, MASK_ENABLE const mask_enable) noexcept;

        ALERT_LIMIT get_alert_limit_register(this INA226 const& self) noexcept;
        void set_alert_limit_register(this INA226 const& self, ALERT_LIMIT const alert_limit) noexcept;

        MANUFACTURER_ID get_manufacturer_id_register(this INA226 const& self) noexcept;

        DIE_ID get_die_id_register(this INA226 const& self) noexcept;

        bool initialized_ = false;

        std::float32_t current_scale_ = 0.0F;

        I2CDevice i2c_device_ = I2CDevice{};
    };

    template <std::size_t SIZE>
    inline void INA226::write_bytes(this INA226 const& self,
                                    std::uint8_t const address,
                                    std::array<std::uint8_t, SIZE> const& data) noexcept
    {
        self.i2c_device_.write_bytes(address, data);
    }

    template <std::size_t SIZE>
    inline std::array<std::uint8_t, SIZE> INA226::read_bytes(this INA226 const& self,
                                                             std::uint8_t const address) noexcept
    {
        return self.i2c_device_.read_bytes<SIZE>(address);
    }

}; // namespace INA226

#endif // INA226_HPP