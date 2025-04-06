#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "quaternion3d.hpp"
#include "vector3d.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <bitset>
#include <cassert>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <numbers>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace Utility {

    template <typename T>
    inline T differentiate(T const value,
                           T const prev_value,
                           T const sampling_time,
                           T const prev_derivative,
                           T const time_constant)
    {
        if (time_constant + sampling_time == static_cast<T>(0)) {
            throw std::runtime_error{"Division by 0"};
        }
        return (value - prev_value + prev_derivative * time_constant) / (time_constant + sampling_time);
    }

    template <typename T>
    inline T differentiate(T const value, T const prev_value, T const sampling_time)
    {
        if (sampling_time == static_cast<T>(0)) {
            throw std::runtime_error{"Division by 0"};
        }
        return (value - prev_value) / sampling_time;
    }

    template <typename T>
    inline T integrate(T const value, T const prev_value, T const sampling_time) noexcept
    {
        return (value + prev_value) * static_cast<T>(0.5F) * sampling_time;
    }

    template <typename T>
    inline T degrees_to_radians(T const degrees) noexcept
    {
        return degrees * std::numbers::pi_v<T> / static_cast<T>(360.0);
    }

    template <typename T>
    inline T radians_to_degrees(T const radians) noexcept
    {
        return radians * static_cast<T>(360.0) / std::numbers::pi_v<T>;
    }

    template <std::unsigned_integral UInt>
    inline void write_bits(UInt& data,
                           UInt const write_data,
                           std::uint8_t const write_size,
                           std::uint8_t const write_position) noexcept
    {
        UInt mask = ((1U << write_size) - 1U) << (write_position - write_size + 1U);
        UInt temp = (write_data << (write_position - write_size + 1U));
        temp &= mask;
        data &= ~mask;
        data |= temp;
    }

    template <std::unsigned_integral UInt>
    inline void write_bit(UInt& data, bool const write_data, std::uint8_t const write_position) noexcept
    {
        write_data ? (data |= (1U << write_position)) : (data &= ~(1U << write_position));
    }

    template <std::unsigned_integral UInt>
    inline UInt read_bits(UInt data, std::uint8_t const read_size, std::uint8_t const read_position) noexcept
    {
        UInt mask = ((1U << read_size) - 1U) << (read_position - read_size + 1U);
        data &= mask;
        data >>= (read_position - read_size + 1U);

        return data;
    }

    template <std::unsigned_integral UInt>
    inline bool read_bit(UInt data, std::uint8_t const read_position) noexcept
    {
        return (data & (1U << read_position)) ? true : false;
    }

    inline std::uint16_t big_endian_bytes_to_word(std::array<std::uint8_t, 2UL> const bytes) noexcept
    {
        return static_cast<std::uint16_t>(bytes[0UL] << 8UL) | static_cast<std::uint16_t>(bytes[1UL]);
    }

    inline std::uint16_t little_endian_bytes_to_word(std::array<std::uint8_t, 2UL> const bytes) noexcept
    {
        return static_cast<std::uint16_t>(bytes[0UL]) | static_cast<std::uint16_t>(bytes[1UL] << 8UL);
    }

    inline std::array<std::uint8_t, 2UL> word_to_big_endian_bytes(std::uint16_t const word) noexcept
    {
        return std::array<std::uint8_t, 2UL>{static_cast<std::uint8_t>(word >> 8UL), static_cast<std::uint8_t>(word)};
    }

    inline std::array<std::uint8_t, 2UL> word_to_little_endian_bytes(std::uint16_t const word) noexcept
    {
        return std::array<std::uint8_t, 2UL>{static_cast<std::uint8_t>(word), static_cast<std::uint8_t>(word >> 8UL)};
    }

    inline std::uint32_t big_endian_bytes_to_dword(std::array<std::uint8_t, 4UL> const bytes) noexcept
    {
        return static_cast<std::uint32_t>(bytes[0UL] << 24UL) | static_cast<std::uint32_t>(bytes[1UL] << 16UL) |
               static_cast<std::uint32_t>(bytes[2UL] << 8UL) | static_cast<std::uint16_t>(bytes[3UL]);
    }

    inline std::uint32_t little_endian_bytes_to_dword(std::array<std::uint8_t, 4UL> const bytes) noexcept
    {
        return static_cast<std::uint32_t>(bytes[0UL]) | static_cast<std::uint32_t>(bytes[1UL] << 8UL) |
               static_cast<std::uint32_t>(bytes[2UL] << 16UL) | static_cast<std::uint32_t>(bytes[3UL] << 24UL);
    }

    inline std::array<std::uint8_t, 4UL> dword_to_big_endian_bytes(std::uint32_t const dword) noexcept
    {
        return std::array<std::uint8_t, 4UL>{static_cast<std::uint8_t>(dword >> 24UL),
                                             static_cast<std::uint8_t>(dword >> 16UL),
                                             static_cast<std::uint8_t>(dword >> 8UL),
                                             static_cast<std::uint8_t>(dword)};
    }

    inline std::array<std::uint8_t, 4UL> dword_to_little_endian_bytes(std::uint32_t const dword) noexcept
    {
        return std::array<std::uint8_t, 4UL>{static_cast<std::uint8_t>(dword),
                                             static_cast<std::uint8_t>(dword >> 8UL),
                                             static_cast<std::uint8_t>(dword >> 16UL),
                                             static_cast<std::uint8_t>(dword >> 24UL)};
    }

    template <typename From, typename To>
    inline To
    rescale(From const from_value, From const from_min, From const from_max, To const to_min, To const to_max) noexcept
    {
        return (std::clamp(from_value, from_min, from_max) - from_min) * (to_max - to_min) / (from_max - from_min) +
               to_min;
    }

    template <std::unsigned_integral UInt>
    inline UInt reflection(UInt const data) noexcept
    {
        UInt reflection = 0U;

        for (std::uint8_t i = 0U; i < std::bit_width(data); ++i) {
            write_bit(reflection, read_bit(data, i), std::bit_width(data) - 1U - i);
        }

        return reflection;
    }

    template <std::unsigned_integral UInt, std::size_t SIZE>
    inline UInt calculate_crc(std::array<std::uint8_t, SIZE> const& data,
                              UInt const init,
                              UInt const polynomial,
                              UInt const xor_out,
                              bool const reflect_in,
                              bool const reflect_out) noexcept
    {
        UInt crc = init;
        UInt msb_mask = 1U << (std::bit_width(crc) - 1U);
        UInt crc_mask = (1U << std::bit_width(crc)) - 1U;

        for (std::uint8_t byte : data) {
            if (reflect_in) {
                byte = reflection(byte);
            }
            crc ^= byte << (std::bit_width(crc) - 8U);

            for (std::uint8_t bit{}; bit < 8U; ++bit) {
                if (crc & msb_mask) {
                    crc = (crc << 1U) ^ polynomial;
                } else {
                    crc <<= 1U;
                }
            }
        }

        if (reflect_out) {
            crc = reflect(crc);
        }
        crc ^= xor_out;

        return crc & crc_mask;
    }

    inline std::uint32_t counter_to_freq_hz(std::uint32_t const count,
                                            std::uint32_t const prescaler,
                                            std::uint32_t const clock_freq_hz,
                                            std::uint32_t const clock_divider = 0UL) noexcept
    {
        return clock_freq_hz / ((count + 1UL) * (prescaler + 1UL) * (clock_divider + 1UL));
    }

    inline std::uint32_t counter_to_time_us(std::uint32_t const count,
                                            std::uint32_t const prescaler,
                                            std::uint32_t const clock_freq_hz,
                                            std::uint32_t const clock_divider = 0UL) noexcept
    {
        return 1000000UL / clock_freq_hz * ((count + 1UL) * (prescaler + 1UL) * (clock_divider + 1UL));
    }

    inline std::uint32_t freq_hz_to_counter(std::uint32_t const freq_hz,
                                            std::uint32_t const prescaler,
                                            std::uint32_t const clock_freq_hz,
                                            std::uint32_t const clock_divider = 0UL) noexcept
    {
        return clock_freq_hz / ((prescaler + 1UL) * (clock_divider + 1UL) * (freq_hz)) - 1UL;
    }

    inline std::uint32_t time_us_to_counter(std::uint32_t const time_us,
                                            std::uint32_t const prescaler,
                                            std::uint32_t const clock_freq_hz,
                                            std::uint32_t const clock_divider = 0UL) noexcept
    {
        return time_us / 1000000UL * clock_freq_hz / ((prescaler + 1UL) * (clock_divider + 1UL)) - 1UL;
    }

    template <typename T>
    inline T accel_to_roll(Vector3D<T> const& accel) noexcept
    {
        return std::atan2(accel.y, accel.z) * 180.0F / std::numbers::pi_v<T>;
    }

    template <typename T>
    inline T accel_to_pitch(Vector3D<T> const& accel) noexcept
    {
        return -(std::atan2(accel.x, std::sqrt(accel.y * accel.y + accel.z * accel.z)) * 180.0F) /
               std::numbers::pi_v<T>;
    }

    template <typename T>
    inline T accel_to_yaw(Vector3D<T> const& accel) noexcept
    {
        return static_cast<T>(0);
    }

    template <typename T>
    inline Vector3D<T> accel_to_roll_pitch_yaw(Vector3D<T> const& accel) noexcept
    {
        return Vector3D<T>{.x = accel_to_roll(accel), .y = accel_to_pitch(accel), .z = accel_to_yaw(accel)};
    }

    template <typename T>
    inline Vector3D<T> quaternion_to_gravity(Quaternion3D<T> const& quaternion) noexcept
    {
        return Vector3D<T>{.x = 2 * (quaternion.x * quaternion.z - quaternion.w * quaternion.y),
                           .y = 2 * (quaternion.w * quaternion.x + quaternion.y * quaternion.z),
                           .z = quaternion.w * quaternion.w - quaternion.x * quaternion.x -
                                quaternion.y * quaternion.y + quaternion.z * quaternion.z};
    }

    template <typename T>
    inline T quaternion_to_roll(Quaternion3D<T> const& quaternion) noexcept
    {
        auto const gravity = quaternion_to_gravity(quaternion);

        return std::atan2(gravity.y, gravity.z);
    }

    template <typename T>
    inline T quaternion_to_pitch(Quaternion3D<T> const& quaternion) noexcept
    {
        auto const gravity = quaternion_to_gravity(quaternion);
        auto const pitch = std::atan2(gravity.x, std::sqrt(gravity.y * gravity.y + gravity.z * gravity.z));

        return (gravity.z < 0) ? (pitch > 0 ? std::numbers::pi_v<T> - pitch : -std::numbers::pi_v<T> - pitch) : pitch;
    }

    template <typename T>
    inline T quaternion_to_yaw(Quaternion3D<T> const& quaternion) noexcept
    {
        return std::atan2(2 * quaternion.x * quaternion.y - 2 * quaternion.w * quaternion.z,
                          2 * quaternion.w * quaternion.w + 2 * quaternion.x * quaternion.x - 1);
    }

    template <typename T>
    inline Vector3D<T> quaternion_to_roll_pitch_yaw(Quaternion3D<T> const& quaternion) noexcept
    {
        return Vector3D<T>{.x = quaternion_to_roll(quaternion),
                           .y = quaternion_to_pitch(quaternion),
                           .z = quaternion_to_yaw(quaternion)};
    }

}; // namespace Utility

#endif // UTILITY_HPP