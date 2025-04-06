#ifndef VECTOR3D_HPP
#define VECTOR3D_HPP

#include "quaternion3d.hpp"
#include <cmath>
#include <compare>
#include <concepts>
#include <exception>
#include <stdexcept>
#include <utility>

namespace Utility {

    template <typename T>
    struct Vector3D {
        T distance(this Vector3D const& self, Vector3D const& other) noexcept
        {
            return std::sqrt(std::pow(self.x - other.x, 2) + std::pow(self.y - other.y, 2) +
                             std::pow(self.z - other.z, 2));
        }

        T magnitude(this Vector3D const& self) noexcept
        {
            return std::sqrt(std::pow(self.x, 2) + std::pow(self.y, 2) + std::pow(self.z, 2));
        }

        Vector3D rotated(this Vector3D const& self, Quaternion3D<T> const& quaternion) noexcept
        {
            Quaternion3D p(0, self.x, self.y, self.z);
            p *= quaternion;
            p *= quaternion.conjugated();
            return Vector3D{p.x, p.y, p.z};
        }

        void rotate(this Vector3D& self, Quaternion3D<T> const& quaternion) noexcept
        {
            Quaternion3D p(0, self.x, self.y, self.z);
            p *= quaternion;
            p *= quaternion.conjugated();
            self.x = p.x;
            self.y = p.y;
            self.z = p.z;
        }

        Vector3D normalized(this Vector3D const& self) noexcept
        {
            const auto im{T{1} / self.magnitude()};
            return Vector3D{self.x * im, self.y * im, self.z * im};
        }

        void normalize(this Vector3D& self) noexcept
        {
            const auto im{T{1} / self.magnitude()};
            self *= im;
        }

        Vector3D& operator+=(this Vector3D& self, Vector3D const& other) noexcept
        {
            self.x += other.x;
            self.y += other.y;
            self.z += other.z;
            return self;
        }

        Vector3D& operator-=(this Vector3D& self, Vector3D const& other) noexcept
        {
            self.x -= other.x;
            self.y -= other.y;
            self.z -= other.z;
            return self;
        }

        Vector3D& operator*=(this Vector3D& self, T const factor)
        {
            self.x *= factor;
            self.y *= factor;
            self.z *= factor;
            return self;
        }

        Vector3D& operator/=(this Vector3D& self, T const factor)
        {
            if (factor == static_cast<T>(0)) {
                throw std::runtime_error{"Disivion by 0"};
            }

            self.x *= factor;
            self.y *= factor;
            self.z *= factor;
            return self;
        }

        template <typename C>
        explicit operator Vector3D<C>(this Vector3D const& self) noexcept
        {
            return Vector3D<C>{static_cast<C>(self.x), static_cast<C>(self.y), static_cast<C>(self.z)};
        }

        bool operator<=>(this Vector3D const& self, Vector3D const& other) noexcept = default;

        T x{};
        T y{};
        T z{};
    };

    template <typename T>
    Vector3D<T> operator+(Vector3D<T> const& left, Vector3D<T> const& right) noexcept
    {
        return Vector3D<T>{left.x + right.x, left.y + right.y, left.z + right.z};
    }

    template <typename T>
    Vector3D<T> operator-(Vector3D<T> const& left, Vector3D<T> const& right) noexcept
    {
        return Vector3D<T>{left.x - right.x, left.y - right.y, left.z - right.z};
    }

    template <typename T>
    Vector3D<T> operator*(T const factor, Vector3D<T> const& vector)
    {
        return Vector3D<T>{vector.x * factor, vector.y * factor, vector.z * factor};
    }

    template <typename T>
    Vector3D<T> operator*(Vector3D<T> const& vector, T const factor)
    {
        return factor * vector;
    }

    template <typename T>
    Vector3D<T> operator/(Vector3D<T> const& vector, T const factor)
    {
        if (factor == static_cast<T>(0)) {
            throw std::runtime_error{"Division by zero"};
        }

        return Vector3D<T>{vector.x / factor, vector.y / factor, vector.z / factor};
    }

}; // namespace Utility

#endif // VECTOR3D_HPP