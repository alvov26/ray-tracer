//
// Created by Aleksandr Lvov on 2021-11-07.
//

#pragma once
#include <utility>

#include "Utility.h"
#include "Vec3.h"
#include "stbi_image.h"

class Texture {
public:
    virtual Colour value(FloatT u, FloatT v, const Point3& p) const = 0;
    virtual ~Texture() = default;
};

class SolidColour : public Texture {
    Colour colour_;

public:
    explicit SolidColour(const Colour &colour) : colour_(colour) {}

    Colour value(FloatT u, FloatT v, const Point3& p) const override {
        return colour_;
    }
};

class CheckerTexture : public Texture {
public:
    CheckerTexture(std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd)
    : even_(std::move(even)), odd_(std::move(odd)) {}

    CheckerTexture(Colour c1, Colour c2)
    : even_(std::make_shared<SolidColour>(c1)) , odd_(std::make_shared<SolidColour>(c2)) {}

    Colour value(double u, double v, const Point3& p) const override {
        auto sines = sin(10*p.x()) * sin(10*p.y()) * sin(10*p.z());
        if (sines < 0) return odd_->value(u, v, p);
        else return even_->value(u, v, p);
    }

public:
    std::shared_ptr<Texture> even_;
    std::shared_ptr<Texture> odd_;
};

class ImageTexture : public Texture {
    Image image_;
public:
    explicit ImageTexture(Image i) : image_(std::move(i)) {}

    Colour value(FloatT u, FloatT v, const Point3 &p) const override {
        u = clamp(u, 0.0, 1.0);
        v = 1.0 - clamp(v, 0.0, 1.0);

        auto width = image_.width();
        auto height = image_.height();

        auto i = static_cast<size_t>(u * width);
        auto j = static_cast<size_t>(v * height);

        if (i >= width)  i = width-1;
        if (j >= height) j = height-1;

        return image_.at(i, j).toVec3();
    }
};

