//
// Created by Aleksandr Lvov on 2021-12-29.
//

#pragma once

#include "Hittable.h"

class Translate : public Hittable {
    std::shared_ptr<Hittable> obj_;
    Vec3 offset_;

public:
    Translate(const std::shared_ptr<Hittable> &obj, const Vec3 &offset);

    std::optional<HitRecord> intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const override;
    std::optional<AABB> boundingBox(FloatT time0, FloatT time1) const override;
};
