//
// Created by Aleksandr Lvov on 2021-11-01.
//

#pragma once

#include "Vec3.h"
#include <string_view>
#include <vector>

class Pixel {
    using u_char = unsigned char;
    u_char red, green, blue;
    [[maybe_unused]] u_char alpha;

    Pixel(u_char r, u_char g, u_char b, u_char a = 0)
        : red(r), green(g), blue(b), alpha(a) {}

public:
    static Pixel fromColour(const Vec3 &v) {
        return {
                static_cast<u_char>(clamp(v.x(), 0, 1) * 255),
                static_cast<u_char>(clamp(v.y(), 0, 1) * 255),
                static_cast<u_char>(clamp(v.z(), 0, 1) * 255)};
    }

    static Pixel fromRGB(u_char r, u_char g, u_char b) {
        return {r, g, b};
    }

    Colour toColour() const {
        return {red / 255., green / 255., blue / 255.};
    }
};

class Image {
    size_t width_, height_;
    std::vector<Pixel> data_;

public:
    Image(size_t w, size_t h);
    Image(size_t w, size_t h, std::vector<Pixel> buffer);
    static Image readFromFile(std::string_view filename);
    size_t width() const { return width_; }
    size_t height() const { return height_; }

public:
    Pixel &at(size_t x, size_t y);
    const Pixel &at(size_t x, size_t y) const;
    void writeToFile(std::string_view filename);
};
