//
// Created by Aleksandr Lvov on 2021-11-02.
//

#pragma once
#include "Utility.h"
#include <cstddef>
#include <cmath>
#include <iosfwd>

class Vec3 {
    value_t data_[3];

public: // constructors
    Vec3() : data_{0, 0, 0} {}
    Vec3(value_t x, value_t y, value_t z) : data_{x, y, z} {}

    inline static Vec3 random(value_t min = -1, value_t max = 1);
    inline static Vec3 randomUnitVec();
    inline static Vec3 randomInHemisphere(const Vec3& normal);

public: // const access
    const value_t& x() const { return data_[0]; }
    const value_t& y() const { return data_[1]; }
    const value_t& z() const { return data_[2]; }

    const value_t& operator[](size_t i) const { return data_[i]; }

public: // non-const access
    value_t& x() { return data_[0]; }
    value_t& y() { return data_[1]; }
    value_t& z() { return data_[2]; }

    value_t& operator[](size_t i) { return data_[i]; }

public: // methods
    bool isNearZero() const {
        using std::abs;
        return abs(x()) < kEpsilon
            && abs(y()) < kEpsilon
            && abs(z()) < kEpsilon;
    }

    Vec3 cross(const Vec3& v) const {
        auto new_x = y() * v.z() - z() * v.y();
        auto new_y = z() * v.x() - x() * v.z();
        auto new_z = x() * v.y() - y() * v.x();
        return {new_x, new_y, new_z};
    }

    value_t dot(const Vec3& v) const {
        return x() * v.x() + y() * v.y() + z() * v.z();
    }

    value_t length_squared() const {
        return dot(*this);
    }

    value_t length() const {
        return std::sqrt(length_squared());
    }

    inline Vec3 normalized() const;
};

using Point3 = Vec3;
using Colour = Vec3;

inline Vec3 operator+(Vec3 lhs, const Vec3& rhs) {
    for (auto i : range(3)) lhs[i] += rhs[i];
    return lhs;
}

inline Vec3 operator-(Vec3 lhs, const Vec3& rhs) {
    for (auto i : range(3)) lhs[i] -= rhs[i];
    return lhs;
}

inline Vec3 operator*(Vec3 lhs, const Vec3& rhs) {
    for (auto i : range(3)) lhs[i] *= rhs[i];
    return lhs;
}

inline Vec3 operator*(Vec3 lhs, value_t rhs) {
    for (auto i : range(3)) lhs[i] *= rhs;
    return lhs;
}

inline Vec3 operator/(Vec3 lhs, value_t rhs) {
    for (auto i : range(3)) lhs[i] /= rhs;
    return lhs;
}

inline Vec3 operator-(Vec3 v) {
    return v * -1;
}

Vec3 Vec3::normalized() const { return (*this) / length(); }

Vec3 Vec3::random(value_t min, value_t max) {
    return {
            random_value_t(min, max),
            random_value_t(min, max),
            random_value_t(min, max)
    };
}

Vec3 Vec3::randomInHemisphere(const Vec3 &normal) {
    auto v = Vec3::random();
    if (v.dot(normal) > 0) return v;
    return -v;
}

std::ostream& operator<<(std::ostream& os, const Vec3& v);