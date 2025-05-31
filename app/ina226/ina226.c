#include "ina226.h"
#include <assert.h>
#include <string.h>

static ina226_err_t ina226_bus_init(ina226_t const* ina226)
{
    assert(ina226);

    if (ina226->interface.bus_init) {
        return ina226->interface.bus_init(ina226->interface.bus_user);
    }

    return INA226_ERR_NULL;
}

static ina226_err_t ina226_bus_deinit(ina226_t const* ina226)
{
    assert(ina226);

    if (ina226->interface.bus_deinit) {
        return ina226->interface.bus_deinit(ina226->interface.bus_user);
    }

    return INA226_ERR_NULL;
}

static ina226_err_t
ina226_bus_write(ina226_t const* ina226, uint8_t write_address, uint8_t const* write_data, size_t write_size)
{
    assert(ina226 && write_data);

    if (ina226->interface.bus_write) {
        return ina226->interface.bus_write(ina226->interface.bus_user, write_address, write_data, write_size);
    }

    return INA226_ERR_NULL;
}

static ina226_err_t ina226_bus_read(ina226_t const* ina226, uint8_t read_address, uint8_t* read_data, size_t read_size)
{
    assert(ina226 && read_data);

    if (ina226->interface.bus_read) {
        return ina226->interface.bus_read(ina226->interface.bus_user, read_address, read_data, read_size);
    }

    return INA226_ERR_NULL;
}

ina226_err_t ina226_initialize(ina226_t* ina226, ina226_config_t const* config, ina226_interface_t const* interface)
{
    assert(ina226 && config && interface);

    memset(ina226, 0, sizeof(*ina226));
    memcpy(&ina226->config, config, sizeof(*config));
    memcpy(&ina226->interface, interface, sizeof(*interface));

    return ina226_bus_init(ina226);
}

ina226_err_t ina226_deinitialize(ina226_t* ina226)
{
    assert(ina226);

    ina226_err_t err = ina226_bus_deinit(ina226);

    memset(ina226, 0, sizeof(*ina226));

    return err;
}

ina226_err_t ina226_get_current_scaled(ina226_t const* ina226, float32_t* scaled)
{
    assert(ina226 && scaled);

    int16_t raw = {};

    ina226_err_t err = ina226_get_current_raw(ina226, &raw);

    *scaled = (float32_t)raw * ina226->config.current_scale;

    return err;
}

ina226_err_t ina226_get_bus_voltage_scaled(ina226_t const* ina226, float32_t* scaled)
{
    assert(ina226 && scaled);

    int16_t raw = {};

    ina226_err_t err = ina226_get_bus_voltage_raw(ina226, &raw);

    *scaled = (float32_t)raw * INA226_BUS_VOLTAGE_SCALE;

    return err;
}

ina226_err_t ina226_get_shunt_voltage_scaled(ina226_t const* ina226, float32_t* scaled)
{
    assert(ina226 && scaled);

    assert(ina226 && scaled);

    int16_t raw = {};

    ina226_err_t err = ina226_get_shunt_voltage_raw(ina226, &raw);

    *scaled = (float32_t)raw * INA226_SHUNT_VOLTAGE_SCALE;

    return err;
}

ina226_err_t ina226_get_power_scaled(ina226_t const* ina226, float32_t* scaled)
{
    assert(ina226 && scaled);

    assert(ina226 && scaled);

    int16_t raw = {};

    ina226_err_t err = ina226_get_power_raw(ina226, &raw);

    *scaled = (float32_t)raw * ina226_current_to_power_scale(ina226->config.current_scale);

    return err;
}

ina226_err_t ina226_get_current_raw(ina226_t const* ina226, int16_t* raw)
{
    assert(ina226 && raw);

    ina226_current_reg_t reg = {};

    ina226_err_t err = ina226_get_current_reg(ina226, &reg);

    *raw = reg.current;

    return err;
}

ina226_err_t ina226_get_bus_voltage_raw(ina226_t const* ina226, int16_t* raw)
{
    assert(ina226 && raw);

    ina226_bus_voltage_reg_t reg = {};

    ina226_err_t err = ina226_get_bus_voltage_reg(ina226, &reg);

    *raw = reg.bus_voltage;

    if (*raw & (1 << 14)) {
        *raw |= 0x8000;
    }

    return err;
}

ina226_err_t ina226_get_shunt_voltage_raw(ina226_t const* ina226, int16_t* raw)
{
    assert(ina226 && raw);

    ina226_shunt_voltage_reg_t reg = {};

    ina226_err_t err = ina226_get_shunt_voltage_reg(ina226, &reg);

    *raw = reg.shunt_voltage;

    return err;
}

ina226_err_t ina226_get_power_raw(ina226_t const* ina226, int16_t* raw)
{
    assert(ina226 && raw);

    ina226_power_reg_t reg = {};

    ina226_err_t err = ina226_get_power_reg(ina226, &reg);

    *raw = reg.power;

    return err;
}

ina226_err_t ina226_get_config_reg(ina226_t const* ina226, ina226_config_reg_t* reg)
{
    assert(ina226 && reg);

    uint8_t data[2] = {};

    ina226_err_t err = ina226_bus_read(ina226, INA226_REG_ADDRESS_CONFIG, data, sizeof(data));

    reg->reset_bit = (data[0] >> 7U) & 0x01U;
    reg->averaging_mode = (data[0] >> 1U) & 0x07U;
    reg->bus_voltage_conversion_time = (data[0] & 0x01U) << 2U;
    reg->bus_voltage_conversion_time |= (data[1] >> 6U) & 0x03U;
    reg->shunt_voltage_conversion_time = (data[1] >> 3U) & 0x07U;
    reg->operating_mode = (data[1] >> 0U) & 0x07U;

    return err;
}

ina226_err_t ina226_set_config_reg(ina226_t const* ina226, ina226_config_reg_t const* reg)
{
    assert(ina226 && reg);

    uint8_t data[2] = {};

    ina226_err_t err = ina226_bus_read(ina226, INA226_REG_ADDRESS_CONFIG, data, sizeof(data));

    data[0] &= ~((0x01U << 7U) | (0x07U << 1U) | 0x80U);
    data[1] &= ~((0x03U << 6U) | (0x07U << 3U) | 0x07U);

    data[0] |= (reg->reset_bit & 0x01U) << 7U;
    data[0] |= (reg->averaging_mode & 0x07U) << 1U;
    data[0] |= (reg->bus_voltage_conversion_time & 0x80U);
    data[1] |= (reg->bus_voltage_conversion_time & 0x3U) << 6U;
    data[1] |= (reg->shunt_voltage_conversion_time & 0x07U) << 3U;
    data[1] |= (reg->operating_mode & 0x07U) << 0U;

    err |= ina226_bus_write(ina226, INA226_REG_ADDRESS_CONFIG, data, sizeof(data));

    return err;
}

ina226_err_t ina226_get_shunt_voltage_reg(ina226_t const* ina226, ina226_shunt_voltage_reg_t* reg)
{
    assert(ina226 && reg);

    uint8_t data[2] = {};

    ina226_err_t err = ina226_bus_read(ina226, INA226_REG_ADDRESS_SHUNT_VOLTAGE, data, sizeof(data));

    reg->shunt_voltage = (int16_t)(((data[0] & 0xFF) << 8) | (data[0] & 0xFF));

    return err;
}

ina226_err_t ina226_get_bus_voltage_reg(ina226_t const* ina226, ina226_bus_voltage_reg_t* reg)
{
    assert(ina226 && reg);

    uint8_t data[2] = {};

    ina226_err_t err = ina226_bus_read(ina226, INA226_REG_ADDRESS_BUS_VOLTAGE, data, sizeof(data));

    reg->bus_voltage = (int16_t)(((data[0] & 0xFF) << 8) | (data[0] & 0xFF));

    return err;
}

ina226_err_t ina226_get_power_reg(ina226_t const* ina226, ina226_power_reg_t* reg)
{
    assert(ina226 && reg);

    uint8_t data[2] = {};

    ina226_err_t err = ina226_bus_read(ina226, INA226_REG_ADDRESS_POWER, data, sizeof(data));

    reg->power = (int16_t)(((data[0] & 0xFF) << 8) | (data[0] & 0xFF));

    return err;
}

ina226_err_t ina226_get_current_reg(ina226_t const* ina226, ina226_current_reg_t* reg)
{
    assert(ina226 && reg);

    uint8_t data[2] = {};

    ina226_err_t err = ina226_bus_read(ina226, INA226_REG_ADDRESS_CURRENT, data, sizeof(data));

    reg->current = (int16_t)(((data[0] & 0xFF) << 8) | (data[0] & 0xFF));

    return err;
}

ina226_err_t ina226_get_calibration_reg(ina226_t const* ina226, ina226_calibration_reg_t* reg)
{
    assert(ina226 && reg);

    uint8_t data[2] = {};

    ina226_err_t err = ina226_bus_read(ina226, INA226_REG_ADDRESS_CALIBRATION, data, sizeof(data));

    reg->full_scale = (int16_t)(((data[0] & 0x7F) << 7) | (data[1] & 0xFF));

    return err;
}

ina226_err_t ina226_set_calibration_reg(ina226_t const* ina226, ina226_calibration_reg_t const* reg)
{
    assert(ina226 && reg);

    uint8_t data[2] = {};

    ina226_err_t err = ina226_bus_read(ina226, INA226_REG_ADDRESS_CALIBRATION, data, sizeof(data));

    data[0] &= ~0x7F;
    data[1] &= ~0xFF;

    data[0] |= (uint8_t)((reg->full_scale >> 8) & 0x7F);
    data[1] |= (uint8_t)(reg->full_scale & 0xFF);

    return err;
}

ina226_err_t ina226_get_mask_enable_reg(ina226_t const* ina226, ina226_mask_enable_reg_t* reg)
{
    assert(ina226 && reg);

    uint8_t data[2] = {};

    ina226_err_t err = ina226_bus_read(ina226, INA226_REG_ADDRESS_MASK_ENABLE, data, sizeof(data));

    reg->shunt_voltage_over_voltage = (data[0] >> 7U) & 0x01U;
    reg->shunt_voltage_under_voltage = (data[0] >> 6U) & 0x01U;
    reg->bus_voltage_over_voltage = (data[0] >> 5U) & 0x01U;
    reg->bus_voltage_under_voltage = (data[0] >> 4U) & 0x01U;
    reg->power_over_limit = (data[0] >> 3U) & 0x01U;
    reg->conversion_ready = (data[0] >> 2U) & 0x01U;
    reg->alert_function_flag = (data[1] >> 4U) & 0x01U;
    reg->conversion_ready_flag = (data[1] >> 3U) & 0x01U;
    reg->math_overflow_flag = (data[1] >> 2U) & 0x01U;
    reg->alert_polarity = (data[1] >> 1U) & 0x01U;
    reg->alert_latch_enable = (data[1] >> 0U) & 0x01U;

    return err;
}

ina226_err_t ina226_set_mask_enable_reg(ina226_t const* ina226, ina226_mask_enable_reg_t const* reg)
{
    assert(ina226 && reg);

    uint8_t data[2] = {};

    ina226_err_t err = ina226_bus_read(ina226, INA226_REG_ADDRESS_MASK_ENABLE, data, sizeof(data));

    data[0] &= ~((0x01U << 7U) | (0x01U << 6U) | (0x01U << 5U) | (0x01U << 4U) | (0x01U << 3U) | (0x01U << 2U));
    data[1] &= ~((0x01 << 4U) | (0x01 << 3U) | (0x01 << 2U) | (0x01 << 1U) | (0x01 << 0U));

    data[0] |= (reg->shunt_voltage_over_voltage & 0x01U) << 7U;
    data[0] |= (reg->shunt_voltage_under_voltage & 0x01U) << 6U;
    data[0] |= (reg->bus_voltage_over_voltage & 0x01U) << 5U;
    data[0] |= (reg->bus_voltage_under_voltage & 0x01U) << 4U;
    data[0] |= (reg->power_over_limit & 0x01U) << 3U;
    data[0] |= (reg->conversion_ready & 0x01U) << 2U;
    data[1] |= (reg->alert_function_flag & 0x01U) << 4U;
    data[1] |= (reg->conversion_ready_flag & 0x01U) << 3U;
    data[1] |= (reg->math_overflow_flag & 0x01U) << 2U;
    data[1] |= (reg->alert_polarity & 0x01U) << 1U;
    data[1] |= (reg->alert_latch_enable & 0x01U) << 0U;

    err |= ina226_bus_write(ina226, INA226_REG_ADDRESS_MASK_ENABLE, data, sizeof(data));

    return err;
}

ina226_err_t ina226_get_alert_limit_reg(ina226_t const* ina226, ina226_alert_limit_reg_t* reg)
{
    assert(ina226 && reg);

    uint8_t data[2] = {};

    ina226_err_t err = ina226_bus_read(ina226, INA226_REG_ADDRESS_ALERT_LIMIT, data, sizeof(data));

    reg->alert_limit = (int16_t)(((data[0] & 0xFF) << 8) | (data[1] & 0xFF));

    return err;
}

ina226_err_t ina226_set_alert_limit_reg(ina226_t const* ina226, ina226_alert_limit_reg_t const* reg)
{
    assert(ina226 && reg);

    uint8_t data[2] = {};

    data[0] |= (uint8_t)((reg->alert_limit >> 8) & 0xFF);
    data[1] |= (uint8_t)(reg->alert_limit & 0xFF);

    return ina226_bus_write(ina226, INA226_REG_ADDRESS_ALERT_LIMIT, data, sizeof(data));
}

ina226_err_t ina226_get_manufacturer_id_reg(ina226_t const* ina226, ina226_manufacturer_id_reg_t* reg)
{
    assert(ina226 && reg);

    uint8_t data[2] = {};

    ina226_err_t err = ina226_bus_read(ina226, INA226_REG_ADDRESS_MANUFACTURER_ID, data, sizeof(data));

    reg->manufacturer_id = ((data[0] & 0xFFU) << 8U) | (data[1] & 0xFFU);

    return err;
}

ina226_err_t ina226_get_die_id_reg(ina226_t const* ina226, ina226_die_id_reg_t* reg)
{
    assert(ina226 && reg);

    uint8_t data[2] = {};

    ina226_err_t err = ina226_bus_read(ina226, INA226_REG_ADDRESS_DIE_ID, data, sizeof(data));

    reg->device_id = (data[0] & 0xFFU) | ((data[1] >> 4U) & 0x0FU);
    reg->device_revision_id = (data[1] >> 0U) & 0x0FU;

    return err;
}
