#include "ina226.hpp"
#include <utility>

namespace INA226 {

    INA226::INA226(I2CDevice&& i2c_device, Config const& config) noexcept :
        i2c_device_{std::forward<I2CDevice>(i2c_device)}
    {
        this->initialize(config);
    }

    INA226::~INA226() noexcept
    {
        this->deinitialize();
    }

    std::optional<std::float32_t> INA226::get_current_scaled(this INA226 const& self) noexcept
    {
        return self.get_current_raw().transform(
            [&self](std::int16_t const raw) { return static_cast<std::float32_t>(raw) / self.current_scale_; });
    }

    std::optional<std::float32_t> INA226::get_bus_voltage_scaled(this INA226 const& self) noexcept
    {
        return self.get_bus_voltage_raw().transform(
            [&self](std::int16_t const raw) { return static_cast<std::float32_t>(raw) / self.current_scale_; });
    }

    std::optional<std::float32_t> INA226::get_shunt_voltage_scaled(this INA226 const& self) noexcept
    {
        return self.get_shunt_voltage_raw().transform(
            [&self](std::int16_t const raw) { return static_cast<std::float32_t>(raw) / self.current_scale_; });
    }

    std::optional<std::float32_t> INA226::get_power_scaled(this INA226 const& self) noexcept
    {
        return self.get_power_raw().transform(
            [&self](std::int16_t const raw) { return static_cast<std::float32_t>(raw) / self.current_scale_; });
    }

    void INA226::write_byte(this INA226 const& self, std::uint8_t const address, std::uint8_t const data) noexcept
    {
        self.i2c_device_.write_byte(address, data);
    }

    std::uint8_t INA226::read_byte(this INA226 const& self, std::uint8_t const address) noexcept
    {
        return self.i2c_device_.read_byte(address);
    }

    std::optional<std::int16_t> INA226::get_current_raw(this INA226 const& self) noexcept
    {
        return self.initialized_ ? std::optional<std::int16_t>{std::bit_cast<std::int16_t>(self.get_current_register())}
                                 : std::optional<std::int16_t>{std::nullopt};
    }

    std::optional<std::int16_t> INA226::get_bus_voltage_raw(this INA226 const& self) noexcept
    {
        return self.initialized_
                   ? std::optional<std::int16_t>{std::bit_cast<std::int16_t>(self.get_bus_voltage_register())}
                   : std::optional<std::int16_t>{std::nullopt};
    }

    std::optional<std::int16_t> INA226::get_shunt_voltage_raw(this INA226 const& self) noexcept
    {
        return self.initialized_
                   ? std::optional<std::int16_t>{std::bit_cast<std::int16_t>(self.get_shunt_voltage_register())}
                   : std::optional<std::int16_t>{std::nullopt};
    }

    std::optional<std::int16_t> INA226::get_power_raw(this INA226 const& self) noexcept
    {
        return self.initialized_ ? std::optional<std::int16_t>{std::bit_cast<std::int16_t>(self.get_power_register())}
                                 : std::optional<std::int16_t>{std::nullopt};
    }

    bool INA226::is_valid_manufacturer_id(this INA226 const& self) noexcept
    {
        return self.get_manufacturer_id() == MANUFACTURER_ID_VAL;
    }

    std::uint16_t INA226::get_manufacturer_id(this INA226 const& self) noexcept
    {
        return std::bit_cast<std::uint16_t>(self.get_manufacturer_id_register());
    }

    void INA226::initialize(this INA226& self, Config const& config) noexcept
    {
        if (self.is_valid_manufacturer_id()) {
            self.initialize(config);
            self.initialized_ = true;
        }
    }

    void INA226::deinitialize(this INA226& self) noexcept
    {
        if (self.is_valid_manufacturer_id()) {
            self.deinitialize();
            self.initialized_ = false;
        }
    }

    CONFIG INA226::get_config_register(this INA226 const& self) noexcept
    {
        return std::bit_cast<CONFIG>(self.read_bytes<sizeof(CONFIG)>(std::to_underlying(RA::CONFIG)));
    }

    void INA226::set_config_register(this INA226 const& self, CONFIG const config) noexcept
    {
        self.write_bytes(std::to_underlying(RA::CONFIG),
                         std::bit_cast<std::array<std::uint8_t, sizeof(CONFIG)>>(config));
    }

    SHUNT_VOLTAGE INA226::get_shunt_voltage_register(this INA226 const& self) noexcept
    {
        return std::bit_cast<SHUNT_VOLTAGE>(
            self.read_bytes<sizeof(SHUNT_VOLTAGE)>(std::to_underlying(RA::SHUNT_VOLTAGE)));
    }

    BUS_VOLTAGE INA226::get_bus_voltage_register(this INA226 const& self) noexcept
    {
        return std::bit_cast<BUS_VOLTAGE>(self.read_bytes<sizeof(BUS_VOLTAGE)>(std::to_underlying(RA::BUS_VOLTAGE)));
    }

    POWER INA226::get_power_register(this INA226 const& self) noexcept
    {
        return std::bit_cast<POWER>(self.read_bytes<sizeof(POWER)>(std::to_underlying(RA::POWER)));
    }

    CURRENT INA226::get_current_register(this INA226 const& self) noexcept
    {
        return std::bit_cast<CURRENT>(self.read_bytes<sizeof(CURRENT)>(std::to_underlying(RA::CURRENT)));
    }

    CALIBRATION INA226::get_calibration_register(this INA226 const& self) noexcept
    {
        return std::bit_cast<CALIBRATION>(self.read_bytes<sizeof(CALIBRATION)>(std::to_underlying(RA::CALIBRATION)));
    }

    void INA226::set_calibration_register(this INA226 const& self, CALIBRATION const calibration) noexcept
    {
        self.write_bytes(std::to_underlying(RA::CALIBRATION),
                         std::bit_cast<std::array<std::uint8_t, sizeof(CALIBRATION)>>(calibration));
    }

    MASK_ENABLE INA226::get_mask_enable_register(this INA226 const& self) noexcept
    {
        return std::bit_cast<MASK_ENABLE>(self.read_bytes<sizeof(MASK_ENABLE)>(std::to_underlying(RA::MASK_ENABLE)));
    }

    void INA226::set_mask_enable_register(this INA226 const& self, MASK_ENABLE const mask_enable) noexcept
    {
        self.write_bytes(std::to_underlying(RA::MASK_ENABLE),
                         std::bit_cast<std::array<std::uint8_t, sizeof(MASK_ENABLE)>>(mask_enable));
    }

    ALERT_LIMIT INA226::get_alert_limit_register(this INA226 const& self) noexcept
    {
        return std::bit_cast<ALERT_LIMIT>(self.read_bytes<sizeof(ALERT_LIMIT)>(std::to_underlying(RA::ALERT_LIMIT)));
    }

    void INA226::set_alert_limit_register(this INA226 const& self, ALERT_LIMIT const alert_limit) noexcept
    {
        self.write_bytes(std::to_underlying(RA::ALERT_LIMIT),
                         std::bit_cast<std::array<std::uint8_t, sizeof(ALERT_LIMIT)>>(alert_limit));
    }

    MANUFACTURER_ID INA226::get_manufacturer_id_register(this INA226 const& self) noexcept
    {
        return std::bit_cast<MANUFACTURER_ID>(
            self.read_bytes<sizeof(MANUFACTURER_ID)>(std::to_underlying(RA::MANUFACTURER_ID)));
    }

    DIE_ID INA226::get_die_id_register(this INA226 const& self) noexcept
    {
        return std::bit_cast<DIE_ID>(self.read_bytes<sizeof(DIE_ID)>(std::to_underlying(RA::DIE_ID)));
    }

}; // namespace INA226