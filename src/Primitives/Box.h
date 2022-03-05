//
// Created by Aleksandr Lvov on 2021-12-29.
//

#pragma once

#include "Hittable.h"
#include "HittableList.h"

class Box : public Hittable {
    Point3 min_point_, max_point_;
    HittableList sides_;
    std::shared_ptr<Material> material_;

public:
    Box(const Point3 &minPoint, const Point3 &maxPoint, const std::shared_ptr<Material> &material);

    std::optional<HitRecord> intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const override;
    std::optional<AABB> boundingBox(FloatT time0, FloatT time1) const override;
};
