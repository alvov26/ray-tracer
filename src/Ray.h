//
// Created by Aleksandr Lvov on 2021-11-02.
//

#pragma once

#include "Vec3.h"

class Ray {
    Point3 origin_;
    Vec3 direction_;

public:
    Ray(Point3 orig, Vec3 dir)
    : origin_(orig), direction_(dir) {}

    Point3 origin()  const { return origin_; }
    Vec3 direction() const { return direction_; }

    Point3 at(value_t t) const {
        return origin_ + direction_ * t;
    }
};
