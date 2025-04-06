#ifndef ROTATION3D_HPP
#define ROTATION3D_HPP

#include "vector3d.hpp"
#include <array>
#include <cmath>
#include <compare>
#include <concepts>
#include <cstdlib>
#include <utility>

namespace Utility {

    template <typename T>
    struct Rotation3D {
        Rotation3D& operator+=(this Rotation3D& self, Rotation3D const& other) noexcept
        {
            self.x += other.x;
            self.y += other.y;
            self.z += other.z;
            return self;
        }

        Rotation3D& operator-=(this Rotation3D& self, Rotation3D const& other) noexcept
        {
            self.x -= other.x;
            self.y -= other.y;
            self.z -= other.z;
            return self;
        }

        Rotation3D& operator*=(this Rotation3D& self, Rotation3D const& other) noexcept
        {
            auto new_x = Vector3D<T>{self.x.x * other.x.x + self.x.y * other.y.x + self.x.z * other.z.x,
                                     self.x.x * other.x.y + self.x.y * other.y.y + self.x.z * other.z.y,
                                     self.x.x * other.x.z + self.x.y * other.y.z + self.x.z * other.z.z};

            auto new_y = Vector3D<T>{self.y.x * other.x.x + self.y.y * other.y.x + self.y.z * other.z.x,
                                     self.y.x * other.x.y + self.y.y * other.y.y + self.y.z * other.z.y,
                                     self.y.x * other.x.z + self.y.y * other.y.z + self.y.z * other.z.z};

            auto new_z = Vector3D<T>{self.z.x * other.x.x + self.z.y * other.y.x + self.z.z * other.z.x,
                                     self.z.x * other.x.y + self.z.y * other.y.y + self.z.z * other.z.y,
                                     self.z.x * other.x.z + self.z.y * other.y.z + self.z.z * other.z.z};

            self.x = new_x;
            self.y = new_y;
            self.z = new_z;

            return self;
        }

        Rotation3D& operator*=(this Rotation3D& self, T const factor) noexcept
        {
            self.x *= factor;
            self.y *= factor;
            self.z *= factor;
            return self;
        }

        Rotation3D& operator/=(this Rotation3D& self, T const factor) noexcept
        {
            self.x /= factor;
            self.y /= factor;
            self.z /= factor;
            return self;
        }

        bool operator<=>(this Rotation3D const& self, Rotation3D const& other) noexcept = default;

        Vector3D<T> x{};
        Vector3D<T> y{};
        Vector3D<T> z{};
    };

    template <typename T>
    Rotation3D<T> operator+(Rotation3D<T> const& left, Rotation3D<T> const& right) noexcept
    {
        return Rotation3D<T>{left.x + right.x, left.y + right.y, left.z + right.z};
    }

    template <typename T>
    Rotation3D<T> operator-(Rotation3D<T> const& left, Rotation3D<T> const& right) noexcept
    {
        return Rotation3D<T>{left.x - right.x, left.y - right.y, left.z - right.z};
    }

    template <typename T>
    Rotation3D<T> operator*(Rotation3D<T> const& left, Rotation3D<T> const& right) noexcept
    {
        auto new_x = Vector3D<T>{left.x.x * right.x.x + left.x.y * right.y.x + left.x.z * right.z.x,
                                 left.x.x * right.x.y + left.x.y * right.y.y + left.x.z * right.z.y,
                                 left.x.x * right.x.z + left.x.y * right.y.z + left.x.z * right.z.z};

        auto new_y = Vector3D<T>{left.y.x * right.x.x + left.y.y * right.y.x + left.y.z * right.z.x,
                                 left.y.x * right.x.y + left.y.y * right.y.y + left.y.z * right.z.y,
                                 left.y.x * right.x.z + left.y.y * right.y.z + left.y.z * right.z.z};

        auto new_z = Vector3D<T>{left.z.x * right.x.x + left.z.y * right.y.x + left.z.z * right.z.x,
                                 left.z.x * right.x.y + left.z.y * right.y.y + left.z.z * right.z.y,
                                 left.z.x * right.x.z + left.z.y * right.y.z + left.z.z * right.z.z};

        return Rotation3D<T>{new_x, new_y, new_z};
    }

    template <typename T>
    Rotation3D<T> operator*(T const factor, Rotation3D<T> const& matrix) noexcept
    {
        return Rotation3D<T>{matrix.x * factor, matrix.y * factor, matrix.z * factor};
    }

    template <typename T>
    Rotation3D<T> operator*(Rotation3D<T> const& matrix, T const factor) noexcept
    {
        return Rotation3D<T>{matrix.x * factor, matrix.y * factor, matrix.z * factor};
    }

    template <typename T>
    Rotation3D<T> operator/(Rotation3D<T> const& matrix, T const factor) noexcept
    {
        return Rotation3D<T>{matrix.x / factor, matrix.y / factor, matrix.z / factor};
    }

}; // namespace Utility

#endif // ROTATION3D_HPP
