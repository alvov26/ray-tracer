//
// Created by Aleksandr Lvov on 2021-11-02.
//

#pragma once

#include "Vec3.h"

struct Ray {
    Point3 origin;
    Vec3   direction;
    FloatT time = 0;

    [[nodiscard]]
    Point3 at(FloatT t) const {
        return origin + direction * t;
    }
};
