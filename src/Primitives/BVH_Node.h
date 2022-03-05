//
// Created by Aleksandr Lvov on 2021-11-05.
//
#pragma once

#include "Hittable.h"
#include "HittableList.h"

class BVH_Node : public Hittable {
    AABB box_;
    std::shared_ptr<Hittable> left_, right_;
    using Iterator = std::vector<std::shared_ptr<Hittable>>::iterator;

public:
    BVH_Node() = default;
    BVH_Node(const HittableList& list, FloatT time0, FloatT time1);
    BVH_Node(Iterator begin, Iterator end, FloatT time0, FloatT time1);

    std::optional<HitRecord> intersect(const Ray &ray, FloatT min_dist, FloatT max_dist) const override;
    std::optional<AABB> boundingBox(FloatT time0, FloatT time1) const override;

    ~BVH_Node() override = default;
};
