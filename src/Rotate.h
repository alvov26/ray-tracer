//
// Created by Aleksandr Lvov on 2021-12-29.
//

#pragma once

#include "Hittable.h"
class RotateY : public Hittable {
    std::shared_ptr<Hittable> obj_;
    FloatT sin_theta_, cos_theta_;
    std::optional<AABB> aabb_;

public:
    RotateY(std::shared_ptr<Hittable> obj, FloatT angle);

    std::optional<HitRecord> intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const override;
    std::optional<AABB> boundingBox(FloatT time0, FloatT time1) const override;
};
