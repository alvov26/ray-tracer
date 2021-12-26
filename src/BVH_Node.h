//
// Created by Aleksandr Lvov on 2021-11-05.
//
#pragma once

#include "Hittable.h"
#include "HittableList.h"

class BVH_Node : public Hittable {
    AABB box;
    std::shared_ptr<Hittable> left, right;

public:
    BVH_Node(const HittableList &list, FloatT time0, FloatT time1);
    BVH_Node(const std::vector<std::shared_ptr<Hittable>> &objects,
             size_t start, size_t end, FloatT time0, FloatT time1);

    std::optional<HitRecord> intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const override;

    std::optional<AABB> boundingBox(FloatT time0, FloatT time1) const override;
};
