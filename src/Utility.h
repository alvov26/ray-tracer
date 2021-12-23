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

inline value_t random_value_t() {
    thread_local static std::uniform_real_distribution<value_t> distribution(0, 1);
    thread_local static std::mt19937_64 generator; // NOLINT(cert-msc51-cpp)
    return distribution(generator);
}

inline value_t random_value_t(value_t min, value_t max) {
    return min + (max-min) * random_value_t();
}

inline int random_int(int min, int max) {
    return static_cast<int>(random_value_t(min, max+1));
}

constexpr auto range(size_t start, size_t end) {
    struct iterator {
        size_t i;
        void operator++() { ++i; }
        void operator+=(size_t j) { i+=j; }
        bool operator!=(const iterator& rhs) const { return i != rhs.i; }
        const size_t &operator*() const { return i; }
    };
    struct wrapper {
        size_t start_;
        size_t end_;
        auto begin() { return iterator{start_}; }
        auto end()   { return iterator{end_}; }
    };
    return wrapper{start, end};
}

constexpr auto range(size_t n) { return range(0, n); }

