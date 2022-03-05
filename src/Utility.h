//
// Created by Aleksandr Lvov on 2021-11-02.
//

#pragma once

#include <cmath>
#include <cstddef>
#include <random>

using FloatT = double;

#if !defined(NDEBUG)
constexpr auto kDebug = true;
#else
constexpr auto kDebug = false;
#endif

constexpr auto kEpsilon = std::numeric_limits<FloatT>::epsilon() * 1000;
constexpr auto kInfinity = std::numeric_limits<FloatT>::infinity();
constexpr auto kPi = M_PI;

inline FloatT clamp(FloatT x, FloatT min, FloatT max) {
    return x < min ? min : (x > max ? max : x);
}

inline FloatT degreesToRadians(FloatT deg) {
    constexpr auto a = kPi / 180;
    return deg * a;
}

inline FloatT randomFloatT() {
    static std::uniform_real_distribution<FloatT> distribution(0, 1);
    static std::mt19937_64 generator;// NOLINT(cert-msc51-cpp)
    return distribution(generator);
}

inline FloatT randomFloatT(FloatT min, FloatT max) {
    return min + (max - min) * randomFloatT();
}

inline int randomInt(int min, int max) {
    return static_cast<int>(randomFloatT(min, max + 1));
}

constexpr auto range(size_t start, size_t end) {
    struct iterator {
        size_t i;
        void operator++() { ++i; }
        void operator+=(size_t j) { i += j; }
        bool operator!=(const iterator &rhs) const { return i != rhs.i; }
        const size_t &operator*() const { return i; }
    };
    struct wrapper {
        size_t start_;
        size_t end_;
        auto begin() { return iterator{start_}; }
        auto end() { return iterator{end_}; }
    };
    return wrapper{start, end};
}

constexpr auto range(size_t n) { return range(0, n); }
