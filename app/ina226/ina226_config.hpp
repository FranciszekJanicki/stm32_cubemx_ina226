#ifndef INA226_CONFIG_HPP
#define INA226_CONFIG_HPP

#include "i2c_device.hpp"
#include "ina226_registers.hpp"
#include <cstdint>

#define PACKED __attribute__((__packed__))

namespace INA226 {

    using I2CDevice = STM32_Utility::I2CDevice;

    enum struct SA : std::uint8_t {
        A1_GND_A0_GND = 0b1000000,
        A1_GND_A0_VS = 0b1000001,
        A1_GND_A0_SDA = 0b1000010,
        A1_GND_A0_SCL = 0b1000011,
        A1_VS_A0_GND = 0b1000100,
        A1_VS_A0_VS = 0b1000101,
        A1_VS_A0_SDA = 0b1000110,
        A1_VS_A0_SCL = 0b1000111,
        A1_SDA_A0_GND = 0b1010000,
        A1_SDA_A0_VS = 0b1001001,
        A1_SDA_A0_SDA = 0b1001010,
        A1_SDA_A0_SCL = 0b1001011,
        A1_SCL_A0_GND = 0b1001100,
        A1_SCL_A0_VS = 0b1001101,
        A1_SCL_A0_SDA = 0b1001110,
        A1_SCL_A0_SCL = 0b1001111,
    };

    enum struct RA : std::uint8_t {
        CONFIG = 0x00,
        SHUNT_VOLTAGE = 0x01,
        BUS_VOLTAGE = 0x02,
        POWER = 0x03,
        CURRENT = 0x04,
        CALIBRATION = 0x05,
        MASK_ENABLE = 0x06,
        ALERT_LIMIT = 0x07,
        MANUFACTURER_ID = 0xFE,
        DIE_ID = 0xFF,
    };

    enum struct AVG : std::uint8_t {
        SAMPLES_1 = 0b000,
        SAMPLES_4 = 0b001,
        SAMPLES_16 = 0b010,
        SAMPLES_64 = 0b011,
        SAMPLES_128 = 0b100,
        SAMPLES_256 = 0b101,
        SAMPLES_512 = 0b110,
        SAMPLES_1024 = 0b111,
    };

    enum struct VBUSCT : std::uint8_t {
        TIME_140US = 0b000,
        TIME_204US = 0b001,
        TIME_332US = 0b010,
        TIME_558US = 0b011,
        TIME_1MS1 = 0b100,
        TIME_2MS116 = 0b101,
        TIME_4MS156 = 0b110,
        TIME_8MS244 = 0b111,
    };

    enum struct VSHCT : std::uint8_t {
        TIME_140US = 0b000,
        TIME_204US = 0b001,
        TIME_332US = 0b010,
        TIME_558US = 0b011,
        TIME_1MS1 = 0b100,
        TIME_2MS116 = 0b101,
        TIME_4MS156 = 0b110,
        TIME_8MS244 = 0b111,
    };

    enum struct Mode : std::uint8_t {
        POWER_DOWN = 0b000,
        VSH_TRIGGERED = 0b001,
        VBUS_TRIGGERED = 0b010,
        VSH_VBUS_TRIGGERED = 0b011,
        VSH_CONTINUOUS = 0b101,
        VBUS_CONTINUOUS = 0b110,
        VSH_VBUS_CONTINUOUS = 0b111,
    };

    struct Config {};

    constexpr auto MANUFACTURER_ID_VAL = 0b0101010001001001;

    inline std::float32_t current_range_to_scale(std::float32_t const current_range) noexcept
    {
        return current_range / static_cast<std::float32_t>(1U << 15U);
    }

    inline std::float32_t scale_and_shunt_resistance_to_calibration(std::float32_t const scale,
                                                                    std::float32_t const shunt_resistance) noexcept
    {
        return 0.00512F / (scale * shunt_resistance);
    }

    inline std::float32_t shunt_voltage_and_calibration_to_current(std::float32_t const shunt_voltage,
                                                                   std::float32_t const calibration) noexcept
    {
        return (shunt_voltage * calibration) / static_cast<std::float32_t>(1U << 11U);
    }

    constexpr auto BUS_VOLTAGE_RANGE = 41.0F;

    inline std::float32_t bus_voltage_scale() noexcept
    {
        return BUS_VOLTAGE_RANGE / static_cast<std::float32_t>(1U << 15U);
    }

    inline std::float32_t current_to_power_scale(std::float32_t const current_scale) noexcept
    {
        return 25.0F * current_scale;
    }

    inline std::float32_t current_and_bus_voltage_to_power(std::float32_t const current,
                                                           std::float32_t const bus_voltage) noexcept
    {
        return (current * bus_voltage) / 20000.0F;
    }

    inline std::float32_t calibration_to_corrected_calibration(std::float32_t const calibration,
                                                               std::float32_t const shunt_current,
                                                               std::float32_t const device_current) noexcept
    {
        return calibration * shunt_current / device_current;
    }

}; // namespace INA226

#undef PACKED

#endif // INA226_CONFIG_HPP