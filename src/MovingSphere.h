//
// Created by Aleksandr Lvov on 2021-11-05.
//

#pragma once

#include <utility>

#include "Vec3.h"
#include "Ray.h"
#include "Hittable.h"

class MovingSphere final: public Hittable {
    Point3 center_;
    value_t radius_;

    Vec3 velocity_;

    std::shared_ptr<Material> material_;
public:
    MovingSphere(
            Point3 center,
            value_t radius,
            Vec3 velocity,
            std::shared_ptr<Material> material)
    : center_(center)
    , radius_(radius)
    , velocity_(velocity)
    , material_(std::move(material)) {}

    std::optional<HitRecord> intersect(
            const Ray& ray, value_t min_dist, value_t max_dist) const override;

    Point3 center(value_t time) const { return center_ + velocity_ * time; }

    std::optional<AABB> boundingBox(value_t time0, value_t time1) const override;
};
