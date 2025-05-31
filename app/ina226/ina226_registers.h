#ifndef INA226_INA226_REGISTERS_H
#define INA226_INA226_REGISTERS_H

#include <stdint.h>

#define PACKED __attribute__((packed))

typedef struct {
    uint8_t reset_bit : 1;
    uint8_t averaging_mode : 3;
    uint8_t bus_voltage_conversion_time : 3;
    uint8_t shunt_voltage_conversion_time : 3;
    uint8_t operating_mode : 3;
} PACKED ina226_config_reg_t;

typedef struct {
    int16_t shunt_voltage : 16;
} PACKED ina226_shunt_voltage_reg_t;

typedef struct {
    int16_t bus_voltage : 15;
} PACKED ina226_bus_voltage_reg_t;

typedef struct {
    int16_t power : 16;
} PACKED ina226_power_reg_t;

typedef struct {
    int16_t current : 16;
} PACKED ina226_current_reg_t;

typedef struct {
    int16_t full_scale : 15;
} PACKED ina226_calibration_reg_t;

typedef struct {
    uint8_t shunt_voltage_over_voltage : 1;
    uint8_t shunt_voltage_under_voltage : 1;
    uint8_t bus_voltage_over_voltage : 1;
    uint8_t bus_voltage_under_voltage : 1;
    uint8_t power_over_limit : 1;
    uint8_t conversion_ready : 1;
    uint8_t alert_function_flag : 1;
    uint8_t conversion_ready_flag : 1;
    uint8_t math_overflow_flag : 1;
    uint8_t alert_polarity : 1;
    uint8_t alert_latch_enable : 1;
} PACKED ina226_mask_enable_reg_t;

typedef struct {
    int16_t alert_limit : 16;
} PACKED ina226_alert_limit_reg_t;

typedef struct {
    uint16_t manufacturer_id : 16;
} PACKED ina226_manufacturer_id_reg_t;

typedef struct {
    uint16_t device_id : 12;
    uint8_t device_revision_id : 4;
} PACKED ina226_die_id_reg_t;

#undef PACKED

#endif // INA226_INA226_REGISTERS_H