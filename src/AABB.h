//
// Created by Aleksandr Lvov on 2021-11-05.
//

#pragma once

#include "Ray.h"
#include "Vec3.h"

class AABB {
    Point3 minimum_, maximum_;

public:
    AABB() = default;
    AABB(const Point3 &min, const Point3 &max)
        : minimum_(min), maximum_(max) {}

    Point3 min() const { return minimum_; }
    Point3 max() const { return maximum_; }

    bool doesIntersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const {
        for (auto i : range(3)) {
            auto invD = 1.0f / ray.direction()[i];
            auto t0 = (min()[i] - ray.origin()[i]) * invD;
            auto t1 = (max()[i] - ray.origin()[i]) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);

            min_dist = std::max(min_dist, t0);
            max_dist = std::min(max_dist, t1);
            if (max_dist <= min_dist)
                return false;
        }
        return true;
    }
};

inline AABB surroundingBox(const AABB &box1, const AABB &box2) {
    auto minimum = Point3{
            std::min(box1.min().x(), box2.min().x()),
            std::min(box1.min().y(), box2.min().y()),
            std::min(box1.min().z(), box2.min().z()),
    };
    auto maximum = Point3{
            std::max(box1.max().x(), box2.max().x()),
            std::max(box1.max().y(), box2.max().y()),
            std::max(box1.max().z(), box2.max().z()),
    };
    return {minimum, maximum};
}
