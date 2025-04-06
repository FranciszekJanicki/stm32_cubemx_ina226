#ifndef QUATERNION3D_HPP
#define QUATERNION3D_HPP

#include <cmath>
#include <compare>
#include <concepts>
#include <cstdlib>
#include <stdexcept>
#include <tuple>
#include <utility>

namespace Utility {

    template <typename T>
    struct Quaternion3D {
        Quaternion3D conjugated(this Quaternion3D const& self) noexcept
        {
            return Quaternion3D{self.w, -self.x, -self.y, -self.z};
        }

        void conjugate(this Quaternion3D& self) noexcept
        {
            self.x = -self.x;
            self.y = -self.y;
            self.z = -self.z;
        }

        T magnitude(this Quaternion3D const& self) noexcept
        {
            return std::sqrt(std::pow(self.w, 2) + std::pow(self.x, 2) + std::pow(self.y, 2) + std::pow(self.z, 2));
        }

        Quaternion3D normalized(this Quaternion3D const& self) noexcept
        {
            const auto im{static_cast<T>(1) / self.magnitude()};
            return Quaternion3D{self.w * im, self.x * im, self.y * im, self.z * im};
        }

        void normalize(this Quaternion3D& self) noexcept
        {
            const auto im{static_cast<T>(1) / self.magnitude()};
            self *= im;
        }

        Quaternion3D& operator+=(this Quaternion3D& self, Quaternion3D const& other)
        {
            self.w += other.w;
            self.x += other.x;
            self.y += other.y;
            self.z += other.z;
            return self;
        }

        Quaternion3D& operator-=(this Quaternion3D& self, Quaternion3D const& other)
        {
            self.w -= other.w;
            self.x -= other.x;
            self.y -= other.y;
            self.z -= other.z;
            return self;
        }

        Quaternion3D& operator*=(this Quaternion3D& self, Quaternion3D const& other)
        {
            self.w = self.w * other.w - self.x * other.x - self.y * other.y - self.z * other.z;
            self.x = self.w * other.x + self.x * other.w + self.y * other.z - self.z * other.y;
            self.y = self.w * other.y - self.x * other.z + self.y * other.w + self.z * other.x;
            self.z = self.w * other.z + self.x * other.y - self.y * other.x + self.z * other.w;
            return self;
        }

        Quaternion3D& operator*=(this Quaternion3D& self, T const factor)
        {
            self.w *= factor;
            self.x *= factor;
            self.y *= factor;
            self.z *= factor;
            return self;
        }

        Quaternion3D& operator/=(this Quaternion3D& self, T const factor)
        {
            if (factor == static_cast<T>(0)) {
                throw std::runtime_error{"Division by zero"};
            }

            self.w /= factor;
            self.x /= factor;
            self.y /= factor;
            self.z /= factor;
            return self;
        }

        template <typename C>
        explicit operator Quaternion3D<C>(this Quaternion3D const& self) noexcept
        {
            return Quaternion3D<C>{static_cast<C>(self.w),
                                   static_cast<C>(self.x),
                                   static_cast<C>(self.y),
                                   static_cast<C>(self.z)};
        }

        bool operator<=>(this Quaternion3D const& self, Quaternion3D const& other) noexcept = default;

        T w{};
        T x{};
        T y{};
        T z{};
    };

    template <typename T>
    Quaternion3D<T> operator+(Quaternion3D<T> const& left, Quaternion3D<T> const& right) noexcept
    {
        return Quaternion3D<T>{left.w + right.w, left.x + right.x, left.y + right.y, left.z + right.z};
    }

    template <typename T>
    Quaternion3D<T> operator-(Quaternion3D<T> const& left, Quaternion3D<T> const& right) noexcept
    {
        return Quaternion3D<T>{left.w - right.w, left.x - right.x, left.y - right.y, left.z + right.z};
    }

    template <typename T>
    Quaternion3D<T> operator*(Quaternion3D<T> const& left, Quaternion3D<T> const& right) noexcept
    {
        return Quaternion3D<T>{left.w * right.w - left.x * right.x - left.y * right.y - left.z * right.z,
                               left.w * right.x + left.x * right.w + left.y * right.z - left.z * right.y,
                               left.w * right.y - left.x * right.z + left.y * right.w + left.z * right.x,
                               left.w * right.z + left.x * right.y - left.y * right.x + left.z * right.w};
    }

    template <typename T>
    Quaternion3D<T> operator*(Quaternion3D<T> const& quaternion, T const factor)
    {
        return Quaternion3D<T>{quaternion.w * factor,
                               quaternion.x * factor,
                               quaternion.y * factor,
                               quaternion.z * factor};
    }

    template <typename T>
    Quaternion3D<T> operator*(T const factor, Quaternion3D<T> const& quaternion)
    {
        return quaternion * factor;
    }

    template <typename T>
    Quaternion3D<T> operator/(Quaternion3D<T> const& quaternion, T const factor)
    {
        if (factor == static_cast<T>(0)) {
            throw std::runtime_error{"Disivion by zero"};
        }

        return Quaternion3D<T>{quaternion.w / factor,
                               quaternion.x / factor,
                               quaternion.y / factor,
                               quaternion.z / factor};
    }

}; // namespace Utility

#endif // QUATERNION3D_HPP