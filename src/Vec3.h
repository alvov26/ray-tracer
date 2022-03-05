//
// Created by Aleksandr Lvov on 2021-11-02.
//

#pragma once
#include "Utility.h"
#include <cmath>
#include <cstddef>

class Vec3 {
    FloatT data_[3];

public:// constructors
    Vec3() : data_{0, 0, 0} {}
    Vec3(FloatT x, FloatT y, FloatT z) : data_{x, y, z} {}

    inline static Vec3 random(FloatT min = -1, FloatT max = 1);
    inline static Vec3 randomInSphere();
    inline static Vec3 randomInHemisphere(const Vec3 &normal);

public:// const access
    const FloatT &x() const { return data_[0]; }
    const FloatT &y() const { return data_[1]; }
    const FloatT &z() const { return data_[2]; }

    const FloatT &operator[](size_t i) const { return data_[i]; }

public:// non-const access
    FloatT &x() { return data_[0]; }
    FloatT &y() { return data_[1]; }
    FloatT &z() { return data_[2]; }

    FloatT &operator[](size_t i) { return data_[i]; }

public:// methods
    bool isNearZero() const {
        using std::abs;
        return abs(x()) < kEpsilon && abs(y()) < kEpsilon && abs(z()) < kEpsilon;
    }

    Vec3 cross(const Vec3 &v) const {
        auto new_x = y() * v.z() - z() * v.y();
        auto new_y = z() * v.x() - x() * v.z();
        auto new_z = x() * v.y() - y() * v.x();
        return {new_x, new_y, new_z};
    }

    FloatT dot(const Vec3 &v) const {
        return x() * v.x() + y() * v.y() + z() * v.z();
    }

    FloatT lengthSquared() const {
        return dot(*this);
    }

    FloatT length() const {
        return std::sqrt(lengthSquared());
    }

    inline Vec3 normalized() const;
};

using Point3 = Vec3;
using Colour = Vec3;

inline Vec3 operator+(Vec3 lhs, const Vec3 &rhs) {
    for (auto i : range(3)) lhs[i] += rhs[i];
    return lhs;
}

inline Vec3 operator-(Vec3 lhs, const Vec3 &rhs) {
    for (auto i : range(3)) lhs[i] -= rhs[i];
    return lhs;
}

inline Vec3 operator*(Vec3 lhs, const Vec3 &rhs) {
    for (auto i : range(3)) lhs[i] *= rhs[i];
    return lhs;
}

inline Vec3 operator*(Vec3 lhs, FloatT rhs) {
    for (auto i : range(3)) lhs[i] *= rhs;
    return lhs;
}

inline Vec3 operator/(Vec3 lhs, FloatT rhs) {
    for (auto i : range(3)) lhs[i] /= rhs;
    return lhs;
}

inline Vec3 operator-(Vec3 v) {
    return v * -1;
}

Vec3 Vec3::normalized() const { return (*this) / length(); }

Vec3 Vec3::random(FloatT min, FloatT max) {
    return {
            randomFloatT(min, max),
            randomFloatT(min, max),
            randomFloatT(min, max)
    };
}

Vec3 Vec3::randomInSphere() {
    auto v = Vec3::random();
    while (v.lengthSquared() >= 1) {
        v = Vec3::random();
    }
    return v.normalized();
}

Vec3 Vec3::randomInHemisphere(const Vec3 &normal) {
    auto v = Vec3::randomInSphere();
    if (v.dot(normal) > 0) return v;
    return -v;
}