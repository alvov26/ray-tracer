//
// Created by Aleksandr Lvov on 2021-11-02.
//

#pragma once

#include <cstddef>
#include <cmath>
#include <random>

using value_t = double;

#if !defined(NDEBUG)
constexpr auto kDebug = true;
#else
constexpr auto kDebug = false;
#endif

constexpr auto kEpsilon  = std::numeric_limits<value_t>::epsilon() * 1000;
constexpr auto kInfinity = std::numeric_limits<value_t>::infinity();
constexpr auto kPi = M_PI;

inline value_t clamp(value_t x, value_t min, value_t max) {
    return x < min ? 0 : x > max ? 1 : x;
}

inline value_t degreesToRadians(value_t deg) {
    constexpr auto a = kPi / 180;
    return deg * a;
}

inline value_t random_value_t(value_t min = 0.0, value_t max = 1.0) {
    static std::uniform_real_distribution<value_t> distribution(min, max);
    static std::mt19937_64 generator; // NOLINT(cert-msc51-cpp)
    return distribution(generator);
}

constexpr auto range(size_t n) {
    struct iterator {
        size_t i;
        void operator++() { ++i; }
        bool operator!=(const iterator& rhs) const { return i != rhs.i; }
        const size_t &operator*() const { return i; }
    };
    struct wrapper {
        size_t n;
        auto begin() { return iterator{0}; }
        auto end()   { return iterator{n}; }
    };
    return wrapper{n};
}

