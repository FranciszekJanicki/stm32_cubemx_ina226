#ifndef INA226_REGISTERS_HPP
#define INA226_REGISTERS_HPP

#include <cstdint>

#define PACKED __attribute__((__packed__))

namespace INA226 {

    struct CONFIG {
        std::uint8_t rst : 1;
        std::uint8_t : 3;
        std::uint8_t avg : 3;
        std::uint8_t vbusct : 3;
        std::uint8_t vshct : 3;
        std::uint8_t mode : 3;
    } PACKED;

    struct SHUNT_VOLTAGE {
        std::uint8_t sign : 1;
        std::uint16_t sd : 15;
    } PACKED;

    struct BUS_VOLTAGE {
        std::uint8_t : 1;
        std::uint16_t bd : 15;
    };

    struct POWER {
        std::uint16_t pd : 15;
    } PACKED;

    struct CURRENT {
        std::uint16_t cd : 16;
    } PACKED;

    struct CALIBRATION {
        std::uint8_t : 1;
        std::uint16_t fs : 15;
    } PACKED;

    struct MASK_ENABLE {
        std::uint8_t sol : 1;
        std::uint8_t sul : 1;
        std::uint8_t bol : 1;
        std::uint8_t bul : 1;
        std::uint8_t pol : 1;
        std::uint8_t cnvr : 1;
        std::uint8_t : 5;
        std::uint8_t aff : 1;
        std::uint8_t cvrf : 1;
        std::uint8_t ovf : 1;
        std::uint8_t apol : 1;
        std::uint8_t len : 1;
    } PACKED;

    struct ALERT_LIMIT {
        std::uint16_t aul : 16;
    } PACKED;

    struct MANUFACTURER_ID {
        std::uint16_t id : 16;
    } PACKED;

    struct DIE_ID {
        std::uint16_t did : 12;
        std::uint8_t rid : 4;
    } PACKED;

}; // namespace INA226

#undef PACKED

#endif // INA226_REGISTERS_HPP