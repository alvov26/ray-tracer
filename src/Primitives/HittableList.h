//
// Created by Aleksandr Lvov on 2021-11-02.
//

#pragma once

#include "../Ray.h"
#include "Hittable.h"
#include <memory>
#include <vector>

class HittableList final : public Hittable {
    std::vector<std::shared_ptr<Hittable>> objects_;

public:
    HittableList() = default;
    HittableList(std::initializer_list<std::shared_ptr<Hittable>>);
    const std::vector<std::shared_ptr<Hittable>> &getObjects() const;

    void add(const std::shared_ptr<Hittable> &obj) {
        objects_.push_back(obj);
    }

    std::optional<HitRecord> intersect(
            const Ray &ray, FloatT min_dist, FloatT max_dist) const override;

    std::optional<AABB> boundingBox(FloatT time0, FloatT time1) const override;
};
