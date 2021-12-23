//
// Created by Aleksandr Lvov on 2021-11-02.
//

#pragma once

#include "Vec3.h"

class Ray {
    Point3 origin_;
    Vec3 direction_;
    value_t time_;
public:
    Ray(Point3 orig, Vec3 dir, value_t time = 0)
    : origin_(orig), direction_(dir), time_(time) {}

    Point3 origin()  const { return origin_; }
    Vec3 direction() const { return direction_; }
    value_t time() const { return time_; }

    Point3 at(value_t t) const {
        return origin_ + direction_ * t;
    }
};
