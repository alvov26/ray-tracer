//
// Created by Aleksandr Lvov on 2021-11-05.
//

#pragma once

#include <utility>

#include "Hittable.h"
#include "Ray.h"
#include "Vec3.h"

class MovingSphere final : public Hittable {
    Point3 center_;
    FloatT radius_;

    Vec3 velocity_;

    std::shared_ptr<Material> material_;

public:
    MovingSphere(
            Point3 center,
            FloatT radius,
            Vec3 velocity,
            std::shared_ptr<Material> material)
        : center_(center), radius_(radius), velocity_(velocity), material_(std::move(material)) {}

    std::optional<HitRecord> intersect(
            const Ray &ray, FloatT min_dist, FloatT max_dist) const override;

    Point3 center(FloatT time) const { return center_ + velocity_ * time; }

    std::optional<AABB> boundingBox(FloatT time0, FloatT time1) const override;
};
